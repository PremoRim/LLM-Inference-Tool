第4周 学习笔记（MySQL进阶+工具链综合）

一、本周核心目标

- MySQL：掌握索引、事务、存储过程的核心用法与实操

- 工具链：熟练使用CMake、Git、GDB，打通C+++MySQL全流程

- 项目落地：完成C++连接MySQL的用户登录功能（可运行、可提交GitHub）

二、MySQL进阶（核心重点）

（一）索引基础与优化

1. 三种核心索引

- 主键索引：表默认自带，唯一、非空，InnoDB聚簇索引，查询效率最高

- 唯一索引：字段值唯一（可含NULL），适合用户名、手机号等唯一字段

- 普通索引：无唯一性约束，仅用于加速查询（如chat_record.user_id）

2. 索引核心命令（必记）

-- 1. 创建索引
CREATE UNIQUE INDEX 索引名 ON 表名(字段名); -- 唯一索引
CREATE INDEX 索引名 ON 表名(字段名);       -- 普通索引

-- 2. 查看索引
SHOW INDEX FROM 表名;
SHOW CREATE TABLE 表名; -- 同时查看表结构和索引

-- 3. 删除索引
DROP INDEX 索引名 ON 表名;

3. EXPLAIN执行计划（判断索引是否生效）

EXPLAIN SELECT * FROM 表名 WHERE 条件; -- 分析SQL执行效率

核心字段（重点看）：

- type：查询类型（优先级：const > ref > range > ALL）

- key：实际使用的索引名（NULL表示索引未生效）

- rows：MySQL预估扫描行数（越小越快）

4. 索引优化关键（避坑）

- 最左匹配原则：复合索引(a,b,c)，仅支持 a=?、a=?and b=?、a=?and b=?and c=?

- 索引失效场景（必背）：
        

  - 对索引字段使用函数（如YEAR(create_time)=2026）

  - 隐式类型转换、模糊查询%开头（如LIKE '%张三'）

  - OR两边未全部加索引、使用!=/<>/IS NOT NULL

- 慢查询开启：SET GLOBAL slow_query_log=1; SET GLOBAL long_query_time=1;

（二）事务（ACID+隔离级别）

1. 事务核心命令

-- 开启事务
START TRANSACTION;

-- 执行SQL（如转账、插入等操作）
UPDATE user SET money=money-100 WHERE id=1;
UPDATE user SET money=money+100 WHERE id=2;

-- 提交（永久生效）/回滚（撤销操作）
COMMIT;
-- ROLLBACK;

2. ACID四大特性（必记）

- A（原子性）：事务内操作全成功或全失败，不可拆分

- C（一致性）：事务执行前后，数据完整性不变

- I（隔离性）：多个事务并发执行，互不干扰

- D（持久性）：事务提交后，数据永久保存到磁盘

3. 事务并发问题

- 脏读：读到其他事务未提交的临时数据

- 不可重复读：同一事务内，两次查询结果不一致

- 幻读：同一事务内，范围查询出现新增/减少的记录

4. 四种隔离级别（MySQL默认第三种）

-- 查看隔离级别
SELECT @@transaction_isolation;

-- 设置会话隔离级别（临时生效）
SET SESSION TRANSACTION ISOLATION LEVEL 隔离级别;

- READ UNCOMMITTED：最低级别，允许脏读

- READ COMMITTED：禁止脏读，允许不可重复读

- REPEATABLE READ（默认）：禁止脏读、不可重复读，可防幻读

- SERIALIZABLE：最高级别，串行执行，无并发问题（性能最低）

（三）存储过程

1. 核心语法（创建+调用）

-- 1. 创建存储过程（DELIMITER修改结束符，避免与;冲突）
DELIMITER //
CREATE PROCEDURE 存储过程名(IN 输入参数 类型, OUT 输出参数 类型)
BEGIN
    -- 执行逻辑（如查询、赋值）
    SELECT id INTO 输出参数 FROM user WHERE username=输入参数;
END //
DELIMITER ;

-- 2. 调用存储过程
CALL 存储过程名(输入值, @输出变量);
SELECT @输出变量; -- 查看输出结果

