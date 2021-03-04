#include <iostream>
#include <unistd.h>
#include<sys/types.h> 
#include<sys/stat.h> 

bool daemonize()
{
    // create son , close father
    pid_t pid = fork();
    if (pid < 0)
    {
        return false;
    }
    else if( pid > 0 )
    {
        exit( 0 );
    }
    // 设置文件权限掩码。当进程创建新文件(使用open( const char *pathname, 
    // int flags,mode_t mode))系统调用)时，文件权限将是mode & 0777
    umask( 0 );
// 创建新的回话，设置本进程为进程组的首领
    pid_t sid = setsid();
    if ( sid < 0 )
    {
        return false;
    }

// 切换工作目录
    if( (chdir("/")) < 0 )
    {
        return false;
    }
    // 关闭标准输入设备、输出设备和标准错误输出设备
    close( STDIN_FILENO );
    close( STDOUT_FILENO );
    close( STDERR_FILENO );

    // 将标准输入、标准输出和标准错误输入都定向到 /dev/null 文件
    open ("/dev/null", O_RDONLY );
    open ("/dev/null", O_RDWR );
    open ("/dev/null", O_RDWR );
    return 0;
}

int main()
{
    return 0;
}