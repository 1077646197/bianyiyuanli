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

 // 关键字表，第一列是关键字，第二列是编号
char* keywords[][2] = {
    {"int", "1"}, {"void", "2"}, {"break", "3"}, {"float", "4"}, {"while", "5"},
    {"do", "6"}, {"struct", "7"}, {"const", "8"}, {"case", "9"}, {"for", "10"},
    {"return", "11"}, {"if", "12"}, {"default", "13"}, {"else", "14"},{"main", "15"}
};
int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

// 界符表，第一列是界符，第二列是编号
char* delimiters[][2] = {
    {"-", "1"}, {"/", "2"}, {"(", "3"}, {")", "4"}, {"==", "5"}, {"<=", "6"},
    {"<", "7"}, {"+", "8"}, {"*", "9"}, {">", "10"}, {"=", "11"}, {",", "12"},
    {";", "13"}, {"++", "14"}, {"{", "15"}, {"}", "16"}
};
int delimiter_count = sizeof(delimiters) / sizeof(delimiters[0]);

char identifiers[100][50]; // 标识符表
int id_count = 0;

char C1[100][50]; // 常整数表
int C1_count = 0;

char C2[100][50]; // 常实数表
int C2_count = 0;

char CT[100][50]; // 字符常量表
int CT_count = 0;

char ST[100][50]; // 字符串常量表
int ST_count = 0;

char Token[100][50]; // Token串表
int Token_count = 0;

int iscorret = 1;//是否正确

// 向标识符表添加元素
void add_to_identifiers(const char* str) {
    for (int i = 0; i < id_count; i++) {
        if (strcmp(identifiers[i], str) == 0) return;
    }
    strcpy(identifiers[id_count++], str);
}

// 向常整数表添加元素
void add_to_C1(const char* str) {
    for (int i = 0; i < C1_count; i++) {
        if (strcmp(C1[i], str) == 0) return;
    }
    strcpy(C1[C1_count++], str);
}

// 向常实数表添加元素
void add_to_C2(const char* str) {
    for (int i = 0; i < C2_count; i++) {
        if (strcmp(C2[i], str) == 0) return;
    }
    strcpy(C2[C2_count++], str);
}

// 向字符常量表添加元素
void add_to_CT(const char* str) {
    for (int i = 0; i < CT_count; i++) {
        if (strcmp(CT[i], str) == 0) return;
    }
    strcpy(CT[CT_count++], str);
}

// 向字符串常量表添加元素
void add_to_ST(const char* str) {
    for (int i = 0; i < ST_count; i++) {
        if (strcmp(ST[i], str) == 0) return;
    }
    strcpy(ST[ST_count++], str);
}

// 检查是否是关键字，是则返回编号，否则返回0
int is_keyword(const char* str) {
    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(keywords[i][0], str) == 0) return atoi(keywords[i][1]);
    }
    return 0;
}

// 检查是否是界符，是则返回编号，否则返回0
int is_delimiter(const char* str) {
    for (int i = 0; i < delimiter_count; i++) {
        if (strcmp(delimiters[i][0], str) == 0) return atoi(delimiters[i][1]);
    }
    return 0;
}

