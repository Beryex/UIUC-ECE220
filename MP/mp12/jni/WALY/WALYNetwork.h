//									tab:8
//
// WALYNetwork.h - header file for the WALY library network abstraction
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
// Version:	    1.01
// Creation Date:   4 January 2013
// Filename:	    WALYNetwork.h
// History:
//	SL	1.00	4 January 2013
//		First written.
//	SL	1.01	6 January 2013
//		Bug slaying.  Need to know which fds app thinks are open.
//

#if !defined(__WALY_NETWORK_H)
#define __WALY_NETWORK_H

#include "WALY.h"

#include <pthread.h>

#include <netinet/in.h>
#include <signal.h>
#include <sys/select.h>


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
// A note on the data model:
//   * data passed IN to network queues should be dynamically allocated;
//     the network will delete[] them
//   * data returned from network queues are dynamically allocated; the
//     application should delete[] them
//   *** NOTE the operator: delete[]
//

namespace WALY_1_0_0 {

class Network {
    public:
        static const int32_t MAX_CONNECT = 32;

	//
	// Used for pending connections--returned from connect, delivered
	// to CONNECTION events along with corresponding ConnectionID.
	//
	class PendingID {
	    private:
		int32_t val;
	        PendingID (int32_t fromInt) : val (fromInt) { }
		operator int32_t() const { return val; }
	    	friend class Network;
	    public:
	        PendingID () : val (-1) { }
		bool isValid () const { return (0 <= val); }
		void invalidate () { val = -1; }
		// app has only SDL event code field, which is an int...
		//   (and construction from int is private)
		// use these as (PendingID ==/!= int)
		bool operator== (int32_t other) const { return (val == other); }
		bool operator!= (int32_t other) const { return (val != other); }
		// use these as (int ==/!= PendingID)
		friend bool operator== (int32_t other, const PendingID& p) {
		    return (other == p.val);
		}
		friend bool operator!= (int32_t other, const PendingID& p) {
		    return (other != p.val);
		}
	};

	class ConnectionID {
	    private:
		int32_t fd;
	        ConnectionID (int32_t fromFd) : fd (fromFd) { }
		operator int32_t() const { return fd; }
	    	friend class Network;
	    public:
	        // for reading from an event...
		ConnectionID (const Event* e);
	        ConnectionID () : fd (-1) { }
		bool isValid () const { return (0 <= fd); }
		void invalidate () { fd = -1; }
		// app has only SDL event code field, which is an int...
		//   (and construction from int is private)
		// use these as (ConnectionID ==/!= int)
		bool operator== (int32_t other) const { return (fd == other); }
		bool operator!= (int32_t other) const { return (fd != other); }
		// use these as (int ==/!= ConnectionID)
		friend bool operator== (int32_t other, const ConnectionID& c) {
		    return (other == c.fd);
		}
		friend bool operator!= (int32_t other, const ConnectionID& c) {
		    return (other != c.fd);
		}
	};

    private:

	//
	// STL is supported on Android, but ... may be best not to 
	// make use of templates for now.
	//

	//
	// Application code should not make use of these queues, so they're
	// private to Network.  Too dangerous to let them block in
	// both directions, but I don't want to deal with vagaries of thread
	// scheduling...could call sched_yield, I guess.
	//

	class Queue {
	    public:
		typedef enum {
		    CMD_NONE,		// no command--never sent, but
		    			//   used for peekCmd

		    CMD_CONNECT,	// request to connect
		    // sent from application to connector
		    //    id:   (ignored)
		    //    len:  sizeof (struct sockaddr_in)
		    //    data: a struct sockaddr_in in network order
		    // NOTE: sent by tryToConnect routine; ports MUST be the 
		    //       same on both sides; allocate data with 
		    //       new uint8_t[]

		    CMD_READY,		// connection established
		    // sent from connector to sender, then receiver
		    // forwarded from receiver to application
		    //    id:   new connection ID (>= 0)
		    //    len:  sizeof (struct sockaddr_in)
		    //    data: returned struct sockadrr_in in network order
		    // NOTE: app is responsible for freeing the address;
		    //       other recipients MAY NOT USE IT (racing with
		    //       app's delete[])

		    CMD_FAILED,		// connection failed
		    // sent from connector to application
		    //    id:   CONN_INVALID
		    //    len:  sizeof (struct sockaddr_in)
		    //    data: returned struct sockadrr_in in network order

		    CMD_SEND,		// send data
		    // sent from application to sender
		    //    id:   connection ID (>= 0)
		    //    len:  of data
		    //    data: bytes -- allocate with new uint8_t[]

		    CMD_RECV,		// data/closure received
		    // sent from receiver to application
		    //    id:   connection ID (>= 0)
		    //    len:  of data
		    //    data: bytes -- free with delete[]

		    CMD_CLOSE,		// close connection
		    // sent from application to sender
		    // forwarded from sender to receiver
		    //    id:   new connection ID (>= 0)
		    //    len:  0
		    //    data: NULL

		    NUM_QUEUE_CMDS
		} queue_cmd_t;

		typedef struct {
		    queue_cmd_t  cmd;
		    ConnectionID id;
		    uint32_t     len;
		    uint8_t*     data;
		} queue_elt_t;

	    private:
		// must be a power of two
		static const int32_t QUEUE_LEN = 16;

		pthread_mutex_t lock;
		uint32_t        head;
		uint32_t        tail;
		queue_elt_t     queue[QUEUE_LEN];
		pthread_cond_t  readerAsleep;
		pthread_cond_t  writerAsleep;

