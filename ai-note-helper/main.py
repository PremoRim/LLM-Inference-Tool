#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
AI智能笔记助手 - 纯后端API服务（简历版）
核心功能：OCR识别 + AI总结 + RAG问答
技术栈：FastAPI + 阿里云API + MySQL + Python
适配：CentOS7 + 清华源
"""
from fastapi import FastAPI, UploadFile, File, HTTPException, Body
from fastapi.middleware.cors import CORSMiddleware
import requests
import pymysql
import uuid
import time

# ===================== 配置（替换为你的信息）=====================
# 阿里云API密钥（OCR/大模型通用）
ALIYUN_API_KEY = "sk-ba52214c095e476fa9ffb33d53b9de11"
# MySQL数据库配置（本地/服务器）
DB_CONFIG = {
    "host": "localhost",       # 数据库地址
    "user": "root",            # 数据库用户名
    "password": "123456", # 数据库密码
    "database": "ai_notes",    # 数据库名（需提前创建）
    "charset": "utf8mb4"
}
# ===================== 初始化FastAPI =====================
app = FastAPI(
    title="AI智能笔记助手API",
    description="基于阿里云OCR+大模型的笔记识别/总结/问答接口（简历版）",
    version="1.0.0"
)

# 解决跨域（前端/Postman调用必备）
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# ===================== 数据库初始化（极简版）=====================
def init_db():
    """初始化数据库（创建notes表）"""
    try:
        # 连接数据库（先手动创建ai_notes库：CREATE DATABASE ai_notes DEFAULT CHARSET utf8mb4;）
        conn = pymysql.connect(**DB_CONFIG)
        cursor = conn.cursor()
        # 创建极简笔记表
        cursor.execute("""
            CREATE TABLE IF NOT EXISTS notes (
                id VARCHAR(36) PRIMARY KEY,
                filename VARCHAR(255),
                ocr_text TEXT,
                summary TEXT,
                create_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
        """)
        conn.commit()
        conn.close()
        print("✅ 数据库初始化成功")
    except Exception as e:
        print(f"❌ 数据库初始化失败：{str(e)}")
        raise e

# 启动时初始化数据库
init_db()

# ===================== 阿里云API调用（核心）=====================
def call_aliyun_ocr(file_content: bytes) -> str:
    """调用阿里云通用OCR识别文本"""
    url = "https://dashscope.aliyuncs.com/api/v1/services/ocr/general-ocr"
    headers = {"Authorization": f"Bearer {ALIYUN_API_KEY}", "Content-Type": "application/json"}
    payload = {
        "model": "general-ocr-v3",
        "input": {"image": file_content.hex()}  # 图片转16进制
    }
    try:
        res = requests.post(url, headers=headers, json=payload, timeout=60)
        if res.status_code != 200:
            raise Exception(f"OCR调用失败：{res.json().get('message')}")
        # 提取识别文本
        ocr_text = "\n".join([item["content"] for item in res.json()["output"]["elements"]])
        return ocr_text.strip()
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"OCR识别失败：{str(e)}")

def call_aliyun_llm(prompt: str) -> str:
    """调用阿里云qwen-turbo大模型"""
    url = "https://dashscope.aliyuncs.com/api/v1/services/aigc/text-generation/generation"
    headers = {"Authorization": f"Bearer {ALIYUN_API_KEY}", "Content-Type": "application/json"}
    payload = {
        "model": "qwen-turbo",
        "input": {"messages": [{"role": "user", "content": prompt}]},
        "parameters": {"max_tokens": 1000, "temperature": 0.3}
    }
    try:
        res = requests.post(url, headers=headers, json=payload, timeout=60)
        if res.status_code != 200:
            raise Exception(f"大模型调用失败：{res.json().get('message')}")
        return res.json()["output"]["text"].strip()
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"大模型生成失败：{str(e)}")

# ===================== 核心API接口 =====================
@app.post("/api/ocr", summary="OCR文本识别")
async def ocr(file: UploadFile = File(...)):
    """上传图片/PDF，返回识别文本+笔记ID"""
    # 生成唯一笔记ID
    note_id = str(uuid.uuid4())
    # 读取文件并调用OCR
    file_content = await file.read()
    ocr_text = call_aliyun_ocr(file_content)
    # 保存到数据库
    conn = pymysql.connect(**DB_CONFIG)
    cursor = conn.cursor()
    cursor.execute("INSERT INTO notes (id, filename, ocr_text) VALUES (%s, %s, %s)",
                   (note_id, file.filename, ocr_text))
    conn.commit()
    conn.close()
    # 返回结果
    return {
        "code": 200,
        "msg": "识别成功",
        "data": {"note_id": note_id, "ocr_text": ocr_text}
    }

@app.post("/api/summary", summary="AI笔记总结（RAG基础）")
async def summary(note_id: str = Body(...)):
    """根据笔记ID生成AI总结"""
    # 从数据库获取OCR文本
    conn = pymysql.connect(**DB_CONFIG)
    cursor = conn.cursor(pymysql.cursors.DictCursor)
    cursor.execute("SELECT ocr_text FROM notes WHERE id = %s", (note_id,))
    note = cursor.fetchone()
    conn.close()
    if not note:
        raise HTTPException(status_code=404, detail="笔记不存在")
    # 构造总结Prompt（RAG核心：只基于原文总结）
    prompt = f"分点总结以下文本，仅基于文本内容，不编造：\n{note['ocr_text']}"
    summary_text = call_aliyun_llm(prompt)
    # 更新数据库
    conn = pymysql.connect(**DB_CONFIG)
    cursor = conn.cursor()
    cursor.execute("UPDATE notes SET summary = %s WHERE id = %s", (summary_text, note_id))
    conn.commit()
    conn.close()
    # 返回结果
    return {
        "code": 200,
        "msg": "总结成功",
        "data": {"note_id": note_id, "summary": summary_text}
    }

@app.post("/api/chat", summary="RAG笔记问答")
async def chat(note_id: str = Body(...), question: str = Body(...)):
    """基于笔记原文回答问题（杜绝幻觉）"""
    # 从数据库获取原文
    conn = pymysql.connect(**DB_CONFIG)
    cursor = conn.cursor(pymysql.cursors.DictCursor)
    cursor.execute("SELECT ocr_text FROM notes WHERE id = %s", (note_id,))
    note = cursor.fetchone()
    conn.close()
    if not note:
        raise HTTPException(status_code=404, detail="笔记不存在")
    # 构造RAG Prompt（核心：限定只基于原文回答）
    prompt = f"""仅基于以下文本回答问题，没有相关内容则说“笔记中未找到相关内容”：
文本：{note['ocr_text']}
问题：{question}"""
    answer = call_aliyun_llm(prompt)
    # 返回结果
    return {
        "code": 200,
        "msg": "问答成功",
        "data": {"answer": answer}
    }

@app.get("/api/health", summary="健康检查")
async def health():
    """接口健康检查（部署后验证用）"""
    return {
        "code": 200,
        "msg": "服务正常运行",
        "data": {"time": time.strftime("%Y-%m-%d %H:%M:%S")}
    }

# ===================== 启动服务 =====================
if __name__ == "__main__":
    import uvicorn
    # 启动服务（0.0.0.0允许外网访问，端口8000）
    uvicorn.run(app="main:app", host="0.0.0.0", port=8000, log_level="info")