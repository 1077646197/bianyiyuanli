#pragma once
/*·程序定义
程序 → 声明列表
声明列表 → 声明 声明列表 | ε
声明 → 变量声明 | 赋值语句 | if语句 | while语句

// 变量声明
变量声明 → 类型说明符 标识符 分号
类型说明符 → 整型 | 空类型

// 赋值语句
赋值语句 → 标识符 赋值运算符 表达式 分号

// 表达式（保持不变）
表达式 → 加法表达式
加法表达式 → 乘法表达式 (加法运算符 乘法表达式 | 减法运算符 乘法表达式)*
乘法表达式 → 基本表达式 (乘法运算符 基本表达式 | 除法运算符 基本表达式)*
基本表达式 → 标识符 | 数字 | 左括号 表达式 右括号

// if 语句
if语句 → 如果 左括号 条件 右括号 代码块 (否则 代码块)?
条件 → 表达式 关系运算符 表达式
关系运算符 → 大于 | 小于 | 等于 | 不等于 | 大于等于 | 小于等于

// while 循环
while语句 → 当 左括号 条件 右括号 代码块

// 代码块
代码块 → 左花括号 声明列表 右花括号

// 终结符（词法单元）
整型 : "int"
空类型 : "void"
标识符 : [a-zA-Z_][a-zA-Z0-9_]*
数字 : [0-9]+
分号 : ";"
赋值运算符 : "="
加法运算符 : "+"
减法运算符 : "-"
乘法运算符 : "*"
除法运算符 : "/"
左括号 : "("
右括号 : ")"
如果 : "if"
否则 : "else"
当 : "while"
左花括号 : "{"
右花括号 : "}"
大于 : ">"
小于 : "<"
等于 : "=="
不等于 : "!="
大于等于 : ">="
小于等于 : "<="
 */




#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
 // 全局变量
int token_index = 0;           // 当前Token索引
char* current_token;           // 当前Token

// 向前查看n个Token
char* lookahead(int n);
// 消耗当前Token并移动到下一个
void consume();
// 匹配特定类型的Token并消耗
void match(const char* token_type);
// 错误处理
void syntax_error(const char* message);
// 解析函数（对应文法非终结符）
void parse_program();           // 程序 → 声明列表
void parse_declaration_list();  // 声明列表 → 声明 声明列表 | ε
void parse_declaration();       // 声明 → 变量声明 | 赋值语句 | if语句 | while语句
void parse_var_decl();          // 变量声明 → 类型说明符 标识符 分号
void parse_type_specifier();    // 类型说明符 → 整型 | 空类型
void parse_assign_stmt();       // 赋值语句 → 标识符 赋值运算符 表达式 分号
void parse_if_stmt();           // if语句 → 如果 左括号 条件 右括号 代码块 (否则 代码块)?
void parse_while_stmt();        // while语句 → 当 左括号 条件 右括号 代码块
void parse_condition();         // 条件 → 表达式 关系运算符 表达式
void parse_relop();             // 关系运算符 → 大于 | 小于 | 等于 | 不等于 | 大于等于 | 小于等于
void parse_block();             // 代码块 → 左花括号 声明列表 右花括号
void parse_expr();              // 表达式 → 加法表达式
void parse_additive_expr();     // 加法表达式 → 乘法表达式 (加法运算符 乘法表达式 | 减法运算符 乘法表达式)*
void parse_multiplicative_expr(); // 乘法表达式 → 基本表达式 (乘法运算符 基本表达式 | 除法运算符 基本表达式)*
void parse_primary_expr();      // 基本表达式 → 标识符 | 数字 | 左括号 表达式 右括号

char* lookahead(int n) {
    if (token_index + n - 1 < Token_count) {
        return Token[token_index + n - 1];
    }
    return NULL; // 到达Token流末尾
}