	    public:
		Queue () : head (0), tail (0) { 
		    // too bad: PTHREAD_MUTEX_INITIALIZER is not robust enough
		    // to use in a C++ constructor initialization...
		    if (0 != pthread_mutex_init (&lock, NULL) ||
			0 != pthread_cond_init (&readerAsleep, NULL) ||
			0 != pthread_cond_init (&writerAsleep, NULL)) {
			throw std::exception ();
		    }
		}
		bool enqueue (queue_cmd_t cmd, ConnectionID id, uint32_t len, 
			      uint8_t* data, bool shouldBlock = false) {
		    bool retVal = true;
		    if (0 != pthread_mutex_lock (&lock)) {
			return false;
		    }
		    while (1) {
			if (tail < head + QUEUE_LEN) {
			    uint32_t idx = (tail++ % QUEUE_LEN);
			    queue[idx].cmd  = cmd;
			    queue[idx].id   = id;
			    queue[idx].len  = len;
			    queue[idx].data = data;
			    pthread_cond_signal (&readerAsleep);
			    break;
			}
			if (!shouldBlock || 
			    0 != pthread_cond_wait (&writerAsleep, &lock)) {
			    retVal = false;
			}
		    }
		    (void)pthread_mutex_unlock (&lock);
		    return retVal;
		}
		bool enqueue (const queue_elt_t* elt,
			      bool shouldBlock = false) {
		    return enqueue (elt->cmd, elt->id, elt->len, elt->data,
				    shouldBlock);
		}
		bool dequeue (queue_elt_t* elt, bool shouldBlock = false) {
		    bool retVal = true;
		    if (0 != pthread_mutex_lock (&lock)) {
			return false;
		    }
		    while (1) {
			if (tail > head) {
			    uint32_t idx = (head++ % QUEUE_LEN);
			    *elt = queue[idx];
			    pthread_cond_signal (&writerAsleep);
			    break;
			} 
			if (!shouldBlock || 
			    0 != pthread_cond_wait (&readerAsleep, &lock)) {
			    retVal = false;
			    break;
			}
		    }
		    (void)pthread_mutex_unlock (&lock);
		    return retVal;
		}
		void peekCmd (queue_elt_t* elt) {
		    elt->cmd = CMD_NONE;
		    if (0 == pthread_mutex_lock (&lock)) {
			if (tail > head) {
			    *elt = queue[head % QUEUE_LEN];
			} 
			(void)pthread_mutex_unlock (&lock);
		    }
		}
	};
	    
        static Queue toApplication;
        static Queue toConnector;
        static Queue toSender;
        static Queue toReceiver;
	// pending connections managed by Network code (in application thread)
	static struct sockaddr_in* pending[MAX_CONNECT];
	static bool cancelled[MAX_CONNECT];
	// fds that app thinks are open currently
	static fd_set appThinksOpen;

        static bool init () {
	    pthread_attr_t attr;
	    pthread_t connectorID;
	    pthread_t senderID;
	    pthread_t receiverID;

	    // ignore broken pipes
	    (void)signal (SIGPIPE, SIG_IGN);

	    memset (pending, 0, sizeof (pending));
	    FD_ZERO (&appThinksOpen);

	    if (0 != pthread_attr_init (&attr) ||
	        0 != pthread_attr_setdetachstate 
			(&attr, PTHREAD_CREATE_DETACHED) ||
	        0 != pthread_create (&connectorID, &attr, connector, NULL) ||
		0 != pthread_create (&senderID, &attr, sender, NULL) ||
		0 != pthread_create (&receiverID, &attr, receiver, NULL)) {
	        return false;
	    }
	    return true;
	}

	static void quit () {
	    // threads are detached, so we just ignore them
	}

	static bool waitForConnection (int32_t fd, Queue::queue_elt_t* elt);
	static void* connector (void* arg);

	static bool blockingWrite (int32_t fd, uint32_t len, uint8_t* data);
	static void* sender (void* arg);

	static void sendRecvToApp (int32_t fd, uint32_t len, uint8_t* data);
	static void* receiver (void* arg);

	friend class Screen;  // Screen code calls init/quit and reads from
			      //   the toApplication queue

	// only app should call the non-internal version
	static void closeConnectionInternal (ConnectionID id);

    public:
        static uint32_t getLocalIPv4 (); // returned in host byte order
	static bool networkAvailable () {
	    return (INADDR_NONE != getLocalIPv4 ());
	}

	// Arguments are IPv4 address and TCP port number in host byte order
	// (Results--either CMD_READY or CMD_FAILED--are returned in order
	// of request.  Up to app not to repeat.)
        static void tryToConnect (uint32_t addr, uint16_t port);

	// called in Screen's event-handling loop
	static bool pollEvent (bool acceptReady, bool acceptRecv, Event* evt);

	// used from application--sender thread deletes data (on success)
	static bool sendAndDelete (ConnectionID id, uint8_t* data, 
				   uint32_t len);
	static bool sendData (ConnectionID id, void* data, uint32_t len) {
	    uint8_t* copy = new uint8_t[len];
	    memcpy (copy, data, len);
	    bool retVal = sendAndDelete (id, copy, len);
	    if (!retVal) {
	        delete[] copy;
	    }
	    return retVal;
	}
	// only app should call the non-internal version
	static void closeConnection (ConnectionID id);
	// address and port in host byte order
	static PendingID openConnection (uint32_t addr, uint16_t port);
	static void cancelConnection (PendingID id);

	//
	// Applications need a way to break ties in peer-to-peer connections,
	// so we provide a way: given a connection, one of the two sides
	// will return true... (the one with the smaller IP address in
	// host order (so 128.x.y.z is 0x80xxyyzz).
	//
	// CAVEAT: NAT (network address translation) will break this protocol.
	// FIXME: use something more robust.
	//
	static bool shouldLead (ConnectionID id);
};

}

#endif /* __WALY_NETWORK_H */
