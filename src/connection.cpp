#include "connection.h"
#include <string.h>

using namespace std;

Connection::Connection():
    ip("94.130.50.93"),
    terminateMessage("connection terminate") { 

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT_NO);
    inet_aton(ip.c_str(), &servaddr.sin_addr);
}

Connection::~Connection(){}

void Connection::sendMessages(string message){
    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        return ;

    write(sockfd, message.c_str(), message.length());

    sleep(1);

    write(sockfd, terminateMessage.c_str(), terminateMessage.length());
}



