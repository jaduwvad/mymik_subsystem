CC = g++ 

ARCH = $(shell getconf LONG_BIT)
LIBPATH = ./lib
SRCPATH = ./src
OBJPATH = ./obj

INCLUDEFLAG = -I ./include 
LIBFLAG = -L $(LIBPATH) -ltsd -Wl,-rpath,$(LIBPATH)
CURLFLAG = -lcurl 
VERSIONFLAG = -std=c++11

LINKFLAG = -W -Wall -Wfatal-errors -O3 -DNDEBUG

OBJFLAGS = -fPIC
SHARELIBFLAGS = -shared -o


all: csvhandler curlhandler json http elasticsearch libmaker test

csvhandler: $(SRCPATH)/csvhandler.cpp
	$(CC) $(OBJFLAGS) -c $(SRCPATH)/csvhandler.cpp -o $(OBJPATH)/csvhandler.o $(VERSIONFLAG) $(INCLUDEFLAG)

curlhandler: $(SRCPATH)/curlhandler.cpp
	$(CC) $(OBJFLAGS) -c $(SRCPATH)/curlhandler.cpp -o $(OBJPATH)/curlhandler.o $(CURLFLAG) $(INCLUDEFLAG)

json: $(SRCPATH)/json.cpp
	$(CC) $(OBJFLAGS) -c $(SRCPATH)/json.cpp -o $(OBJPATH)/json.o $(VERSIONFLAG) $(INCLUDEFLAG) $(LINKFLAG)

http: $(SRCPATH)/http.cpp
	$(CC) $(OBJFLAGS) -c $(SRCPATH)/http.cpp -o $(OBJPATH)/http.o $(INCLUDEFLAG) $(VERSIONFLAG) $(LINKFLAG)

elasticsearch: $(SRCPATH)/elasticsearch.cpp
	$(CC) $(OBJFLAGS) -c $(SRCPATH)/elasticsearch.cpp -o $(OBJPATH)/elasticsearch.o $(INCLUDEFLAG) $(VERSIONFLAG) $(LINKFLAG) 

libmaker: $(LIBPATH)
	$(CC) $(SHARELIBFLAGS) $(LIBPATH)/libtsd.so $(OBJPATH)/*

test: test.cpp
	$(CC) -o test test.cpp $(VERSIONFLAG) $(LIBFLAG) $(CURLFLAG) $(INCLUDEFLAG) $(LINKFLAG)

