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
    char *dev  = "/dev/ttyAMA0"; //串口二
    fd = OpenDev(dev);
    struct termios   Opt;
    tcgetattr(fd, &Opt);
    cfsetispeed(&Opt, B115200);  
    cfsetospeed(&Opt, B115200);  
    // if (set_Parity(fd,8,1,'N') == FALSE)  {
    //     printf("Set Parity Error\n");
    //     exit (0);
    // }
    fd_set fs_read;
    FD_ZERO(&fs_read);
    FD_SET(fd,&fs_read);
    struct timeval time;
    time.tv_sec = 10;
    time.tv_usec = 0;
    int len;
    char rcv_buf[100];
     char rcv_data[100];
    while (1) //循环读取数据
    {   
        static int count = 0;
        if(select(fd+1,&fs_read,NULL,NULL,&time)>0){
            len = read(fd,rcv_data,sizeof(rcv_data));
            if(len==8)
            {
                strncpy(rcv_buf+count,rcv_data,8);
                count+=8;

            }
            if(len>0&&len<8)
            {
                strncpy(rcv_buf+count,rcv_data,len);
                count+=len;
                printf("count=%d\n",count);
                for(int i=0;i<count;i++){
                    printf("%c",rcv_buf[i]);
                }
                len=count;
                count=0;
                
            }  
        }   
        
    }
    close(fd);  
    exit (0);
}