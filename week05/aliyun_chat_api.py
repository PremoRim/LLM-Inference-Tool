#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
阿里云百炼API 2核2G CentOS7适配版（修复choices解析错误）
"""
import requests
import json
import os
import sys

# 强制UTF-8与系统编码适配
os.environ["LC_ALL"] = "en_US.UTF-8"
os.environ["LANG"] = "en_US.UTF-8"

# ========== 核心配置（替换为你的API-KEY） ==========
API_KEY = "sk-ba52214c095e476fa9ffb33d53b9de11"  # 务必替换为有效密钥
API_URL = "https://dashscope.aliyuncs.com/api/v1/services/aigc/text-generation/generation"

# ========== 请求头（鉴权核心） ==========
headers = {
    "Authorization": f"Bearer {API_KEY}",
    "Content-Type": "application/json; charset=utf-8"
}

# ========== 单轮对话（核心学习点：API调用全流程） ==========
def single_chat(question):
    # 学习点1：Prompt工程（明确指令格式）
    payload = {
        "model": "qwen-turbo",
        "input": {
            "messages": [{"role": "user", "content": question}]
        },
        "parameters": {
            "max_tokens": 200,  # 学习点2：参数控制输出长度
            "temperature": 0.6  # 学习点3：温度控制随机性
        }
    }

    try:
        # 学习点4：POST请求与超时处理
        response = requests.post(
            API_URL,
            headers=headers,
            data=json.dumps(payload, ensure_ascii=False).encode("utf-8"),
            timeout=60
        )
        response.encoding = "utf-8"  # 强制UTF-8解析响应
        result = response.json()

        # 学习点5：响应结构解析（核心修复：适配百炼真实返回）
        if response.status_code == 200 and "output" in result:
            # 百炼核心响应路径：result["output"]["text"]
            return result["output"]["text"].strip()
        else:
            return f"调用失败：状态码{response.status_code}，详情{json.dumps(result, ensure_ascii=False)}"

    except KeyError as e:
        return f"解析失败（键缺失）：{str(e)}，响应内容：{response.text if 'response' in locals() else '无响应'}"
    except requests.exceptions.Timeout:
        return "调用超时：云端模型加载中，重试即可"
    except Exception as e:
        return f"未知错误：{str(e)}"

# ========== 多轮对话（学习点6：上下文管理） ==========
chat_history = []
def multi_chat(question):
    # 拼接历史上下文（学习点：多轮对话的Prompt构造）
    messages = []
    for user_msg, ass_msg in chat_history:
        messages.append({"role": "user", "content": user_msg})
        messages.append({"role": "assistant", "content": ass_msg})
    messages.append({"role": "user", "content": question})

    payload = {
        "model": "qwen-turbo",
        "input": {"messages": messages},
        "parameters": {"max_tokens": 200, "temperature": 0.5}
    }

    try:
        response = requests.post(
            API_URL,
            headers=headers,
            data=json.dumps(payload, ensure_ascii=False).encode("utf-8"),
            timeout=60
        )
        response.encoding = "utf-8"
        result = response.json()

        if response.status_code == 200 and "output" in result:
            answer = result["output"]["text"].strip()
            chat_history.append((question, answer))
            return answer
        else:
            return f"多轮调用失败：{response.status_code}，{json.dumps(result, ensure_ascii=False)}"
    except Exception as e:
        return f"多轮对话错误：{str(e)}"

# ========== 测试运行（验证学习成果） ==========
if __name__ == "__main__":
    # 替换为你的有效API-KEY后执行
    print("===== 单轮对话测试（API调用核心） =====")
    q1 = "请用一句话说明AI应用落地中Python后端优化的核心方向"
    print(f"用户问题：{q1}")
    print(f"模型回答：{single_chat(q1)}\n")

    print("===== 多轮对话测试（上下文管理） =====")
    q2 = "其中推理速度优化具体有哪些工程化手段？"
    print(f"用户问题：{q2}")
    print(f"模型回答：{multi_chat(q2)}")
