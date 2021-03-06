#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<unistd.h>  
#include<sys/types.h>  
#include<sys/stat.h>  
#include<fcntl.h>  
#include<termios.h>  
#include<errno.h>  
 
#define FALSE -1  
#define TRUE 0  
 
int speed_arr[] = { B38400, B19200, B9600, B4800, B2400, B1200, B300,B38400, B19200, B9600, B4800, B2400, B1200, B300, };  
int name_arr[] =  {  38400,  19200,  9600,  4800,  2400,  1200,  300, 38400,  19200,  9600,  4800,  2400,  1200,  300, };  
 
void set_speed(int fd, int speed)
{  
    int   i;   
    int   status;   
    struct termios   Opt;  
    tcgetattr(fd, &Opt);   
    for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++) 
    {   
        if  (speed == name_arr[i]) 
        {       
            tcflush(fd, TCIOFLUSH);       
            cfsetispeed(&Opt, speed_arr[i]);    
            cfsetospeed(&Opt, speed_arr[i]);     
            status = tcsetattr(fd, TCSANOW, &Opt);    
            if  (status != 0) 
            {          
                perror("tcsetattr fd1");    
                return;       
            }      
            tcflush(fd,TCIOFLUSH);     
        }    
    }  
}  
 
int set_Parity(int fd,int databits,int stopbits,int parity)  
{   
    struct termios options;   
    if  ( tcgetattr( fd,&options)  !=  0) 
    {   
        perror("SetupSerial 1");       
        return(FALSE);    
    }  
    options.c_cflag &= ~CSIZE;   
    switch (databits)   
    {     
        case 7:       
            options.c_cflag |= CS7;   
            break;  
        case 8:       
            options.c_cflag |= CS8;  
            break;     
        default:      
            fprintf(stderr,"Unsupported data size\n"); return (FALSE);    
    }  
    switch (parity)   
    {     
        case 'n':  
        case 'N':      
            options.c_cflag &= ~PARENB;   /* Clear parity enable */  
            options.c_iflag &= ~INPCK;     /* Enable parity checking */   
            break;    
        case 'o':     
        case 'O':       
            options.c_cflag |= (PARODD | PARENB);   
            options.c_iflag |= INPCK;             /* Disnable parity checking */   
            break;    
        case 'e':    
        case 'E':     
            options.c_cflag |= PARENB;     /* Enable parity */      
            options.c_cflag &= ~PARODD;      
            options.c_iflag |= INPCK;       /* Disnable parity checking */  
            break;  
        case 'S':   
        case 's':  /*as no parity*/     
            options.c_cflag &= ~PARENB;  
            options.c_cflag &= ~CSTOPB;break;    
        default:     
            fprintf(stderr,"Unsupported parity\n");      
            return (FALSE);    
    }    
    switch (stopbits)  
    {     
        case 1:      
            options.c_cflag &= ~CSTOPB;    
            break;    
        case 2:      
            options.c_cflag |= CSTOPB;    
            break;  
        default:      
            fprintf(stderr,"Unsupported stop bits\n");    
            return (FALSE);   
    }   
    /* Set input parity option */   
    if (parity != 'n')     
        options.c_iflag |= INPCK;   
    tcflush(fd,TCIFLUSH);  
    options.c_cc[VTIME] = 150;   
    options.c_cc[VMIN] = 0; /* Update the options and do it NOW */  
    if (tcsetattr(fd,TCSANOW,&options) != 0)     
    {   
        perror("SetupSerial 3");     
        return (FALSE);    
    }   
    return (TRUE);    
}  
 
int main()  
{  
    int fd;  
    fd = open("/dev/ttySC0",O_RDWR);  
    if(fd == -1)  
    {  
        perror("serialport error\n");  
    }  
    else  
    {  
        printf("open ");  
        printf("%s",ttyname(fd));  
        printf(" succesfully\n");  
    }  
 
    set_speed(fd,115200);  
    if (set_Parity(fd,8,1,'N') == FALSE)  {  
        printf("Set Parity Error\n");  
        exit (0);  
    }  
    char write_buff[] = "we have received the data";
    char read_buff[128];
    char read_result[512];   
    int lenTmp;    
    while(1)  
    {  
        if((lenTmp = read(fd, read_buff, 512))>0)  
        {  
            printf("Len: %d:",lenTmp);  
            read_buff[lenTmp] = '\0';  
            printf("%s \n",read_buff);
            strcat(read_result,read_buff);
            if(lenTmp < 8){
                printf("%s\n",read_result );
                memset(read_result, 0, sizeof(read_result));

                int n= write(fd,write_buff,strlen(write_buff));  
                printf("字符串实际长度%zd \n", strlen(write_buff));
                printf("成功写入%d \n", n);
            }  
        }  
    }  
    close(fd);  
    return 0;  
}