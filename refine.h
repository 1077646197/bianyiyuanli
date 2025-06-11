#pragma once
//���ļ���������Ԫʽ���н��м��Ż���֧��������������ĺϲ��������������Ԫʽ
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



// �����������жϲ������Ƿ�Ϊ���ͳ���
int is_integer_constant(char* operand) {
    for (int i = 0; i < const_count; i++) {
        if (strcmp(const_table[i].value, operand) == 0 && const_table[i].type == 1) {
            return 1; // �����ͳ���
        }
    }
    return 0; // �������ͳ���
}

// ��鲢�Ż�����������Ԫʽ
void optimize_integer_quadruples() {
    printf("\n��ʼ�Ż�����������Ԫʽ...\n");
    int optimized_count = 0; // ��¼�Ż�����

    // �Ӻ���ǰ������Ԫʽ
    int i = quad_count - 1;
    while (i >= 0) {
        Quadruple* q = &quad[i];

        // �����������������+��-��*��/���ҽ��Ϊ��ʱ��������"T"��ͷ��
        if (strncmp(q->result, "T", 1) == 0 &&
            (strcmp(q->op, "+") == 0 || strcmp(q->op, "-") == 0 ||
                strcmp(q->op, "*") == 0 || strcmp(q->op, "/") == 0)) {

            // ��������������Ƿ��Ϊ���ͳ���
            if (is_integer_constant(q->arg1) && is_integer_constant(q->arg2)) {
                int val1 = atoi(q->arg1);
                int val2 = atoi(q->arg2);
                int result_val;

                // ִ����������
                switch (q->op[0]) {
                case '+': result_val = val1 + val2; break;
                case '-': result_val = val1 - val2; break;
                case '*': result_val = val1 * val2; break;
                case '/':
                    if (val2 == 0) {
                        semantic_error("���󣺳�������Ϊ��");
                    }
                    result_val = val1 / val2;
                    break;
                default: i--; continue; // ���������������
                }

                // ת��Ϊ�ַ�������ӵ�������
                char result_str[50];
                sprintf(result_str, "%d", result_val);
                add_constant(result_str, 1); // ����1Ϊ����

                // ���������ø���ʱ�����ĵط��滻Ϊ����ֵ
                char* old_temp = q->result;
                for (int j = 0; j < quad_count; j++) {
                    if (strcmp(quad[j].arg1, old_temp) == 0) {
                        strcpy(quad[j].arg1, result_str);
                    }
                    if (strcmp(quad[j].arg2, old_temp) == 0) {
                        strcpy(quad[j].arg2, result_str);
                    }
                }

                // ����Ԫʽ����ɾ����ǰ��Ԫʽ
                for (int j = i; j < quad_count - 1; j++) {
                    quad[j] = quad[j + 1];
                }
                quad_count--; // ��Ԫʽ������һ
                optimized_count++;

                printf("�Ż��ɹ���������Ԫʽ %d��%s %s %s -> %s��\n",
                    i + 1, q->op, q->arg1, q->arg2, old_temp);

                // ע�⣺ɾ����Ԫʽ��i����Ҫ�ݼ�����Ϊ��һ����Ԫʽ�Ѿ��Ƶ���ǰλ��
                continue;
            }
        }
        i--; // ֻ����δɾ����Ԫʽʱ�ŵݼ�����
    }

    printf("����������Ԫʽ�Ż���ɣ����Ż� %d ����Ԫʽ\n", optimized_count);
}

// ������ṹ����
typedef struct {
    int start;          // ��������ʼ��Ԫʽ����
    int end;            // �����������Ԫʽ����
    int next[2];        // ��̻��������������������true��false��֧��
    int next_count;     // ��̻���������
    int prev[10];       // ǰ�����������������10����
    int prev_count;     // ǰ������������
} BasicBlock;

BasicBlock blocks[100];  // ����������
int block_count = 0;     // ����������

