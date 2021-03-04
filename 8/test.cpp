#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

int main()
{
    char * temp = {"GET /index.html HTTP/1.1"};
    // cin >> temp;
    char* url = strpbrk( temp, " \t" );
    // cout << "url = " << url << endl;
    /* 如果请求行中没有空白字符或“\t”字符，则HTTP请求必有问题 */
    if ( ! url )
    {
        cout << "badbad";
    }
    // *url++ = '\0';

    char* method = temp;
    if ( strcasecmp( method, "GET" ) == 0 ) /* only support method get */
    {
        printf( "The request method is GET\n" );
    }

    return 0;
}