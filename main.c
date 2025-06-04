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
#define MAX_SOURCE_SIZE 1000
char source[MAX_SOURCE_SIZE];


int main() 
{
    FILE* file = fopen("program.txt", "r"); // 以只读模式打开文件

    if (file == NULL) { // 检查文件是否打开失败
        perror("Error opening file"); // 输出错误信息
        return ; // 退出程序
    }
    else  printf("打开文件 ");

    // 安全读取文件内容到source数组
    if (fgets(source, sizeof(source), file)) { // 读取一行（可能不完整）
        // 去除行末的换行符（如果有的话）
        size_t len = strlen(source);
        if (len > 0 && source[len - 1] == '\n') {
            source[len - 1] = '\0';
        }
    }
    else {
        // 如果文件为空或读取失败，清空source
        source[0] = '\0';
    }

    // 处理文件中可能存在的多行内容（补充完整读取逻辑）
    char tempBuffer[MAX_SOURCE_SIZE] = { 0 };
    while (!feof(file) && !ferror(file)) { // 循环读取直到文件结束或出错
        if (fgets(tempBuffer, sizeof(tempBuffer), file)) {
            size_t sourceLen = strlen(source);
            size_t tempLen = strlen(tempBuffer);

            // 确保总长度不超过MAX_SOURCE_SIZE
            if (sourceLen + tempLen < MAX_SOURCE_SIZE) {
                strcat(source, tempBuffer);

                // 去除追加内容后的换行符（可选）
                if (tempBuffer[tempLen - 1] == '\n') {
                    source[sourceLen + tempLen - 1] = '\0';
                }
            }
            else {
                // 处理超出缓冲区的情况（示例中简单截断）
                strncat(source, tempBuffer, MAX_SOURCE_SIZE - sourceLen - 1);
                source[MAX_SOURCE_SIZE - 1] = '\0';
                break; // 缓冲区已满，停止读取
            }
        }
    }

    fclose(file); // 关闭文件
//===========================================
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
    // 调用词法分析函数（原scan函数）
 //======================================
    scan(source);
    if (iscorret == 1) {
        print_token_sequence();
        print_tables();
    }
    else {
        printf("编译错误！！！！");
    }
    return 0;
}
