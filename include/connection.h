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
#define SSH_PORT_NO 55555

class Connection {
public:
    Connection();
    ~Connection();

    void sendMessages(std::string message);
    void sendFile(std::string filename);
    bool getConnection();
    struct sockaddr_in servaddr;

private:
    int sockfd;
    std::string ip;
    std::string terminateMessage;

    std::string localFilePath;
    std::string scpDstPath;
};


