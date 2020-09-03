#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <asm/termios.h>
 
#define DEV_NAME  "/dev/ttyAMA0"
 
 
int main (int argc, char *argv[])
{
    int fd;
    int len, i,ret;
    char writeBuff[] = "we have received the message";
    char readBuff[256];

 
    fd = open(DEV_NAME, O_RDWR | O_NOCTTY);
    if(fd < 0) {
        perror(DEV_NAME);
        return -1;
    }

    struct termios SerialPortSettings;  /* Create the structure                          */

    tcgetattr(fd, &SerialPortSettings); /* Get the current attributes of the Serial port */

    cfsetispeed(&SerialPortSettings,B115200); /* Set Read  Speed as 115200                      */
    cfsetospeed(&SerialPortSettings,B115200); /* Set Write Speed as 115200                     */

    SerialPortSettings.c_cc[VMIN] = 8; /* Read at least 10 characters */
    
    while(1){

        len = read(fd, readBuff, sizeof(readBuff));
        if (len < 0) {
            printf("read error \n");
            return -1;
        }else{
            printf("%s", readBuff);
            len = write(fd, writeBuff, sizeof(writeBuff));
            if (len < 0) {
                printf("write data error \n");
            }
        }
        
    }
 
    return(0);
}
