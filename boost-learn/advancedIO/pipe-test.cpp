#include <iostream>
#include <sys/signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

using namespace std;
int main()
{
    int fd[2];
    int ret = pipe(fd);
    if (ret < 0)
    {
        cout << "Create pipe failed" << endl;
        return 0;
    }
    const char *msg = "hello";
    cout << strlen(msg) << endl;
    int sz = 0;
    while (true)
    {
        write(fd[1], msg, strlen(msg));
        sz += 5;
        cout << sz << endl;
        if (sz > 65536)
            break;
    }
    char buf[512];
    memset(buf, 0, sizeof(buf));
    while((ret = read(fd[0], buf, sizeof(buf) - 1)) != 0){
        cout << buf << endl;
        memset(buf, 0, sizeof(buf));
    }
    cout << ret << endl;
    cout << buf << endl;
    return 0;
}