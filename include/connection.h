#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <vector>
#define MAX_LINE 1024
#define PORT_NO 50000

class Connection {
public:
    Connection();
    ~Connection();

    void sendMessages(std::string message);
    bool getConnection();
    struct sockaddr_in servaddr;

private:
    int sockfd;
    char line[MAX_LINE];
    std::string ip;
    std::string terminateMessage;
};


