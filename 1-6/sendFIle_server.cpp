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
#include <sys/sendfile.h>

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
    const char *file_name = argv[3];

    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons(port);

    int fiedfd = open( file_name, O_RDONLY );
    assert( fiedfd > 0 );
    struct stat stat_buf;
    fstat( fiedfd, &stat_buf );
    
    int sock = socket( AF_INET, SOCK_STREAM, 0 );
    assert( sock >= 0 );

    int ret = bind( sock, (struct sockaddr*)&address, sizeof(address));
    assert( ret != -1);

    ret = listen( sock, 5 );
    assert( ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept( sock, (struct sockaddr*)&client, &client_addrlength );

    if ( connfd < 0 )
    {
        printf("error");
    }
    else
    {
        sendfile( connfd, fiedfd, NULL, stat_buf.st_size );
        close( connfd );
    }
    close( sock );
    return 0;
}