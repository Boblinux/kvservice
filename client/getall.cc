#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <thread>
 
#define PORT 9981

void getall(int conndfd) {
    std::string message = "getall()";
    int n=send(conndfd, (char*)message.data(), message.size(),0); //将数据发送给服务器端
    if(n >= 0)
    {
        printf("Send %d byte to Server\n",n);
    }
    char buf[2048] = "";
    while((n=recv(conndfd, buf, 2048, 0)) > 0) //接受服务器端 发过来的信息
    {
        printf("%s\n",buf);
    }
    exit(0);
}

int main()
{
    int conndfd;
    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serverAddr.sin_port=htons(PORT);
 
    if(-1==(conndfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))) //客户端 创建
    {
        printf("Create Socket Error\n");
    }
    if(-1==connect(conndfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr))) //客户端建立连接
    {
        printf("Connetc Error\n");
        exit(0);
    }
    //std::thread th_1(&putid_1, conndfd);
    //std::thread th_2(&putid_2, conndfd);
    std::thread th_3(&getall, conndfd);

    //th_1.join();
    //th_2.join();
    th_3.join();

    close(conndfd);
    return 0;
}