#include <libssh2.h>
#include "connection.h"
#include <string.h>

using namespace std;

Connection::Connection():
    ip("94.130.50.93"),
    localFilePath("/tmp/mymik/results/"),
    scpDstPath("/var/www/vhosts/my-mik.de/Sourcing_File/"),
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

void Connection::sendFile(string filename){
    unsigned long hostaddr;
    int sock, i, auth_pw = 1;
    struct sockaddr_in sin;
    const char *fingerprint;
    LIBSSH2_SESSION *session = NULL;
    LIBSSH2_CHANNEL *channel;
    const char *username="root";
    const char *password="!Geld7914";
    const char *loclfile = (localFilePath+filename).c_str();
    const char *scppath = (scpDstPath + filename).c_str();
    FILE *local;
    int rc;
    char mem[1024];
    size_t nread;
    char *ptr;
    struct stat fileinfo;

    cout<<loclfile<<endl;
    cout<<scppath<<endl;
 
    hostaddr = inet_addr(ip.c_str());
 
    rc = libssh2_init (0);

    if (rc != 0) 
        return;
 
    local = fopen(loclfile, "rb");
    if (!local) 
        return;
 
    stat(loclfile, &fileinfo);
 
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sock) 
        return;
 
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SSH_PORT_NO);
    sin.sin_addr.s_addr = hostaddr;
    if (connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) != 0) 
        return;
 
    session = libssh2_session_init();

    if(!session)
        return;
 
    rc = libssh2_session_handshake(session, sock);

    if(rc)
        return;
 
    fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

    if (auth_pw) 
        if (libssh2_userauth_password(session, username, password)) 
            goto shutdown;
    else
        if (libssh2_userauth_publickey_fromfile(session, username, "~/.ssh/id_rsa.pub", "~/.ssh/id_rsa", password)) 
            goto shutdown;
 
    channel = libssh2_scp_send(session, scppath, fileinfo.st_mode & 0777, (unsigned long)fileinfo.st_size);

    if (!channel) {
        char *errmsg;
        int errlen;
        int err = libssh2_session_last_error(session, &errmsg, &errlen, 0);

        goto shutdown;
    }
 
    do {
        nread = fread(mem, 1, sizeof(mem), local);
        if (nread <= 0) 
            break;
        ptr = mem;
 
        do {
            rc = libssh2_channel_write(channel, ptr, nread);

            if (rc < 0) 
                break;
            else {
                ptr += rc;
                nread -= rc;
            }
        } while (nread);
 
    } while (1);
 
    libssh2_channel_send_eof(channel);
    libssh2_channel_wait_eof(channel);
    libssh2_channel_wait_closed(channel);
    libssh2_channel_free(channel);

    channel = NULL;
 
 shutdown:
 
    if(session) {
        libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
        libssh2_session_free(session);
    }

    close(sock);
    if (local)
        fclose(local);
 
    libssh2_exit();
    return;
}


