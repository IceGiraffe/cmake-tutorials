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

void testIpConvert()
{
    in_addr p;
    // 该函数具有不可重入性
    p.s_addr = inet_addr("192.168.1.1");
    auto res = inet_ntoa(p);
    p.s_addr = inet_addr("192.168.1.2");
    res = inet_ntoa(p);
    cout << res << endl;
    printSplitLine();
    return;
}

static bool stop = false;
static void handle_term(int sig)
{
    stop = true;
}

void testSocket()
{
}

int main(int argc, char *argv[])
{
    signal(SIGTERM, handle_term);
    if (argc <= 3)
    {
        printf("usage: %s ip_address port_number backlog\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    const short port = atoi(argv[2]);
    int backlog = atoi(argv[3]);

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // create an IPv4 address
    sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);
    //  172.21.18.244
    int ret = bind(sock, (sockaddr *)&address, sizeof(address));

    assert(ret != -1);

    ret = listen(sock, backlog);
    // 哪怕不accept，也会进入到ESTABLISHED状态
    while(!stop){
        sleep(1);
    }

    close(sock);
    return 0;
}