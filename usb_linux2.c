/**********************************************************************
代码说明：使用串口二测试的，发送的数据是字符，
但是没有发送字符串结束符号，所以接收到后，后面加上了结束符号。
我测试使用的是单片机发送数据到第二个串口，测试通过。
**********************************************************************/

#include     <stdio.h>      /*标准输入输出定义*/
#include     <stdlib.h>     /*标准函数库定义*/
#include     <unistd.h>     /*Unix 标准函数定义*/
#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     <fcntl.h>      /*文件控制定义*/
#include     <termios.h>    /*PPSIX 终端控制定义*/
#include     <errno.h>      /*错误号定义*/


#define FALSE  -1
#define TRUE   0
/*********************************************************************/
int OpenDev(char *Dev)
{
    int fd = open( Dev, O_RDWR );         //| O_NOCTTY | O_NDELAY   
    if (-1 == fd)   
    {           
        perror("Can't Open Serial Port");
        return -1;      
    }   
    else    
        return fd;
}
int main(int argc, char **argv){
    int fd;
    int nread;
    char buff[512];
    char *dev  = "/dev/ttyS1"; //串口二
    fd = OpenDev(dev);
    struct termios   Opt;
    tcgetattr(fd, &Opt);
    cfsetispeed(&Opt, B115200);  
    cfsetospeed(&Opt, B115200);  
    // if (set_Parity(fd,8,1,'N') == FALSE)  {
    //     printf("Set Parity Error\n");
    //     exit (0);
    // }
    while (1) //循环读取数据
    {   
        while((nread = read(fd, buff, 512))>0)
        { 
            printf("\nLen %d\n",nread); 
            buff[nread+1] = '\0';   
            printf( "\n%s", buff);   
        }
    }
    //close(fd);  
    // exit (0);
}