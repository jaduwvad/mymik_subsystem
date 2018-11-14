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

ESFLAG = $(INCLUDEFLAG) $(VERSIONFLAG) $(LINKFLAG)

OBJFLAGS = -fPIC
SHARELIBFLAGS = -shared -o


all: csvhandler curlhandler json http elasticsearch eshandler libmaker test

csvhandler: $(SRCPATH)/csvhandler.cpp
	$(CC) $(OBJFLAGS) -c $(SRCPATH)/csvhandler.cpp -o $(OBJPATH)/csvhandler.o $(VERSIONFLAG) $(INCLUDEFLAG)

curlhandler: $(SRCPATH)/curlhandler.cpp
	$(CC) $(OBJFLAGS) -c $(SRCPATH)/curlhandler.cpp -o $(OBJPATH)/curlhandler.o $(CURLFLAG) $(INCLUDEFLAG)

json: $(SRCPATH)/json.cpp
	$(CC) $(OBJFLAGS) -c $(SRCPATH)/json.cpp -o $(OBJPATH)/json.o $(ESFLAG)

http: $(SRCPATH)/http.cpp
	$(CC) $(OBJFLAGS) -c $(SRCPATH)/http.cpp -o $(OBJPATH)/http.o $(ESFLAG)

elasticsearch: $(SRCPATH)/elasticsearch.cpp
	$(CC) $(OBJFLAGS) -c $(SRCPATH)/elasticsearch.cpp -o $(OBJPATH)/elasticsearch.o $(ESFLAG) 

eshandler: $(SRCPATH)/eshandler.cpp
	$(CC) $(OBJFLAGS) -c $(SRCPATH)/eshandler.cpp -o $(OBJPATH)/eshandler.o $(ESFLAG)

libmaker: $(LIBPATH)
	$(CC) $(SHARELIBFLAGS) $(LIBPATH)/libtsd.so $(OBJPATH)/*

test: test.cpp
	$(CC) -o test test.cpp $(ESFLAG) $(LIBFLAG) $(CURLFLAG)