// 消耗当前Token并移动到下一个
void consume() {
    if (token_index < Token_count) {
        token_index++;
        current_token = lookahead(1);
    }
    else {
        current_token = NULL;
    }
}

// 匹配特定类型的Token并消耗
void match(const char* token_type) {
    if (current_token && strstr(current_token, token_type)) {
        consume();
    }
    else {
        char error_msg[100];
        sprintf(error_msg, "期望 %s，却发现 %s", token_type, current_token ? current_token : "EOF");
        syntax_error(error_msg);
    }
}


// 语法错误处理
void syntax_error(const char* message) {
    printf("语法错误: %s\n", message);
    iscorret = 0; // 与词法分析的错误标志统一
}

// 解析程序：program → 声明列表
void parse_program() {
    parse_declaration_list();
    // 程序结束时应无剩余Token
    if (token_index < Token_count) {
        syntax_error("程序末尾存在多余Token");
    }
}

// 解析声明列表：declaration_list → 声明 声明列表 | ε
void parse_declaration_list() {
    if (current_token &&
        (strstr(current_token, "(K 1)") ||   // int
            strstr(current_token, "(K 2)") ||   // void
            strstr(current_token, "(I ") ||     // 标识符（赋值语句/if/while）
            strstr(current_token, "(K 12)") ||  // if
            strstr(current_token, "(K 5)"))) {  // while
        parse_declaration();
        parse_declaration_list();
    }
    // 空产生式情况：不做处理
}

// 解析声明：declaration → 变量声明 | 赋值语句 | if语句 | while语句
//void parse_declaration() {
//    if (strstr(current_token, "(K 1)") || strstr(current_token, "(K 2)")) {
//        // 变量声明
//        parse_var_decl();
//    }
//    else if (strncmp(current_token, "(I ", 3) == 0 && current_token[3] != ')') {
//        // 精确匹配标识符 (I name)
//        char* next_token = lookahead(1);
//        if (next_token && strstr(next_token, "(P 11)")) { // 赋值运算符"="
//            parse_assign_stmt();
//        }
//        else {
//            syntax_error("标识符后缺少赋值运算符");
//        }
//    }
//    else if (strstr(current_token, "(K 12)")) { // if
//        parse_if_stmt();
//    }
//    else if (strstr(current_token, "(K 5)")) { // while
//        parse_while_stmt();
//    }
//    else {
//        syntax_error("缺少声明或类型不支持");
//    }
//}
void parse_declaration() {
    if (strstr(current_token, "(K 1)") || strstr(current_token, "(K 2)")) {
        // 变量声明
        parse_var_decl();
    }
    else if (strncmp(current_token, "(I ", 3) == 0 && current_token[3] != ')') {
        // 标识符可能是赋值语句或if/while的条件
        char* next_token = lookahead(1);
        if (next_token && strstr(next_token, "(P 11)")) { // 赋值运算符"="
            parse_assign_stmt();
        }
        else if (next_token && strstr(next_token, "(P 3)")) { // "("
            // 检查是否为if/while的条件
            char* next_next_token = lookahead(2);
            if (next_next_token &&
                (strstr(next_next_token, "(K 12)") || strstr(next_next_token, "(K 5)"))) {
                syntax_error("if/while关键字位置错误");
            }
            else {
                syntax_error("标识符后缺少赋值运算符");
            }
        }
        else {
            syntax_error("标识符后缺少赋值运算符");
        }
    }
    else if (strstr(current_token, "(K 12)")) { // if
        parse_if_stmt();
    }
    else if (strstr(current_token, "(K 5)")) { // while
        parse_while_stmt();
    }
    else {
        syntax_error("缺少声明或类型不支持");
    }
}
// 解析变量声明：var_decl → 类型说明符 标识符 分号
void parse_var_decl() {
    parse_type_specifier();
    if (current_token && strstr(current_token, "(I ")) {
        consume(); // 消耗标识符
        if (current_token && strstr(current_token, "(P 13)")) { // 分号";"
            consume(); // 消耗分号
        }
        else {
            syntax_error("变量声明缺少分号");
        }
    }
    else {
        syntax_error("变量声明缺少标识符");
    }
}

