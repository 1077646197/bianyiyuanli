#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 假设这些变量和函数在词法分析器中已定义
extern char identifiers[100][50];    // 存储标识符的二维数组，每行代表一个标识符
extern int id_count;                 // 标识符计数器，记录当前标识符数量
extern char C1[100][50];             // 存储整型常量的二维数组，每行代表一个整型常量
extern int C1_count;                 // 整型常量计数器
extern char C2[100][50];             // 存储浮点型常量的二维数组
extern int C2_count;                 // 浮点型常量计数器
extern char Token[100][20];          // 存储所有词法单元的二维数组，每个词法单元格式如"(K 1)"
extern int Token_count;              // 词法单元总数
extern int token_index;              // 当前处理的词法单元索引
extern char* current_token;          // 当前词法单元指针
extern char* lookahead(int n);       // 向前查看n个词法单元，用于预读
extern void consume();               // 消耗当前词法单元，指向下一个
extern void match(const char* token_type);  // 匹配并消耗指定类型的词法单元，不匹配则报错

// 符号表结构 - 存储程序中定义的变量信息
typedef struct {
    char name[50];      // 变量名
    int type;           // 类型：1-int, 2-float, 3-char
    int initialized;    // 是否已初始化（0-未初始化，1-已初始化）
    int scope;          // 作用域层级（0-全局，1-第一层块，依此类推）
} Symbol;

void typetrans(int type)
{
    switch (type)
    {
        case 1:
            printf("i\t");break;
        case 2:
            printf("r\t");break;
        case 3:
            printf("c\t");break;
        case 4:
            printf("b\t");break;
        case 5:
            printf("a\t");break;
        case 6:
            printf("d\t");break;
    default:
        break;
    }
}

Symbol symbol_table[100];  // 符号表，最多存储100个符号
int symbol_count = 0;      // 当前符号表中的符号数量
int current_scope = 0;     // 当前作用域层级，0表示全局作用域

// 函数声明
void semantic_error(const char* message);        // 语义错误处理，输出错误信息并终止程序
int find_symbol(const char* name, int scope);    // 在符号表中查找符号，返回索引或-1
void add_symbol(const char* name, int type);     // 向符号表添加符号，检查重复定义
void check_variable(const char* name);           // 检查变量是否存在且已初始化（用于读取操作）
void set_initialized(const char* name);          // 设置变量为已初始化状态
int get_type(const char* name);                  // 获取变量类型，不存在则报错
void analyze_program();                          // 分析整个程序，语义分析入口点
void analyze_declaration_list();                 // 分析声明列表（变量声明、语句等）
void analyze_var_decl();                         // 分析变量声明语句
void analyze_statement();                        // 分析各类语句（赋值、if、while等）
void analyze_assignment();                       // 分析赋值语句
void analyze_if_stmt();                          // 分析if语句
void analyze_while_stmt();                       // 分析while语句
void analyze_block();                            // 分析代码块（由{}包围的语句组）
int check_expression_type();                     // 检查表达式类型，返回类型码


// 语义分析入口 - 初始化并开始分析整个程序
void analyze_program() {
    token_index = 0;               // 重置Token索引
    current_token = lookahead(1);  // 获取第一个Token
    current_scope = 0;             // 从全局作用域开始

    // 解析整个程序
    analyze_declaration_list();

    // 检查是否有未处理的Token
    if (token_index < Token_count) {
        semantic_error("程序末尾有多余Token");
    }

    printf("语义分析成功\n");
}


// 错误处理 - 输出错误信息并终止程序
void semantic_error(const char* message) {
    printf("语义错误: %s\n", message);
    exit(1);  // 终止程序执行
}

// 在符号表中查找变量 - 从当前作用域逐级向上查找
int find_symbol(const char* name, int scope) {
    for (int i = symbol_count - 1; i >= 0; i--) {
        if (strcmp(symbol_table[i].name, name) == 0 &&
            symbol_table[i].scope <= scope) {
            return i;  // 返回符号表索引
        }
    }
    return -1;  // 未找到
}

