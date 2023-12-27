//									tab:8
//
// WALYNetwork.cpp - source file for the WALY library network abstraction
//
// "Copyright (c) 2013 by Steven S. Lumetta."
//
// Permission to use, copy, modify, and distribute this software and its
// documentation for any purpose, without fee, and without written agreement is
// hereby granted, provided that the above copyright notice and the following
// two paragraphs appear in all copies of this software.
// 
// IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
// ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
// DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
// EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED 
// OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY 
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE 
// PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
// THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE, 
// SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
//
// Author:	    Steve Lumetta
// Version:	    1.03
// Creation Date:   4 January 2013
// Filename:	    WALYNetwork.cpp
// History:
//	SL	1.00	4 January 2013
//		First written.
//	SL	1.01	6 January 2013
//		Bug slaying.  Need to know which fds app thinks are open.
//	SL	1.02	22 January 2013
//		Fixed a few conversion issues with 64-bit Linux (more to go).
//	SL	1.03	28 January 2013
//		Fixed a couple more conversion issues.
//

#include "WALY.h"

#include <pthread.h>

#include <poll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>

//
// I wrote some SDL-based networking for the lines game a couple of years
// ago, but all of the platforms I am targeting support pthreads and BSD
// sockets, so I may as well skip the SDL networking and use lower-level
// abstractions.
//
// I will start with support for creating peer-to-peer connections and
// sending messages.
//   - Both sides must know the other's IP address in this case, and
//     numerical HOST ORDER (so 128.x.y.z becomes 0x80xxyyzz) is used
//     to decide who connects and who accepts.  The smaller IP address
//     accepts, and the larger connects.  See the shouldLead function, too.
//   - I will prepend the message length to give a datagram abstraction 
//     over TCP.
//
// In terms of concurrency, I will create three other threads: one to
// manage setting up connections, a second to manage sending, and a 
// third to manage receiving.  That way, users shouldn't run into TCP
// deadlock possibilities, at least if they pay attention to full 
// queue errors.
//
// Simple lock-based queues will be used to pass info from thread to
// thread.  They're only single-producer, single-consumer, so they could
// be lockless, but Posix provides portable memory barriers implicitly,
// and ... ARM is potentially a source of problems, which I'd rather
// not address explicitly when compiling for Android and webOS.
//
// Actually, since I'm locking them, I may as well use one queue per
// recipient, so ... multi-producer ...
//
// It's probably worth noting here that most of WALY is not designed
// for multithreading, so ... the threads here only interact via their
// queues...
//

using namespace std;

namespace WALY_1_0_0 {

Network::Queue      Network::toApplication;
Network::Queue      Network::toConnector;
Network::Queue      Network::toReceiver;
Network::Queue      Network::toSender;
struct sockaddr_in* Network::pending[MAX_CONNECT];
bool                Network::cancelled[MAX_CONNECT];
fd_set              Network::appThinksOpen;

// applications need a way to create a ConnectionID from a 
// WALY_CONNECTION_EVENT...
Network::ConnectionID::ConnectionID (const Event* e) {
    fd = (WALY_CONNECTION_EVENT == e->type ?
          (intptr_t)e->user.data1 : -1);
}

bool
Network::waitForConnection (int32_t fd, Queue::queue_elt_t* elt)
{
    struct sockaddr_in* desiredAddr = (struct sockaddr_in*)elt->data;
    int32_t             client_fd;
    struct sockaddr_in  remAddr;
    socklen_t	        len;

    len = sizeof (remAddr);
//Screen::debug () << "entering accept" << endl;
    while (0 <= (client_fd = accept (fd, (struct sockaddr*)&remAddr, &len))) {
//Screen::debug () << "accepted " << hex << ntohl (remAddr.sin_addr.s_addr)
//<< dec << " port " << ntohs (remAddr.sin_port) << endl;
        if (sizeof (remAddr) == len &&
	    remAddr.sin_addr.s_addr == desiredAddr->sin_addr.s_addr) {
	    elt->id = client_fd;
	    return true;
	}
	Screen::debug () << "unwanted connection!" << endl;
	len = sizeof (remAddr);
    }
//Screen::debug () << "accept on " << fd << "failed! " << strerror (errno) << endl;
    return false;
}

void* 
Network::connector (void* arg) 
{
    Queue::queue_elt_t elt;

    while (toConnector.dequeue (&elt, true)) {
	struct sockaddr_in* remAddr;

	if (Queue::CMD_CONNECT != elt.cmd || sizeof (*remAddr) != elt.len) {
	    Screen::debug () << "bad command sent to connector: "
	    		     << elt.cmd << endl;
	    continue;
	}
	remAddr = (struct sockaddr_in*)elt.data;

	struct sockaddr_in addr;	// local address
	int32_t		   fd;		// socket file descriptor
	int		   one = 1;	// 'true' for setting socket option

	elt.cmd = Queue::CMD_FAILED;	// in case of failure...
	elt.id  = -1;

	if (-1 <= (fd = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP))) {
	    // We have a socket.  Now we need to decide who plays the
	    // server role--the SMALLER IP address (using HOST order, since
	    // network order may vary...).
	    uint32_t myAddr = getLocalIPv4 ();
	    if (myAddr < ntohl (remAddr->sin_addr.s_addr)) {
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = remAddr->sin_port;
		memset (&addr.sin_zero, 0, sizeof (addr.sin_zero));
		if (0 == setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &one, 
				     sizeof (one)) &&
		    0 == bind (fd, (struct sockaddr*)&addr, sizeof (addr)) &&
		    0 == listen (fd, 100) && waitForConnection (fd, &elt)) {
		    // waitForConnection fills in id.
		    elt.cmd = Queue::CMD_READY;
		}
		// Close server fd in either case.
		(void)close (fd);
	    } else {
		// This machine calls connect.
//Screen::debug () << "entering connect" << endl;
		if (0 == connect (fd, (struct sockaddr*)remAddr,
				  sizeof (*remAddr))) {
//Screen::debug () << "connect success" << endl;
		    elt.cmd = Queue::CMD_READY;
		    elt.id  = fd;
		} else {
//Screen::debug () << "connect failed: " << strerror (errno) << endl;
		    (void)close (fd);
		}
	    }
	}

