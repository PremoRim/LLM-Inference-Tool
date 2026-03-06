-- ==============================================
-- 第三周 MySQL 学习内容整合脚本
-- 适用场景：LLM-Inference-Tool 项目的数据库初始化
-- 执行方式：登录MySQL后，使用 SOURCE 命令执行
-- ==============================================

-- 1. 创建数据库（不存在则创建）
CREATE DATABASE IF NOT EXISTS llm_chat 
DEFAULT CHARACTER SET utf8mb4 
COLLATE utf8mb4_unicode_ci;

-- 2. 切换到目标数据库
USE llm_chat;

-- 3. 创建用户表（不存在则创建）
CREATE TABLE IF NOT EXISTS `user` (
  `id` INT PRIMARY KEY AUTO_INCREMENT COMMENT '用户ID（主键，自增）',
  `username` VARCHAR(50) NOT NULL UNIQUE COMMENT '用户名（唯一，不可为空）',
  `password` VARCHAR(100) NOT NULL COMMENT '用户密码（不可为空）',
  `create_time` DATETIME DEFAULT CURRENT_TIMESTAMP COMMENT '用户创建时间（自动填充当前时间）',
  `status` TINYINT DEFAULT 1 COMMENT '用户状态：1-正常，0-禁用'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户信息表';

-- 4. 创建聊天记录表（不存在则创建）
CREATE TABLE IF NOT EXISTS `chat_record` (
  `id` INT PRIMARY KEY AUTO_INCREMENT COMMENT '聊天记录ID（主键，自增）',
  `user_id` INT NOT NULL COMMENT '关联的用户ID（外键）',
  `prompt` TEXT NOT NULL COMMENT '用户的提问内容',
  `response` TEXT NOT NULL COMMENT '大模型的回复内容',
  `create_time` DATETIME DEFAULT CURRENT_TIMESTAMP COMMENT '聊天时间（自动填充当前时间）',
  `model_name` VARCHAR(50) DEFAULT 'Qwen-1.8B' COMMENT '本次对话使用的大模型名称',
  -- 外键约束：关联用户表，当用户被删除时，其聊天记录也自动删除
  FOREIGN KEY (`user_id`) REFERENCES `user`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户聊天记录表';

-- 5. 为聊天记录表添加索引（加速联表查询和条件查询）
-- 注意：IF NOT EXISTS 语法在 MySQL 8.0 及以上版本支持
-- 如果你的MySQL版本较低，执行此句可能报错，可手动检查索引是否存在
CREATE INDEX IF NOT EXISTS idx_chat_user_id ON `chat_record`(`user_id`);

-- 6. 插入测试用户数据（避免重复插入）
INSERT IGNORE INTO `user` (`username`, `password`) VALUES
('zhangsan', '123456'),
('lisi', '654321'),
('wangwu', '987654');

-- 7. 插入测试聊天记录数据（避免重复插入）
-- 注意：这里的 user_id 需要与上面插入的用户ID对应
INSERT IGNORE INTO `chat_record` (`user_id`, `prompt`, `response`, `model_name`) VALUES
(1, '你好，大模型！', '你好呀！有什么我可以帮助你的吗？', 'Qwen-1.8B'),
(1, '什么是LLaMA.cpp？', 'LLaMA.cpp是一个轻量级的大语言模型推理框架，由C语言编写，非常适合在资源有限的设备上运行。', 'Qwen-1.8B'),
(2, '如何用MySQL进行联表查询？', '可以使用INNER JOIN或LEFT JOIN语句，例如：SELECT * FROM A JOIN B ON A.id = B.a_id;', 'Phi-2');

-- 8. 基础查询示例（执行后可查看效果）
-- 查看所有用户
-- SELECT * FROM `user`;

-- 查看所有聊天记录
-- SELECT * FROM `chat_record`;

-- 9. 联表查询示例（INNER JOIN）
-- 查询有聊天记录的用户及其对话内容
-- SELECT 
--   u.username AS '用户名',
--   c.prompt AS '用户提问',
--   c.response AS '模型回复'
-- FROM `user` u
-- INNER JOIN `chat_record` c ON u.id = c.user_id;

-- 10. 联表查询示例（LEFT JOIN）
-- 查询所有用户及其聊天记录数（无记录则显示0）
-- SELECT 
--   u.username AS '用户名',
--   COUNT(c.id) AS '聊天次数'
-- FROM `user` u
-- LEFT JOIN `chat_record` c ON u.id = c.user_id
-- GROUP BY u.id;

-- ==============================================
-- 脚本执行完毕
-- 说明：
-- 1. 本脚本可安全重复执行，不会因对象已存在而报错。
-- 2. 所有操作均已添加注释，便于理解和维护。
-- 3. 包含了建库、建表、加索引、插测试数据的完整流程。
-- ==============================================
