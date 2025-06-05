#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void semantic_error(const char* message);        // 输出语义错误信息并终止程序
int find_symbol(const char* name, int scope);    // 在符号表中查找变量（返回索引或-1）
void add_symbol(const char* name, int type);     // 添加新变量到符号表（检查重复）
void check_variable(const char* name);           // 检查变量是否存在且已初始化
void set_initialized(const char* name);          // 标记变量为已初始化状态
int get_type(const char* name);                  // 获取变量的类型码
int check_expression_type();                     // 检查表达式类型（返回类型码）
void generate_quad(const char* op, const char* arg1, const char* arg2, const char* result); // 生成四元式
void generate_assign_quad(const char* src, const char* dest); // 生成赋值四元式（=）
void generate_if_quad(const char* cond, const char* target); // 生成条件跳转四元式（if）
void generate_goto_quad(const char* target);     // 生成无条件跳转四元式（goto）
void enter_scope();                              // 进入新作用域（增加作用域层级）
void exit_scope();                               // 退出当前作用域（减少作用域层级）
void add_activation_record(const char* name, int type); // 添加变量到活动记录表
int add_constant(const char* value, int type);   // 添加常量到常数表（去重）
char* get_constant_quad(const char* value, int type); // 获取常量的四元式表示（如C1）
void analyze_program();                          // 分析整个程序
void analyze_declaration_list();                 // 分析声明列表
void analyze_var_decl();                         // 分析变量声明语句
void analyze_statement();                        // 分析各类语句
void analyze_assignment();                       // 分析赋值语句
void analyze_if_stmt();                          // 分析if语句
void analyze_while_stmt();                       // 分析while语句
void analyze_block();                            // 分析代码块（{...}）
void print_symbol_table();                       // 打印符号表内容
void print_quadruples();                         // 打印四元式列表
void print_activation_records();                 // 打印活动记录表
void print_constant_table();                     // 打印常数表
void typetrans(int type);                        // 将类型码转换为字符表示（i/r/c等）

// 外部变量声明（假设来自词法分析器）
extern char identifiers[100][50];    // 标识符表
extern int id_count;                 // 标识符数量
extern char C1[100][50];             // 整型常量表
extern int C1_count;                 // 整型常量数量
extern char C2[100][50];             // 浮点常量表
extern int C2_count;                 // 浮点常量数量
extern char Token[100][20];          // Token表
extern int Token_count;              // Token数量
extern int token_index;              // 当前Token索引
extern char* current_token;          // 当前Token
extern char* lookahead(int n);       // 预读Token
extern void consume();               // 消耗当前Token
extern void match(const char* token_type); // 匹配并消耗指定类型的Token

// 符号表结构
typedef struct {
    char name[50];      // 变量名
    int type;           // 类型：1-int, 2-float, 3-char
    int initialized;    // 是否已初始化（0-未初始化，1-已初始化）
    int scope;          // 作用域层级（0-全局，1-第一层块，依此类推）
} Symbol;

// 四元式结构
typedef struct {
    char op[20];        // 操作符
    char arg1[50];      // 操作数1
    char arg2[50];      // 操作数2（可选）
    char result[50];    // 结果或跳转目标
} Quadruple;

// 活动记录表结构
typedef struct {
    char name[50];      // 变量名
    int type;           // 类型
    int size;           // 占用空间（字节）
    int offset;         // 栈偏移量
    int scope;          // 作用域层级
} ActivationRecord;

// 常数表结构
typedef struct {
    char value[50];     // 常量值
    int type;           // 类型（1-int, 2-float, 3-char, 4-string）
} Constant;

// 全局变量
Symbol symbol_table[100];      // 符号表
int symbol_count = 0;          // 符号表当前大小
int current_scope = 0;         // 当前作用域层级

Quadruple quad[200];           // 四元式表
int quad_count = 0;            // 四元式计数器

ActivationRecord activ_rec[100]; // 活动记录表
int activ_count = 0;             // 活动记录计数器
int current_offset = 0;          // 当前栈偏移量

Constant const_table[100];     // 常数表
int const_count = 0;           // 常数表计数器

// 类型转换函数（保持原typetrans逻辑）
void typetrans(int type) {
    switch (type) {
    case 1: printf("i"); break;  // int类型
    case 2: printf("r"); break;  // float类型
    case 3: printf("c"); break;  // char类型
    case 4: printf("b"); break;  // bool类型
    case 5: printf("a"); break;  // 数组类型
    case 6: printf("d"); break;  // 结构体类型
    default: printf("?"); break; // 未知类型
    }
}

// 语义错误处理
void semantic_error(const char* message) {
    printf("语义错误: %s\n", message);
    exit(1);
}

