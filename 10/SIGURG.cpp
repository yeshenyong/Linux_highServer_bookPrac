#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>

#define BUF_SIZE 1024

static int connfd;

/* SIGURG信号处理函数 */
void sig_urg( int sig )
{
    int save_errno = errno;
    char buffer[ BUF_SIZE ];
    memset( buffer, '\0', sizeof(buffer) );
    int ret = recv( connfd, buffer, BUF_SIZE-1, MSG_OOB);   /* 接收带外数据 */
    printf("got %d bytes of obb data '%s'\n", ret, buffer );
    errno = save_errno;
}

void addsig( int sig, void ( *sig_handler )( int ) )
{
    struct sigaction sa;
    memset( &sa, 0, sizeof( sa ) );
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset( &sa.sa_mask );
    assert( sigaction( sig, &sa, NULL) != -1 );
} 

int main( int argc, char* argv[] )
{
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    int ret = 0;
    struct sockaddr_in address;
    bzero( &address, sizeof(address) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );

    int sock = socket( PF_INET, SOCK_STREAM, 0);
    assert (sock >= 0);
    ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
    if( ret == -1 )
    {
        printf("errno is %d\n", errno );
        return -1;
    }
    ret = listen(sock, 5);
    assert (ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof( client );
    connfd = accept( sock, ( struct sockaddr* )&client, &client_addrlength );

    if( connfd < 0 )
    {
        printf("errno is %d\n",errno);
    }
    else
    {
        addsig( SIGHUP, sig_urg );
        /* 使用SIGURG信号之前，我们必须设置socket的宿主进程或进程组 */
        fcntl( connfd, F_SETOWN, getpid() );

        char buffer[ BUF_SIZE ];
        while( 1 )
        {
            /* 循环接收普通数据 */
            memset( buffer, '\0', sizeof(buffer));
            ret = recv(connfd, buffer, BUF_SIZE-1, 0);
            if( ret <= 0 )
            {
                break;
            }
            printf("got %d bytes of normal data '%s'\n",ret,buffer);
        }
        close(connfd);
    }
    close(sock);
    return 0;
}
