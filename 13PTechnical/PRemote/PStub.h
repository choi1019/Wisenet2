#pragma once

#include <02Platform/Component/Component.h>
#include <13PTechnical/PRemote/IStub.h>
#include <01Base/Aspect/Exception.h>

#include <sys/socket.h> /* 소켓 관련 함수 */
#include <arpa/inet.h>  /* 소켓 지원을 위한 각종 함수 */
#include <unistd.h>     /* 각종 시스템 함수 */

#define MAXLINE     1024

class PStub : public Component, public IStub
{
private:
    struct sockaddr_in serveraddr;
    int server_sockfd;
    int client_len;
    char buf[MAXLINE];
public:
	PStub(int nComponentId = _PStub_Id, const char* sComponentName = _PStub_Name) {

    }
	~PStub() override {

    }

    void Initialize() override {

    }
    void Finalize() override {

    }

    void Start() override {
        // create socket
        if((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            throw Exception((int)(IStub::EException::eSocket));
        }

        // connect
        serveraddr.sin_family = AF_INET;
    //    serveraddr.sin_addr.s_addr = inet_addr("192.168.175.132");
        serveraddr.sin_addr.s_addr = inet_addr("locahost");
        serveraddr.sin_port = htons(10000); 
        /* 서버에 연결을 시도한다. */
        if (connect(server_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1){
            throw Exception((int)(IStub::EException::eConnect));
        }

        // write
        memset(buf, 0x00, MAXLINE);
        char *temp = buf;
        temp = "Test";
        if(write(server_sockfd, buf, MAXLINE) <= 0){
            throw Exception((int)(IStub::EException::eWrite));
        }

        // reply
        memset(buf, 0x00, MAXLINE);
        /* 서버로부터 데이터를 읽는다. */
        if(read(server_sockfd, buf, MAXLINE) <= 0){
            throw Exception((int)(IStub::EException::eRead));
        }
    
        close(server_sockfd);
        printf("read : %s", buf);
    }

    void Stop() override {

    }
};