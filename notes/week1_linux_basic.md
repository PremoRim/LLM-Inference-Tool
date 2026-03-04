# Week1 Linux 基础 + Git 核心（Day01-Day03）
## Day01：用户管理 + 文件权限 + Git 基础
### 1. 用户与组
- useradd 用户名：创建用户
- passwd 用户名：设置密码
- groupadd 组名：创建组
- usermod -aG 组名 用户名：将用户加入组

### 2. 文件权限
- ls -l：查看权限（rwx 对应读/写/执行）
- chmod 权限 文件：修改权限（如 chmod 755 file.sh）
- chown 用户:组 文件：修改所有者

### 3. Git 基础命令
- git init：初始化仓库
- git add .：暂存所有修改
- git commit -m "备注"：提交
- git remote add origin 仓库地址：绑定远程
- git push -u origin main：首次推送

---

## Day02：文件操作 + find/grep + 管道重定向 + Git 分支
### 1. 文件/目录操作
- mkdir -p 路径：创建目录（-p 递归创建）
- rm -rf 路径：删除（谨慎，-r 递归，-f 强制）
- cp -r 源 目标：复制目录
- mv 源 目标：移动/重命名

### 2. find（找文件）
- 语法：find 路径 -name "文件名"
- 示例：find /code -name "*.md"

### 3. grep（找内容）
- 语法：grep "关键词" 文件名
- 示例：grep "linux" *.txt / grep -n "root" /etc/passwd

### 4. 管道 & 重定向
| 符号 | 作用         | 示例                          |
|------|--------------|-------------------------------|
| >    | 覆盖写入     | ls -l > file_list.txt          |
| >>   | 追加写入     | echo "新内容" >> file_list.txt |
| |    | 管道（连命令）| ls -l | grep txt / ps -ef | grep root |

### 5. Git 分支管理
| 命令                  | 作用           | 示例                          |
|-----------------------|----------------|-------------------------------|
| git branch            | 查看分支       | git branch                    |
| git branch 分支名     | 新建分支       | git branch day2-notes          |
| git checkout 分支名   | 切换分支       | git checkout main              |
| git checkout -b 分支名| 新建并切换     | git checkout -b day2-notes     |
| git merge 分支名      | 合并分支       | git merge day2-notes           |

---

## Day03：进程/系统/网络 + Git 日志回滚（AI 基建核心）
### 1. 进程管理（LLM 推理服务监控）
- ps aux：查看所有进程（a=所有用户，u=用户信息，x=无终端进程）
- top：实时监控（q 退出，P按CPU排序，M按内存排序）
- kill PID：优雅终止进程
- kill -9 PID：强制终止（仅用于无响应进程）
- pkill 进程名：按名称批量终止

### 2. 系统信息查看（服务器运维必备）
- free -h：查看内存（重点看 available 可用内存）
- df -h：查看磁盘（阿里云服务器重点看/根目录和数据盘）
- uptime：系统负载（数值<CPU核心数为正常）
- uname -a：内核版本（适配CUDA/推理框架依赖）

### 3. 网络命令（服务部署与端口排查）
- ip a：查看服务器内网/外网IP
- ping -c 3 域名：测试网络连通性（-c 3 避免持续ping）
- ss -tulnp：查看端口占用（t=TCP、u=UDP、l=监听、n=数字端口、p=关联进程）

### 4. Git 日志与回滚（代码版本管理）
- git log --oneline：精简查看提交历史
- git reset --hard HEAD~1：回滚上一版（谨慎，会丢失未提交修改）

### 5. CMake 
- mkdir build && cd build
- cmake .. CMake上个目录的代码
- make 生成可执行文件
