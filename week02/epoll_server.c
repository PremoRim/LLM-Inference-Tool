#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>

#define PORT 8888
#define MAX_EVENTS 10  // 单次最多处理10个事件

int main() {
    // 1. 创建服务端socket（和Day11一样）
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {AF_INET, htons(PORT), INADDR_ANY};
    // 端口复用（避免重启绑定失败）
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);

    // 2. 创建epoll实例
    int epoll_fd = epoll_create(1);
    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;  // 监控读事件
    ev.data.fd = server_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);  // 注册监听fd

    while (1) {
        // 3. 等待事件（阻塞，-1表示无限等）
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i=0; i<n; i++) {
            int fd = events[i].data.fd;

            // 4. 处理新连接
            if (fd == server_fd) {
                int client_fd = accept(server_fd, 0, 0);
                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);  // 注册客户端fd
                printf("新客户端连接：%d\n", client_fd);
            }

            // 5. 处理客户端数据
            else {
                char buf[1024] = {0};
                int len = recv(fd, buf, 1024, 0);
                if (len <= 0) {
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);  // 移除fd
                    close(fd);
                    printf("客户端断开：%d\n", fd);
                } else {
                    printf("收到%d消息：%s\n", fd, buf);
                    send(fd, buf, len, 0);  // 回显
                }
            }
        }
    }

    close(epoll_fd);
    close(server_fd);
    return 0;
}