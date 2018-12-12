#include <libssh2.h>
#include "connection.h"
#include <string.h>

using namespace std;

Connection::Connection():
    _ip("94.130.50.93"),
    _localFilePath("/tmp/mymik/results/"),
    _scpDstPath("/var/www/vhosts/my-mik.de/Sourcing_File/"),
    _terminateMessage("connection terminate") { 

    _sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT_NO);
    inet_aton(_ip.c_str(), &servaddr.sin_addr);
}

Connection::~Connection(){}

void Connection::sendMessages(string message){
    if( connect(_sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        return ;

    write(_sockfd, message.c_str(), message.length());

    sleep(1);

    write(_sockfd, _terminateMessage.c_str(), _terminateMessage.length());
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
    string loclfile= _localFilePath + filename;
    string scppath= _scpDstPath + filename;
    FILE *local;
    int rc;
    char mem[MAX_LINE];
    size_t nread;
    char *ptr;
    struct stat fileinfo;
 
    local = fopen(loclfile.c_str(), "rb");
    if (!local) {
        cout<<"Can't open local file"<<loclfile<<endl;
        return;
    }

    hostaddr = inet_addr(_ip.c_str());
    rc = libssh2_init(0);
    if (rc != 0) {
        cout<<"libssh2 initialization failed"<<rc<<endl;
        return;
    }
  
    stat(loclfile.c_str(), &fileinfo);
 
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sock) {
        cout<<"failed to create socket!"<<endl;
        return;
    }
 
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SSH_PORT_NO);
    sin.sin_addr.s_addr = hostaddr;
    if (connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) != 0) {
        cout<<"failed to connect!"<<endl;;
        return;
    }
 
    session = libssh2_session_init();

    if(!session)
        return;
 
    rc = libssh2_session_handshake(session, sock);

    if(rc) {
        cout<<"Failure establishing SSH session:"<<rc<<endl;
        return;
    }
 
    fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

    if (auth_pw) {
        if (libssh2_userauth_password(session, username, password)) {
            cout<<"Authentication by password failed."<<endl;
            goto shutdown;
        }
    } else {
        if (libssh2_userauth_publickey_fromfile(session, username, "~/.ssh/id_rsa.pub", "~/.ssh/id_rsa", password)) {
            cout<<"Authentication by public key failed"<<endl;
            goto shutdown;
        }
    }
    channel = libssh2_scp_send(session, scppath.c_str(), fileinfo.st_mode & 0777, (unsigned long)fileinfo.st_size);
 
    if (!channel) {
        char *errmsg;
        int errlen;
        int err = libssh2_session_last_error(session, &errmsg, &errlen, 0);

        cout<<"Unable to open a session: "<<err<<errmsg<<endl;
        goto shutdown;
    }
 
    do {
        nread = fread(mem, 1, sizeof(mem), local);
        if (nread <= 0) 
            break;
        ptr = mem;
 
        do {
            rc = libssh2_channel_write(channel, ptr, nread);
            if (rc < 0) {
                cout<<"ERROR"<<rc<<endl;
                break;
            }
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

