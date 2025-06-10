#pragma once
#define MAX_QUAD 1000
#define MAX_SYMBOL 500
void active_information() {
    if (symbol_count == 0 || quad_count == 0) return;

    // 使用足够大的静态数组（根据实际需求调整）
    int active2[MAX_SYMBOL][2] = { 0 }; // 临时活跃状态数组
    int active2_saved[MAX_QUAD][MAX_SYMBOL][2] = { 0 }; // 全局活跃状态存储

    // 初始化最后一个四元式的out集
    for (int j = 0; j < symbol_count; j++) {
        active2[j][1] = 0; // out[B] 初始为空
    }

    // 逆序遍历四元式
    for (int i = quad_count - 1; i >= 0; i--) {
        Quadruple* q = &quad[i];
        int def_idx = -1;
        const char* result = q->result;

        // 处理定义变量（跳过临时变量和常量）
        if (strcmp(result, "_") != 0 && !(result[0] == 'T' && isdigit(result[1]))){
            def_idx = find_symbol(result, current_scope); // 可能为-1（非当前作用域）
        }

        // 处理使用变量（arg1和arg2）
        int use_vars[2] = { -1, -1 };
            int use_count = 0;

            // 检查arg1
            const char* arg1 = q->arg1;
            if (strcmp(arg1, "_") != 0 && !(arg1[0] == 'T' && isdigit(arg1[1]) && !isdigit((unsigned char)arg1[0]))) {
                int idx = find_symbol(arg1, current_scope);
                if (idx != -1) use_vars[use_count++] = idx;
            }

        // 检查arg2
        const char* arg2 = q->arg2;
            if (strcmp(arg2, "_") != 0 && !(arg2[0] == 'T' && isdigit(arg2[1]) && !isdigit((unsigned char)arg2[0]))){
                int idx = find_symbol(arg2, current_scope);
                if (idx != -1) use_vars[use_count++] = idx;
            }

        // 确定后继指令
        int successors[2] = { -1, -1 };
            int succ_count = 0;

            if (strcmp(q->op, "if") == 0 || strcmp(q->op, "while") == 0) {
                int target_quad;
                // 解析跳转标签（格式为"T数字"）
                    if (sscanf(result, "T%d", &target_quad) == 1 && target_quad <= quad_count && target_quad > 0) {
                        successors[succ_count++] = target_quad - 1; // 跳转目标
                    }
                // 条件跳转总有顺序后继（条件为假时执行）
                if (i + 1 < quad_count) {
                    successors[succ_count++] = i + 1;
                }
            }
            else if (strcmp(q->op, "goto") == 0) {
                int target_quad;
                if (sscanf(result, "T%d", &target_quad) == 1 && target_quad <= quad_count && target_quad > 0) {
                    successors[succ_count++] = target_quad - 1; // 唯一后继
                }
            }
            else {
                // 普通指令：后继为下一条指令
                if (i + 1 < quad_count) {
                    successors[succ_count++] = i + 1;
                }
            }

        // 重置并合并后继的IN活跃状态
        memset(active2, 0, sizeof(active2));
        for (int s = 0; s < succ_count; s++) {
            int succ_idx = successors[s];
            if (succ_idx >= 0 && succ_idx < quad_count) {
                for (int j = 0; j < symbol_count; j++) {
                    // 关键修复：使用后继指令的IN状态（非OUT状态）
                    active2[j][1] |= active2_saved[succ_idx][j][0];
                }
            }
        }

        // 计算当前指令的IN活跃状态
        for (int j = 0; j < symbol_count; j++) {
            int is_used = 0;
            for (int k = 0; k < use_count; k++) {
                if (use_vars[k] == j) {
                    is_used = 1;
                    break;
                }
            }
            // 数据流方程：in[B] = use[B] OR (out[B] AND not def[B])
            active2[j][0] = is_used || (active2[j][1] && (def_idx != j));
        }

        // 保存当前指令的活跃状态
        memcpy(active2_saved[i], active2, sizeof(active2));
    }

    // 打印结果（仅限当前作用域符号）
    printf("\n活跃变量分析结果:\n");
    printf("四元式\t变量名\t前活跃\t后活跃\n");
    for (int i = 0; i < quad_count; i++) {
        for (int j = 0; j < symbol_count; j++) {
            // 跳过非当前作用域符号
            if (symbol_table[j].scope != current_scope) continue;

            printf("%d\t%-8s\t%s\t%s\n", i + 1,
                symbol_table[j].name,
                active2_saved[i][j][0] ? "是" : "否",  // IN状态
                active2_saved[i][j][1] ? "是" : "否"); // OUT状态
        }
    }
}