#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
 
int main()
{
    int sockSrv = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addrSrv;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_addr.s_addr =htonl(INADDR_ANY);
    addrSrv.sin_port = htons(8888);
 
    bind(sockSrv, (const struct sockaddr *)&addrSrv, sizeof(struct sockaddr_in));
    listen(sockSrv, 5);
 
    sockaddr_in addrClient;
    socklen_t len=sizeof(addrClient);
 
    getchar();
    int sockConn = accept(sockSrv, (struct sockaddr *)&addrClient, &len);
    printf("accept success\n");
 
    close(sockConn);
    close(sockSrv);
 
    return 0;
}