// 扫描输入字符串，识别各类单词并存储到Token数组
void scan(const char* source) {
    int i = 0;
    int n = strlen(source);
    while (i < n) {
        while (isspace(source[i])) i++; // 跳过空白字符
        if (i >= n) break;

        // 处理双字符界符
        if (i + 1 < n) {
            char two[3] = { source[i], source[i + 1], '\0' };
            int delimiter_num = is_delimiter(two);
            if (delimiter_num) {
                if (Token_count < 100) { // 防止数组越界
                    sprintf(Token[Token_count], "(P %d)", delimiter_num);
                    Token_count++;
                }
                i += 2;
                continue;
            }
        }

        // 处理单字符界符
        char one[2] = { source[i], '\0' };
        int delimiter_num = is_delimiter(one);
        if (delimiter_num) {
            if (Token_count < 100) {
                sprintf(Token[Token_count], "(P %d)", delimiter_num);
                Token_count++;
            }
            i++;
            continue;
        }

        // 处理关键字或标识符
        if (isalpha(source[i]) || source[i] == '_') {
            char id[50] = { 0 };
            int j = 0;
            while (i < n && (isalpha(source[i]) || isdigit(source[i]) || source[i] == '_')) {
                id[j++] = source[i++];
            }
            id[j] = '\0';
            int keyword_num = is_keyword(id);
            if (keyword_num) {
                if (Token_count < 100) {
                    sprintf(Token[Token_count], "(K %d)", keyword_num);
                    Token_count++;
                }
            }
            else {
                add_to_identifiers(id);
                if (Token_count < 100) {
                    sprintf(Token[Token_count], "(I %d)", id_count);
                    Token_count++;
                }
            }
            continue;
        }

        // 处理字符常量
        if (source[i] == '\'') {
            i++;
            char ct[2] = { source[i], '\0' };
            i++;
            if (source[i] != '\'') {
                iscorret = 0;
                return;
            }
            i++;
            add_to_CT(ct);
            if (Token_count < 100) {
                sprintf(Token[Token_count], "(CT %d)", CT_count);
                Token_count++;
            }
            continue;
        }

        // 处理字符串常量
        if (source[i] == '"') {
            i++;
            char st[50] = { 0 };
            int j = 0;
            while (i < n && source[i] != '"') {
                st[j++] = source[i++];
            }
            st[j] = '\0';
            i++;
            add_to_ST(st);
            if (Token_count < 100) {
                sprintf(Token[Token_count], "(ST %d)", ST_count);
                Token_count++;
            }
            continue;
        }

        // 处理数字（整数或浮点数）
        if (isdigit(source[i]) || source[i] == '0') {
            if (source[i] == '0') {
                i++;
                if (i < n && (source[i] == 'x' || source[i] == 'X')) {
                    i++;
                    char num[50] = "0x";
                    int j = 1;
                    while (i < n && (isdigit(source[i]) || (source[i] >= 'a' && source[i] <= 'f') || (source[i] >= 'A' && source[i] <= 'F'))) {
                        num[j++] = source[i++];
                    }
                    num[j] = '\0';
                    long dec = strtol(num, NULL, 16);
                    char dec_str[50];
                    sprintf(dec_str, "%ld", dec);
                    add_to_C1(dec_str);
                    if (Token_count < 100) {
                        sprintf(Token[Token_count], "(C1 %d)", C1_count);
                        Token_count++;
                    }
                    continue;
                }
                else {
                    char num[50] = "0";
                    int j = 1;
                    while (i < n && isdigit(source[i])) {
                        num[j++] = source[i++];
                    }
                    num[j] = '\0';
                    add_to_C1(num);
                    if (Token_count < 100) {
                        sprintf(Token[Token_count], "(C1 %d)", C1_count);
                        Token_count++;
                    }
                    continue;
                }
            }
            else {
                char num[50] = { 0 };
                int j = 0;
                while (i < n && isdigit(source[i])) {
                    num[j++] = source[i++];
                }
                num[j] = '\0';
                if (i < n && (source[i] == '.' || (source[i] == 'e' || source[i] == 'E'))) {
                    char float_num[50];
                    sprintf(float_num, "%s", num);
                    int float_valid = 1;  // 标记浮点数是否合法
                    if (source[i] == '.') {
                        float_num[j++] = source[i++];
                        if (i < n && isdigit(source[i])) {
                            while (i < n && isdigit(source[i])) {
                                float_num[j++] = source[i++];
                            }
                        }
                        else {
                            float_valid = 0;
                        }
                    }
                    if (i < n && (source[i] == 'e' || source[i] == 'E')) {
                        float_num[j++] = source[i++];
                        int exp_start = j;  // 记录指数部分起始位置

                        if (i < n && (source[i] == '+' || source[i] == '-')) {
                            float_num[j++] = source[i++];
                        }

                        // 读取指数部分的数字
                        int exp_digit_count = 0;  // 新增：统计指数部分数字个数
                        while (i < n && isdigit(source[i])) {
                            float_num[j++] = source[i++];
                            exp_digit_count++;
                        }

                        // 检查指数部分是否有数字（如"12e+"中exp_digit_count=0）
                        if (exp_digit_count == 0) {
                            float_valid = 0;
                        }
                    }

                    float_num[j] = '\0';

                    // 新增：若浮点数不合法，标记错误并返回
                    if (!float_valid) {
                        iscorret = 0;
                        return;
                    }

                    add_to_C2(float_num);
                    if (Token_count < 100) {
                        sprintf(Token[Token_count], "(C2 %d)", C2_count);
                        Token_count++;
                    }
                }
                else {
                    add_to_C1(num);
                    if (Token_count < 100) {
                        sprintf(Token[Token_count], "(C1 %d)", C1_count);
                        Token_count++;
                    }
                }
                continue;
            }
        }

        // 处理以.开头的浮点数
        if (source[i] == '.') {
            char float_num[50] = ".";
            i++;
            if (i < n && isdigit(source[i])) {
                int j = 1;
                while (i < n && isdigit(source[i])) {
                    float_num[j++] = source[i++];
                }
                if (i < n && (source[i] == 'e' || source[i] == 'E')) {
                    float_num[j++] = source[i++];
                    if (i < n && (source[i] == '+' || source[i] == '-')) {
                        float_num[j++] = source[i++];
                    }
                    while (i < n && isdigit(source[i])) {
                        float_num[j++] = source[i++];
                    }
                }
                float_num[j] = '\0';
                add_to_C2(float_num);
                if (Token_count < 100) {
                    sprintf(Token[Token_count], "(C2 %d)", C2_count);
                    Token_count++;
                }
            }
            else {
                if (Token_count < 100) {
                    iscorret = 0;
                }
                return;
            }
            continue;
        }

        // 其他错误情况
        if (Token_count < 100) {
            iscorret = 0;
        }
        return;
    }
}

