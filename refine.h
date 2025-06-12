#pragma once
//本文件对已有四元式序列进行简单优化，支持连续数字运算的合并，消除多余的四元式
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

// 基本块结构定义
typedef struct {
    int start;          // 基本块起始四元式索引
    int end;            // 基本块结束四元式索引
    int next[2];        // 后继基本块索引（最多两个：true和false分支）
    int next_count;     // 后继基本块数量
    int prev[10];       // 前驱基本块索引（最多10个）
    int prev_count;     // 前驱基本块数量
} BasicBlock;

BasicBlock blocks[100];  // 基本块数组
int block_count = 0;     // 基本块数量

// 为四元式划分基本块
void partition_basic_blocks() {
    // 重置基本块计数
    block_count = 0;

    // 步骤1：标记入口点
    int entry_points[100] = { 0 };  // 入口点标记数组
    int entry_count = 0;

    // 第一个四元式总是入口点
    entry_points[entry_count++] = 0;

    // 扫描所有四元式，标记入口点
    for (int i = 0; i < quad_count; i++) {
        // 检查是否为跳转目标
        for (int j = 0; j < quad_count; j++) {
            // if语句的条件跳转目标
            if (strcmp(quad[j].op, "if") == 0 &&
                strcmp(quad[j].arg1, quad[i].result) == 0) {
                entry_points[entry_count++] = i;
                break;
            }

            // goto语句的跳转目标
            if (strcmp(quad[j].op, "goto") == 0 &&
                strcmp(quad[j].result, quad[i].result) == 0) {
                entry_points[entry_count++] = i;
                break;
            }

            // while循环的开始和结束位置
            if (strcmp(quad[j].op, "wl") == 0 &&
                strcmp(quad[j].result, quad[i].result) == 0) {
                entry_points[entry_count++] = i;
                break;
            }

            if (strcmp(quad[j].op, "we") == 0 &&
                strcmp(quad[j].result, quad[i].result) == 0) {
                entry_points[entry_count++] = i;
                break;
            }
        }

        // 检查是否为跳转指令后的指令
        if (i > 0 && (strcmp(quad[i - 1].op, "goto") == 0 ||
            strcmp(quad[i - 1].op, "if") == 0 ||
            strcmp(quad[i - 1].op, "ie") == 0 ||
            strcmp(quad[i - 1].op, "we") == 0)) {
            entry_points[entry_count++] = i;
        }
    }

    // 步骤2：去重并排序入口点
    for (int i = 0; i < entry_count; i++) {
        for (int j = i + 1; j < entry_count; j++) {
            if (entry_points[i] == entry_points[j]) {
                entry_points[j] = -1;  // 标记重复项
            }
        }
    }

    // 排序并移除重复项
    int sorted_entries[100] = { 0 };
    int sorted_count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entry_points[i] != -1) {
            sorted_entries[sorted_count++] = entry_points[i];
        }
    }

    // 对入口点进行排序
    for (int i = 0; i < sorted_count - 1; i++) {
        for (int j = 0; j < sorted_count - i - 1; j++) {
            if (sorted_entries[j] > sorted_entries[j + 1]) {
                int temp = sorted_entries[j];
                sorted_entries[j] = sorted_entries[j + 1];
                sorted_entries[j + 1] = temp;
            }
        }
    }

    // 步骤3：划分基本块
    for (int i = 0; i < sorted_count; i++) {
        blocks[block_count].start = sorted_entries[i];
        blocks[block_count].next_count = 0;
        blocks[block_count].prev_count = 0;

        // 确定基本块的结束位置
        if (i < sorted_count - 1) {
            blocks[block_count].end = sorted_entries[i + 1] - 1;
        }
        else {
            blocks[block_count].end = quad_count - 1;
        }

        block_count++;
    }

    // 步骤4：建立基本块之间的控制流关系
    for (int i = 0; i < block_count; i++) {
        // 检查基本块的最后一条指令
        int last_index = blocks[i].end;

        // 如果是条件跳转指令
        if (strcmp(quad[last_index].op, "if") == 0) {
            // 查找跳转目标所在的基本块
            char* target = quad[last_index].arg1;
            for (int j = 0; j < block_count; j++) {
                if (strcmp(quad[blocks[j].start].result, target) == 0) {
                    blocks[i].next[blocks[i].next_count++] = j;
                    break;
                }
            }

            // 条件不满足时的下一个基本块
            if (i < block_count - 1) {
                blocks[i].next[blocks[i].next_count++] = i + 1;
            }
        }
        // 如果是无条件跳转指令
        else if (strcmp(quad[last_index].op, "goto") == 0) {
            // 查找跳转目标所在的基本块
            char* target = quad[last_index].result;
            for (int j = 0; j < block_count; j++) {
                if (strcmp(quad[blocks[j].start].result, target) == 0) {
                    blocks[i].next[blocks[i].next_count++] = j;
                    break;
                }
            }
        }
        // 如果是if结束或while结束指令
        else if (strcmp(quad[last_index].op, "ie") == 0 ||
            strcmp(quad[last_index].op, "we") == 0) {
            // 查找跳转目标所在的基本块
            char* target = quad[last_index].result;
            for (int j = 0; j < block_count; j++) {
                if (strcmp(quad[blocks[j].start].result, target) == 0) {
                    blocks[i].next[blocks[i].next_count++] = j;
                    break;
                }
            }
        }
        // 如果是普通指令且不是最后一个基本块
        else if (i < block_count - 1) {
            blocks[i].next[blocks[i].next_count++] = i + 1;
        }
    }

    // 步骤5：建立前驱关系
    for (int i = 0; i < block_count; i++) {
        for (int j = 0; j < blocks[i].next_count; j++) {
            int next_block = blocks[i].next[j];
            blocks[next_block].prev[blocks[next_block].prev_count++] = i;
        }
    }
}

// 打印基本块信息
void print_basic_blocks() {
    printf("\n基本块划分结果:\n");
    printf("基本块数量: %d\n", block_count);

    for (int i = 0; i < block_count; i++) {
        printf("\n基本块 %d:\n", i);
        printf("起始四元式: %d, 结束四元式: %d\n",
            blocks[i].start + 1, blocks[i].end + 1);

        printf("四元式内容:\n");
        for (int j = blocks[i].start; j <= blocks[i].end; j++) {
            printf("%d\t(%s\t%s\t%s\t%s)\n",
                j + 1, quad[j].op, quad[j].arg1, quad[j].arg2, quad[j].result);
        }

        printf("后继基本块: ");
        for (int j = 0; j < blocks[i].next_count; j++) {
            printf("%d ", blocks[i].next[j]);
        }
        printf("\n前驱基本块: ");
        for (int j = 0; j < blocks[i].prev_count; j++) {
            printf("%d ", blocks[i].prev[j]);
        }
        printf("\n");
    }
}