// 添加变量到符号表 - 检查重复定义并添加新符号
void add_symbol(const char* name, int type) {
    if (find_symbol(name, current_scope) != -1) {
        semantic_error("重复定义的变量");
    }

    Symbol new_symbol;
    strcpy(new_symbol.name, name);
    new_symbol.type = type;
    new_symbol.initialized = 0;  // 初始化为未初始化状态
    new_symbol.scope = current_scope;

    if (symbol_count < 100) {
        symbol_table[symbol_count++] = new_symbol;
    }
    else {
        semantic_error("符号表溢出");
    }
}

// 检查变量是否存在且已初始化 - 用于读取变量值之前的检查
void check_variable(const char* name) {
    int index = find_symbol(name, current_scope);
    if (index == -1) {
        semantic_error("未定义的变量");
    }

    // 对于赋值语句右侧的变量，检查是否已初始化
    // 赋值语句左侧的变量允许未初始化（如int a = 10;）
    if (current_token && strstr(current_token, "(P 11)") == NULL) {  // 非赋值语句左侧
        if (!symbol_table[index].initialized) {
            semantic_error("使用未初始化的变量");
        }
    }
}

// 设置变量为已初始化 - 在变量被赋值后调用
void set_initialized(const char* name) {
    int index = find_symbol(name, current_scope);
    if (index != -1) {
        symbol_table[index].initialized = 1;
    }
}

// 获取变量类型 - 返回变量的类型码，不存在则报错
int get_type(const char* name) {
    int index = find_symbol(name, current_scope);
    if (index == -1) {
        semantic_error("未定义的变量");
    }
    return symbol_table[index].type;
}

// 类型检查：返回表达式类型 - 识别常量或变量的类型
int check_expression_type() {
    if (current_token && strstr(current_token, "(I ")) {
        char name[50];
        sscanf(current_token, "(I %s)", name);
        return get_type(name);  // 变量类型
    }
    else if (current_token && strstr(current_token, "(C1 ")) {
        return 1;  // int类型常量
    }
    else if (current_token && strstr(current_token, "(C2 ")) {
        return 2;  // float类型常量
    }
    else if (current_token && strstr(current_token, "(CT ")) {
        return 3;  // char类型常量
    }
    // 其他情况可扩展...
    return 0;
}

// 分析声明列表 - 处理连续的声明和语句
void analyze_declaration_list() {
    while (current_token && token_index < Token_count) {
        if (strstr(current_token, "(K 1)") || strstr(current_token, "(K 4)")) {
            // 变量声明（int或float）
            analyze_var_decl();
        }
        else if (strstr(current_token, "(I ")) {
            // 赋值语句或函数调用（以标识符开头）
            analyze_statement();
        }
        else if (strstr(current_token, "(K 12)")) {
            // if语句
            analyze_if_stmt();
        }
        else if (strstr(current_token, "(K 5)")) {
            // while语句
            analyze_while_stmt();
        }
        else {
            semantic_error("不支持的语句类型");
        }
    }
}

// 分析变量声明 - 处理类型声明和初始化
void analyze_var_decl() {
    int type;
    if (strstr(current_token, "(K 1)")) {  // int类型
        type = 1;
        consume();  // 消耗类型关键字
    }
    else if (strstr(current_token, "(K 4)")) {  // float类型
        type = 2;
        consume();
    }
    else {
        semantic_error("无效的类型说明符");
    }

    if (current_token && strstr(current_token, "(I ")) {
        int id;
        // 1. 解析Token中的索引
        sscanf(current_token, "(I %d)", &id);

        // 3. 从标识符表获取名称并复制到name数组
        char name[50];
        strcpy(name, identifiers[id - 1]); // id-1是数组索引（从0开始）

        // 4. 添加到符号表并消耗Token
        add_symbol(name, type);
        consume();
    

        // 处理初始化（如果有）
        if (current_token && strstr(current_token, "(P 11)")) {  // 赋值运算符"="
            consume();
            int expr_type = check_expression_type();
            if (expr_type != type && expr_type != 0) {
                semantic_error("类型不匹配");
            }
            set_initialized(name);  // 标记为已初始化
            consume();  // 消耗表达式
        }

        match("(P 13)");  // 消耗分号";"
    }
    else {
        semantic_error("变量声明缺少标识符");
    }
}

