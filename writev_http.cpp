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
    const char *file_name = argv[3];

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

    if ( connfd < 0 )
    {
        printf("error\n");
    }
    else
    {
        /* use for save http answer static row | headers and a space row buffer */
        char header_buf[ BUFFER_SIZE ];
        memset( header_buf, 0, BUFFER_SIZE );

        /* use for save target file content's app buffer */
        char *file_buf;

        /* use for get target file shuxing.such as file size and mulu */
        struct stat file_stat;

        /* use for sign target if or not successfully file */
        bool valid = true;

        /* buffer_area header_buf use how size space */
        int len = 0;
        
        //no exits
        if( (stat( file_name, &file_stat )) < 0 )
        {
            valid = false;
        }
        else
        {
            if ( S_ISDIR( file_stat.st_mode ) ) //is director
            {
                valid = false;
            }
            else if (file_stat.st_mode & S_IROTH)   //have read root
            {
                // 动态分配缓冲区 file_buf，并指定其大小为目标文件的大小file_stat.st_size+1，然后将目标文件读入缓冲区file_buf
                int fd = open( file_name, O_RDONLY );
                file_buf = new char [ file_stat.st_size + 1];
                memset( file_buf, 0, file_stat.st_size + 1);
                if( read( fd, file_buf, file_stat.st_size) < 0 )
                {
                    valid = false;
                }
            }
            else
            {
                valid = false;
            }
        }
        if( valid )
        {
// 将可变个参数(...)按照format格式化成字符串，然后将其复制到str中。
// (1) 如果格式化后的字符串长度 < size，则将此字符串全部复制到str中，并给其后添加一个字符串结束符('\0')；
// (2) 如果格式化后的字符串长度 >= size，则只将其中的(size-1)个字符复制到str中，并给其后添加一个字符串结束符('\0')，返回值为欲写入的字符串长度。
            // 下面这部分内容将http应答的状态行，“content-length” 头部字段和一个空行依次加入header_buf中
            ret = snprintf( header_buf, BUFFER_SIZE-1, "%s %s\r\n", "HTTP/1.1", status_line[0] );
            len += ret;
            ret = snprintf( header_buf+len, BUFFER_SIZE-1-len, "Content-Length: %d\r\n", file_stat.st_size);
            len +=ret;
            ret = snprintf( header_buf+len, BUFFER_SIZE-1-len, "%s", "\r\n");
// 利用writev 将header_buf和file_buf的内容一并写入
            struct iovec iv[2];
            iv[ 0 ].iov_base = header_buf;
            iv[ 0 ].iov_len = strlen( header_buf );
            iv[ 1 ].iov_base = file_buf;
            iv[ 1 ].iov_len = strlen(file_buf);
            ret = writev( connfd, iv, 2 );
        }
        else
        {
            ret = snprintf( header_buf, BUFFER_SIZE - 1, "%s %s\r\n", "HTTP/1.1", status_line[1]);
            len += ret;
            ret = snprintf( header_buf+len, BUFFER_SIZE-1-len, "%s", "\r\n");
            send(connfd, header_buf, strlen(header_buf), 0);
        }
        close( connfd );
        delete[] file_buf;
    }
    close( sock );
    return 0;
}