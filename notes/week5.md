# 第五周学习笔记：Python 速成（AI 工程方向）
**适用**：AI 后端/工程化学习｜可直接提交 GitHub

---

# 1 Python 基础语法（与 C++ 对照）
## 1.1 核心特性
- **动态类型**：不用声明类型，变量类型自动推导
- **缩进语法**：用 4 空格代替 `{}`，缩进错误直接报错
- **无指针/无手动内存管理**：GC 自动回收

## 1.2 基本数据类型
- 数字：`int` / `float`
- 字符串：`str`（单双引号都行）
- 容器：
  - `list`：有序可改 `[1,2,3]` → 类似 C++ 数组/vector
  - `dict`：键值对 `{"k":v}` → 类似 C++ map
  - `tuple`：不可改 `(1,2,3)`
  - `set`：无序不重复

## 1.3 流程控制
```python
# 条件
if a > 10:
    pass
elif a == 10:
    pass
else:
    pass

# 循环
for i in range(1, 101):  # 1~100
    print(i)

while count < 10:
    count += 1
```

---

# 2 函数、异常、文件
## 2.1 函数
- 定义：`def func(参数=默认值):`
- 支持多返回值（返回元组）
- 匿名函数：`lambda x: x+1`

## 2.2 异常处理
```python
try:
    可能出错代码
except FileNotFoundError:
    处理文件不存在
except Exception as e:
    处理其他错误
finally:
    无论如何都会执行
```

## 2.3 文件操作（推荐 with）
```python
with open("a.txt", "r", encoding="utf-8") as f:
    lines = f.readlines()

with open("out.txt", "w") as f:
    f.write("内容")
```

---

# 3 面向对象 OOP
## 3.1 类与对象
```python
class 类名:
    def __init__(self, a, b):  # 构造函数
        self.a = a
        self.b = b

    def method(self):
        return self.a
```

- `self` 等价于 C++ `this`
- 继承：`class 子类(父类):`
- 调用父类：`super().方法()`

## 3.2 与 C++ 对比
- Python 无真正私有，`__变量` 只是改名
- 无析构函数，GC 自动管理
- 多继承天然支持

---

# 4 NumPy（AI 张量基础）
## 4.1 核心作用
- 多维数组/矩阵运算
- 所有深度学习框架基础

## 4.2 常用操作
```python
import numpy as np

a = np.array([[1,2],[3,4]])
a.shape    # 形状
a.reshape(4)  # 重塑
np.mean(a, axis=0)  # 按列均值
a + 1      # 广播运算
```

---

# 5 Pandas + Requests + JSON
## 5.1 Pandas（表格数据）
- `DataFrame` = Excel 表格
- 常用：
  - `pd.read_csv()`
  - `df.head()`
  - `df.loc[条件, 列]`
  - `df.to_csv()`

## 5.2 Requests（网络请求）
```python
resp = requests.get(url)
data = resp.json()
```

## 5.3 JSON
- `json.loads(json_str)` → 字典
- `json.dumps(dict)` → JSON 字符串

---

# 6 PyTorch CPU 基础
## 6.1 张量 Tensor
- 对标 NumPy，用于神经网络计算
- CPU 版直接装：
```bash
pip install torch --index-url https://download.pytorch.org/whl/cpu
```

## 6.2 常用操作
```python
import torch

x = torch.tensor([[1.,2.],[3.,4.]])
x.shape
x.reshape(1,1,2,2)
torch.matmul(x, x)  # 矩阵乘
x.numpy()           # 转回 numpy
```

---

# 7 Hugging Face + Transformers 大模型推理
## 7.1 安装
```bash
pip install transformers accelerate sentencepiece
```

## 7.2 推理流程
1. 加载 `tokenizer`（文本 → token）
2. 加载 `model`
3. 构建 `pipeline`
4. 输入 prompt → 生成结果

## 7.3 Phi-3-mini 关键点
- 轻量 CPU 可跑
- 必须按格式拼 prompt：`<|user|>...<|assistant|>`
- `device_map="cpu"`
