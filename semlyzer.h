/* 翻译文法设计 

/* 程序结构 
<程序> → <声明列表> {analyze_program(); }

/* 声明列表 
<声明列表> → <声明> <声明列表> {analyze_declaration_list(); }
<声明列表> → ε{ ε }

/* 声明 
<声明> → <变量声明> {analyze_var_decl(); }
<声明> → <赋值语句> {analyze_assignment(); }
<声明> → <if语句> {analyze_if_stmt(); }
<声明> → <while语句> {analyze_while_stmt(); }
<声明> → <struct语句> {analyze_struct_stmt(); }

/* 变量声明 
<变量声明> → <类型说明符> <标识符> <初始化> ? <分号> {
    add_symbol(name, type, 0);
    add_activation_record(name, type);
    if (initialized) set_initialized(name);
}

/* 类型说明符 
<类型说明符> → 整型{ type = 1; }
<类型说明符> → 浮点型{ type = 2; }
<类型说明符> → 字符型{ type = 3; }
<类型说明符> → 布尔型{ type = 4; }
<类型说明符> → struct <标识符> { type = 6; }

/* 初始化 
<初始化> → = <表达式>{
    check_type_compatibility(lhs_type, expr_type);
    generate_assign_quad(expr_result, lhs);
    initialized = true;
}

/* 赋值语句 
<赋值语句> → <标识符> = <表达式> <分号>{
    check_variable(lhs);
    generate_assign_quad(expr_result, lhs);
}

/* 表达式 
<表达式> → <加法表达式> {expr_result = parse_expression_for_assignment(); }

/* 加法表达式 
<加法表达式> → <乘法表达式> {result = parse_term(); }
<加法表达式> → <加法表达式> +<乘法表达式> {
    generate_quad("+", result, operand, temp);
    result = temp;
}
<加法表达式> → <加法表达式> -<乘法表达式> {
    generate_quad("-", result, operand, temp);
    result = temp;
}

/* 乘法表达式 
<乘法表达式> → <基本表达式> {result = parse_factor(); }
<乘法表达式> → <乘法表达式>* <基本表达式> {
    generate_quad("*", result, operand, temp);
    result = temp;
}
<乘法表达式> → <乘法表达式> / <基本表达式> {
    generate_quad("/", result, operand, temp);
    result = temp;
}

/* 基本表达式 
<基本表达式> → <标识符> {check_variable(name); result = name; }
<基本表达式> → <数字> {result = get_constant_quad(value, type); }
<基本表达式> →(<表达式>) { result = parse_expression_for_assignment(); }

/* if语句 
<if语句> → if (<条件>) < 代码块 > <else子句> ? {
    generate_if_quad(cond_result, target_true);
    analyze_block();
    if (has_else) {
        generate_quad("el", "_", "_", "_");
        analyze_block();
    }
    generate_quad("ie", "_", "_", "_");
}

/* 条件 
<条件> → <表达式> <关系运算符> <表达式> {
    generate_quad(rel_op, left, right, temp);
    cond_result = temp;
}

/* 关系运算符 
<关系运算符> → > {rel_op = ">"; }
<关系运算符> → < {rel_op = "<"; }
<关系运算符> → == {rel_op = "=="; }
<关系运算符> → <= {rel_op = "<="; }
<关系运算符> → != {rel_op = "!="; }
<关系运算符> → >= {rel_op = ">="; }

/* else子句 
<else子句> → else <代码块> {has_else = true; }
<else子句> → ε{ has_else = false; }

/* while语句 
<while语句> → while (<条件>) < 代码块 > {
    generate_quad("wl", "_", "_", "_");
    generate_while_quad(cond_result, exit_label);
    analyze_block();
    generate_quad("we", "_", "_", "_");
}

/* struct语句 
<struct语句> → struct <标识符> { <成员列表> } <分号> {
    add_symbol(name, 6, 3);
    enter_scope();
    analyze_struct_members();
    exit_scope();
}

/* 成员列表 
<成员列表> → <成员声明> <成员列表> {analyze_struct_members(); }
<成员列表> → ε{ ε }

/* 成员声明 
<成员声明> → <类型说明符> <标识符> <初始化> ? <分号> {
    add_symbol(name, type, 2);
    add_activation_record(name, type);
    if (initialized) generate_struct_init_quad();
}

/* 代码块 
<代码块> →{ <声明列表> } {
    enter_scope();
    analyze_declaration_list();
    exit_scope();
}

/* 辅助产生式 
<标识符> → I{ name = identifiers[id - 1]; }
<数字> → C1{ value = C1[cid - 1]; type = 1; }
<数字> → C2{ value = C2[cid - 1]; type = 2; }
<分号> →; { ε }
<左括号> →({ ε }
    <右括号> →) {
    ε
}
<左花括号> →{ {ε}
<右花括号> → } {ε}
*/

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* parse_expression_for_assignment();
char* parse_term();
char* parse_factor();
void semantic_error(const char* message);        // 输出语义错误信息并终止程序
int find_symbol(const char* name, int scope);    // 在符号表中查找变量（返回索引或-1）
void add_symbol(const char* name, int type, int kind);     // 添加新变量到符号表（检查重复）
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
void print_type_table();
void typetrans(int type);                        // 将类型码转换为字符表示（i/r/c等）
char* parse_condition_expression();
void generate_while_quad(const char* cond, const char* target);
void analyze_struct_stmt();
void typetrans(int type);
void optimize_integer_quadruples();      //优化
void active_information();
void analyze_function_stmt();
void analyze_array_decl();
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
    int initialized;    // 种类
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
    case 6: printf("d<——"); break;  // 结构体类型
    case 7:  printf("—"); break;     //函数类型
    default: printf("?"); break; // 未知类型
    }
}

