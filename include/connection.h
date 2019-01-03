#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <vector>

#define MAX_LINE 1024
#define PORT_NO 17914
#define SSH_PORT_NO 55555

class Connection {
public:
    Connection();
    ~Connection();

    //Send a string to connected shop server
    void sendMessages(std::string message);
    //Send a file to connected shop server
    void sendFile(std::string filename);

private:
    struct sockaddr_in servaddr;
    int _sockfd;
    std::string _ip;
    std::string _terminateMessage;

    std::string _localFilePath;
    std::string _scpDstPath;
};


