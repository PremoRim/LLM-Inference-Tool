# 第1周学习笔记：Linux 基础操作 + Git 核心实操
## 第1天（2026.3.3）：用户管理 + 文件权限 + Git 仓库管理
### 1. 用户管理核心命令
- useradd 用户名：创建用户
  - 示例：useradd testuser
  - 报错解决：删除用户时提示「进程占用」，需先 kill 进程（kill -9 进程ID）再删除
- passwd 用户名：设置用户密码
  - 示例：passwd testuser（按提示输入密码，终端不显示输入内容为正常）
- usermod -aG 组名 用户名：追加用户到指定组
  - 示例：usermod -aG root testuser（-a=追加，-G=指定组，必加 -a 避免覆盖原有组）
- userdel -r 用户名：删除用户+家目录
  - 示例：userdel -r testuser（-r 避免残留家目录）

### 2. 文件权限核心命令
- ls -l 文件名：查看文件详细权限
  - 示例：ls -l linux_test.txt（输出格式：-rw-r--r-- 1 root root 0 Mar 3 16:00 linux_test.txt）
  - 解读：-rw-r--r-- → 第一位=文件类型（-=文件，d=目录），后9位=所有者/组/其他权限
- chmod 755 文件名：修改文件权限（数字法）
  - 示例：chmod 755 linux_test.txt（7= rwx，5= rx，5= rx → 所有者可读可写可执行，组/其他可读可执行）
- chmod a+x 文件名：修改文件权限（符号法）
  - 示例：chmod a+x linux_test.txt（a=所有用户，+=添加，x=执行权限）
- chown 用户:组 文件名：修改文件所属用户/组
  - 示例：chown testuser:testuser linux_test.txt

### 3. Git 仓库建立+拉取+推送（核心工作流）
| 命令                          | 核心作用                                  | 详细说明 + 示例                                                                 |
|-------------------------------|-------------------------------------------|--------------------------------------------------------------------------------|
| git init                      | 初始化本地 Git 仓库                      | 执行后生成 .git 隐藏文件夹，仅第一次创建仓库时执行；<br>示例：cd /code/LLM-Inference-Tool && git init |
| git remote add origin 仓库地址 | 关联远程 GitHub 仓库                      | origin=远程仓库别名（约定俗成），仓库地址=GitHub 的 SSH/HTTPS 地址；<br>示例：git remote add origin git@github.com:PremoRim/LLM-Inference-Tool.git |
| git remote -v                 | 验证远程仓库关联是否正确                  | 输出拉取/推送地址，确认无错误；<br>示例：git remote -v（输出 origin 对应的仓库地址） |
| git add 文件名/.              | 暂存本地修改                              | . = 暂存当前目录所有修改，提交前必须执行；<br>示例：git add README.md 或 git add . |
| git commit -m "备注信息"      | 提交修改到本地仓库                        | -m=指定备注，备注需简洁明了（规范：init/feat/fix/docs 开头）；<br>示例：git commit -m "init: 初始化仓库" |
| git pull origin main          | 拉取远程最新代码（推送前先拉取）          | 避免本地和远程代码冲突，main=远程分支名（老仓库为 master）；<br>示例：git pull origin main |
| git push -u origin main       | 推送本地修改到远程仓库（第一次推送）      | -u=绑定本地 main 和远程 main，后续只需 git push；<br>示例：git push -u origin main |
| git push                      | 推送本地修改到远程仓库（绑定后）          | 简化版推送，无需重复写 origin main；<br>示例：git push |
| git clone 仓库地址            | 克隆远程仓库到本地（替代 init+remote add） | 一步到位：初始化+关联+拉取；<br>示例：git clone git@github.com:PremoRim/LLM-Inference-Tool.git |

### 4. 关键报错解决
- Git 推送报错「src refspec main does not match any」：本地分支是 master，远程是 main，执行 git branch -M main 重命名后再推送
- Git 权限报错「Permission denied」：检查 GitHub 的 SSH 公钥是否配置正确
- 用户删除报错「user is currently used by process」：用 exit 退出用户会话（而非 su root），或 kill -9 占用进程后再删除
[权限测试：dev_user 可编辑笔记]
