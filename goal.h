#pragma once
/*void active_information() {
    if (symbol_count == 0 || quad_count == 0) return;

    int active2[100][2] = { 0 }; // ��ʱ��Ծ״̬����
    memset(active2, 0, sizeof(active2)); // ��ʼ��

    // ȫ�ֻ�Ծ״̬�洢���飨ȷ�����ļ�������������
    int active2_saved[200][100][2] = { 0 };

    // ��ʼ�����һ����Ԫʽ��out��
    for (int j = 0; j < symbol_count; j++) {
        active2[j][1] = 0;
    }

    for (int i = quad_count - 1; i >= 0; i--) {
        Quadruple* q = &quad[i];
        int def_idx = -1;
        const char* result = q->result; // �������򻯴���

        // ������������ų���ʱ�����ͳ�����
        if (strcmp(result, "_") != 0) {
            if (result[0] == 'T') { // ��ʱ��������T1��
                continue; // ��������ű�������Ծ����
            }
            else if (!isdigit((unsigned char)result[0])) { // �����ֿ�ͷ�������Ǳ���
                def_idx = find_symbol(result, current_scope); // ���ҷ��ű�
            }
        }

        // ����ʹ�ñ�����arg1��arg2��
        int use_vars[2] = { -1, -1 };
        int use_count = 0;

        // ����arg1
        const char* arg1 = q->arg1;
        if (strcmp(arg1, "_") != 0) {
            if (!isdigit((unsigned char)arg1[0]) && arg1[0] != 'T') {
                int idx = find_symbol(arg1, current_scope);
                if (idx != -1) use_vars[use_count++] = idx;
            }
        }

        // ����arg2
        const char* arg2 = q->arg2;
        if (strcmp(arg2, "_") != 0) {
            if (!isdigit((unsigned char)arg2[0]) && arg2[0] != 'T') {
                int idx = find_symbol(arg2, current_scope);
                if (idx != -1) use_vars[use_count++] = idx;
            }
        }

        // ��������Ԫʽ����������
        int successors[2] = { -1, -1 };
        int succ_count = 0;
        if (strcmp(q->op, "if") == 0 || strcmp(q->op, "while") == 0) {
            int target_quad = 0;
            if (sscanf(result, "T%d", &target_quad) == 1 && target_quad > 0) {
                successors[succ_count++] = target_quad - 1;
            }
            if (i + 1 < quad_count) successors[succ_count++] = i + 1;
        }
        else if (strcmp(q->op, "goto") == 0) {
            int target_quad = 0;
            if (sscanf(result, "T%d", &target_quad) == 1 && target_quad > 0) {
                successors[succ_count++] = target_quad - 1;
            }
        }
        else {
            if (i + 1 < quad_count) successors[succ_count++] = i + 1;
        }

        // �ϲ����out��
        memset(active2, 0, sizeof(active2)); // ���õ�ǰout��
        for (int s = 0; s < succ_count; s++) {
            int succ_idx = successors[s];
            if (succ_idx >= 0 && succ_idx < quad_count) {
                for (int j = 0; j < symbol_count; j++) {
                    active2[j][1] |= active2_saved[succ_idx][j][1];
                }
            }
        }

        // ����in��
        for (int j = 0; j < symbol_count; j++) {
            bool in_use = (use_count > 0) && (use_vars[0] == j || use_vars[1] == j);
            bool in_out_not_def = active2[j][1] && (def_idx != j);
            active2[j][0] = in_use || in_out_not_def;
        }

        // ������������out����Ӱ�죨����ʱ������
        if (def_idx != -1) {
            active2[def_idx][1] = 0;
        }

        // ���浱ǰ��Ծ״̬��ʹ��memcpy��ȷ���ݲ�����
        memcpy(active2_saved[i], active2, sizeof(active2));
    }

    // ��ӡ�����ʡ�Բ��ִ��룬ȷ����ʽ�ַ��������ƥ�䣩
    printf("\n��Ծ�����������:\n");
    printf("��Ԫʽ\t������\tǰ��Ծ\t���Ծ\n");
    for (int i = 0; i < quad_count; i++) {
        for (int j = 0; j < symbol_count; j++) {
            printf("%d\t%-8s\t%s\t%s\n", i + 1,
                symbol_table[j].name,
                active2_saved[i][j][0] ? "��" : "��",
                active2_saved[i][j][1] ? "��" : "��");
        }
    }
}*/