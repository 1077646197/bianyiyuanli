#pragma once
/*void active_information() {
    if (symbol_count == 0 || quad_count == 0) return;

    int active2[100][2] = { 0 }; // 临时活跃状态数组
    memset(active2, 0, sizeof(active2)); // 初始化

    // 全局活跃状态存储数组（确保在文件作用域声明）
    int active2_saved[200][100][2] = { 0 };

    // 初始化最后一个四元式的out集
    for (int j = 0; j < symbol_count; j++) {
        active2[j][1] = 0;
    }

    for (int i = quad_count - 1; i >= 0; i--) {
        Quadruple* q = &quad[i];
        int def_idx = -1;
        const char* result = q->result; // 新增，简化代码

        // 处理定义变量（排除临时变量和常量）
        if (strcmp(result, "_") != 0) {
            if (result[0] == 'T') { // 临时变量（如T1）
                continue; // 不加入符号表，跳过活跃分析
            }
            else if (!isdigit((unsigned char)result[0])) { // 非数字开头，可能是变量
                def_idx = find_symbol(result, current_scope); // 查找符号表
            }
        }

        // 处理使用变量（arg1和arg2）
        int use_vars[2] = { -1, -1 };
        int use_count = 0;

        // 处理arg1
        const char* arg1 = q->arg1;
        if (strcmp(arg1, "_") != 0) {
            if (!isdigit((unsigned char)arg1[0]) && arg1[0] != 'T') {
                int idx = find_symbol(arg1, current_scope);
                if (idx != -1) use_vars[use_count++] = idx;
            }
        }

        // 处理arg2
        const char* arg2 = q->arg2;
        if (strcmp(arg2, "_") != 0) {
            if (!isdigit((unsigned char)arg2[0]) && arg2[0] != 'T') {
                int idx = find_symbol(arg2, current_scope);
                if (idx != -1) use_vars[use_count++] = idx;
            }
        }

        // 处理后继四元式（控制流）
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

        // 合并后继out集
        memset(active2, 0, sizeof(active2)); // 重置当前out集
        for (int s = 0; s < succ_count; s++) {
            int succ_idx = successors[s];
            if (succ_idx >= 0 && succ_idx < quad_count) {
                for (int j = 0; j < symbol_count; j++) {
                    active2[j][1] |= active2_saved[succ_idx][j][1];
                }
            }
        }

        // 计算in集
        for (int j = 0; j < symbol_count; j++) {
            bool in_use = (use_count > 0) && (use_vars[0] == j || use_vars[1] == j);
            bool in_out_not_def = active2[j][1] && (def_idx != j);
            active2[j][0] = in_use || in_out_not_def;
        }

        // 处理变量定义对out集的影响（非临时变量）
        if (def_idx != -1) {
            active2[def_idx][1] = 0;
        }

        // 保存当前活跃状态（使用memcpy正确传递参数）
        memcpy(active2_saved[i], active2, sizeof(active2));
    }

    // 打印结果（省略部分代码，确保格式字符串与参数匹配）
    printf("\n活跃变量分析结果:\n");
    printf("四元式\t变量名\t前活跃\t后活跃\n");
    for (int i = 0; i < quad_count; i++) {
        for (int j = 0; j < symbol_count; j++) {
            printf("%d\t%-8s\t%s\t%s\n", i + 1,
                symbol_table[j].name,
                active2_saved[i][j][0] ? "是" : "否",
                active2_saved[i][j][1] ? "是" : "否");
        }
    }
}*/