// 分析赋值语句 - 检查左值和右值类型匹配
void analyze_assignment() {
    char name[50];
    sscanf(current_token, "(I %s)", name);
    check_variable(name);  // 检查变量是否存在且已初始化（如果是读取操作）
    int var_type = get_type(name);
    consume();  // 消耗左值标识符

    match("(P 11)");  // 消耗赋值运算符"="

    int expr_type = check_expression_type();
    if (expr_type != var_type && expr_type != 0) {
        semantic_error("赋值类型不匹配");
    }

    set_initialized(name);  // 标记为已初始化（即使之前已初始化）
    consume();  // 消耗表达式
    match("(P 13)");  // 消耗分号";"
}

// 分析语句 - 区分不同类型的语句（目前仅支持赋值）
void analyze_statement() {
    if (current_token && strstr(current_token, "(I ")) {
        char* next = lookahead(1);
        if (next && strstr(next, "(P 11)")) {  // 赋值语句（标识符后跟"="）
            analyze_assignment();
        }
        else {
            semantic_error("未知的语句类型");
        }
    }
}

// 分析if语句 - 处理条件表达式和代码块
void analyze_if_stmt() {
    match("(K 12)");  // 消耗"if"关键字
    match("(P 3)");   // 消耗左括号"("

    // 检查条件表达式类型（必须为int类型）
    int cond_type = check_expression_type();
    if (cond_type != 1) {  // 要求条件为int类型（简化处理）
        semantic_error("条件表达式必须为整数类型");
    }
    consume();  // 消耗条件表达式

    match("(P 4)");  // 消耗右括号")"

    // 进入新作用域（if代码块）
    current_scope++;

    // 分析if代码块
    analyze_block();

    // 退出作用域
    current_scope--;

    // 处理else部分（如果有）
    if (current_token && strstr(current_token, "(K 14)")) {
        consume();  // 消耗"else"关键字
        current_scope++;  // 进入else代码块作用域
        analyze_block();
        current_scope--;  // 退出else代码块作用域
    }
}

// 分析while语句 - 处理循环条件和循环体
void analyze_while_stmt() {
    match("(K 5)");  // 消耗"while"关键字
    match("(P 3)");  // 消耗左括号"("

    // 检查条件表达式类型（必须为int类型）
    int cond_type = check_expression_type();
    if (cond_type != 1) {  // 要求条件为int类型
        semantic_error("条件表达式必须为整数类型");
    }
    consume();  // 消耗条件表达式

    match("(P 4)");  // 消耗右括号")"

    // 进入新作用域（循环体）
    current_scope++;

    // 分析循环体
    analyze_block();

    // 退出作用域
    current_scope--;
}

// 分析代码块 - 处理由{}包围的语句组，管理作用域
void analyze_block() {
    match("(P 15)");  // 消耗左花括号"{"

    // 进入新作用域
    current_scope++;

    // 分析块内声明列表
    analyze_declaration_list();

    // 退出作用域
    current_scope--;

    match("(P 16)");  // 消耗右花括号"}"
}
void printsem()
{
    printf("\n符号表内容:\n");
    printf("名称\t类型\t初始化\t作用域\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("%s\t", symbol_table[i].name);
        typetrans(symbol_table[i].type);
        printf("%d\t%d\n", symbol_table[i].initialized, symbol_table[i].scope);
    }
}