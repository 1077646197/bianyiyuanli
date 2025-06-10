#pragma once
//本文件对已有四元式序列进行简单优化，支持连续数字运算的并
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




// 辅助函数：判断操作数是否为整型常量
int is_integer_constant(char* operand) {
    for (int i = 0; i < const_count; i++) {
        if (strcmp(const_table[i].value, operand) == 0 && const_table[i].type == 1) {
            return 1; // 是整型常量
        }
    }
    return 0; // 不是整型常量
}

// 检查并优化整数运算四元式
void optimize_integer_quadruples() {
    printf("\n开始优化整数运算四元式...\n");
    int optimized_count = 0; // 记录优化次数

    // 从后往前遍历四元式
    int i = quad_count - 1;
    while (i >= 0) {
        Quadruple* q = &quad[i];

        // 仅处理算术运算符（+、-、*、/）且结果为临时变量（以"T"开头）
        if (strncmp(q->result, "T", 1) == 0 &&
            (strcmp(q->op, "+") == 0 || strcmp(q->op, "-") == 0 ||
                strcmp(q->op, "*") == 0 || strcmp(q->op, "/") == 0)) {

            // 检查两个操作数是否均为整型常量
            if (is_integer_constant(q->arg1) && is_integer_constant(q->arg2)) {
                int val1 = atoi(q->arg1);
                int val2 = atoi(q->arg2);
                int result_val;

                // 执行算术运算
                switch (q->op[0]) {
                case '+': result_val = val1 + val2; break;
                case '-': result_val = val1 - val2; break;
                case '*': result_val = val1 * val2; break;
                case '/':
                    if (val2 == 0) {
                        semantic_error("错误：除数不能为零");
                    }
                    result_val = val1 / val2;
                    break;
                default: i--; continue; // 非算术运算符跳过
                }

                // 转换为字符串并添加到常数表
                char result_str[50];
                sprintf(result_str, "%d", result_val);
                add_constant(result_str, 1); // 类型1为整型

                // 将所有引用该临时变量的地方替换为常量值
                char* old_temp = q->result;
                for (int j = 0; j < quad_count; j++) {
                    if (strcmp(quad[j].arg1, old_temp) == 0) {
                        strcpy(quad[j].arg1, result_str);
                    }
                    if (strcmp(quad[j].arg2, old_temp) == 0) {
                        strcpy(quad[j].arg2, result_str);
                    }
                }

                // 从四元式表中删除当前四元式
                for (int j = i; j < quad_count - 1; j++) {
                    quad[j] = quad[j + 1];
                }
                quad_count--; // 四元式数量减一
                optimized_count++;

                printf("优化成功：消除四元式 %d（%s %s %s -> %s）\n",
                    i + 1, q->op, q->arg1, q->arg2, old_temp);

                // 注意：删除四元式后，i不需要递减，因为下一个四元式已经移到当前位置
                continue;
            }
        }
        i--; // 只有在未删除四元式时才递减索引
    }

    printf("整数运算四元式优化完成！共优化 %d 个四元式\n", optimized_count);
}
