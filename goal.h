#pragma once
#define MAX_QUAD 1000
#define MAX_SYMBOL 500
void active_information() {
    if (symbol_count == 0 || quad_count == 0) return;

    // ʹ���㹻��ľ�̬���飨����ʵ�����������
    int active2[MAX_SYMBOL][2] = { 0 }; // ��ʱ��Ծ״̬����
    int active2_saved[MAX_QUAD][MAX_SYMBOL][2] = { 0 }; // ȫ�ֻ�Ծ״̬�洢

    // ��ʼ�����һ����Ԫʽ��out��
    for (int j = 0; j < symbol_count; j++) {
        active2[j][1] = 0; // out[B] ��ʼΪ��
    }

    // ���������Ԫʽ
    for (int i = quad_count - 1; i >= 0; i--) {
        Quadruple* q = &quad[i];
        int def_idx = -1;
        const char* result = q->result;

        // �����������������ʱ�����ͳ�����
        if (strcmp(result, "_") != 0 && !(result[0] == 'T' && isdigit(result[1]))){
            def_idx = find_symbol(result, current_scope); // ����Ϊ-1���ǵ�ǰ������
        }

        // ����ʹ�ñ�����arg1��arg2��
        int use_vars[2] = { -1, -1 };
            int use_count = 0;

            // ���arg1
            const char* arg1 = q->arg1;
            if (strcmp(arg1, "_") != 0 && !(arg1[0] == 'T' && isdigit(arg1[1]) && !isdigit((unsigned char)arg1[0]))) {
                int idx = find_symbol(arg1, current_scope);
                if (idx != -1) use_vars[use_count++] = idx;
            }

        // ���arg2
        const char* arg2 = q->arg2;
            if (strcmp(arg2, "_") != 0 && !(arg2[0] == 'T' && isdigit(arg2[1]) && !isdigit((unsigned char)arg2[0]))){
                int idx = find_symbol(arg2, current_scope);
                if (idx != -1) use_vars[use_count++] = idx;
            }

        // ȷ�����ָ��
        int successors[2] = { -1, -1 };
            int succ_count = 0;

            if (strcmp(q->op, "if") == 0 || strcmp(q->op, "while") == 0) {
                int target_quad;
                // ������ת��ǩ����ʽΪ"T����"��
                    if (sscanf(result, "T%d", &target_quad) == 1 && target_quad <= quad_count && target_quad > 0) {
                        successors[succ_count++] = target_quad - 1; // ��תĿ��
                    }
                // ������ת����˳���̣�����Ϊ��ʱִ�У�
                if (i + 1 < quad_count) {
                    successors[succ_count++] = i + 1;
                }
            }
            else if (strcmp(q->op, "goto") == 0) {
                int target_quad;
                if (sscanf(result, "T%d", &target_quad) == 1 && target_quad <= quad_count && target_quad > 0) {
                    successors[succ_count++] = target_quad - 1; // Ψһ���
                }
            }
            else {
                // ��ָͨ����Ϊ��һ��ָ��
                if (i + 1 < quad_count) {
                    successors[succ_count++] = i + 1;
                }
            }

        // ���ò��ϲ���̵�IN��Ծ״̬
        memset(active2, 0, sizeof(active2));
        for (int s = 0; s < succ_count; s++) {
            int succ_idx = successors[s];
            if (succ_idx >= 0 && succ_idx < quad_count) {
                for (int j = 0; j < symbol_count; j++) {
                    // �ؼ��޸���ʹ�ú��ָ���IN״̬����OUT״̬��
                    active2[j][1] |= active2_saved[succ_idx][j][0];
                }
            }
        }

        // ���㵱ǰָ���IN��Ծ״̬
        for (int j = 0; j < symbol_count; j++) {
            int is_used = 0;
            for (int k = 0; k < use_count; k++) {
                if (use_vars[k] == j) {
                    is_used = 1;
                    break;
                }
            }
            // ���������̣�in[B] = use[B] OR (out[B] AND not def[B])
            active2[j][0] = is_used || (active2[j][1] && (def_idx != j));
        }

        // ���浱ǰָ��Ļ�Ծ״̬
        memcpy(active2_saved[i], active2, sizeof(active2));
    }

    // ��ӡ��������޵�ǰ��������ţ�
    printf("\n��Ծ�����������:\n");
    printf("��Ԫʽ\t������\tǰ��Ծ\t���Ծ\n");
    for (int i = 0; i < quad_count; i++) {
        for (int j = 0; j < symbol_count; j++) {
            // �����ǵ�ǰ���������
            if (symbol_table[j].scope != current_scope) continue;

            printf("%d\t%-8s\t%s\t%s\n", i + 1,
                symbol_table[j].name,
                active2_saved[i][j][0] ? "��" : "��",  // IN״̬
                active2_saved[i][j][1] ? "��" : "��"); // OUT״̬
        }
    }
}