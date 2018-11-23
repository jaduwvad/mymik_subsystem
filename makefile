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


all: srcdatafile json http elasticsearch eshandler libmaker test

srcdatafile: $(SRCPATH)/srcdatafile.cpp
	$(CC) $(OBJFLAGS) -c $(SRCPATH)/srcdatafile.cpp -o $(OBJPATH)/srcdatafile.o $(VERSIONFLAG) $(INCLUDEFLAG) $(CURLFLAG)

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

