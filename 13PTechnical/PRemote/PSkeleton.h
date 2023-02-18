#pragma once

#include <02Platform/Component/Component.h>
#include <13PTechnical/PRemote/ISkeleton.h>
#include <01Base/Aspect/Exception.h>

#include <sys/socket.h>
//#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXBUF 1024 /* 클라이언트와 길이를 맞춘다 */

#define PORT 12346
#define BUF_SIZE 1024

class PSkeleton : public Component, public ISkeleton
{
private:
        int server_sockfd, client_sockfd;
        socklen_t client_len;
        int n;
        char buf[MAXBUF];
        struct sockaddr_in clientaddr, serveraddr;

public:
	PSkeleton(int nComponentId = _PSkeleton_Id, const char* sComponentName = _PSkeleton_Name) {
    }
	~PSkeleton() override {
    }

    void Initialize() override {
    }

    void Finalize() override {
    }

    void Start() override {

        client_len = sizeof(clientaddr);
        // socket
        if((server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
                throw Exception((int)(ISkeleton::EException::eSocket));
        }
        // bind
        bzero(&serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serveraddr.sin_port = htons(10000);    
        bind(server_sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));

        // listen
        listen(server_sockfd, 5);
    
        while(1){
            // accept
            memset(buf, 0x00, MAXBUF);
            client_sockfd = accept(server_sockfd, (struct sockaddr *) &clientaddr, &client_len);
            printf("New client connect: %s\n", inet_ntoa(clientaddr.sin_addr));

            // read
            if((n = read(client_sockfd, buf, MAXBUF)) <= 0) {
                close(client_sockfd);
                continue;
            }
            // write
            if(write(client_sockfd, buf, MAXBUF) <= 0) {
                perror("write error : ");
                close(client_sockfd);
            }
            close(client_sockfd);
        }
        close(server_sockfd);

    }
    void Stop() override {

    }
};