# 第二周（Day8-Day14）核心笔记（浓缩版）
## 核心主题：Linux进阶 + 网络编程 + Shell脚本进阶
### 一、Day8：Linux文件IO + CMake多文件编译
#### 1. 文件IO核心命令（重定向/管道）
| 符号/命令 | 作用 | 示例 |
|-----------|------|------|
| `>` | 覆盖输出到文件 | `ls -l > out.txt` |
| `>>` | 追加输出到文件 | `ls -l >> out.txt` |
| `2>` | 重定向错误输出 | `ls /xxx 2> err.txt` |
| `&>` | 重定向所有输出 | `ls /xxx &> all.log` |
| `|` | 管道（前命令输出作为后命令输入） | `ps -ef | grep llm` |
| `tee` | 同时输出到屏幕+文件 | `echo "hello" | tee log.txt` |
| `xargs` | 批量执行命令 | `find . -name "*.log" | xargs rm` |

#### 2. CMake多文件项目核心
```cmake
cmake_minimum_required(VERSION 3.10)
project(xxx)
add_executable(main main.c func.c)  # 多文件编译
```

### 二、Day9：进程管理高级 + GDB调试
#### 1. 进程管理高频命令
| 命令 | 作用 |
|------|------|
| `ps aux` | 查看所有进程（含资源占用） |
| `pgrep 进程名` | 只输出进程PID |
| `pkill 进程名`/`killall 进程名` | 批量终止进程 |
| `nohup 命令 &` | 后台运行进程（断开终端不终止） |
| `tail -f 日志文件` | 实时查看日志 |
| `jobs`/`fg`/`bg` | 查看/调回前台/调后台任务 |

#### 2. GDB调试5个核心命令
- `b 行号/函数名`：打断点
- `r`：运行程序
- `n`：单步执行
- `p 变量名`：查看变量值
- `bt`：查看调用栈

### 三、Day10：Linux网络基础命令
| 命令 | 作用 |
|------|------|
| `ip a` | 查看网卡/IP信息 |
| `ss -tulnp` | 查看监听端口（含进程） |
| `lsof -i:端口号` | 查看指定端口占用进程 |
| `ping 地址` | 测试网络连通性 |
| `telnet 地址 端口` | 测试端口是否可连接 |
| `curl 网址` | 模拟HTTP请求 |

#### Git分支核心操作
- `git checkout -b dev`：新建并切换分支
- `git merge dev`：合并dev分支到当前分支
- `git log`/`git diff`：查看提交日志/文件差异

### 四、Day11：TCP Socket编程（C语言）
#### 1. 服务端核心流程（8步）
```
socket() → 配置地址结构体 → bind() → listen() → accept() → recv()/send() → close()
```

#### 2. 客户端核心流程（6步）
```
socket() → 配置服务器地址 → connect() → send()/recv() → close()
```

#### 3. 核心函数易错点
- `htons(端口)`：端口需转网络字节序
- `INADDR_ANY`：监听所有网卡IP
- `accept()`：阻塞函数，需先启动服务端

### 五、Day12：IO模型 + select多路复用
#### 1. 三种IO模型核心区别
- 阻塞IO：默认模式，调用recv/send/accept会等数据
- 非阻塞IO：用`fcntl(fd, F_SETFL, O_NONBLOCK)`设置，无数据立即返回
- 多路复用（select）：单线程监控多个文件描述符，有事件才处理

#### 2. select核心逻辑
```c
fd_set readfds;
FD_ZERO(&readfds);
FD_SET(sockfd, &readfds);
select(最大fd+1, &readfds, NULL, NULL, NULL);  // 监控读事件
if (FD_ISSET(fd, &readfds)) {  // 判断fd是否有事件
    // 处理连接/数据
}
```

### 六、Day13：epoll多路复用（高性能）
#### 1. epoll核心函数
- `epoll_create()`：创建epoll实例
- `epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev)`：添加/修改/删除监控fd
- `epoll_wait(epfd, events, 最大事件数, 超时时间)`：等待事件

#### 2. epoll vs select核心优势
- select：最大监控fd有限（1024），每次需遍历所有fd
- epoll：无fd数量限制，只遍历有事件的fd，高并发更高效
- LT模式（默认）：事件未处理会重复触发；ET模式：只触发一次（需一次性读完数据）

### 七、Day14：第二周复盘 + 服务器监控Shell脚本
#### 1. 实用监控脚本核心功能
```bash
#!/bin/bash
# 1. 监控CPU/内存/磁盘
echo "=== 资源监控 ==="
top -bn1 | head -5  # CPU信息
free -h             # 内存信息
df -h               # 磁盘信息

# 2. 监控指定端口
echo "=== 端口监控 ==="
ss -tulnp | grep 8888

# 3. 输出到日志
exec &> server_monitor.log
```

### 关键点回顾
1. Linux重定向/管道是处理文本/日志的核心，`tee`可同时输出到屏幕和文件；
2. TCP Socket是网络编程基础，epoll是高并发服务的核心方案；
3. 进程/端口监控是服务器运维必备，Shell脚本可批量实现监控逻辑。