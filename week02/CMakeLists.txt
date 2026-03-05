cmake_minimum_required(VERSION 3.10)  # 最低CMake版本
project(tcp_socket_demo)              # 项目名

# 设置C语言标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 编译服务端
add_executable(server epoll_server.c)
# 编译客户端
add_executable(client client.c)

# 链接系统网络库（部分Linux系统需要）
target_link_libraries(server PRIVATE pthread)
target_link_libraries(client PRIVATE pthread)