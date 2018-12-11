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

static int waitsocket(int socket_fd, LIBSSH2_SESSION *session)
{
    struct timeval timeout;
    int rc;
    fd_set fd;
    fd_set *writefd = NULL;
    fd_set *readfd = NULL;
    int dir;
 
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
 
    FD_ZERO(&fd);
 
    FD_SET(socket_fd, &fd);
 
    /* now make sure we wait in the correct direction */ 
    dir = libssh2_session_block_directions(session);

 
    if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
        readfd = &fd;
 
    if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
        writefd = &fd;
 
    rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);
 
    return rc;
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
    const char *loclfile=(localFilePath + filename).c_str();
    const char *scppath=(scpDstPath + filename).c_str();
    FILE *local;
    int rc;
    char mem[1024*100];
    size_t nread;
    char *ptr;
    struct stat fileinfo;
    time_t start;
    long total = 0;
    int duration;
    size_t prev;
 
    hostaddr = inet_addr(ip.c_str());
    rc = libssh2_init (0);

    if (rc != 0) {
        fprintf (stderr, "libssh2 initialization failed (%d)\n", rc);
        return;
    }
    cout<<loclfile<<endl;
    local = fopen(loclfile, "rb");
    if (!local) {
        fprintf(stderr, "Can't local file %s\n", loclfile);
        return;
    }
 
    stat(loclfile, &fileinfo);
 
    sock = socket(AF_INET, SOCK_STREAM, 0);
 
    sin.sin_family = AF_INET;
    sin.sin_port = htons(55555);
    sin.sin_addr.s_addr = hostaddr;
    if (connect(sock, (struct sockaddr*)(&sin),
            sizeof(struct sockaddr_in)) != 0) {
        fprintf(stderr, "failed to connect!\n");
        return;
    }
 
    session = libssh2_session_init();

    if(!session)
        return;
 
    libssh2_session_set_blocking(session, 0);

 
    while ((rc = libssh2_session_handshake(session, sock)) == LIBSSH2_ERROR_EAGAIN);
    if(rc) {
        fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
        return;
    }
 
    fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

    if (auth_pw) {
        while ((rc = libssh2_userauth_password(session, username, password)) == LIBSSH2_ERROR_EAGAIN);
        if (rc) {
            fprintf(stderr, "Authentication by password failed.\n");
            goto shutdown;
        }
    } else {
        while ((rc = libssh2_userauth_publickey_fromfile(session, username,
                                                         "~/.ssh/id_rsa.pub",
                                                         "/home/username/.ssh/id_rsa",
                                                         password)) == LIBSSH2_ERROR_EAGAIN);
        if (rc) {
            fprintf(stderr, "\tAuthentication by public key failed\n");
            goto shutdown;
        }
    }
 
    do {
        channel = libssh2_scp_send(session, scppath, fileinfo.st_mode & 0777,
                                   (unsigned long)fileinfo.st_size);
/*
        if ((!channel) && (libssh2_session_last_errno(session) != LIBSSH2_ERROR_EAGAIN)) {
            cout<<libssh2_session_last<<endl;
            char *err_msg;
 
            libssh2_session_last_error(session, &err_msg, NULL, 0);
            fprintf(stderr, "%s\n", err_msg);
            goto shutdown;
        }
*/
    } while (!channel);

    start = time(NULL);
    do {
        nread = fread(mem, 1, sizeof(mem), local);
        if (nread <= 0) 
            break;
        ptr = mem;
 
        total += nread;
 
        prev = 0;
        do {
            while ((rc = libssh2_channel_write(channel, ptr, nread)) ==
                   LIBSSH2_ERROR_EAGAIN) {
                waitsocket(sock, session);
                prev = 0;
            }
            if (rc < 0) {
                fprintf(stderr, "ERROR %d total %ld / %d prev %d\n", rc,
                        total, (int)nread, (int)prev);
                break;
            }
            else {
                prev = nread;
 
                /* rc indicates how many bytes were written this time */ 
                nread -= rc;
                ptr += rc;
            }
        } while (nread);
    } while (!nread); /* only continue if nread was drained */ 
 
    duration = (int)(time(NULL)-start);
 
    fprintf(stderr, "%ld bytes in %d seconds makes %.1f bytes/sec\n",
           total, duration, total/(double)duration);
    while (libssh2_channel_send_eof(channel) == LIBSSH2_ERROR_EAGAIN);
    while (libssh2_channel_wait_eof(channel) == LIBSSH2_ERROR_EAGAIN);
    while (libssh2_channel_wait_closed(channel) == LIBSSH2_ERROR_EAGAIN);

 
    libssh2_channel_free(channel);

    channel = NULL;
 
 shutdown:
 
    while (libssh2_session_disconnect(session,"Normal Shutdown, Thank you for playing") ==
           LIBSSH2_ERROR_EAGAIN);
    libssh2_session_free(session);

 
    close(sock);
    fprintf(stderr, "all done\n");
 
    libssh2_exit();

 
    return;
}


