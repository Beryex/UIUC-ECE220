.ALL := ../mp12

CFLAGS = -g -Wall -DWALY_TARGET_CYGWIN

LFLAGS = -g -LWALY -LSDL
G++ = g++

INCLUDE = -IWALY -I.
LIBS = -lSDL_image -lSDL_ttf -lSDL_mixer -lSDL -lpthread -lm

HEADERS = TurtleScreen.h mp5.h mp12.h Makefile
OBJ_FILES = mp12.o TurtleScreen.o mp12main.o mp5.o

WALY_HEADERS = Makefile WALY/WALY.h WALY/WALYButton.h WALY/WALYDataFile.h     \
	       WALY/WALYDigitDraw.h WALY/WALYEntryBox.h WALY/WALYEntryBoxIP.h \
	       WALY/WALYFilter.h WALY/WALYFont.h WALY/WALYFrame.h             \
	       WALY/WALYImage.h WALY/WALYLabel.h WALY/WALYNetwork.h	      \
	       WALY/WALYPlatform.h WALY/WALYScreen.h WALY/WALYSDLStubs.h      \
	       WALY/WALYSound.h WALY/WALYWidget.h WALYmain.h
WALY_FILES = WALYmain.o

%.o: %.c ${HEADERS} ${WALY_HEADERS}
	${G++} -c ${CFLAGS} -o $@ ${INCLUDE} $<

%.o: %.cpp ${HEADERS} ${WALY_HEADERS}
	${G++} -c ${CFLAGS} -o $@ ${INCLUDE} $<

../mp12: ${OBJ_FILES} ${WALY_FILES} WALY/libWALY.a
	${G++} ${LFLAGS} -o ../mp12 ${OBJ_FILES} ${WALY_FILES} -lWALY ${LIBS}

WALY/libWALY.a::
	(cd WALY ; make)

clean::
	rm -f *.o *~ *.stackdump
	(cd WALY ; make clean)

clear: clean
	rm -f ../mp12
	(cd WALY ; make clear)