// Ϊ��Ԫʽ���ֻ�����
void partition_basic_blocks() {
    // ���û��������
    block_count = 0;

    // ����1�������ڵ�
    int entry_points[100] = { 0 };  // ��ڵ�������
    int entry_count = 0;

    // ��һ����Ԫʽ������ڵ�
    entry_points[entry_count++] = 0;

    // ɨ��������Ԫʽ�������ڵ�
    for (int i = 0; i < quad_count; i++) {
        // ����Ƿ�Ϊ��תĿ��
        for (int j = 0; j < quad_count; j++) {
            // if����������תĿ��
            if (strcmp(quad[j].op, "if") == 0 &&
                strcmp(quad[j].arg1, quad[i].result) == 0) {
                entry_points[entry_count++] = i;
                break;
            }

            // goto������תĿ��
            if (strcmp(quad[j].op, "goto") == 0 &&
                strcmp(quad[j].result, quad[i].result) == 0) {
                entry_points[entry_count++] = i;
                break;
            }

            // whileѭ���Ŀ�ʼ�ͽ���λ��
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

        // ����Ƿ�Ϊ��תָ����ָ��
        if (i > 0 && (strcmp(quad[i - 1].op, "goto") == 0 ||
            strcmp(quad[i - 1].op, "if") == 0 ||
            strcmp(quad[i - 1].op, "ie") == 0 ||
            strcmp(quad[i - 1].op, "we") == 0)) {
            entry_points[entry_count++] = i;
        }
    }

    // ����2��ȥ�ز�������ڵ�
    for (int i = 0; i < entry_count; i++) {
        for (int j = i + 1; j < entry_count; j++) {
            if (entry_points[i] == entry_points[j]) {
                entry_points[j] = -1;  // ����ظ���
            }
        }
    }

    // �����Ƴ��ظ���
    int sorted_entries[100] = { 0 };
    int sorted_count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entry_points[i] != -1) {
            sorted_entries[sorted_count++] = entry_points[i];
        }
    }

    // ����ڵ��������
    for (int i = 0; i < sorted_count - 1; i++) {
        for (int j = 0; j < sorted_count - i - 1; j++) {
            if (sorted_entries[j] > sorted_entries[j + 1]) {
                int temp = sorted_entries[j];
                sorted_entries[j] = sorted_entries[j + 1];
                sorted_entries[j + 1] = temp;
            }
        }
    }

    // ����3�����ֻ�����
    for (int i = 0; i < sorted_count; i++) {
        blocks[block_count].start = sorted_entries[i];
        blocks[block_count].next_count = 0;
        blocks[block_count].prev_count = 0;

        // ȷ��������Ľ���λ��
        if (i < sorted_count - 1) {
            blocks[block_count].end = sorted_entries[i + 1] - 1;
        }
        else {
            blocks[block_count].end = quad_count - 1;
        }

        block_count++;
    }

    // ����4������������֮��Ŀ�������ϵ
    for (int i = 0; i < block_count; i++) {
        // ������������һ��ָ��
        int last_index = blocks[i].end;

        // �����������תָ��
        if (strcmp(quad[last_index].op, "if") == 0) {
            // ������תĿ�����ڵĻ�����
            char* target = quad[last_index].arg1;
            for (int j = 0; j < block_count; j++) {
                if (strcmp(quad[blocks[j].start].result, target) == 0) {
                    blocks[i].next[blocks[i].next_count++] = j;
                    break;
                }
            }

            // ����������ʱ����һ��������
            if (i < block_count - 1) {
                blocks[i].next[blocks[i].next_count++] = i + 1;
            }
        }
        // �������������תָ��
        else if (strcmp(quad[last_index].op, "goto") == 0) {
            // ������תĿ�����ڵĻ�����
            char* target = quad[last_index].result;
            for (int j = 0; j < block_count; j++) {
                if (strcmp(quad[blocks[j].start].result, target) == 0) {
                    blocks[i].next[blocks[i].next_count++] = j;
                    break;
                }
            }
        }
        // �����if������while����ָ��
        else if (strcmp(quad[last_index].op, "ie") == 0 ||
            strcmp(quad[last_index].op, "we") == 0) {
            // ������תĿ�����ڵĻ�����
            char* target = quad[last_index].result;
            for (int j = 0; j < block_count; j++) {
                if (strcmp(quad[blocks[j].start].result, target) == 0) {
                    blocks[i].next[blocks[i].next_count++] = j;
                    break;
                }
            }
        }
        // �������ָͨ���Ҳ������һ��������
        else if (i < block_count - 1) {
            blocks[i].next[blocks[i].next_count++] = i + 1;
        }
    }

    // ����5������ǰ����ϵ
    for (int i = 0; i < block_count; i++) {
        for (int j = 0; j < blocks[i].next_count; j++) {
            int next_block = blocks[i].next[j];
            blocks[next_block].prev[blocks[next_block].prev_count++] = i;
        }
    }
}

// ��ӡ��������Ϣ
void print_basic_blocks() {
    printf("\n�����黮�ֽ��:\n");
    printf("����������: %d\n", block_count);

    for (int i = 0; i < block_count; i++) {
        printf("\n������ %d:\n", i);
        printf("��ʼ��Ԫʽ: %d, ������Ԫʽ: %d\n",
            blocks[i].start + 1, blocks[i].end + 1);

        printf("��Ԫʽ����:\n");
        for (int j = blocks[i].start; j <= blocks[i].end; j++) {
            printf("%d\t(%s\t%s\t%s\t%s)\n",
                j + 1, quad[j].op, quad[j].arg1, quad[j].arg2, quad[j].result);
        }

        printf("��̻�����: ");
        for (int j = 0; j < blocks[i].next_count; j++) {
            printf("%d ", blocks[i].next[j]);
        }
        printf("\nǰ��������: ");
        for (int j = 0; j < blocks[i].prev_count; j++) {
            printf("%d ", blocks[i].prev[j]);
        }
        printf("\n");
    }
}