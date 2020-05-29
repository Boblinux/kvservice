#include <stdio.h>
#include <stdlib.h>
#include <string.h>						
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>					

#include <iostream> // std::cout
#include <thread>   // std::thread
#include <string>
#include <map>

std::map<std::string, std::string> kv_map;

void SocketTask() {
    std::cout << "SocketTask start" << std::endl;

    exit(0);
}

void StringConvertTask(int socketfd) {
    std::cout << "StringConvertTask start" << std::endl;
    int recv_len = 0;
	char recv_buf[1024] = "";	// 接收缓冲区
	int connfd = (int)socketfd; // 传过来的已连接套接字
 
	// 接收数据
	while((recv_len = recv(connfd, recv_buf, sizeof(recv_buf), 0)) > 0)
	{
		//printf("recv_buf: %s\n", recv_buf); // 打印数据
        std::string recvstring = recv_buf;
        std::string key = "";
        std::string value = "";
        int len = recvstring.size();
        for(int i=0; i<len && recvstring[i] != '\n'; i++) {
            if(recvstring[i] != ':') key += recvstring[i];
            else {
                value = recvstring.substr(i+1,len-i);
                break;
            }
        }
        if(value != "") {
            kv_map.insert(std::pair<std::string, std::string>(key, value));
			//map 遍历
			std::cout << "map begin..." << std::endl;
			std::map<std::string, std::string>::iterator iter;
			iter = kv_map.begin();
			while(iter != kv_map.end()) {
				std::cout << iter->first << " : " << iter->second << std::endl;
				iter++;
			}

        } else {
			std::cout << "put begin..." << std::endl;
			std::cout << "key: " << key << "value: " << kv_map[key] << std::endl;
            std::string temp = kv_map[key];
			std::cout << "temp: " << temp << std::endl;
            char sendvalue[1024] = "";
            int j=0;
            for(; j<temp.size(); j++) sendvalue[j] = temp[j];
            sendvalue[j] = '\0';
			std::cout << "sendvalue: " << sendvalue << std::endl;

            send(connfd, sendvalue, 1024, 0); // 给客户端回数据
        }
		//send(connfd, recv_buf, recv_len, 0); // 给客户端回数据
	}
	
	printf("client closed!\n");
	close(connfd);	//关闭已连接套接字

    exit(0);
}

void kvPutTask() {
    std::cout << "kvPutTask start" << std::endl;

    exit(0);
}

void kvGetTask() {
    std::cout << "kvGetTask start" << std::endl;

    exit(0);
}

/*
 * ===  FUNCTION  =========================================================
 *         Name:  main
 *  Description:  program entry routine.
 * ========================================================================
 */
int main(int argc, const char *argv[]) {
	int sockfd = 0;				// 套接字
	int connfd = 0;
	int err_log = 0;
	struct sockaddr_in my_addr;	// 服务器地址结构体
	unsigned short port = 8080; // 监听端口
	pthread_t thread_id;
	
	printf("TCP Server Started at port %d!\n", port);
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);   // 创建TCP套接字
	if(sockfd < 0)
	{
		perror("socket error");
		exit(-1);
	}
	
	bzero(&my_addr, sizeof(my_addr));	   // 初始化服务器地址
	my_addr.sin_family = AF_INET;
	my_addr.sin_port   = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	printf("Binding server to port %d\n", port);
	
	// 绑定
	err_log = bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));
	if(err_log != 0)
	{
		perror("bind");
		close(sockfd);		
		exit(-1);
	}
	
	// 监听，套接字变被动
	err_log = listen(sockfd, 10);
	if( err_log != 0)
	{
		perror("listen");
		close(sockfd);		
		exit(-1);
	}
	
	printf("Waiting client...\n");

    for(;;) {
        char cli_ip[INET_ADDRSTRLEN] = "";	   // 用于保存客户端IP地址
		struct sockaddr_in client_addr;		   // 用于保存客户端地址
		socklen_t cliaddr_len = sizeof(client_addr);   // 必须初始化!!!
		
		//获得一个已经建立的连接	
		connfd = accept(sockfd, (struct sockaddr*)&client_addr, &cliaddr_len);   							
		if(connfd < 0)
		{
			perror("accept this time");
			continue;
		}
		
		// 打印客户端的 ip 和端口
		inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
		printf("----------------------------------------------\n");
		printf("client ip=%s,port=%d\n", cli_ip,ntohs(client_addr.sin_port));
		
		if(connfd > 0)
		{
			//std::thread sockettask(SocketTask);
            //sockettask.detach();
            //string str = "";
            std::thread stringconverttask(StringConvertTask, connfd);
            stringconverttask.detach();

            //std::thread kvputtask(kvPutTask);
            //kvputtask.detach();

            //std::thread kvgettask(kvGetTask);
            //kvgettask.detach();
		}

    }
    
    return EXIT_SUCCESS;
}  /* ----------  end of function main  ---------- */