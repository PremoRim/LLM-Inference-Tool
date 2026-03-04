# 1. 指定CMake最低版本要求（和安装的版本匹配，比如3.10）
# 注释：低于该版本的CMake会报错，避免兼容问题
cmake_minimum_required(VERSION 3.10)

# 2. 定义项目名称（自定义，比如helloworld）
# 注释：项目名仅用于标识，不影响可执行文件名称
project(helloworld)

# 3. 设置C++标准（C++11是基础，兼容所有编译器）
# 注释：强制使用C++11，避免编译器用旧标准导致语法报错
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. 添加可执行文件
# 注释：
# 第一个参数：生成的可执行文件名称（比如hello_app）
# 第二个参数：要编译的源码文件路径（src/hello.cpp）
add_executable(hello_app src/hello.cpp)
