#pragma once
/*·
//程序定义
程序 → 声明列表
声明列表 → 声明 声明列表 | ε
声明 → 变量声明 | 赋值语句 | if语句 | while语句 | 自增语句 | 自减语句 | 结构体声明 | 结构体变量声明

// 新增：结构体声明
结构体声明 → struct 标识符 { 结构体成员列表 } ;
结构体成员列表 → 结构体成员 结构体成员列表 | ε
结构体成员 → 类型说明符 标识符 ;

// 新增：结构体变量声明
结构体变量声明 → struct 标识符 标识符 (赋值运算符 结构体初始化表达式)? ;
结构体初始化表达式 → { 表达式列表 }
表达式列表 → 表达式 (, 表达式)*

// 变量声明
变量声明 → 类型说明符 标识符 (赋值运算符 表达式)? ;
类型说明符 → int | void | struct 标识符  // 新增结构体类型

// 赋值语句
赋值语句 → 标识符 = 表达式 ;

// 表达式（保持不变）
表达式 → 加法表达式
加法表达式 → 乘法表达式     (+ 乘法表达式 | - 乘法表达式)的*闭包
乘法表达式 → 基本表达式     (* 基本表达式 | / 基本表达式)的*闭包
基本表达式 → 标识符 | 数字 | ( 表达式 )

// if 语句
if语句 → if ( 条件 ) 代码块 (else 代码块)？
条件 → 表达式 关系运算符 表达式
关系运算符 → > | < | == | != | >= | <=

// while 循环
while语句 → while ( 条件 ) 代码块

//自增语句
自增语句 → 标识符 ++ ;

//自减语句
自减语句 → 标识符 -- ;

// 代码块
代码块 → { 声明列表 }

// 终结符（词法单元）
标识符 : [a-z或A-Z或_]  [a-z或A-Z或0-9或_]的*闭包
数字 : [0-9]的+闭包

 */



#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
 

 // 全局变量
int token_index = 0;           // 当前Token索引
char* current_token;           // 当前Token


//函数声明
int is_token_type(const char* token, const char* type);// 精确匹配Token类型
char* lookahead(int n);// 向前查看n个Token
void sync_to_statement_end();// 同步函数：跳过Token直到遇到指定类型或语句结束
void consume();// 消耗当前Token并移动到下一个
void match(const char* token_type);// 匹配特定类型的Token并消耗
void match_prefix(const char* prefix);
void syntax_error(const char* message);// 错误处理


// 解析函数声明（对应文法非终结符）
void parse_program();           // 程序 → 声明列表
void parse_declaration_list();  // 声明列表 → 声明 声明列表 | ε
void parse_declaration();       // 声明 → 变量声明 | 赋值语句 | if语句 | while语句 | 自增语句 | 自减语句 | 结构体声明 | 结构体变量声明
void parse_var_decl();          // 变量声明 → 类型说明符 标识符 (赋值运算符 表达式)? ;
void parse_type_specifier();    // 类型说明符 → int | void
void parse_assign_stmt();       // 赋值语句 → 标识符 = 表达式 ;
void parse_if_stmt();           // if语句 → if语句 → if ( 条件 ) 代码块 (else 代码块)？
void parse_while_stmt();        // while语句 → while ( 条件 ) 代码块
void parse_condition();         // 条件 → 表达式 关系运算符 表达式
void parse_relop();             // 关系运算符 → > | < | == | != | >= | <=
void parse_block();             // 代码块 → { 声明列表 }
void parse_expr();              // 表达式 → 加法表达式
void parse_additive_expr();     // 加法表达式 → 乘法表达式     (+ 乘法表达式 | - 乘法表达式)的*闭包
void parse_multiplicative_expr(); // 乘法表达式 → 基本表达式     (* 基本表达式 | / 基本表达式)的*闭包
void parse_primary_expr();      // 基本表达式 → 标识符 | 数字 | ( 表达式 )
void parse_incr_stmt();         //自增语句 → 标识符++;
void parse_decr_stmt();         //自减语句 → 标识符--;
void parse_struct_decl();         // 结构体声明 → struct 标识符 { 结构体成员列表 } ;
void parse_struct_member_list();  // 结构体成员列表 → 结构体成员 结构体成员列表 | ε
void parse_struct_member();       // 结构体成员 → 类型说明符 标识符 ;
void parse_struct_var_decl();     // 结构体变量声明 → struct 标识符 标识符 (赋值运算符 结构体初始化表达式)? ;
void parse_struct_init_expr();    // 结构体初始化表达式 → { 表达式列表 }
void parse_expr_list();           // 表达式列表 → 表达式 (, 表达式)*


// 精确匹配Token类型（如"(K 5)"）
int is_token_type(const char* token, const char* type) {
    return strcmp(token, type) == 0;
}

// 向后查看n个Token
char* lookahead(int n) {
    if (token_index + n - 1 < Token_count) {
        return Token[token_index + n - 1];
    }
    return NULL; // 到达Token流末尾
}

