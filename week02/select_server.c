#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>

#define PORT 8888
#define MAX_FD 1024  // select最大监控数

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {AF_INET, htons(PORT), INADDR_ANY};
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);

    fd_set read_fds;
    int max_fd = server_fd;

    while (1) {
        // 每次循环都要重新初始化集合（select会修改集合）
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);

        // 内核监控
        select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        // 处理新连接
        if (FD_ISSET(server_fd, &read_fds)) {
            int client_fd = accept(server_fd, 0, 0);
            if (client_fd > max_fd) max_fd = client_fd;
            FD_SET(client_fd, &read_fds);  // 新增客户端fd到集合
            printf("新客户端连接：%d\n", client_fd);
        }

        // 处理客户端数据
        for (int i=server_fd+1; i<=max_fd; i++) {
            if (FD_ISSET(i, &read_fds)) {
                char buf[1024] = {0};
                int len = recv(i, buf, 1024, 0);
                if (len <= 0) {
                    close(i);
                    FD_CLR(i, &read_fds);  // 移除断开的fd
                    printf("客户端断开：%d\n", i);
                } else {
                    printf("收到%d消息：%s\n", i, buf);
                    send(i, buf, len, 0);  // 回显
                }
            }
        }
    }
    close(server_fd);
    return 0;
}