/*·程序定义
35464648489ktutvfkdt
程序结构
<程序> → <函数定义>
<函数定义> → <返回类型> main() <函数体>
<返回类型> → int | void
<函数体> → { <语句列表> }

数据类型与变量
<数据类型> → int | float | char
<变量声明> → <数据类型> <标识符>;
<标识符> → <字母><字母数字>*
<字母> → a|b|...|z|A|B|...|Z
<字母数字> → <字母>|<数字>
<数字> → 0|1|...|9

表达式
<表达式> → <变量> | <常量> | <表达式><运算符><表达式>
<变量> → <标识符>
<常量> → <整数常量> | <字符常量>
<整数常量> → [0-9]+
<字符常量> → 'a'|'b'|...|'z'
<运算符> → + | - | * | / | =

语句
<语句列表> → <语句> | <语句><语句列表>
<语句> → <赋值语句> |   <条件语句> | <循环语句> | <声明语句>
<赋值语句> → <变量> = <表达式>;
<字符串> → "..."
<条件语句> → if (<表达式>) <语句> | if (<表达式>) <语句> else <语句>
<循环语句> → while (<表达式>) <语句>
<声明语句> → <变量声明>
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


int main() 
{
    read_file("program.txt");
    // 调用词法分析函数（原scan函数）
    //======================================
    scan(source);
    if (iscorret == 1) {
        print_token_sequence();
        print_tables();
    }
    else {
        printf("编译失败，程序存在错误！！！！\n");
    }
    printf("\n");
//===========================================
 /*
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
    printf("\n语法分析成功\n"); */
//==================================================
// 
    // 执行语义分析
    analyze_program();
    // 输出符号表结果
    printf("\n符号表内容:\n");
    printf("名称\t类型\t初始化\t作用域\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("%s\t", symbol_table[i].name);
        typetrans(symbol_table[i].type);
        printf("\t%d\t%d\n", symbol_table[i].initialized,symbol_table[i].scope);
    }
    return 0;
}
