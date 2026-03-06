cmake_minimum_required(VERSION 3.10)
project(mysql_login)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 MySQL 库（通过 mysql_config）
find_program(MYSQL_CONFIG NAMES mysql_config PATHS /usr/bin /usr/local/bin)
if (NOT MYSQL_CONFIG)
    message(FATAL_ERROR "mysql_config 未找到，请安装 mariadb-devel 或 mysql-community-devel")
endif()

# 获取编译和链接参数
execute_process(
    COMMAND ${MYSQL_CONFIG} --cflags
    OUTPUT_VARIABLE MYSQL_CFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND ${MYSQL_CONFIG} --libs
    OUTPUT_VARIABLE MYSQL_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# 添加可执行文件
add_executable(mysql_login mysql_login.cpp)

# 设置编译选项和链接库
target_compile_options(mysql_login PRIVATE ${MYSQL_CFLAGS})
target_link_libraries(mysql_login PRIVATE ${MYSQL_LIBS})