// 解析类型说明符：type_specifier → 整型 | 空类型
void parse_type_specifier() {
    if (current_token && (strstr(current_token, "(K 1)") || strstr(current_token, "(K 2)"))) {
        consume(); // 消耗类型说明符
    }
    else {
        syntax_error("缺少类型说明符");
    }
}

// 解析赋值语句：assign_stmt → 标识符 赋值运算符 表达式 分号
void parse_assign_stmt() {
    match("(I "); // 标识符
    match("(P 11)"); // 赋值运算符"="
    parse_expr();
    match("(P 13)"); // 分号";"
}

// 解析if语句：if_stmt → 如果 左括号 条件 右括号 代码块 (否则 代码块)?
void parse_if_stmt() {
    match("(K 12)"); // "if"
    match("(P 3)"); // "("
    parse_condition();
    match("(P 4)"); // ")"
    parse_block();

    // 可选的else部分
    if (current_token && strstr(current_token, "(K 14)")) { // "else"
        consume();
        parse_block();
    }
}

// 解析while语句：while_stmt → 当 左括号 条件 右括号 代码块
void parse_while_stmt() {
    match("(K 5)"); // "while"
    match("(P 3)"); // "("
    parse_condition();
    match("(P 4)"); // ")"
    parse_block();
}

// 解析条件：condition → 表达式 关系运算符 表达式
void parse_condition() {
    parse_expr();
    parse_relop();
    parse_expr();
}

// 解析关系运算符：relop → 大于 | 小于 | 等于 | 不等于 | 大于等于 | 小于等于
void parse_relop() {
    if (current_token && (strstr(current_token, "(P 10)") ||  // ">"
        strstr(current_token, "(P 7)") ||   // "<"
        strstr(current_token, "(P 5)") ||   // "=="
        strstr(current_token, "(P 11)") ||  // "=" (赋值，需区分)
        strstr(current_token, "(P 6)"))) {  // "<="
        consume();
    }
    else {
        syntax_error("缺少关系运算符");
    }
}

// 解析代码块：block → 左花括号 声明列表 右花括号
void parse_block() {
    match("(P 15)"); // "{"
    parse_declaration_list();
    match("(P 16)"); // "}"
}

// 解析表达式（简化示例，完整实现需分层处理加减乘除）
void parse_expr() {
    parse_additive_expr();
}

//解析加法表达式
void parse_additive_expr() {
    parse_multiplicative_expr();
    while (current_token && (strstr(current_token, "(P 8)") || strstr(current_token, "(P 1)"))) {
        // 加法运算符"+"或减法运算符"-"
        consume();
        parse_multiplicative_expr();
    }
}

// 解析乘法表达式：multiplicative_expr → 基本表达式 (乘法运算符 基本表达式 | 除法运算符 基本表达式)*
void parse_multiplicative_expr() {
    parse_primary_expr();
    while (current_token && (strstr(current_token, "(P 9)") || strstr(current_token, "(P 2)"))) {
        // 乘法运算符"*"或除法运算符"/"
        consume();
        parse_primary_expr();
    }
}

// 解析基本表达式：primary_expr → 标识符 | 数字 | 左括号 表达式 右括号
void parse_primary_expr() {
    if (current_token && strstr(current_token, "(I ")) {
        consume(); // 消耗标识符
    }
    else if (current_token && (strstr(current_token, "(C1 ") || strstr(current_token, "(C2 "))) {
        consume(); // 消耗常量
    }
    else if (current_token && strstr(current_token, "(P 3)")) { // "("
        consume();
        parse_expr();
        match("(P 4)"); // ")"
    }
    else {
        syntax_error("无效的表达式开始");
    }
}
#pragma once
