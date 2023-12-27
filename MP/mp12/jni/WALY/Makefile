.ALL = libWALY.a

#
# WARNING: THIS FILE HAS BEEN GENERATED AUTOMATICALLY BY
# WALY TARGET SETUP.  IF YOU WANT TO EDIT, EDIT THE ORIGINAL
# IN target-files/cygwin/jni-WALY-Makefile.def
#

CFLAGS = -g -Wall -DWALY_TARGET_CYGWIN -DAPP_NAME='"mp12"' \
	 -DAPP_TITLE='"ECE220 Spring 2018 MP12"'
#CFLAGS = -Wall

INCLUDE = -I..

HEADERS = Makefile WALY.h WALYButton.h WALYDataFile.h WALYDigitDraw.h         \
	  WALYEntryBox.h WALYEntryBoxIP.h WALYFilter.h WALYFont.h WALYFrame.h \
	  WALYImage.h WALYLabel.h WALYNetwork.h WALYPlatform.h WALYScreen.h   \
	  WALYSDLStubs.h WALYSound.h WALYWidget.h 
WALYFILES = WALYButton.o WALYDigitDraw.o WALYEntryBox.o WALYEntryBoxIP.o   \
	    WALYFilter.o WALYFont.o WALYFrame.o WALYImage.o WALYNetwork.o  \
	    WALYPlatform.o WALYScreen.o WALYSound.o WALYWidget.o

G++ = g++
AR = ar

%.o: %.cpp Makefile ${HEADERS}
	${G++} -c ${CFLAGS} -o $@ ${INCLUDE} $<

libWALY.a: ${WALYFILES}
	rm -f libWALY.a
	${AR} -rc libWALY.a ${WALYFILES}

clean::
	rm -f *.o *~ *.stackdump
	# need to remove for installer
	rm -f libWALY.a

clear: clean
	rm -f libWALY.a

