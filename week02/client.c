#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // inet_addr头文件

#define SERVER_PORT 8888
#define SERVER_IP "127.0.0.1"  // 本地回环地址
#define BUF_SIZE 1024

int main() {
    // 1. 创建socket
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket创建失败");
        exit(EXIT_FAILURE);
    }

    // 2. 配置服务器地址
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);  // IP转换

    // 3. 连接服务器
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect连接服务器失败");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    printf("已连接到服务器 %s:%d\n", SERVER_IP, SERVER_PORT);

    // 4. 发送消息给服务器
    const char* msg = "你好，服务器！我是客户端";
    send(client_fd, msg, strlen(msg), 0);
    printf("已发送消息：%s\n", msg);

    // 5. 接收服务器回复
    char buffer[BUF_SIZE] = {0};
    ssize_t recv_len = recv(client_fd, buffer, BUF_SIZE - 1, 0);
    if (recv_len == -1) {
        perror("recv接收回复失败");
    } else {
        printf("收到服务器回复：%s\n", buffer);
    }
    while(1){
        sleep(1);
    }
    // 6. 关闭连接
    close(client_fd);
    return 0;
}