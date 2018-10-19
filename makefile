CC = g++

ARCH = $(shell getconf LONG_BIT)
LIBPATH = ./lib
SRCPATH = ./src
OBJPATH = ./obj

CFLAGS = -I ./include -L $(LIBPATH) -ltsd -Wl,-rpath,$(LIBPATH) -lcurl

SHAREOBJFLAGS = -fPIC
SHARELIBFLAGS = -shared -o


all: ThreatSrcData test

ThreatSrcData: $(SRCPATH)/ThreatSrcData.cpp
	$(CC) $(SHAREOBJFLAGS) -c $(SRCPATH)/ThreatSrcData.cpp -o $(OBJPATH)/ThreatSrcData.o $(CFLAGS)
	$(CC) $(SHARELIBFLAGS) $(LIBPATH)/libtsd.so $(OBJPATH)/ThreatSrcData.o

test: test.cpp
	$(CC) -o test test.cpp $(CFLAGS)

