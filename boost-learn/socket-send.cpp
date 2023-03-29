#include <iostream>
#include <sys/signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        printf("Usage: %s ip_address port_number\n", basename(argv[0]));
        return 0;
    }
    const char *ip = argv[1];
    auto port = atoi(argv[2]);

    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_port = htons(port);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);
    if (connect(sockfd, (sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        printf("Connect failed\n");
    }
    else
    {
        const char *oob_data = "abc";
        const char *normal_data = "123";
        send(sockfd, normal_data, strlen(normal_data), 0);
        send(sockfd, normal_data, strlen(normal_data), 0);
        send(sockfd, normal_data, strlen(normal_data), 0);
        // 带外数据会截断正常数据
        send(sockfd, oob_data, strlen(oob_data), MSG_OOB);
        send(sockfd, normal_data, strlen(normal_data), 0);
    }
    close(sockfd);
    return 0;
}