2. 参数类型

- IN：仅输入参数（调用时传入值）

- OUT：仅输出参数（调用后获取结果）

- INOUT：既可以输入，也可以输出

三、工具链综合（CMake+Git+GDB）

（一）CMake（多文件模块化+链接库）

1. 项目目录结构（固定模板）

mysql_login/          # 项目根目录
├── include/          # 头文件目录
│   └── user.h        # 类/函数声明
├── src/              # 源文件目录
│   ├── user.cpp      # 类/函数实现
│   └── main.cpp      # 主函数
├── build/            # 编译目录（避免污染源文件）
└── CMakeLists.txt    # CMake核心配置文件

2. CMakeLists.txt 核心配置（必记）

cmake_minimum_required(VERSION 3.10) # 最低版本要求
project(mysql_login)                # 项目名

set(CMAKE_CXX_STANDARD 11)          # 设置C++标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)        # 包含头文件目录

# 生成静态库（把源文件打包，避免重复编译）
add_library(user_lib STATIC src/user.cpp)

# 生成可执行文件，链接静态库和MySQL库
add_executable(login src/main.cpp)
target_link_libraries(login user_lib mysqlcppconn)

3. 编译运行命令

cd build          # 进入编译目录
cmake ..          # 生成Makefile（..表示上一级目录）
make              # 编译项目
./login           # 运行可执行文件

（二）Git（版本管理+远程提交）

核心命令（必记）

# 1. 标签管理（版本标记）
git tag v0.1          # 创建标签（标记当前版本）
git tag               # 查看所有标签
git push origin v0.1  # 推送标签到远程仓库

# 2. 版本回退
git log --oneline     # 查看提交记录（精简版）
git reset --hard 版本号 # 回退到指定版本
git push -f           # 强制推送回退后的版本（谨慎使用）

# 3. 分支管理
git branch dev        # 创建dev分支（开发分支）
git checkout dev      # 切换到dev分支
git merge dev         # 合并dev分支到当前分支
git push              # 推送分支到远程

（三）GDB（调试C++程序）

核心调试命令（必记）

gdb ./login           # 启动GDB，关联可执行文件
b 行号/函数名         # 设置断点（如b main.cpp:20）
r                    # 运行程序（触发断点）
n                    # 单步执行（不进入函数内部）
s                    # 单步执行（进入函数内部）
p 变量名             # 查看变量值（如p conn）
bt                   # 查看函数调用栈（报错时定位问题）
c                    # 继续执行到下一个断点
q                    # 退出GDB

四、C++连接MySQL（项目核心）

1. 安装依赖库


2. 最小连接代码（核心模板）

#include <mysql_connection.h>
#include <cppconn/driver.h>
using namespace sql; // 引入MySQL命名空间

int main() {
    try {
        // 1. 获取驱动
        Driver* driver = get_driver_instance();
        // 2. 连接数据库（IP:端口，用户名，密码）
        Connection* conn = driver->connect("tcp://127.0.0.1:3306", "root", "你的密码");
        // 3. 选择数据库
        conn->setSchema("llm_chat");
        
        // 后续操作（查询、插入等）
        
        // 4. 释放资源
        delete conn;
    } catch (SQLException &e) {
        // 捕获异常（避免程序崩溃）
        cout << "连接失败：" << e.what() << endl;
        return 1;
    }
    return 0;
}

3. 登录功能核心逻辑

- 接收用户输入的用户名和密码

- 通过C++连接MySQL，执行查询SQL（SELECT * FROM user WHERE username=? AND password=?）

- 判断查询结果，返回登录成功/失败

五、周末复盘与验收

1. 核心知识点复盘

- MySQL：索引（创建、优化、失效）、事务（ACID、隔离级别）、存储过程（参数、调用）

- 工具链：CMake模块化、Git版本管理、GDB调试

- 项目：C++连接MySQL，实现登录功能，全流程跑通

2. 最终验收成果

- MySQL：完整的索引、事务、存储过程实操SQL笔记

- 项目：可运行的C++登录程序（连接MySQL，能验证用户名密码）

- GitHub：上传所有代码（SQL、CMake、C++），编写README.md
