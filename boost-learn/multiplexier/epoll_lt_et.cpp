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

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10

int setNonBlocking(int fd)
{
    int oldOption = fcntl(fd, F_GETFL);
    int newOption = oldOption | O_NONBLOCK;
    fcntl(fd, F_SETFL, newOption);
    return oldOption;
}

void addfd(int epollfd, int fd, bool enable_et)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    if (enable_et)
    {
        event.events |= EPOLLET;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setNonBlocking(fd);
    return;
}

void lt(epoll_event *events, int number, int epollfd, int listenfd)
{
    char buf[BUFFER_SIZE];
    for (int i = 0; i < number; ++i)
    {
        int sockfd = events[i].data.fd;
        // 新连接
        if (sockfd == listenfd)
        {
            sockaddr_in client_address;
            socklen_t client_len = sizeof(client_address);
            int connfd = accept(listenfd, (sockaddr *)&client_address, &client_len);
            addfd(epollfd, connfd, false);
        }
        // 有新数据
        else if (events[i].events & EPOLLIN)
        {
            printf("event trigger once\n");
            memset(buf, 0, BUFFER_SIZE);
            int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
            if (ret <= 0)
            {
                close(sockfd);
                printf("Close fd\n");
                continue;
            }
            printf("get %d bytes in lt mode\n", ret);
        }
        else
        {
            printf("something else happened\n");
        }
    }
}

void et(epoll_event *events, int number, int epollfd, int listenfd)
{
    char buf[BUFFER_SIZE];
    for (int i = 0; i < number; ++i)
    {
        int sockfd = events[i].data.fd;
        // 新连接
        if (sockfd == listenfd)
        {
            sockaddr_in client_address;
            socklen_t client_len = sizeof(client_address);
            int connfd = accept(listenfd, (sockaddr *)&client_address, &client_len);
            addfd(epollfd, connfd, true);
        }
        // 有新数据
        else if (events[i].events & EPOLLIN)
        {
            printf("event trigger once\n");
            // 这部分代码只会被触发一次，因此必须循环读取数据
            while (true)
            {
                memset(buf, 0, BUFFER_SIZE);
                int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
                if (ret < 0)
                {
                    // 对于非阻塞IO，需要判断errno
                    if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
                    {
                        printf("read again\n");
                        break;
                    }
                    close(sockfd);
                    printf("Close fd\n");
                    break;
                }
                else if (ret == 0)
                {
                    close(sockfd);
                    printf("Close fd\n");
                }
                else
                {
                    printf("get %d bytes in et mode\n", ret);
                }
            }
        }
        else
        {
            printf("something else happened\n");
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    const short port = atoi(argv[2]);
    const short mode = atoi(argv[3]);
    int ret = 0;
    sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    ret = bind(listenfd, (sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);

    // 每个使用 ET 模式的丈件描述符都应该是非阻宏的，如果文件描述符是阻宸的，那么读或写操作将会因为没有后续的事件而一直处于限塞状态（饥渴状态）。
    addfd(epollfd, listenfd, true);

    while (true)
    {
        int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if (ret < 0)
        {
            printf("Epoll failure\n");
            return 1;
        }
        if (mode)
            lt(events, ret, epollfd, listenfd);
        else
            et(events, ret, epollfd, listenfd);
    }

    close(listenfd);
    return 0;
}