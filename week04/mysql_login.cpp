#include <iostream>
#include <cstring>
#include <mysql/mysql.h>

using namespace std;

// 数据库配置（改这里！把密码换成你的 MySQL 密码）
const char* DB_HOST = "127.0.0.1";    // 本地数据库填 127.0.0.1
const char* DB_USER = "root";         // 用户名（默认 root）
const char* DB_PASS = "123456"; // 替换成你的密码！！！
const char* DB_NAME = "llm_chat";     // 数据库名
const unsigned int DB_PORT = 3306;    // 端口（默认 3306）

// 错误处理封装
void print_mysql_error(MYSQL* mysql, const char* msg) {
    cerr << msg << "：" << mysql_error(mysql) << endl;
}

void print_stmt_error(MYSQL_STMT* stmt, const char* msg) {
    cerr << msg << "：" << mysql_stmt_error(stmt) << endl;
}

// 登录函数：验证用户名密码 + 统计登录次数（带事务）
bool user_login(const string& username, const string& password) {
    // 1. 初始化 MySQL 句柄
    MYSQL* mysql = mysql_init(nullptr);
    if (!mysql) {
        cerr << "初始化 MySQL 句柄失败！" << endl;
        return false;
    }

    // 2. 连接数据库
    if (!mysql_real_connect(mysql, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, nullptr, 0)) {
        print_mysql_error(mysql, "连接数据库失败");
        mysql_close(mysql);
        return false;
    }

    // 3. 设置字符编码（避免中文乱码）
    mysql_set_character_set(mysql, "utf8mb4");

    // 4. 开启事务
    if (mysql_query(mysql, "START TRANSACTION") != 0) {
        print_mysql_error(mysql, "开启事务失败");
        mysql_close(mysql);
        return false;
    }

    bool login_success = false;
    MYSQL_STMT* stmt = nullptr;

    try {
        // 5. 预处理：验证用户名密码
        const char* check_sql = "SELECT id FROM user WHERE username = ? AND password = ?";
        stmt = mysql_stmt_init(mysql);
        if (!stmt) {
            throw runtime_error("初始化预处理语句失败");
        }

        if (mysql_stmt_prepare(stmt, check_sql, strlen(check_sql)) != 0) {
            print_stmt_error(stmt, "预处理 SQL 失败");
            throw runtime_error("预处理失败");
        }

        // 绑定参数（用户名 + 密码）
        MYSQL_BIND params[2];
        memset(params, 0, sizeof(params));

        // 参数1：用户名
        params[0].buffer_type = MYSQL_TYPE_STRING;
        params[0].buffer = (char*)username.c_str();
        params[0].buffer_length = username.length();

        // 参数2：密码
        params[1].buffer_type = MYSQL_TYPE_STRING;
        params[1].buffer = (char*)password.c_str();
        params[1].buffer_length = password.length();

        if (mysql_stmt_bind_param(stmt, params) != 0) {
            print_stmt_error(stmt, "绑定参数失败");
            throw runtime_error("绑定参数失败");
        }

        // 执行查询
        if (mysql_stmt_execute(stmt) != 0) {
            print_stmt_error(stmt, "执行查询失败");
            throw runtime_error("执行查询失败");
        }

        // 获取结果（判断是否存在该用户）
        MYSQL_RES* res = mysql_stmt_result_metadata(stmt);
        if (res) {
            MYSQL_ROW row;
            mysql_stmt_store_result(stmt);
            if (mysql_stmt_num_rows(stmt) > 0) {
                login_success = true; // 用户名密码正确

                // 6. 更新登录次数
                const char* update_sql = "UPDATE user SET login_count = login_count + 1 WHERE username = ?";
                MYSQL_STMT* update_stmt = mysql_stmt_init(mysql);
                if (!update_stmt || mysql_stmt_prepare(update_stmt, update_sql, strlen(update_sql)) != 0) {
                    print_stmt_error(update_stmt, "预处理更新 SQL 失败");
                    throw runtime_error("预处理更新失败");
                }

                // 绑定用户名参数
                MYSQL_BIND update_param;
                memset(&update_param, 0, sizeof(update_param));
                update_param.buffer_type = MYSQL_TYPE_STRING;
                update_param.buffer = (char*)username.c_str();
                update_param.buffer_length = username.length();
                
                
                if (mysql_stmt_bind_param(update_stmt, &update_param) != 0 || mysql_stmt_execute(update_stmt) != 0) {
                    print_stmt_error(update_stmt, "更新登录次数失败");
                    mysql_stmt_close(update_stmt);
                    throw runtime_error("更新登录次数失败");
                }

                    // 2. 查询更新后的登录次数
            const char* select_count_sql = "SELECT login_count FROM user WHERE username = ?";
    MYSQL_STMT* count_stmt = mysql_stmt_init(mysql);
    if (!count_stmt || mysql_stmt_prepare(count_stmt, select_count_sql, strlen(select_count_sql)) != 0) {
        print_stmt_error(count_stmt, "预处理查询登录次数失败");
        mysql_stmt_close(count_stmt);
        mysql_stmt_close(update_stmt);
        throw runtime_error("查询登录次数失败");
    }

    // 绑定用户名参数
    MYSQL_BIND count_param;
    memset(&count_param, 0, sizeof(count_param));
    count_param.buffer_type = MYSQL_TYPE_STRING;
    count_param.buffer = (char*)username.c_str();
    count_param.buffer_length = username.length();
    mysql_stmt_bind_param(count_stmt, &count_param);
    mysql_stmt_execute(count_stmt);

    // 绑定结果缓冲区
    int login_count;
    my_bool count_null;
    MYSQL_BIND count_result;
    memset(&count_result, 0, sizeof(count_result));
    count_result.buffer_type = MYSQL_TYPE_INT;
    count_result.buffer = &login_count;
    count_result.is_null = &count_null;
    mysql_stmt_bind_result(count_stmt, &count_result);

    // 读取并打印登录次数
    mysql_stmt_store_result(count_stmt);
    if (mysql_stmt_fetch(count_stmt) == 0 && !count_null) {
        cout << "登录次数更新成功，本次是第 " << login_count << " 次登录" << endl;
    } else {
        cout << "登录次数更新成功，但查询次数失败" << endl;
        }
                
                mysql_stmt_close(update_stmt);
            }
            mysql_free_result(res);
        }
        mysql_stmt_close(stmt);

        // 7. 提交/回滚事务
        if (login_success) {
            if (mysql_query(mysql, "COMMIT") != 0) {
                print_mysql_error(mysql, "提交事务失败");
                login_success = false;
            } else {
                cout << "登录成功！" << endl;
            }
        } else {
            mysql_query(mysql, "ROLLBACK");
            cout << "用户名或密码错误！" << endl;
        }

    } catch (const exception& e) {
        // 异常时回滚事务
        mysql_query(mysql, "ROLLBACK");
        cerr << "登录过程异常：" << e.what() << endl;
        login_success = false;
    }

    // 8. 释放资源
    mysql_close(mysql);
    return login_success;
}

int main() {
    // 测试登录
    string username, password;
    cout << "请输入用户名：";
    cin >> username;
    cout << "请输入密码：";
    cin >> password;

    user_login(username, password);

    return 0;
}