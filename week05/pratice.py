# 1. 变量与类型
a = 10  # int
b = "AI后端优化"  # str
c = [1, 2, 3]  # list（替代C++数组）
d = {"name": "test", "age": 20}  # dict（替代C++ map）

# 2. 循环与条件（对比C++ for/while）
# Python for循环（遍历） vs C++ for(int i=0; i<10; i++)
for i in range(1, 101):  # 1~100
    if i % 2 == 0:
        print(f"偶数：{i}")
    else:
        continue

# 3. 字符串处理（AI场景常用）
text = "AI应用落地需要Python+MySQL"
print(text.split("+"))  # 分割：['AI应用落地需要Python', 'MySQL']
print(text.replace("需要", "依赖"))  # 替换：AI应用落地依赖Python+MySQL
print(text.lower())  # 小写：ai应用落地需要python+mysql