void print_token_sequence() {
    printf("Token :");
    for (int i = 0; i < Token_count; i++) {
        printf("%s", Token[i]);
    }
}

// 输出各表内容
void print_tables() {
    printf("\nI :");
    for (int i = 0; i < id_count; i++) {
        if (i > 0) printf(" ");
        printf("%s", identifiers[i]);
    }
    printf("\nC1 :");
    for (int i = 0; i < C1_count; i++) {
        if (i > 0) printf(" ");
        printf("%s", C1[i]);
    }
    printf("\nC2 :");
    for (int i = 0; i < C2_count; i++) {
        if (i > 0) printf(" ");
        printf("%s", C2[i]);
    }
    printf("\nCT :");
    for (int i = 0; i < CT_count; i++) {
        if (i > 0) printf(" ");
        printf("%s", CT[i]);
    }
    printf("\nST :");
    for (int i = 0; i < ST_count; i++) {
        if (i > 0) printf(" ");
        printf("%s", ST[i]);
    }
}











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
void parse_declaration() {
    if (strstr(current_token, "(K 1)") || strstr(current_token, "(K 2)")) {
        // 变量声明
        parse_var_decl();
    }
    else if (strstr(current_token, "(I ")) {
        // 检查下一个Token以区分赋值语句/if/while
        char* next_token = lookahead(2);
        if (next_token && strstr(next_token, "(P 11)")) { // 赋值运算符"="
            parse_assign_stmt();
        }
        else if (lookahead(2) && strstr(lookahead(2), "(P 3)")) { // "("
            if (strstr(lookahead(1), "(K 12)")) { // if
                parse_if_stmt();
            }
            else if (strstr(lookahead(1), "(K 5)")) { // while
                parse_while_stmt();
            }
            else {
                syntax_error("非法的语句开头");
            }
        }
        else {
            syntax_error("无法识别的声明类型");
        }
    }
    else {
        syntax_error("缺少声明");
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
    while (current_token && (strstr(current_token, "(P 8)") || strstr(current_token, "(P 1)"))){
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






int main() {
    char source[1000];
    fgets(source, sizeof(source), stdin);
    source[strcspn(source, "\n")] = '\0'; // 去除换行符
    scan(source);
    if (iscorret == 1) {
        print_token_sequence();
        print_tables();
        printf("\n");
    }
    else {
        printf("词法分析ERROR");
        return 1;
    }

    // 初始化语法分析
    token_index = 0;
    current_token = lookahead(1);

    // 语法分析
    parse_program();
    if (!iscorret) {
        printf("\n语法分析失败\n");
        return 1;
    }

    // 输出结果
    printf("语法分析成功\n");
    print_tables();
    return 0;
}
