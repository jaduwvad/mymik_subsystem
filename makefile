CC = g++ 

ARCH = $(shell getconf LONG_BIT)
LIBPATH = ./lib
SRCPATH = ./src
OBJPATH = ./obj

INCLUDEFLAG = -I ./include 
LIBFLAG = -L $(LIBPATH) -ltsd -Wl,-rpath,$(LIBPATH)
CURLFLAG = -lcurl 
VERSIONFLAG = -std=c++11

OBJFLAGS = -fPIC
SHARELIBFLAGS = -shared -o


all: csvhandler curlhandler eshandler json libmaker test

csvhandler: $(SRCPATH)/csvhandler.cpp
	$(CC) $(OBJFLAGS) -c $(SRCPATH)/csvhandler.cpp -o $(OBJPATH)/csvhandler.o $(VERSIONFLAG) $(INCLUDEFLAG)

curlhandler: $(SRCPATH)/curlhandler.cpp
	$(CC) $(OBJFLAGS) -c $(SRCPATH)/curlhandler.cpp -o $(OBJPATH)/curlhandler.o $(CURLFLAG) $(INCLUDEFLAG)

eshandler: $(SRCPATH)/eshandler.cpp
	$(CC) $(OBJFLAGS) -c $(SRCPATH)/eshandler.cpp -o $(OBJPATH)/eshandler.o $(INCLUDEFLAG)

json: $(SRCPATH)/json.cpp
	$(CC) $(OBJFLAGS) -c $(SRCPATH)/json.cpp -o $(OBJPATH)/json.o $(VERSIONFLAG) $(INCLUDEFLAG)

libmaker: $(LIBPATH)
	$(CC) $(SHARELIBFLAGS) $(LIBPATH)/libtsd.so $(OBJPATH)/*

test: test.cpp
	$(CC) -o test test.cpp  $(LIBFLAG) $(CURLFLAG) $(INCLUDEFLAG)

