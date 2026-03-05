#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>  // socket核心头文件
#include <netinet/in.h>  // 网络地址结构

#define PORT 8889        // 监听端口
#define BUF_SIZE 1024    // 缓冲区大小

int main() {
   // 1. 创建socket文件描述符（AF_INET=IPv4， SOCK_STREAM=TCP）
   int server_fd = socket(AF_INET, SOCK_STREAM, 0); 
   if (server_fd == -1) {
   	perror("socket创建失败");
	exit(EXIT_FAILURE);
   }

   // 2. 配置服务器地址结构体
   struct sockaddr_in server_addr;
   memset(&server_addr, 0, sizeof(server_addr)); // 初始化内存
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = INADDR_ANY;     // 监听所有网卡
   server_addr.sin_port = htons(PORT);           // 端口转换

   // 3. 绑定socket和地址端口
   if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
      perror("bind绑定失败");
      close(server_fd);
      exit(EXIT_FAILURE);
   }

   // 4. 开始监听（5为最大等待连接数）
   if (listen(server_fd, 5) == -1) {
      perror("listen监听失败");
      close(server_fd);
      exit(EXIT_FAILURE);
   }
   printf("服务器已启动，监听端口 %d...\n", PORT);

   // 5. 接受客户端连接 （阻塞等待）
   struct sockaddr_in client_addr;
   socklen_t client_addr_len = sizeof(client_addr);
   int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
   if (client_fd == -1) {
      perror("accept接受连接失败");
      close(server_fd);
      exit(EXIT_FAILURE);
   }
   printf("客户端已连接! \n");

   // 6. 接受客户端数据
   char buffer[BUF_SIZE] = {0};
   ssize_t recv_len = recv(client_fd, buffer, BUF_SIZE-1, 0);
   if (recv_len == -1) {
      perror("recv接收数据失败");
   } else {
      printf("收到客户端消息：%s\n", buffer);
      // 7. 回复客户端
      const char* reply = "我是服务器，已收到你的消息";
      send(client_fd, reply, strlen(reply), 0);
   }

   // 8. 关闭连接
   close(client_fd);
   close(server_fd);
   return 0;
}