// 在符号表中查找变量
int find_symbol(const char* name, int scope) {
    for (int i = symbol_count - 1; i >= 0; i--) {
        if (strcmp(symbol_table[i].name, name) == 0 && symbol_table[i].scope <= scope) {
            return i;  // 返回符号表索引
        }
    }
    return -1;  // 未找到
}

// 添加变量到符号表
void add_symbol(const char* name, int type) {
    if (find_symbol(name, current_scope) != -1) {
        semantic_error("重复定义的变量");
    }
    Symbol new_symbol;
    strcpy(new_symbol.name, name);
    new_symbol.type = type;
    new_symbol.initialized = 0;  // 初始化为未初始化状态
    new_symbol.scope = current_scope;
    symbol_table[symbol_count++] = new_symbol;
}

// 检查变量是否存在且已初始化
void check_variable(const char* name) {
    int index = find_symbol(name, current_scope);
    if (index == -1) {
        semantic_error("未定义的变量");
    }
    // 非赋值语句左侧需要检查初始化状态
    if (current_token && strstr(current_token, "(P 11)") == NULL) {
        if (!symbol_table[index].initialized) {
            semantic_error("使用未初始化的变量");
        }
    }
}

// 设置变量为已初始化
void set_initialized(const char* name) {
    int index = find_symbol(name, current_scope);
    if (index != -1) {
        symbol_table[index].initialized = 1;
    }
}

// 获取变量类型
int get_type(const char* name) {
    int index = find_symbol(name, current_scope);
    if (index == -1) {
        semantic_error("未定义的变量");
    }
    return symbol_table[index].type;
}

// 类型检查：返回表达式类型
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
    return 0;  // 未知类型
}

// 生成四元式
void generate_quad(const char* op, const char* arg1, const char* arg2, const char* result) {
    if (quad_count >= 200) {
        semantic_error("四元式表溢出");
    }
    strcpy(quad[quad_count].op, op);
    strcpy(quad[quad_count].arg1, arg1);
    strcpy(quad[quad_count].arg2, arg2);
    strcpy(quad[quad_count].result, result);
    quad_count++;
}

// 生成赋值四元式
void generate_assign_quad(const char* src, const char* dest) {
    generate_quad("=", src, "", dest);
}

// 生成条件跳转四元式
void generate_if_quad(const char* cond, const char* target) {
    generate_quad("if", cond, "", target);
}

// 生成无条件跳转四元式
void generate_goto_quad(const char* target) {
    generate_quad("goto", "", "", target);
}

// 进入新作用域
void enter_scope() {
    current_scope++;
    current_offset = -4;  // 局部变量从BP-4开始分配
}

// 退出当前作用域
void exit_scope() {
    current_scope--;
}

// 添加活动记录
void add_activation_record(const char* name, int type) {
    ActivationRecord rec;
    strcpy(rec.name, name);
    rec.type = type;
    rec.size = (type == 1 || type == 2) ? 4 : 1;  // int/float占4字节，char占1字节
    rec.offset = current_offset;
    rec.scope = current_scope;
    activ_rec[activ_count++] = rec;
    current_offset -= rec.size;  // 栈向下增长
}

// 添加常量到常数表（仅允许int和float类型）
int add_constant(const char* value, int type) {
    // 仅允许类型1（int）和类型2（float）存入常数表
    if (type != 1 && type != 2) {
        semantic_error("常数表仅允许存储实数（int或float）");
    }

    for (int i = 0; i < const_count; i++) {
        if (strcmp(const_table[i].value, value) == 0 && const_table[i].type == type) {
            return i + 1;  // 索引从1开始
        }
    }

    if (const_count >= 100) {
        semantic_error("常数表溢出");
    }
    strcpy(const_table[const_count].value, value);
    const_table[const_count].type = type;
    return ++const_count;
}

// 获取常量的四元式表示
char* get_constant_quad(const char* value, int type) {
    int idx = add_constant(value, type);
    char* buf = (char*)malloc(10);
    sprintf(buf, "%d", idx);
    return buf;
}

// 分析程序
void analyze_program() {
    token_index = 0;
    current_token = lookahead(1);
    current_scope = 0;  // 从全局作用域开始
    enter_scope();      // 进入全局作用域

    analyze_declaration_list();
    exit_scope();       // 退出全局作用域

    // 输出中间结果
    print_symbol_table();
    print_quadruples();
    print_activation_records();
    print_constant_table();

    // 检查是否有未处理的Token
    if (token_index < Token_count) {
        semantic_error("程序末尾有多余Token");
    }
    printf("\n语义分析成功！！！\n");
}

