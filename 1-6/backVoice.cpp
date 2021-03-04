#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>

#define BUFFER_SIZE 1024
/* define two kinds http static code and static message */
static const char* status_line[2] = {"200 OK","500 Internal server error"};

//client
int main(int argc, char *argv[])
{
    if ( argc < 2 )
    {
        printf("usage: %s ip_address port_number\n", basename( argv[0] ));
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi( argv[2] );

    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons(port);

    int sock = socket( AF_INET, SOCK_STREAM, 0 );
    assert( sock >= 0 );

    int ret = bind( sock, (struct sockaddr*)&address, sizeof(address));
    assert( ret != -1);

    ret = listen( sock, 5 );
    assert( ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept( sock, (struct sockaddr*)&client, &client_addrlength );

    if( connfd < 0 )
    {
        printf(" error ");
    }
    else
    {
        int pipefd[2];
        assert( ret != -1);
        ret = pipe(pipefd); /* create pipe */
        /* let connfd full in client data in pipe */
        ret = splice( connfd, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE  | SPLICE_F_MOVE);
        assert( ret != -1);
        ret = splice( pipefd[0], NULL, connfd, NULL, 32768, SPLICE_F_MORE  | SPLICE_F_MOVE);
        assert( ret != -1);
        close( connfd );
    }
    close( sock );
    return 0;
}