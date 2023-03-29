#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <assert.h>

#define BUFFER_SIZE 1023

int setNonBlocking(int fd)
{
    int oldOption = fcntl(fd, F_GETFL);
    int newOption = oldOption | O_NONBLOCK;
    fcntl(fd, F_SETFL, newOption);
    return oldOption;
}

int nonblock_connect(const char *ip, int port, int time)
{
    int ret = 0;
    sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int fdopt = setNonBlocking(sockfd);

    ret = connect(sockfd, (sockaddr *)&address, sizeof(address));

    if (ret == 0)
    {
        // 连接成功
        printf("Connect to server immediately\n");
        return sockfd;
    }
    if (errno != EINPROGRESS)
    {
        printf("Error occured\n");
        return -1;
    }

    fd_set readfds;
    fd_set writefds;
    timeval timeout;

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(sockfd, &writefds);

    timeout.tv_sec = time;
    timeout.tv_usec = 0;

    ret = select(sockfd + 1, NULL, &writefds, NULL, &timeout);

    if (ret <= 0)
    {
        printf("connection time out\n");
        close(sockfd);
        return -1;
    }

    if (!FD_ISSET(sockfd, &writefds))
    {
        printf("No events on sockfd found\n");
        close(sockfd);
        return -1;
    }

    // 写端就绪，可能是出错了，也可能是真的连接成功
    int error = 0;
    socklen_t length = sizeof(error);
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &length) < 0)
    {
        printf("get socket option failed");
        close(sockfd);
        return -1;
    }
    if (error != 0)
    {
        printf("connection failed after select with error\n");
        close(sockfd);
        return -1;
    }

    // cheng gong
    fcntl(sockfd, F_SETFL, fdopt);
    return sockfd;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    const short port = atoi(argv[2]);

    int sockfd = nonblock_connect(ip, port, 10);
    if (sockfd < 0)
    {
        return 1;
    }
    close(sockfd);
    return 0;
}