// 分析声明列表
void analyze_declaration_list() {
    while (current_token && token_index < Token_count) {
        if (strstr(current_token, "(K 1)") || strstr(current_token, "(K 4)") || strstr(current_token, "(K 16)")) {
            analyze_var_decl();  // 变量声明
        }

        else if (strstr(current_token, "(I ")) {
            analyze_statement();  // 赋值语句或函数调用
        }
        else if (strstr(current_token, "(K 12)")) {
            analyze_if_stmt();    // if语句
        }
        else if (strstr(current_token, "(K 5)")) {
            analyze_while_stmt(); // while语句
        }
        else {
            semantic_error("不支持的语句类型");
        }
    }
}

// 分析变量声明（移除char类型对常数表的调用）
void analyze_var_decl() {
    int type;
    if (strstr(current_token, "(K 1)")) {  // int类型
        type = 1;
        consume();
    }
    else if (strstr(current_token, "(K 4)")) {  // float类型
        type = 2;
        consume();
    }
    else if (strstr(current_token, "(K 16)")) {  // char类型（仅添加符号表，不处理常数表）
        type = 3;
        consume();
    }
    else {
        semantic_error("无效的类型说明符");
    }

    if (current_token && strstr(current_token, "(I ")) {
        int id;
        sscanf(current_token, "(I %d)", &id);
        char name[50];
        strcpy(name, identifiers[id - 1]);  // 从标识符表获取名称

        add_symbol(name, type);            // 添加到符号表
        add_activation_record(name, type);  // 添加到活动记录表

        consume();  // 消耗标识符Token

        // 处理初始化（如果有赋值）
        if (strstr(current_token, "(P 11)")) {  // 赋值运算符"="
            consume();  // 消耗"="
            int expr_type = check_expression_type();
            if (expr_type != type && expr_type != 0) {
                semantic_error("类型不匹配");
            }
            // 生成赋值四元式（仅处理int和float，移除char分支）
            if (strstr(current_token, "(C1 ")) {  // 整型常量（对应int）
                int cid;
                sscanf(current_token, "(C1 %d)", &cid);
                char* const_val = get_constant_quad(C1[cid - 1], 1);
                generate_assign_quad(const_val, name);
            }
            else if (strstr(current_token, "(C2 ")) {  // 浮点常量（对应float）
                int cid;
                sscanf(current_token, "(C2 %d)", &cid);
                char* const_val = get_constant_quad(C2[cid - 1], 2);
                generate_assign_quad(const_val, name);
            }
            else if (strstr(current_token, "(I ")) {  // 变量
                int vid;
                sscanf(current_token, "(I %d)", &vid);
                generate_assign_quad(identifiers[vid - 1], name);
            }
            // 移除char类型的CT处理分支
            set_initialized(name);  // 标记为已初始化
            consume();  // 消耗表达式Token
        }

        match("(P 13)");  // 消耗分号";"
    }
    else {
        semantic_error("变量声明缺少标识符");
    }
}
// 分析赋值语句
void analyze_assignment() {
    char name[50];
    sscanf(current_token, "(I %s)", name);
    check_variable(name);  // 检查变量是否存在且已初始化
    int var_type = get_type(name);
    consume();  // 消耗左值标识符

    match("(P 11)");  // 消耗赋值运算符"="

    // 处理右值表达式
    if (strstr(current_token, "(C1 ")) {  // 整型常量
        int cid;
        sscanf(current_token, "(C1 %d)", &cid);
        char* const_val = get_constant_quad(C1[cid - 1], 1);
        generate_assign_quad(const_val, name);
    }
    else if (strstr(current_token, "(I ")) {  // 变量
        int vid;
        sscanf(current_token, "(I %d)", &vid);
        generate_assign_quad(identifiers[vid - 1], name);
    }
    else {
        semantic_error("不支持的右值表达式类型");
    }

    set_initialized(name);  // 标记为已初始化
    consume();  // 消耗表达式
    match("(P 13)");  // 消耗分号";"
}

// 分析语句
void analyze_statement() {
    if (current_token && strstr(current_token, "(I ")) {
        char* next = lookahead(1);
        if (next && strstr(next, "(P 11)")) {  // 赋值语句
            analyze_assignment();
        }
        else {
            semantic_error("未知的语句类型");
        }
    }
}