	//
	// Order is important here.  Once receiver knows about the
	// connection, data may be sent to app.  Once app knows about
	// connection, it may send data to sender.
	//
	// To avoid mis-ordering, we first inform sender, then receiver.
	// Receiver forwards to app, so it knows before receiver sends
	// any data.
	//
	// (Sending directly from connector to app opens a race in which
	// the app closes immediately, which is sent to sender, which
	// forwards to receiver.  Receiver might then see a connection
	// closed before it is opened, which is hard to unravel.)
	//

	if (Queue::CMD_READY == elt.cmd) {
	    if (!toSender.enqueue (&elt, true)) {
		Screen::debug () << "connector enqueue to sender failed"
				 << endl;
	    } else if (!toReceiver.enqueue (&elt, true)) {
		Screen::debug () << "connector enqueue to receiver failed" 
				 << endl;
	    }
	} else  {
	    // Send a failure notice to the application.
	    if (!toApplication.enqueue (&elt, true)) {
		Screen::debug () << "connector enqueue to application failed" 
				 << endl;
	    }
	}
    }

    return NULL;
}

bool
Network::blockingWrite (int32_t fd, uint32_t len, uint8_t* data)
{
    int32_t rval;
    uint32_t off = 0;
    do {
	rval = write (fd, data + off, len - off);
	if (0 > rval) {
	    return false;
	}
	off += rval;
    } while (len > off);
    return true;
}

void* 
Network::sender (void* arg) 
{
    Queue::queue_elt_t elt;
    fd_set validFds;

    FD_ZERO (&validFds);

    while (toSender.dequeue (&elt, true)) {
	switch (elt.cmd) {
	    case Queue::CMD_READY:
//Screen::debug () << "SENDER GOT READY" << endl;
		FD_SET (elt.id, &validFds);
	        break;
    	    case Queue::CMD_SEND: {
		if (!FD_ISSET (elt.id, &validFds)) {
		    // Unknown fd, but perhaps because we closed it but
		    // app still trying to send or sends were already 
		    // enqueued, so generate no debug message.
		    break;
		}
		// send the length first (in network byte order)
		uint32_t len = htonl (elt.len);
		if (!blockingWrite (elt.id, sizeof (len), (uint8_t*)&len) ||
		    !blockingWrite (elt.id, elt.len, elt.data)) {
		    //
		    // We simply stop writing to this connection.
		    // We do not close the descriptor because we
		    // need to make sure that the other threads
		    // are aware that it has closed.  Also, we may
		    // still need to read data (TCP may still be 
		    // delivering in the other direction).
		    //
		    FD_CLR (elt.id, &validFds);
		    break;
		}
		delete[] elt.data;
		break;
	    }
    	    case Queue::CMD_CLOSE:
//Screen::debug () << "SENDER IS CLOSING " << elt.id << endl;
		FD_CLR (elt.id, &validFds);
	    	// forward to receiver
		if (!toReceiver.enqueue (&elt, true)) {
		    Screen::debug ()
			    << "sender enqueue to receiver failed" << endl;
		}
		break;
	    default:
		Screen::debug () << "bad command sent to sender: "
				 << elt.cmd << endl;
		break;
	}
    }

    return NULL;
}

