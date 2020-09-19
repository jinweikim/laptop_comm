#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
using namespace std;



void send_by_socket(string msg){
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("169.254.224.108");
    sockAddr.sin_port = htons(8990);

    if( connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) == -1 ){
        cout << "connect failed" << endl;
    }

    //发送数据
    char send_buff[128]; 
    strcpy(send_buff,msg.c_str());
    int send_len = send(sock,send_buff,static_cast<int>(strlen(send_buff)),0);

    //接收服务器传回的数据
    char szBuffer[MAXBYTE] = {0};
    recv(sock, szBuffer, MAXBYTE, NULL);

    //输出接收到的数据
    cout << "Message form server:" << szBuffer << endl;

    //关闭套接字
    closesocket(sock);

    //终止使用 DLL
    WSACleanup();

    system("pause");
    return 0;
}

int main(){
    string msg = "this is a message from windows";
    send_by_socket(msg);
}

