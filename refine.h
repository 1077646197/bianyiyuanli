#pragma once
//���ļ���������Ԫʽ���н��м��Ż���֧��������������Ĳ�
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
