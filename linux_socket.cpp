#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;
int main(){
    //创建套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("169.254.224.108");  //具体的IP地址
    serv_addr.sin_port = htons(8990);  //端口
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        cout << "Bind error, Port" << endl;
    }
    cout << "Bind successfully" << endl;

    //进入监听状态，等待用户发起请求
    if(listen(serv_sock, 20) == -1){
        cout << "Listen error" << endl;
    }

    cout << "Listening on port[8990]" << endl; 

    //接收客户端请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int conn = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    
    if( conn < 0){
        cout << "error connect" << endl;
    }

    char recvBuf[1024];
    while(1){
        memset(recvBuf, 0, sizeof(recvBuf));
        int len = recv(conn, recvBuf, sizeof(recvBuf), 0);

        // 收到 exit 表示客户端请求断开连接
        if( strcmp(recvBuf, "exit\n") == 0) break;
        cout << "receive message: " <<  recvBuf << endl;

        //向客户端发送数据
        char str[] = "a message from server";
        write(conn, str, sizeof(str));
    }
   
    //关闭套接字
    close(conn);
    close(serv_sock);

    return 0;
}