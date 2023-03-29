#include <iostream>
#include <sys/signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

using namespace std;

void printSplitLine()
{
    cout << "-------------------------------------------------\n"
         << endl;
}


int main(int argc, char *argv[])
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // create an IPv4 address
    sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(12345);
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    // inet_pton(AF_INET, ip, &address.sin_addr);
    //  172.21.18.244
    int ret = bind(sock, (sockaddr *)&address, sizeof(address));

    assert(ret != -1);

    ret = listen(sock, 5);
    // 哪怕不accept，也会进入到ESTABLISHED状态
    // sleep(100);

    sockaddr_in client;
    socklen_t client_length = sizeof(client);
    int connfd = accept(sock, (sockaddr *)&client, &client_length);
    if(connfd < 0){
        printf("errno is %d\n", errno);
    }
    else{
        char remote[INET_ADDRSTRLEN];
        printf("connected with ip: %s and port: %d\n", inet_ntop(AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN), ntohs(client.sin_port));
        const int BUFSIZE = 10;
        char buffer[BUFSIZE];
        memset(buffer, '\0', sizeof(buffer));
        int sz;
        while ((sz = recv(connfd, buffer, BUFSIZE - 1, 0)) != 0)
        {
            cout << "Receive " << sz << " bytes." << endl;
            send(connfd, buffer, BUFSIZE - 1, 0);
        }
        close(connfd);
    }
    close(sock);
    return 0;
}