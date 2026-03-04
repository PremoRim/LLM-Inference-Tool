// src/hello.cpp
// 注释：单行注释，说明文件功能
/*
 * 多行注释：
 * 这是CMake编译的第一个C++程序
 * 功能：打印HelloWorld + 系统信息
 */
#include <iostream>  // 引入输入输出库
#include <string>    // 引入字符串库

// 主函数（程序入口，必须有）
int main() {
    // 定义字符串变量
    std::string msg = "Hello, CMake! (北邮计科大二下)";
    // 打印到控制台
    std::cout << msg << std::endl;
    
    // 打印系统信息（演示变量用法）
    int cpu_core = 2;  // 模拟服务器CPU核心数
    std::cout << "当前服务器CPU核心数：" << cpu_core << std::endl;
    
    return 0;  // 程序正常退出
}