// 分析if语句
void analyze_if_stmt() {
    match("(K 12)");  // 消耗"if"关键字
    match("(P 3)");   // 消耗左括号"("

    // 检查条件表达式类型
    int cond_type = check_expression_type();
    if (cond_type != 1) {  // 要求条件为int类型（简化处理）
        semantic_error("条件表达式必须为整数类型");
    }

    // 生成条件跳转四元式
    char cond[50];
    if (strstr(current_token, "(I ")) {
        int id;
        sscanf(current_token, "(I %d)", &id);
        strcpy(cond, identifiers[id - 1]);
    }
    else if (strstr(current_token, "(C1 ")) {
        int cid;
        sscanf(current_token, "(C1 %d)", &cid);
        sprintf(cond, "C%d", add_constant(C1[cid - 1], 1));
    }
    consume();  // 消耗条件表达式

    match("(P 4)");  // 消耗右括号")"

    // 生成条件跳转和无条件跳转（目标需后续回填）
    char target_true[10], target_false[10];
    sprintf(target_true, "L%d", quad_count + 1);
    generate_if_quad(cond, target_true);
    sprintf(target_false, "L%d", quad_count + 2);
    generate_goto_quad(target_false);

    // 分析if代码块
    enter_scope();
    analyze_block();
    exit_scope();

    // 处理else部分（如果有）
    if (current_token && strstr(current_token, "(K 14)")) {
        // 生成跳转到else后代码的指令
        char target_end[10];
        sprintf(target_end, "L%d", quad_count + 1);
        generate_goto_quad(target_end);

        // 回填else分支的目标
        strcpy(quad[quad_count - 2].result, target_end);

        consume();  // 消耗"else"关键字
        enter_scope();
        analyze_block();
        exit_scope();
    }
    else {
        // 回填else分支的目标（如果没有else，则跳转到if块之后）
        strcpy(quad[quad_count - 1].result, target_false);
    }
}

// 分析while语句
void analyze_while_stmt() {
    match("(K 5)");  // 消耗"while"关键字
    match("(P 3)");  // 消耗左括号"("

    // 记录循环开始位置
    char loop_start[10];
    sprintf(loop_start, "L%d", quad_count);

    // 检查条件表达式类型
    int cond_type = check_expression_type();
    if (cond_type != 1) {
        semantic_error("条件表达式必须为整数类型");
    }

    // 生成条件跳转四元式
    char cond[50];
    if (strstr(current_token, "(I ")) {
        int id;
        sscanf(current_token, "(I %d)", &id);
        strcpy(cond, identifiers[id - 1]);
    }
    else if (strstr(current_token, "(C1 ")) {
        int cid;
        sscanf(current_token, "(C1 %d)", &cid);
        sprintf(cond, "C%d", add_constant(C1[cid - 1], 1));
    }
    consume();  // 消耗条件表达式

    match("(P 4)");  // 消耗右括号")"

    // 生成条件跳转和循环结束位置
    char loop_end[10];
    sprintf(loop_end, "L%d", quad_count + 1);
    generate_if_quad(cond, loop_end);

    // 分析循环体
    enter_scope();
    analyze_block();
    exit_scope();

    // 生成无条件跳转回到循环开始
    generate_goto_quad(loop_start);

    // 回填循环结束位置
    strcpy(quad[quad_count - 2].result, loop_end);
}

// 分析代码块
void analyze_block() {
    match("(P 15)");  // 消耗左花括号"{"
    enter_scope();    // 进入新作用域
    analyze_declaration_list();
    exit_scope();     // 退出作用域
    match("(P 16)");  // 消耗右花括号"}"
}

// 打印符号表（使用typetrans函数）
void print_symbol_table() {
    printf("\n符号表内容:\n");
    printf("名称\t类型\t种类\t作用域\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("%s\t", symbol_table[i].name);
        typetrans(symbol_table[i].type);  // 使用原typetrans函数输出类型
        printf("\t%d\t%d\n",
            symbol_table[i].initialized,
            symbol_table[i].scope);
    }
}

// 打印四元式表
void print_quadruples() {
    printf("\n四元式列表:\n");
    printf("序号\t操作符\t操作数1\t操作数2\t结果\n");
    for (int i = 0; i < quad_count; i++) {
        printf("%d\t(%s\t%s\t%s\t%s)\n",
            i + 1, quad[i].op, quad[i].arg1, quad[i].arg2, quad[i].result);
    }
}

// 打印活动记录表
void print_activation_records() {
    printf("\n活动记录表:\n");
    printf("变量名\t大小\t偏移量\t\n");
    for (int i = 0; i < activ_count; i++) {
        printf("%s",activ_rec[i].name);
        printf("\t%d\t%d\t\n",
            activ_rec[i].size,
            activ_rec[i].offset);
    }
}

// 打印常数表
void print_constant_table() {
    printf("\n常数表:\n");
    printf("序号\t值\t\n");
    for (int i = 0; i < const_count; i++) {
        printf("%d\t%s\t",
            i + 1, const_table[i].value);
        printf("\n");
    }
}
//总打印函数