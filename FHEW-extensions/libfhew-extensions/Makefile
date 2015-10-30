PREFIX=$(HOME)
CC=g++
AR=ar
CFLAGS= -Wall -O3
LDFLAGS= -L. -lfhew -lfftw3 -lfhew-extensions 
INCLUDE=FHEW-extensions.h

all: libfhew-extensions.a cmd

cmd: cmd/dec32 cmd/enc32 cmd/bit_oper cmd/sum32

install: $(INCLUDE) libfhew-extensions.a
	install $(INCLUDE) $(PREFIX)/include
	install libfhew-extensions.a $(PREFIX)/lib

uninstall:
	rm $(PREFIX)/lib/libfhew-extensions.a
	rm $(PREFIX)/include/FHEW-extensions.h

clean:
	rm *.o libfhew-extensions.a cmd/enc32 cmd/dec32 cmd/bit_oper cmd/sum32 || echo nothing to clean

libfhew-extensions.a: FHEW-extensions.o
	$(AR) -q libfhew-extensions.a FHEW-extensions.o

FHEW-extensions.o: FHEW-extensions.h FHEW-extensions.cpp
	$(CC) $(CFLAGS) -c FHEW-extensions.cpp

#fhewTest: fhewTest.cpp libfhew.a
#	$(CC) $(CFLAGS) -o fhewTest fhewTest.cpp $(LDFLAGS)

common.o: cmd/common.cpp cmd/common.h libfhew-extensions.a
	$(CC) $(CFLAGS) -c cmd/common.cpp 

cmd/enc32: cmd/enc32.cpp common.o libfhew-extensions.a
	$(CC) $(CFLAGS) -std=c++11 -o cmd/enc32 cmd/enc32.cpp common.o $(LDFLAGS)

cmd/dec32: cmd/dec32.cpp common.o libfhew-extensions.a
	$(CC) $(CFLAGS) -o cmd/dec32 cmd/dec32.cpp common.o $(LDFLAGS)

cmd/bit_oper: cmd/bit_oper.cpp common.o libfhew-extensions.a
	$(CC) $(CFLAGS) -o cmd/bit_oper cmd/bit_oper.cpp common.o $(LDFLAGS)

cmd/sum32: cmd/sum32.cpp common.o libfhew-extensions.a
	$(CC) $(CFLAGS) -o cmd/sum32 cmd/sum32.cpp common.o $(LDFLAGS)
