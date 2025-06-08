

//<输入输出语句> → printf(<字符串>); | scanf(<格式串>, <变量>);
//<格式串> → "%d" | "%f" | "%c"



#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
 // 关键字表，第一列是关键字，第二列是编号
char* keywords[][2] = {
    {"int", "1"},
    {"void", "2"},
    {"break", "3"},
    {"float", "4"},
    {"while", "5"},
    {"do", "6"},
    {"struct", "7"},
    {"const", "8"},
    {"case", "9"},
    {"for", "10"},
    {"return", "11"},
    {"if", "12"},
    {"default", "13"},
    {"else", "14"},
    {"main", "15"},
    {"char","16"},
    {"bool","17"}
};
int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

// 界符表，第一列是界符，第二列是编号
char* delimiters[][2] = {
    {"-", "1"},
    {"/", "2"},
    {"(", "3"},
    {")", "4"},
    {"==", "5"},
    {"<=", "6"},
    {"<", "7"},
    {"+", "8"},
    {"*", "9"},
    {">", "10"},
    {"=", "11"},
    {",", "12"},
    {";", "13"},
    {"++", "14"},
    {"{", "15"},
    {"}", "16"},
    {"!=","17"},
    {">=","18"},
    {"--", "19"}
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

char Token[100][20]; // Token串表
int Token_count = 0;

char bool[10][20]; // Token串表
int bool_count = 0;

int iscorret = 1;//是否正确

// 向标识符表添加元素
int add_to_identifiers(const char* str) {
    for (int i = 0; i < id_count; i++) {
        if (strcmp(identifiers[i], str) == 0) return i+1;
    }
    strcpy(identifiers[id_count], str);
    return ++id_count;
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
        if (strcmp(CT[i], str) == 0)
            return;
    }
    strcpy(CT[CT_count++], str);
}

// 向字符串常量表添加元素
void add_to_ST(const char* str) {
    for (int i = 0; i < ST_count; i++) {
        if (strcmp(ST[i], str) == 0)
            return;
    }
    strcpy(ST[ST_count++], str);
}

// 检查是否是关键字，是则返回编号，否则返回0
int is_keyword(const char* str) {
    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(keywords[i][0], str) == 0)
            return atoi(keywords[i][1]);
    }
    return 0;
}

// 检查是否是界符，是则返回编号，否则返回0
int is_delimiter(const char* str) {
    for (int i = 0; i < delimiter_count; i++) {
        if (strcmp(delimiters[i][0], str) == 0)
            return atoi(delimiters[i][1]);
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
                int index =  add_to_identifiers(id);
                if (Token_count < 100) {
                    sprintf(Token[Token_count], "(I %d)", index);
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
    printf("Token序列为 :");
    for (int i = 0; i < Token_count; i++) {
        printf("%s", Token[i]);
    }
}

// 输出各表内容
void print_tables() {
    printf("\n标识符表 :");
    for (int i = 0; i < id_count; i++) {
        if (i > 0) printf(" |");
        printf("%s", identifiers[i]);
    }
    printf("\n整数表 :");
    for (int i = 0; i < C1_count; i++) {
        if (i > 0) printf(" |");
        printf("%s", C1[i]);
    }
    printf("\n非整数表 :");
    for (int i = 0; i < C2_count; i++) {
        if (i > 0) printf(" |");
        printf("%s", C2[i]);
    }
    printf("\n字符常量表 :");
    for (int i = 0; i < CT_count; i++) {
        if (i > 0) printf(" |");
        printf("%s", CT[i]);
    }
    printf("\n字符串常量表 :");
    for (int i = 0; i < ST_count; i++) {
        if (i > 0) printf(" |");
        printf("%s", ST[i]);
    }
    printf("\n");
}