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
#include "scanner.h"
#include "gramlyzer.h"
#include "fopen.h"
#include "semlyzer.h"
#include "refine.h"
#include "goal.h"

int main() 
{
    read_file("program.txt");
    printf("============================\n");
    // 调用词法分析函数（原scan函数）
    //======================================
    scan(source);
    if (iscorret == 1) {
        print_token_sequence();
        print_tables();
    }
    else {
        printf("编译失败，程序存在错误！！！！\n");
        return;
    }
    printf("\n");
//===========================================
 
    printf("文法规则:\n");
    printf("============================\n");
    print_grammar();
    printf("============================\n");
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
    printf("\n语法分析成功\n"); 
//==================================================
 
    // 执行语义分析并输出符号表结果
   analyze_program();

    return 0;
}