void
Network::sendRecvToApp (int32_t fd, uint32_t len, uint8_t* data)
{
    Queue::queue_elt_t elt;

    elt.cmd  = Queue::CMD_RECV;
    elt.id   = fd;
    elt.len  = len;
    elt.data = data;
    if (!toApplication.enqueue (&elt, true)) {
	Screen::debug () << "receiver enqueue to application failed" << endl;
    }
}

void* 
Network::receiver (void* arg) 
{
    struct pollfd pfd[MAX_CONNECT];
    Queue::queue_elt_t elt;
    int32_t numFds = 0;
    int32_t curIdx;

    //
    // we read a 4-byte message length into the msgLen field, then
    // allocate a new data block and read data into it until it's full;
    // negative offsets correspond to still needing to read the length...
    //
    int32_t  msgOff[MAX_CONNECT];
    uint32_t msgLen[MAX_CONNECT];
    uint8_t* data[MAX_CONNECT];

    // requested events never change...
    for (curIdx = 0; MAX_CONNECT > curIdx; curIdx++) {
	pfd[curIdx].events = (POLLIN | POLLPRI);
    }

    while (1) {
	// check the incoming queue...
	while (toReceiver.dequeue (&elt)) {
//Screen::debug () << "RECEIVER GOT " << (int)elt.cmd << endl;
	    switch (elt.cmd) {
		case Queue::CMD_READY:
//Screen::debug () << "RECEIVER GOT READY" << endl;
		    // forward to app first...
		    if (!toApplication.enqueue (&elt, true)) {
			Screen::debug () 
				<< "receiver enqueue to application failed" 
				<< endl;
		    }
		    for (curIdx = 0; numFds > curIdx && 0 <= pfd[curIdx].fd; 
			 curIdx++) { 
			// find a free pollfd structure
		    }
		    if (MAX_CONNECT > curIdx) {
			if (numFds == curIdx) {
			    numFds++;
			}
			pfd[curIdx].fd = elt.id;
			msgOff[curIdx] = -(int32_t)sizeof (msgLen[curIdx]);
		    } else {
		        sendRecvToApp (elt.id, 0, NULL);
		    } 
		    break;
		case Queue::CMD_CLOSE:
//Screen::debug () << "RECEIVER IS CLOSING " << elt.id << endl;
		    (void)close (elt.id);
		    for (curIdx = 0; numFds > curIdx; curIdx++) {
		        if (elt.id == pfd[curIdx].fd) {
			    if (0 <= msgOff[curIdx]) {
			        delete[] data[curIdx];
			    }
			    pfd[curIdx].fd = -1;
			    while (0 < numFds && -1 == pfd[numFds].fd) {
				numFds--;
			    }
			    break;
			}
		    }
		    break;
	        default:
		    Screen::debug () << "bad command sent to receiver: "
				     << elt.cmd << endl;
		    break;
	    }
	}

	// wait for incoming data for up to 100 milliseconds...
	if (0 < poll (pfd, numFds, 100)) {
	    for (curIdx = 0; numFds > curIdx; curIdx++) {
		if (0 == ((POLLIN | POLLPRI) & pfd[curIdx].revents)) {
		    continue;
		}
		int32_t rval;
		if (0 > msgOff[curIdx]) {
		    rval = read (pfd[curIdx].fd,
			    ((uint8_t*)&msgLen[curIdx + 1]) + msgOff[curIdx],
			    -msgOff[curIdx]);
		} else {
		    rval = read (pfd[curIdx].fd, data[curIdx] + msgOff[curIdx],
				 msgLen[curIdx] - msgOff[curIdx]);
		}
		if (0 >= rval) {
		    sendRecvToApp (pfd[curIdx].fd, 0, NULL);
		    pfd[curIdx].fd = -1;
		    while (0 < numFds && -1 == pfd[numFds].fd) {
			numFds--;
		    }
		    // wait for sender to tell us to close...
		    // (avoids having connector reopen it before
		    // all threads recognize closure)
		}
		msgOff[curIdx] += rval;
		if (0 == msgOff[curIdx]) {
		    msgLen[curIdx] = ntohl (msgLen[curIdx]);
		    data[curIdx] = new uint8_t[msgLen[curIdx]];
		    // ready to read now! ... but may be out of data,
		    // and can't afford to block...
		} else if (msgLen[curIdx] == (uint32_t)msgOff[curIdx]) {
		    sendRecvToApp (pfd[curIdx].fd, msgLen[curIdx], 
		    		   data[curIdx]);
		    msgOff[curIdx] = -(int32_t)sizeof (msgLen[curIdx]);
		}
	    }
	}
    }

    return NULL;
}