// 同步函数：跳过Token直到遇到指定类型或语句结束
void sync_to_statement_end() {
    while (current_token &&
        !strstr(current_token, "(P 13)") &&  // 分号
        !strstr(current_token, "(P 16)") &&  // 右花括号
        !strstr(current_token, "(K 5)") &&   // while
        !strstr(current_token, "(K 12)")) {  // if
        consume();
    }
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

// 增强版match函数：提供更精确的匹配和错误恢复
void match(const char* token_type) {
    if (current_token && is_token_type(current_token, token_type)) {
        consume();
    }
    else {
        char error_msg[100];
        sprintf(error_msg, "期望 %s，却发现 %s", token_type, current_token ? current_token : "EOF");
        syntax_error(error_msg);
        sync_to_statement_end(); // 跳过到下一个语句
    }
}

//识别前缀版的match函数
void match_prefix(const char* prefix) {
    if (current_token && strncmp(current_token, prefix, strlen(prefix)) == 0) {
        consume();
    }
    else {
        char error_msg[100];
        sprintf(error_msg, "期望以 %s 开头的Token，却发现 %s", prefix, current_token ? current_token : "EOF");
        syntax_error(error_msg);
        sync_to_statement_end();
    }
}

// 语法错误处理
void syntax_error(const char* message) {
    printf("语法错误: %s\n", message);
    iscorret = 0; // 与词法分析的错误标志统一
}

//// 解析程序：program → 声明列表
//void parse_program() {
//    parse_declaration_list();
//    // 程序结束时应无剩余Token
//    if (token_index < Token_count) {
//        syntax_error("程序末尾存在多余Token");
//    }
//}
void parse_program() {
    printf("开始解析程序，Token总数: %d\n", Token_count);
    for (int i = 0; i < Token_count; i++) {
        printf("Token[%d]: %s\n", i, Token[i]);
    }

    parse_declaration_list();

    printf("解析完成后，当前Token索引: %d\n", token_index);
    if (token_index < Token_count) {
        printf("未处理的Token: ");
        for (int i = token_index; i < Token_count; i++) {
            printf("%s ", Token[i]);
        }
        printf("\n");
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

void parse_declaration() {
    // 1. 处理变量声明（int/void 开头）
    if (strstr(current_token, "(K 1)") || strstr(current_token, "(K 2)")) {
        parse_var_decl();
        return;
    }

    // 2. 处理标识符开头的语句
    if (strncmp(current_token, "(I ", 3) == 0 && current_token[3] != ')') {
        char* next_token = lookahead(2);

        // 2.1 赋值语句（标识符 = 表达式）
        if (next_token && strstr(next_token, "(P 11)")) {
            parse_assign_stmt();
            return;
        }

        // 2.2 自增语句（标识符 ++ ;）
        if (next_token && strstr(next_token, "(P 14)")) {
            parse_incr_stmt();
            return;
        }

        // 2.3 自减语句（标识符 -- ;）
        if (next_token && strstr(next_token, "(P 19)")) {
            parse_decr_stmt();
            return;
        }

        // 2.4 带括号的表达式（可能是条件或函数调用）
        if (next_token && strstr(next_token, "(P 3)")) {
            // 保存当前状态用于错误恢复
            char* prev_token = current_token;
            int prev_index = token_index;

            // 消耗标识符和左括号
            consume();  // 消耗标识符
            consume();  // 消耗左括号

            // 解析括号内的表达式
            parse_expr();

            // 检查是否匹配右括号
            if (current_token && strstr(current_token, "(P 4)")) {
                consume();  // 消耗右括号

                // 检查是否为代码块开头，若是则认为是if/while语句
                if (current_token && strstr(current_token, "(P 15)")) {
                    parse_block();
                    return;
                }

                // 若不是代码块，可能是函数调用或表达式语句
                syntax_error("缺少代码块，可能是未支持的函数调用");
            }
            else {
                syntax_error("括号不匹配");
            }

            // 错误恢复：回退到解析前的状态
            token_index = prev_index;
            current_token = prev_token;
            consume();  // 跳过错误的标识符，继续解析后续内容
            return;
        }

        // 2.5 其他情况：标识符后既非=也非(也非++/--
        syntax_error("标识符后缺少赋值运算符/自增/自减运算符/左括号");
        consume();  // 跳过错误Token
        return;
    }

    // 3. 处理while语句
    if (strstr(current_token, "(K 5)")) {
        parse_while_stmt();
        return;
    }

    // 4. 处理if语句
    if (strstr(current_token, "(K 12)")) {
        parse_if_stmt();
        return;
    }

    // 5. 处理结构体相关声明（结构体声明或结构体变量声明）
    if (current_token && strstr(current_token, "(K 7)")) {
        // 保存当前状态用于回溯
        char* prev_token = current_token;
        int prev_index = token_index;

        // 消耗struct关键字
        consume();

        // 检查是否为结构体声明（struct 标识符 { ... }）
        if (current_token && strstr(current_token, "(I ") && lookahead(2) && strstr(lookahead(2), "(P 15)")) {
            token_index = prev_index;
            current_token = prev_token;
            parse_struct_decl();
            return;
        }

        // 检查是否为结构体变量声明（struct 类型名 变量名）
        else if (current_token && strstr(current_token, "(I ") && lookahead(2) && strstr(lookahead(2), "(I ")) {
            token_index = prev_index;
            current_token = prev_token;
            parse_struct_var_decl();
            return;
        }

        // 都不是，错误恢复
        else {
            token_index = prev_index;
            current_token = prev_token;
            syntax_error("无效的结构体声明或变量声明");
            sync_to_statement_end();
            return;
        }
    }

    // 7. 未识别的声明类型
    syntax_error("缺少声明或类型不支持");
    if (current_token) consume();  // 跳过错误Token
}

// 解析变量声明：var_decl → 类型说明符 标识符 (赋值运算符 表达式)? ;
void parse_var_decl() {
    parse_type_specifier();
    if (current_token && strstr(current_token, "(I ")) {
        consume(); // 消耗标识符
        // 可选的赋值部分
        if (current_token && strstr(current_token, "(P 11)")) { // "="
            consume();
            parse_expr();
        }
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

// 解析类型说明符：type_specifier → 整型 | 空类型 | struct 标识符
void parse_type_specifier() {
    // 支持基础类型和结构体类型
    if (current_token &&
        (strstr(current_token, "(K 1)") ||   // int
            strstr(current_token, "(K 2)") ||   // void
            strstr(current_token, "(K 7)")))    // struct
    {
        // 如果是struct类型，需要额外处理
        if (strstr(current_token, "(K 7)")) {
            consume(); // 消耗"struct"
            match_prefix("(I "); // 消耗结构体类型名
        }
        else {
            consume(); // 消耗基础类型(int/void)
        }
    }
    else {
        syntax_error("缺少类型说明符（int/void/struct）");
    }
}

// 解析赋值语句：assign_stmt → 标识符 赋值运算符 表达式 分号
void parse_assign_stmt() {
    match_prefix("(I "); // 标识符
    match("(P 11)"); // 赋值运算符"="
    parse_expr();//
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
        strstr(current_token, "(P 6)") ||   // "<="
        strstr(current_token, "(P 17") ||   // "!="
        strstr(current_token, "(P 18"))) {  // ">="
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

// 新增：解析自增语句 → 标识符 ++ ;
void parse_incr_stmt() {
    match_prefix("(I ");  // 匹配标识符
    match("(P 14)");     // 匹配"++"
    match("(P 13)");     // 匹配";"
}

// 新增：解析自减语句 → 标识符 -- ;
void parse_decr_stmt() {
    match_prefix("(I ");  // 匹配标识符
    match("(P 19)");     // 匹配"--"
    match("(P 13)");     // 匹配";"
}

// 解析结构体声明：struct_decl → struct 标识符 { 结构体成员列表 } ;
void parse_struct_decl() {
    match("(K 7)");              // 匹配"struct"关键字
    match_prefix("(I ");         // 匹配结构体名称（标识符）
    match("(P 15)");             // 匹配"{"
    parse_struct_member_list();  // 解析结构体成员列表
    match("(P 16)");             // 匹配"}"
    match("(P 13)");             // 匹配";"
}

// 解析结构体成员列表：struct_member_list → 结构体成员 结构体成员列表 | ε
void parse_struct_member_list() {
    if (current_token && (strstr(current_token, "(K 1)") || strstr(current_token, "(K 2)"))) {
        parse_struct_member();
        parse_struct_member_list();
    }
    // 空产生式情况：不做处理
}

// 解析结构体成员：struct_member → 类型说明符 标识符 ;
void parse_struct_member() {
    parse_type_specifier();       // 解析类型说明符（int/void）
    match_prefix("(I ");          // 匹配成员变量名（标识符）
    match("(P 13)");              // 匹配";"
}

// 解析结构体变量声明：struct_var_decl → struct 标识符 标识符 (赋值运算符 结构体初始化表达式)? ;
void parse_struct_var_decl() {
    match("(K 7)");              // 匹配"struct"关键字
    match_prefix("(I ");         // 匹配结构体类型名
    match_prefix("(I ");         // 匹配变量名
    // 可选的初始化部分
    if (current_token && strstr(current_token, "(P 11)")) { // "="
        match("(P 11)");
        parse_struct_init_expr();
    }
    match("(P 13)");             // 匹配";"
}

// 解析结构体初始化表达式：struct_init_expr → { 表达式列表 }
void parse_struct_init_expr() {
    match("(P 15)");             // 匹配"{"
    parse_expr_list();            // 解析表达式列表
    match("(P 16)");             // 匹配"}"
}

// 解析表达式列表：expr_list → 表达式 (, 表达式)*
void parse_expr_list() {
    parse_expr();                 // 解析第一个表达式
    while (current_token && strstr(current_token, "(P 12)")) { // 匹配","
        match("(P 12)");
        parse_expr();             // 解析后续表达式
    }
}