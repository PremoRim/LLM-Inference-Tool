# 第三周 MySQL 学习笔记（完整版总结）

---

# 一、本周整体内容
**主题**：MySQL 基础 + 实操
**目标**：
- 会安装、登录、初始化 MySQL
- 会建库、建表（用户表 + 聊天记录表）
- 熟练 CRUD（增删改查）
- 掌握联表查询：INNER JOIN / LEFT JOIN
- 会用索引、常用统计函数
- 完成 10 条标准验收 SQL

---

# 二、安装与初始化
## 1. 安装（轻量版，适合 2核2G 服务器）
- CentOS
  ```bash
  sudo yum install -y mariadb-server mariadb
  sudo systemctl start mariadb
  sudo systemctl enable mariadb
  ```
- Ubuntu
  ```bash
  sudo apt update
  sudo apt install -y mysql-server
  sudo systemctl start mysql
  ```

## 2. 安全初始化
```bash
mysql_secure_installation
```
所有选项 **一律 Y**：
- 设置 root 密码
- 禁止 root 远程登录
- 删除匿名用户
- 删除 test 库
- 刷新权限

## 3. 登录
```bash
mysql -u root -p
```

---

# 三、库与表结构（核心）
## 1. 创建数据库
```sql
CREATE DATABASE llm_chat
DEFAULT CHARACTER SET utf8mb4
COLLATE utf8mb4_unicode_ci;

USE llm_chat;
```

## 2. 用户表 user
```sql
CREATE TABLE `user` (
  id INT PRIMARY KEY AUTO_INCREMENT COMMENT '用户ID',
  username VARCHAR(50) NOT NULL UNIQUE COMMENT '用户名',
  password VARCHAR(100) NOT NULL COMMENT '密码',
  create_time DATETIME DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
  status TINYINT DEFAULT 1 COMMENT '1正常 0禁用'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
```

## 3. 聊天记录表 chat_record
```sql
CREATE TABLE chat_record (
  id INT PRIMARY KEY AUTO_INCREMENT COMMENT '记录ID',
  user_id INT NOT NULL COMMENT '关联用户ID',
  prompt TEXT NOT NULL COMMENT '用户提问',
  response TEXT NOT NULL COMMENT '模型回答',
  create_time DATETIME DEFAULT CURRENT_TIMESTAMP COMMENT '时间',
  model_name VARCHAR(50) DEFAULT 'Qwen-1.8B' COMMENT '模型',
  FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
```

---

# 四、CRUD 完整语法

## 1. 新增 INSERT
```sql
INSERT INTO user (username, password)
VALUES ('zhangsan', '123456');

INSERT INTO chat_record (user_id, prompt, response)
VALUES (1, '你好', '你好呀！');
```

## 2. 查询 SELECT
```sql
-- 全表查询
SELECT * FROM user;

-- 指定字段
SELECT username, create_time FROM user;

-- 条件查询
SELECT * FROM user WHERE id = 1;

-- 模糊查询
SELECT * FROM user WHERE username LIKE '%li%';

-- 排序
SELECT * FROM chat_record ORDER BY create_time DESC;

-- 分页
SELECT * FROM chat_record LIMIT 0, 2;
```

## 3. 修改 UPDATE
**必须加 WHERE，否则全表被改**
```sql
UPDATE user
SET password = 'abc123', status = 0
WHERE id = 1;
```

## 4. 删除 DELETE
**必须加 WHERE，否则全表删除**
```sql
DELETE FROM chat_record WHERE id = 2;
```

---

# 五、联表查询（本周重点）

## 1. INNER JOIN（内连接）
只显示**两边都匹配**的数据
```sql
SELECT
  u.username,
  c.prompt,
  c.response
FROM user u
INNER JOIN chat_record c
ON u.id = c.user_id;
```

## 2. LEFT JOIN（左连接）
左表全部显示，右表没有则为 NULL
```sql
SELECT
  u.username,
  IFNULL(c.prompt, '无聊天记录')
FROM user u
LEFT JOIN chat_record c
ON u.id = c.user_id;
```

---

# 六、索引与常用函数

## 1. 索引（加速查询）
```sql
CREATE INDEX idx_chat_user_id ON chat_record(user_id);
```
作用：
- 加快 WHERE / JOIN 速度
- 数据越多效果越明显

## 2. 常用函数
- 统计数量
  ```sql
  SELECT COUNT(*) FROM user;
  ```
- 分组统计
  ```sql
  SELECT u.username, COUNT(c.id)
  FROM user u
  LEFT JOIN chat_record c ON u.id = c.user_id
  GROUP BY u.id;
  ```
- 空值替换
  ```sql
  IFNULL(c.prompt, '无内容')
  ```
- 时间格式化
  ```sql
  DATE_FORMAT(create_time, '%Y-%m-%d %H:%i:%s')
  ```

---

# 七、10 条验收 SQL（完整版）
1. 创建数据库 `llm_chat`
2. 创建 `user` 表
3. 创建 `chat_record` 表
4. 插入 1 条用户数据
5. 插入 3 条聊天记录
6. 修改用户状态
7. 查询所有禁用用户
8. 查询某个用户的所有聊天记录
9. INNER JOIN 查询有聊天的用户
10. LEFT JOIN 查询所有用户及其聊天次数

---

# 八、本周最重要的 5 条规则
1. **UPDATE / DELETE 必须加 WHERE**
2. 字符串用单引号 `' '`
3. 表名/字段名建议加反引号 `` ` ``
4. 联表必须写 `ON` 关联条件
5. 存储中文/表情必须用 `utf8mb4`