uint32_t 
Network::getLocalIPv4 ()
{
    int32_t fd;

    if (-1 == (fd = socket (PF_INET, SOCK_DGRAM, 0))) {
        return INADDR_NONE;
    }

    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons (80);
    servAddr.sin_addr.s_addr = htonl (0x80202020);
    memset (&servAddr.sin_zero, 0, sizeof (servAddr.sin_zero));
    if (0 != connect (fd, (struct sockaddr*)&servAddr, sizeof (servAddr))) {
	(void)close (fd);
        return INADDR_NONE;
    }

    struct sockaddr_in myAddr;
    socklen_t len = sizeof (myAddr);
    if (0 != getsockname (fd, (struct sockaddr*)&myAddr, &len)) {
	(void)close (fd);
        return INADDR_NONE;
    }

    (void)close (fd);
    return ntohl (myAddr.sin_addr.s_addr);
}

// called in Screen's event-handling loop
bool 
Network::pollEvent (bool acceptReady, bool acceptRecv, Event* evt)
{
    Queue::queue_elt_t elt;
    toApplication.peekCmd (&elt);
    int32_t idx = 0;

    if (Queue::CMD_READY == elt.cmd || Queue::CMD_FAILED == elt.cmd) {

//Screen::debug () << "APP checking " << elt.cmd << endl;

	// Find the pending connection.
	while (pending[idx] != (struct sockaddr_in*)elt.data) {
	    if (MAX_CONNECT == ++idx) {
	        // Pending connection not found?
		Screen::debug () << "result for unrequested connection!"
				 << endl;
		toApplication.dequeue (&elt);
		delete[] elt.data;
		closeConnectionInternal (elt.id);
		return false;
	    }
	}
		
	// Check for cancellation.
	if (cancelled[idx]) {
//Screen::debug () << "cancelled" << endl;
	    pending[idx] = NULL;
	    cancelled[idx] = false;
	    toApplication.dequeue (&elt);
	    delete[] elt.data;
	    if (Queue::CMD_READY == elt.cmd) {
		closeConnectionInternal (elt.id);
	    }
	    return false;
	}
    }

    switch (elt.cmd) {
        case Queue::CMD_NONE:
	    break;
        case Queue::CMD_READY:
//Screen::debug () << "APP queue head " << elt.cmd << endl;
	    if (!acceptReady) {
	        return false;
	    }
	    if (!FD_ISSET (elt.id, &appThinksOpen)) {
//Screen::debug () << "APP RECEIVED READY " << endl;
		FD_SET (elt.id, &appThinksOpen);
		toApplication.dequeue (&elt);
		delete[] elt.data;
		pending[idx] = NULL;
		cancelled[idx] = false;
		evt->user.type  = WALY_CONNECTION_EVENT;
		evt->user.code  = idx; // now a PendingID
		evt->user.data1 = (void*)(intptr_t)elt.id;
		evt->user.data2 = NULL;
		return true;
	    }
	    //
	    // If the app thinks that this fd is already open, we have
	    // a problem.  We close the new version and tell the app
	    // that the pending connection failed.  When the close
	    // completes, the app will receive a 0-length RECV (close)
	    // for its *existing* connection with this id.
	    //
	    closeConnectionInternal (elt.id);
	    // fall through and treat as a CMD_FAILED...
        case Queue::CMD_FAILED:
//Screen::debug () << "APP queue head " << elt.cmd << endl;
	    //
	    // NOTE: CANNOT USE elt.cmd NOR elt.id HERE, since we
	    // may have fallen through from above (these values are 
	    // constant for CMD_FAILED anyway...)
	    //
	    if (acceptReady) {
//Screen::debug () << "APP RECEIVED FAILED " << endl;
		toApplication.dequeue (&elt);
		delete[] elt.data;
		pending[idx] = NULL;
		cancelled[idx] = false;
		evt->user.type  = WALY_CONNECTION_EVENT;
		evt->user.code  = idx; // now a PendingID
		evt->user.data1 = (void*)-1;
		evt->user.data2 = NULL;
		return true;
	    }
	    break;
        case Queue::CMD_RECV:
//Screen::debug () << "APP queue head " << elt.cmd << endl;
	    if (!FD_ISSET (elt.id, &appThinksOpen)) {
	        // data on closed connection--possibly just a close, too
		// (we discard it)
		toApplication.dequeue (&elt);
		delete[] elt.data;
	    } else if (acceptRecv) {
//Screen::debug () << "APP RECEIVED " << elt.len << endl;
		if (0 == elt.len) { // a close...
		    FD_CLR (elt.id, &appThinksOpen);
		}
		toApplication.dequeue (&elt);
		evt->user.type  = WALY_NETWORK_EVENT;
		evt->user.code  = elt.id;
		evt->user.data1 = (void*)(intptr_t)elt.len;
		evt->user.data2 = elt.data;
		return true;
	    }
	    break;
	default:
	    Screen::debug () << "bad command sent to application: "
			     << elt.cmd << endl;
	    toApplication.dequeue (&elt); // discard it
	    break;
    }
    return false;
}