// 种类转换函数（保持原typetrans逻辑）
void kindtrans(int type) {
    switch (type) {
    case 1: printf("v"); break;  // 变量种类
    case 2: printf("d"); break;  // 结构体成员类型
    case 3: printf("t"); break;  // 类型种类
    case 4: printf("c"); break;  // 常量种类
    case 5: printf("f"); break;  // 函数种类
    case 6: printf("vn"); break; //换名形参
    case 7: printf("vf"); break;  //赋值形参
    default: printf("?"); break; // 未知种类
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
void add_symbol(const char* name, int type, int kind) {
    if (find_symbol(name, current_scope) != -1) {
        semantic_error("重复定义的变量");
    }
    Symbol new_symbol;
    strcpy(new_symbol.name, name);
    new_symbol.type = type;
    new_symbol.initialized = kind;  // 初始化为未初始化状态
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
        int id;
        sscanf(current_token, "(I %d)", &id);
        return get_type(identifiers[id - 1]);  // 变量类型
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
    generate_quad("=", src, "_", dest);
}

// 生成条件跳转四元式
void generate_if_quad(const char* cond, const char* target) {
    generate_quad("if", target, "_", "_");
}

// 生成无条件跳转四元式
void generate_goto_quad(const char* target) {
    generate_quad("goto", "_", "_", target);
}

//do四元式
void generate_while_quad(const char* cond, const char* target)
{
    generate_quad("do", target, "_", "_");
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
char* get_constant_quad(char* value, int type) {
    int idx = add_constant(value, type);
    char* buf = (char*)malloc(10);
    sprintf(buf, "%d", idx);
    return value;
}

// 分析程序//////////////////////////////////////////////////////
void analyze_program() {
    token_index = 0;
    current_token = lookahead(1);
    current_scope = 0;  // 从全局作用域开始
    enter_scope();      // 进入全局作用域

    analyze_declaration_list();
    exit_scope();       // 退出全局作用域

    // 输出中间结果
    print_symbol_table();
    print_type_table();
    print_constant_table();
    print_activation_records();
    print_quadruples();

    //active_information();//暂未完成
    // 检查是否有未处理的Token
    printf("%s", Token[token_index]);
    if (token_index < Token_count) {
        semantic_error("程序末尾有多余Token");
    }
    printf("\n语义分析成功！！！\n");
}

// 分析声明列表
void analyze_declaration_list() {
    while (current_token && token_index < Token_count) {
        if (strstr(current_token, "(K 1)") || strstr(current_token, "(K 4)") || strstr(current_token, "(K 16)")|| strstr(current_token, "(K 17)")) {
            analyze_var_decl();  // 变量声明
            //后续可以添加函数判断
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
        else if (strstr(current_token, "(K 7)")) {
            analyze_struct_stmt();//struct
        }
        else if (strstr(current_token, "(P 16)")) {
            break;                         //最后的若为分号退出
        }
        else if (strstr(current_token, "(K 2)")) {
            analyze_function_stmt();//函数板块分析
        }
        else {
            printf("%s", Token[token_index]);
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
    else if (strstr(current_token, "(K 17)")) {  // bool类型（仅添加符号表，不处理常数表）
        type = 4;
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

        add_symbol(name, type, 1);            // 添加到符号表
        add_activation_record(name, type);  // 添加到活动记录表

        consume();  // 消耗标识符Token

        // 处理初始化（如果有赋值）
        if (strstr(current_token, "(P 11)")) {  // 赋值运算符"="
            consume();  // 消耗"="
            int expr_type = check_expression_type();
            if (expr_type != type && expr_type != 0) {
                semantic_error("类型不匹配");
            }
            // 生成赋值四元式（仅处理int和float，bool，移除char分支）
            if (strstr(current_token, "(C1 ")) {  // 整型常量（对应int）
                int cid;
                sscanf(current_token, "(C1 %d)", &cid);
                if (type == 4 && (C1[cid - 1][0] == '0' || C1[cid - 1][0] == '1'))
                {
                    char* const_val = get_constant_quad(C1[cid - 1], 1);
                    generate_assign_quad(const_val, name);
                }
                else if (type == 1)
                {
                    char* const_val = get_constant_quad(C1[cid - 1], 1);
                    generate_assign_quad(const_val, name);
                }
                else 
                {
                     semantic_error("错误的bool声明");
                }
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


void analyze_assignment() {
    int id;
    sscanf(current_token, "(I %d)", &id);
    char* lhs_name = identifiers[id - 1];
    consume();  // 消耗左值标识符

    match("(P 11)");  // 消耗赋值运算符"="

    // 解析右值表达式（支持加减乘除，遵循运算符优先级）
    char* expr_quad = parse_expression_for_assignment();

    // 生成赋值四元式（右值结果 -> 左值变量）

    generate_assign_quad(expr_quad, lhs_name);
    match("(P 13)");  // 消耗分号";"
}

// 解析表达式（处理加减运算）
char* parse_expression_for_assignment() {
    char* result = parse_term(); // 乘除运算结果（T2=4）

    // 处理加减运算符（+ -）
    while (current_token && (strstr(current_token, "(P 8)") || strstr(current_token, "(P 1)"))) {
        char* op = (strstr(current_token, "(P 8)")) ? "+" : "-";
        consume(); // 消耗运算符（如+）

        char* operand = parse_term(); // 修正：

        // 生成加法四元式（如T2+1=T3）
        char temp_result[10];
        sprintf(temp_result, "T%d", quad_count + 1);
        generate_quad(op, result, operand, temp_result);
        strcpy(result, temp_result);
    }

    return result;
}

// 解析项（处理乘除运算）
char* parse_term() {
    char* result = parse_factor(); // 解析因子（如常量2）

    // 处理乘除运算符（* /）
    while (current_token && (strstr(current_token, "(P 9)") || strstr(current_token, "(P 2)"))) {
        char* op = (strstr(current_token, "(P 9)")) ? "*" : "/";
        consume(); // 消耗运算符（如*）

        char* operand = parse_factor(); // 解析下一个因子（如另一个2）

        // 生成乘除四元式（如2*2=T2）
        char temp_result[10];
        sprintf(temp_result, "T%d", quad_count + 1);
        generate_quad(op, result, operand, temp_result);
        strcpy(result, temp_result);
    }

    return result;
}

// 解析因子（变量、常量或括号表达式）
char* parse_factor() {
    char* result = (char*)malloc(50);
    strcpy(result, "");

    if (strstr(current_token, "(I ")) {  // 变量
        int vid;
        sscanf(current_token, "(I %d)", &vid);
        strcpy(result, identifiers[vid - 1]);
        check_variable(identifiers[vid - 1]);
        consume();
    }
    else if (strstr(current_token, "(C1 ")) {  // 整型常量
        int cid;
        sscanf(current_token, "(C1 %d)", &cid);
        strcpy(result, get_constant_quad(C1[cid - 1], 1));
        consume();
    }
    else if (strstr(current_token, "(P 3)")) {  // 左括号"(", 处理括号表达式
        consume(); // 消耗左括号
        result = parse_expression_for_assignment(); // 递归解析表达式
        match("(P 4)"); // 消耗右括号")"
    }
    else {
        semantic_error("表达式因子无效");
    }

    return result;
}




// 分析语句
void analyze_statement() {
    if (current_token && strstr(current_token, "(I ")) {
        char* next = lookahead(2);
        if (next && strstr(next, "(P 11)")) {  // 赋值语句
            analyze_assignment();
        }
        else {
            semantic_error("未知的语句类型");
        }
    }
}

// 分析数组声明语句
void analyze_array_decl() {
    // 解析数组元素类型
    int element_type = 0;
    if (strstr(current_token, "(K 1)")) {  // int类型
        element_type = 1;
        consume();
    }
    else if (strstr(current_token, "(K 4)")) {  // float类型
        element_type = 2;
        consume();
    }
    else if (strstr(current_token, "(K 16)")) {  // char类型
        element_type = 3;
        consume();
    }
    else {
        semantic_error("无效的数组元素类型说明符");
    }

    // 获取数组名称
    if (!strstr(current_token, "(I ")) {
        semantic_error("期望数组名称");
    }
    int array_id;
    sscanf(current_token, "(I %d)", &array_id);
    char* array_name = identifiers[array_id - 1];
    consume();  // 消耗数组名称

    // 检查数组是否已定义
    int array_symbol_idx = find_symbol(array_name, current_scope);
    if (array_symbol_idx != -1) {
        semantic_error("重复定义的数组");
    }

    // 解析数组维度
    int dimensions[8] = { 0 };  // 存储各维度大小，最多8维
    int dimension_count = 0;  // 维度数量

    while (current_token && strstr(current_token, "(P 5)") && dimension_count < 8) {
        consume();  // 消耗左方括号"["

        // 解析维度大小
        if (strstr(current_token, "(C1 ")) {  // 整数常量
            int cid;
            sscanf(current_token, "(C1 %d)", &cid);
            dimensions[dimension_count] = atoi(C1[cid - 1]);
            consume();  // 消耗常量
        }
        else {
            semantic_error("数组维度必须是常量表达式");
        }

        consume();  // 消耗右方括号"]"
        dimension_count++;
    }

    if (dimension_count == 0) {
        semantic_error("数组必须至少有一个维度");
    }

    // 计算数组总大小（元素个数）
    int total_size = 1;
    for (int i = 0; i < dimension_count; i++) {
        total_size *= dimensions[i];
    }

    // 添加数组到符号表（kind=5表示数组）
    add_symbol(array_name, 5, 1);  // 5表示数组类型

    // 添加数组到活动记录表
    add_activation_record(array_name, element_type);

    // 处理初始化（如果有）
    if (strstr(current_token, "(P 11)")) {  // 赋值运算符"="
        consume();  // 消耗"="
        match("(P 15)");  // 消耗左花括号"{"

        // 这里简化处理，实际应递归解析初始化列表
        // 并生成相应的四元式代码

        while (current_token && !strstr(current_token, "(P 16)")) {
            // 解析初始化列表中的元素
            // ...
            consume();  // 消耗元素或逗号
        }

        match("(P 16)");  // 消耗右花括号"}"
        set_initialized(array_name);  // 标记为已初始化
    }

    match("(P 13)");  // 消耗分号";"
}
// 分析结构体语句（定义或初始化）
void analyze_struct_stmt() {
    // 消耗"struct"关键字
    match("(K 7)");  // 假设(K 20)是"struct"的Token类型

    // 获取结构体名称
    if (!strstr(current_token, "(I ")) {
        semantic_error(current_token);
    }
    int struct_id;
    sscanf(current_token, "(I %d)", &struct_id);
    char* struct_name = identifiers[struct_id - 1];
    consume();  // 消耗结构体名称

    // 检查结构体是否已定义
    int struct_type_id = find_symbol(struct_name, current_scope);
    if (struct_type_id != -1) {
        semantic_error(struct_name);
    }

    add_symbol(struct_name, 6, 3);

    // 开始解析结构体成员
    match("(P 15)");  // 消耗左花括号"{"

    while (current_token && !strstr(current_token, "(P 16)")) {  // 直到遇到右花括号"}"
        // 解析成员类型
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
        else if (strstr(current_token, "(K 17)")) {  // char类型（仅添加符号表，不处理常数表）
            type = 4;
            consume();
        }
        else if (strstr(current_token, "(K 7)")) {  // char类型（仅添加符号表，不处理常数表）
            //banalyze_struct_stmt();//struct
        }
        else {
            semantic_error("无效的类型说明符");
        }

        if (current_token && strstr(current_token, "(I ")) {
            int id;
            sscanf(current_token, "(I %d)", &id);
            char name[50];
            strcpy(name, identifiers[id - 1]);  // 从标识符表获取名称

            add_symbol(name, type, 2);            // 添加到符号表
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
                    if (type == 4 && (C1[cid - 1][0] == '0' || C1[cid - 1][0] == '1'))
                    {
                        char* const_val = get_constant_quad(C1[cid - 1], 1);
                        generate_assign_quad(const_val, name);
                    }
                    else if (type == 1)
                    {
                        char* const_val = get_constant_quad(C1[cid - 1], 1);
                        generate_assign_quad(const_val, name);
                    }
                    else
                    {
                        semantic_error("错误的bool声明");
                    }
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
                consume();  // 消耗表达式Token
            }
            match("(P 13)");  // 消耗分号";"
        }

        else {
            semantic_error("变量声明缺少标识符");
        }
    }
    // 消耗右花括号
    match("(P 16)");  // 右花括号"}"
    match("(P 13)");  // 分号";"
}
// 分析if语句
void analyze_if_stmt() {
    match("(K 12)");  // 消耗"if"关键字
    match("(P 3)");   // 消耗左括号"("

    // 解析条件表达式（支持关系运算符）
    char* cond_result = parse_condition_expression();

    // 生成条件跳转四元式
    char target_true[10];//char  target_false[10];
    sprintf(target_true, "T%d", quad_count);
    generate_if_quad(cond_result, target_true);  // 条件表达式结果作为条件
    //sprintf(target_false, "L%d", quad_count + 2);
    //generate_goto_quad(target_false);

    // 释放条件表达式结果的内存
    if (strncmp(cond_result, "T", 1) == 0) {
        free(cond_result);
    }

    if (!strstr(current_token, "(P 4)")) {
        semantic_error("条件表达式缺少右括号')'");
    }
    match("(P 4)");  // 消耗右括号")"

    // 分析if代码块
    analyze_block();


    // 处理else部分（如果有）
    if (current_token && strstr(current_token, "(K 14)")) {
        generate_quad("el", "_", "_", "_");

        // 生成跳转到else后代码的指令
        char target_end[10];
        sprintf(target_end, "T%d", quad_count + 1);
        // generate_goto_quad(target_end);

         // 回填else分支的目标
        strcpy(quad[quad_count - 2].result, target_end);

        consume();  // 消耗"else"关键字
        analyze_block();
    }

    /*else {
         // 回填else分支的目标（如果没有else，则跳转到if块之后）
         strcpy(quad[quad_count - 1].result, target_false);
     }*/
    generate_quad("ie", "_", "_", "_");
}

// 解析条件表达式（支持关系运算符，修正consume调用）
char* parse_condition_expression() {
    // 解析左操作数（如"a"或"a + b"）
    char* left_operand = parse_expression_for_assignment();

    // 检查是否存在关系运算符
    if (current_token &&
        (strstr(current_token, "(P 5)") ||  // ==
            strstr(current_token, "(P 6)") ||  // <=
            strstr(current_token, "(P 7)") ||  // <
            strstr(current_token, "(P 10)") ||  //>
            strstr(current_token, "(P 17)") || //!=
            strstr(current_token, "(P 18)") //>=
            ))
    {

        char rel_op[3] = { 0 }; // 初始化为空字符串，确保字符串正确终止

        if (strstr(current_token, "(P 10)")) { // >
            strcpy(rel_op, ">");
            consume();
        }
        else if (strstr(current_token, "(P 7)")) { // <
            strcpy(rel_op, "<");
            consume();
        }
        else if (strstr(current_token, "(P 5)")) { // ==
            strcpy(rel_op, "==");
            consume();
        }
        else if (strstr(current_token, "(P 6)")) { // <=
            strcpy(rel_op, "<=");
            consume();
        }
        else if (strstr(current_token, "(P 17)")) { // !=
            strcpy(rel_op, "!=");
            consume();
        }
        else if (strstr(current_token, "(P 18)")) { // >=
            strcpy(rel_op, ">=");
            consume();
        }
        else {
            semantic_error("不支持的关系运算符");
        }

        // 解析右操作数（如"10"或"c"）
        char* right_operand = parse_expression_for_assignment();

        // 生成关系表达式的四元式（如 a > 10 → T1 = a > 10）
        char* temp_result = (char*)malloc(10);
        sprintf(temp_result, "T%d", quad_count + 1);
        generate_quad(rel_op, left_operand, right_operand, temp_result);

        // 释放左右操作数的内存（如果是临时变量）
        if (strncmp(left_operand, "T", 1) == 0) {
            free(left_operand);
        }
        if (strncmp(right_operand, "T", 1) == 0) {
            free(right_operand);
        }

        return temp_result;
    }
    // 如果没有关系运算符，直接返回左操作数（如单个变量或常量）
    return left_operand;
}

// 分析while语句
void analyze_while_stmt() {
    generate_quad("wl", "_", "_", "_");
    match("(K 5)");  // 消耗"while"关键字
    match("(P 3)");  // 消耗左括号"("

    // 记录循环开始位置（标签）
    char loop_start[10];
    sprintf(loop_start, "T%d", quad_count);

    // 解析条件表达式（使用与if语句相同的表达式解析函数）
    char* cond_result = parse_condition_expression();

    // 生成条件跳转四元式（条件为真时跳转到循环体，否则跳出循环）
    char loop_exit[10];
    sprintf(loop_exit, "T%d", quad_count);
    generate_while_quad(cond_result, loop_exit);

    // 释放条件表达式结果的内存
    if (strncmp(cond_result, "T", 1) == 0) {
        free(cond_result);
    }

    match("(P 4)");  // 消耗右括号")"

    // 分析循环体
    analyze_block();

    // 生成无条件跳转回到循环开始处
    //generate_goto_quad(loop_start);

    // 回填条件跳转的目标标签（循环结束位置）
    strcpy(quad[quad_count - 2].result, loop_exit);
    generate_quad("we", "_", "_", "_");
}


// 分析函数声明语句
void analyze_function_stmt() {
    // 消耗"void"关键字
    match("(K 2)");  // (K 2)是"void"的Token类型

    // 获取函数名称
    if (!strstr(current_token, "(I ")) {
        semantic_error("函数名称出现错误");
    }
    int func_id;
    sscanf(current_token, "(I %d)", &func_id);
    char* func_name = identifiers[func_id - 1];
    consume();  // 消耗函数名称

    // 检查函数是否已定义
    int func_symbol_idx = find_symbol(func_name, current_scope);
    if (func_symbol_idx != -1) {
        semantic_error("重复定义的函数");
    }

    // 添加函数到符号表（kind=5表示函数）
    add_symbol(func_name, 7, 5);

    // 消耗左括号"("
    match("(P 3)");

    // 解析参数列表
    int param_count = 0;
    while (current_token && !strstr(current_token, "(P 4)")) {
        // 解析参数类型
        int param_type = 0;
        if (strstr(current_token, "(K 1)")) {  // int类型
            param_type = 1;
            consume();
        }
        else if (strstr(current_token, "(K 4)")) {  // float类型
            param_type = 2;
            consume();
        }
        else if (strstr(current_token, "(K 16)")) {  // char类型
            param_type = 3;
            consume();
        }
        else if (strstr(current_token, "(K 17)")) {  // bool类型
            param_type = 4;
            consume();
        }
        else {
            semantic_error("无效的参数类型说明符");
        }

        // 检查是否为引用参数
        int is_reference = 0;
        if (current_token && strstr(current_token, "(P 20)")) {  // &符号
            is_reference = 1;
            consume();
        }

        // 获取参数名称
        if (!strstr(current_token, "(I ")) {
            semantic_error("期望参数却发现错误");
        }
        int param_id;
        sscanf(current_token, "(I %d)", &param_id);
        char* param_name = identifiers[param_id - 1];
        consume();  // 消耗参数名称

        // 添加参数到符号表（kind=7表示引用参数，kind=6表示值参数）
        add_symbol(param_name, param_type, is_reference ? 7 : 6);

        // 添加参数到活动记录表
        add_activation_record(param_name, param_type);

        param_count++;

        //// 检查是否有更多参数
        //if (current_token && strstr(current_token, "(P 12)")) {  // 逗号","
        //    consume();
        //}
        //else if (!strstr(current_token, "(P 4)")) {  // 不是右括号，说明语法错误
        //    semantic_error("参数列表格式错误");
        //}
        if (strstr(current_token, "(P 12)"))
            match("(P 12)");
    }
    // 消耗右括号")"
    match("(P 4)");
    // 解析函数体（代码块）
    analyze_block();
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
    printf("\n符号表内容:\t\t\t\t结构体表内容：\n");
    printf("名称\t类型\t种类\t作用域\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("%s\t", symbol_table[i].name);
        typetrans(symbol_table[i].type);  // 使用原typetrans函数输出类型
        printf("\t");
        kindtrans(symbol_table[i].initialized);
        printf("\t%d\t\t\t", symbol_table[i].scope);

        if (symbol_table[i].initialized == 2)
        {
            printf("%s\t", symbol_table[i].name);
            typetrans(symbol_table[i].type);  // 使用原typetrans函数输出类型
            printf("\t");
            kindtrans(symbol_table[i].initialized);
            printf("\t%d", symbol_table[i].scope);
        }
        printf("\n");
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
        printf("%s", activ_rec[i].name);
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

//打印类型表
void print_type_table()
{
    int i_,r,c,b;
    i_ = r = c = b = 0;
    int d = 0;
    for (int i = 0; i < symbol_count; i++) {
        switch (symbol_table[i].type)
        {
        case 1:
            i_ = 1;
            break;
        case 2:
            r = 1;
            break;
        case 3:
            c = 1;
            break;
        case 4:
            b = 1;
            break;
        case 6:
            d++;
            break;
        default:
            printf("未知类型");
            break;
        }
    }
    printf("类型表为：\n");
    if (i_ == 1)printf("i ");
    if (r == 1)printf("r "); 
    if (b == 1)printf("b "); 
    if (c == 1)printf("c "); 
    printf("\t|\n");
    for (int ii = 1; ii <= d;ii++)
    {
        printf("d  \t|\n");
    }
}


