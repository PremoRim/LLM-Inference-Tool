cmake_minimum_required(VERSION 3.10)
project(mysql_login)

set(CMAKE_CXX_STANDARD 11)
include_directories(include)

add_library(user_lib STATIC src/user.cpp)
add_executable(login src/main.cpp)
target_link_libraries(login user_lib)
