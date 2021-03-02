#include <stdio.h>
#include <netinet/in.h>
#include <bits/socket.h>
#include <arpa/inet.h>
#include <string.h>

void byteorder()
{
    union 
    {
        short value;
        char union_bytes[ sizeof( short ) ];
    }test;
    test.value = 0x0102;
    if ( ( test.union_bytes[0] == 1 ) && (test.union_bytes[1] == 2))
    {
        printf("big endian\n");
    }
    else if( (test.union_bytes[0] == 2 )&& (test.union_bytes[1] == 1))
    {
        printf("little endian\n");
    }
    else
    {
        printf("unknown...\n");
    }
    
}
int main()
{
    byteorder();
    struct in_addr addr1, addr2;
    ulong l1, l2;
    l1 = inet_addr("192.168.0.74");
    l2 = inet_addr("211.100.21.179");
    memcpy(&addr1, &l1, 4);
    memcpy(&addr2, &l2, 4);
    printf("%s : %s\n", inet_ntoa(addr1), inet_ntoa(addr2)); //注意这一句的运行结果   
    printf("%s\n", inet_ntoa(addr1));   
    printf("%s\n", inet_ntoa(addr2));   


    // char* szValue1 = inet_ntoa( "1.2.3.4" );
    // char* szValue2 = inet_ntoa( "10.172.168.1" );
    // printf("address 1: %s\n", szValue1);
    // printf("address 2: %s\n", szValue2);
    return 0;
}