bool
Network::sendAndDelete (ConnectionID id, uint8_t* data, uint32_t len)
{
    Queue::queue_elt_t elt;

    elt.cmd  = Queue::CMD_SEND;
    elt.id   = id;
    elt.len  = len;
    elt.data = data;
    return toSender.enqueue (&elt);
}

void
Network::closeConnectionInternal (ConnectionID id)
{
    Queue::queue_elt_t elt;

    elt.cmd  = Queue::CMD_CLOSE;
    elt.id   = id;
    elt.len  = 0;
    elt.data = NULL;
    if (!toSender.enqueue (&elt)) {
        // we may end up with races on re-creation of this fd, 
	// but app can't block... (FIXME?)  ... could shove it
	// into an fd_set instead and try again to close 
	// it lazily, maybe when app polls for events...
	close (id);
    }
}

void
Network::closeConnection (ConnectionID id)
{
//Screen::debug () << "called close on " << id << std::endl;
    FD_CLR (id, &appThinksOpen);

    closeConnectionInternal (id);
}

Network::PendingID 
Network::openConnection (uint32_t addr, uint16_t port)
{
    int32_t idx;

    for (idx = 0; MAX_CONNECT > idx; idx++) {
        if (NULL == pending[idx]) {
	    pending[idx] = (struct sockaddr_in*)
		    new uint8_t[sizeof (struct sockaddr_in)];
	    pending[idx]->sin_family = AF_INET;
	    pending[idx]->sin_addr.s_addr = htonl (addr);
	    pending[idx]->sin_port = htons (port);
	    memset (&pending[idx]->sin_zero, 0, 
	    	    sizeof (pending[idx]->sin_zero));
	    cancelled[idx] = false;

	    Queue::queue_elt_t elt;
	    elt.cmd  = Queue::CMD_CONNECT;
	    elt.id   = -1; // ignored
	    elt.len  = sizeof (*pending[idx]);
	    elt.data = (uint8_t*)pending[idx];
	    if (toConnector.enqueue (&elt)) {
	        return idx;
	    }
	    delete[] (uint8_t*)pending[idx];
	    pending[idx] = NULL;
	    return -1;
	}
    }
    return -1;
}

void 
Network::cancelConnection (PendingID id)
{
    cancelled[id] = true;
}

bool
Network::shouldLead (ConnectionID id)
{
    // safest here to check the addresses in the TCP connection itself

    sockaddr_in myAddr;
    socklen_t   myLen = sizeof (myAddr);
    sockaddr_in remAddr;
    socklen_t   remLen = sizeof (remAddr);

    if (0 != getsockname (id, (struct sockaddr*)&myAddr, &myLen) || 
	sizeof (myAddr) != myLen ||
        0 != getpeername (id, (struct sockaddr*)&remAddr, &remLen) || 
	sizeof (remAddr) != remLen) {
	// oops
	Screen::debug () << "shouldLead got bad results" << endl;
	return true;
    }
//    Screen::debug () << "Should lead? " << hex 
//	    << ntohl (myAddr.sin_addr.s_addr) << " < " 
//	    << ntohl (remAddr.sin_addr.s_addr) << dec << " ?" << endl;

    return (ntohl (myAddr.sin_addr.s_addr) < ntohl (remAddr.sin_addr.s_addr));
}

}

