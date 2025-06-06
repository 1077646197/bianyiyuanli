#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void semantic_error(const char* message);        // ������������Ϣ����ֹ����
int find_symbol(const char* name, int scope);    // �ڷ��ű��в��ұ���������������-1��
void add_symbol(const char* name, int type);     // ����±��������ű�����ظ���
void check_variable(const char* name);           // �������Ƿ�������ѳ�ʼ��
void set_initialized(const char* name);          // ��Ǳ���Ϊ�ѳ�ʼ��״̬
int get_type(const char* name);                  // ��ȡ������������
int check_expression_type();                     // �����ʽ���ͣ����������룩
void generate_quad(const char* op, const char* arg1, const char* arg2, const char* result); // ������Ԫʽ
void generate_assign_quad(const char* src, const char* dest); // ���ɸ�ֵ��Ԫʽ��=��
void generate_if_quad(const char* cond, const char* target); // ����������ת��Ԫʽ��if��
void generate_goto_quad(const char* target);     // ������������ת��Ԫʽ��goto��
void enter_scope();                              // ����������������������㼶��
void exit_scope();                               // �˳���ǰ�����򣨼���������㼶��
void add_activation_record(const char* name, int type); // ��ӱ��������¼��
int add_constant(const char* value, int type);   // ��ӳ�����������ȥ�أ�
char* get_constant_quad(const char* value, int type); // ��ȡ��������Ԫʽ��ʾ����C1��
void analyze_program();                          // ������������
void analyze_declaration_list();                 // ���������б�
void analyze_var_decl();                         // ���������������
void analyze_statement();                        // �����������
void analyze_assignment();                       // ������ֵ���
void analyze_if_stmt();                          // ����if���
void analyze_while_stmt();                       // ����while���
void analyze_block();                            // ��������飨{...}��
void print_symbol_table();                       // ��ӡ���ű�����
void print_quadruples();                         // ��ӡ��Ԫʽ�б�
void print_activation_records();                 // ��ӡ���¼��
void print_constant_table();                     // ��ӡ������
void typetrans(int type);                        // ��������ת��Ϊ�ַ���ʾ��i/r/c�ȣ�

// �ⲿ�����������������Դʷ���������
extern char identifiers[100][50];    // ��ʶ����
extern int id_count;                 // ��ʶ������
extern char C1[100][50];             // ���ͳ�����
extern int C1_count;                 // ���ͳ�������
extern char C2[100][50];             // ���㳣����
extern int C2_count;                 // ���㳣������
extern char Token[100][20];          // Token��
extern int Token_count;              // Token����
extern int token_index;              // ��ǰToken����
extern char* current_token;          // ��ǰToken
extern char* lookahead(int n);       // Ԥ��Token
extern void consume();               // ���ĵ�ǰToken
extern void match(const char* token_type); // ƥ�䲢����ָ�����͵�Token

// ���ű�ṹ
typedef struct {
    char name[50];      // ������
    int type;           // ���ͣ�1-int, 2-float, 3-char
    int initialized;    // �Ƿ��ѳ�ʼ����0-δ��ʼ����1-�ѳ�ʼ����
    int scope;          // ������㼶��0-ȫ�֣�1-��һ��飬�������ƣ�
} Symbol;

// ��Ԫʽ�ṹ
typedef struct {
    char op[20];        // ������
    char arg1[50];      // ������1
    char arg2[50];      // ������2����ѡ��
    char result[50];    // �������תĿ��
} Quadruple;

// ���¼��ṹ
typedef struct {
    char name[50];      // ������
    int type;           // ����
    int size;           // ռ�ÿռ䣨�ֽڣ�
    int offset;         // ջƫ����
    int scope;          // ������㼶
} ActivationRecord;

// ������ṹ
typedef struct {
    char value[50];     // ����ֵ
    int type;           // ���ͣ�1-int, 2-float, 3-char, 4-string��
} Constant;

// ȫ�ֱ���
Symbol symbol_table[100];      // ���ű�
int symbol_count = 0;          // ���ű�ǰ��С
int current_scope = 0;         // ��ǰ������㼶

Quadruple quad[200];           // ��Ԫʽ��
int quad_count = 0;            // ��Ԫʽ������

ActivationRecord activ_rec[100]; // ���¼��
int activ_count = 0;             // ���¼������
int current_offset = 0;          // ��ǰջƫ����

Constant const_table[100];     // ������
int const_count = 0;           // �����������

// ����ת������������ԭtypetrans�߼���
void typetrans(int type) {
    switch (type) {
    case 1: printf("i"); break;  // int����
    case 2: printf("r"); break;  // float����
    case 3: printf("c"); break;  // char����
    case 4: printf("b"); break;  // bool����
    case 5: printf("a"); break;  // ��������
    case 6: printf("d"); break;  // �ṹ������
    default: printf("?"); break; // δ֪����
    }
}

// ���������
void semantic_error(const char* message) {
    printf("�������: %s\n", message);
    exit(1);
}

// �ڷ��ű��в��ұ���
int find_symbol(const char* name, int scope) {
    for (int i = symbol_count - 1; i >= 0; i--) {
        if (strcmp(symbol_table[i].name, name) == 0 && symbol_table[i].scope <= scope) {
            return i;  // ���ط��ű�����
        }
    }
    return -1;  // δ�ҵ�
}

// ��ӱ��������ű�
void add_symbol(const char* name, int type) {
    if (find_symbol(name, current_scope) != -1) {
        semantic_error("�ظ�����ı���");
    }
    Symbol new_symbol;
    strcpy(new_symbol.name, name);
    new_symbol.type = type;
    new_symbol.initialized = 0;  // ��ʼ��Ϊδ��ʼ��״̬
    new_symbol.scope = current_scope;
    symbol_table[symbol_count++] = new_symbol;
}

// �������Ƿ�������ѳ�ʼ��
void check_variable(const char* name) {
    int index = find_symbol(name, current_scope);
    if (index == -1) {
        semantic_error("δ����ı���");
    }
    // �Ǹ�ֵ��������Ҫ����ʼ��״̬
    if (current_token && strstr(current_token, "(P 11)") == NULL) {
        if (!symbol_table[index].initialized) {
            semantic_error("ʹ��δ��ʼ���ı���");
        }
    }
}

// ���ñ���Ϊ�ѳ�ʼ��
void set_initialized(const char* name) {
    int index = find_symbol(name, current_scope);
    if (index != -1) {
        symbol_table[index].initialized = 1;
    }
}

// ��ȡ��������
int get_type(const char* name) {
    int index = find_symbol(name, current_scope);
    if (index == -1) {
        semantic_error("δ����ı���");
    }
    return symbol_table[index].type;
}

// ���ͼ�飺���ر��ʽ����
int check_expression_type() {
    if (current_token && strstr(current_token, "(I ")) {
        char name[50];
        sscanf(current_token, "(I %s)", name);
        return get_type(name);  // ��������
    }
    else if (current_token && strstr(current_token, "(C1 ")) {
        return 1;  // int���ͳ���
    }
    else if (current_token && strstr(current_token, "(C2 ")) {
        return 2;  // float���ͳ���
    }
    else if (current_token && strstr(current_token, "(CT ")) {
        return 3;  // char���ͳ���
    }
    return 0;  // δ֪����
}

// ������Ԫʽ
void generate_quad(const char* op, const char* arg1, const char* arg2, const char* result) {
    if (quad_count >= 200) {
        semantic_error("��Ԫʽ�����");
    }
    strcpy(quad[quad_count].op, op);
    strcpy(quad[quad_count].arg1, arg1);
    strcpy(quad[quad_count].arg2, arg2);
    strcpy(quad[quad_count].result, result);
    quad_count++;
}

// ���ɸ�ֵ��Ԫʽ
void generate_assign_quad(const char* src, const char* dest) {
    generate_quad("=", src, "", dest);
}

// ����������ת��Ԫʽ
void generate_if_quad(const char* cond, const char* target) {
    generate_quad("if", cond, "", target);
}

// ������������ת��Ԫʽ
void generate_goto_quad(const char* target) {
    generate_quad("goto", "", "", target);
}

// ������������
void enter_scope() {
    current_scope++;
    current_offset = -4;  // �ֲ�������BP-4��ʼ����
}

// �˳���ǰ������
void exit_scope() {
    current_scope--;
}

// ��ӻ��¼
void add_activation_record(const char* name, int type) {
    ActivationRecord rec;
    strcpy(rec.name, name);
    rec.type = type;
    rec.size = (type == 1 || type == 2) ? 4 : 1;  // int/floatռ4�ֽڣ�charռ1�ֽ�
    rec.offset = current_offset;
    rec.scope = current_scope;
    activ_rec[activ_count++] = rec;
    current_offset -= rec.size;  // ջ��������
}

// ��ӳ�����������������int��float���ͣ�
int add_constant(const char* value, int type) {
    // ����������1��int��������2��float�����볣����
    if (type != 1 && type != 2) {
        semantic_error("�����������洢ʵ����int��float��");
    }

    for (int i = 0; i < const_count; i++) {
        if (strcmp(const_table[i].value, value) == 0 && const_table[i].type == type) {
            return i + 1;  // ������1��ʼ
        }
    }

    if (const_count >= 100) {
        semantic_error("���������");
    }
    strcpy(const_table[const_count].value, value);
    const_table[const_count].type = type;
    return ++const_count;
}

// ��ȡ��������Ԫʽ��ʾ
char* get_constant_quad(const char* value, int type) {
    int idx = add_constant(value, type);
    char* buf = (char*)malloc(10);
    sprintf(buf, "%d", idx);
    return buf;
}

// ��������
void analyze_program() {
    token_index = 0;
    current_token = lookahead(1);
    current_scope = 0;  // ��ȫ��������ʼ
    enter_scope();      // ����ȫ��������

    analyze_declaration_list();
    exit_scope();       // �˳�ȫ��������

    // ����м���
    print_symbol_table();
    print_quadruples();
    print_activation_records();
    print_constant_table();

    // ����Ƿ���δ�����Token
    if (token_index < Token_count) {
        semantic_error("����ĩβ�ж���Token");
    }
    printf("\n��������ɹ�������\n");
}

// ���������б�
void analyze_declaration_list() {
    while (current_token && token_index < Token_count) {
        if (strstr(current_token, "(K 1)") || strstr(current_token, "(K 4)") || strstr(current_token, "(K 16)")) {
            analyze_var_decl();  // ��������
        }

        else if (strstr(current_token, "(I ")) {
            analyze_statement();  // ��ֵ����������
        }
        else if (strstr(current_token, "(K 12)")) {
            analyze_if_stmt();    // if���
        }
        else if (strstr(current_token, "(K 5)")) {
            analyze_while_stmt(); // while���
        }
        else {
            semantic_error("��֧�ֵ��������");
        }
    }
}

// ���������������Ƴ�char���ͶԳ�����ĵ��ã�
void analyze_var_decl() {
    int type;
    if (strstr(current_token, "(K 1)")) {  // int����
        type = 1;
        consume();
    }
    else if (strstr(current_token, "(K 4)")) {  // float����
        type = 2;
        consume();
    }
    else if (strstr(current_token, "(K 16)")) {  // char���ͣ�����ӷ��ű�����������
        type = 3;
        consume();
    }
    else {
        semantic_error("��Ч������˵����");
    }

    if (current_token && strstr(current_token, "(I ")) {
        int id;
        sscanf(current_token, "(I %d)", &id);
        char name[50];
        strcpy(name, identifiers[id - 1]);  // �ӱ�ʶ�����ȡ����

        add_symbol(name, type);            // ��ӵ����ű�
        add_activation_record(name, type);  // ��ӵ����¼��

        consume();  // ���ı�ʶ��Token

        // �����ʼ��������и�ֵ��
        if (strstr(current_token, "(P 11)")) {  // ��ֵ�����"="
            consume();  // ����"="
            int expr_type = check_expression_type();
            if (expr_type != type && expr_type != 0) {
                semantic_error("���Ͳ�ƥ��");
            }
            // ���ɸ�ֵ��Ԫʽ��������int��float���Ƴ�char��֧��
            if (strstr(current_token, "(C1 ")) {  // ���ͳ�������Ӧint��
                int cid;
                sscanf(current_token, "(C1 %d)", &cid);
                char* const_val = get_constant_quad(C1[cid - 1], 1);
                generate_assign_quad(const_val, name);
            }
            else if (strstr(current_token, "(C2 ")) {  // ���㳣������Ӧfloat��
                int cid;
                sscanf(current_token, "(C2 %d)", &cid);
                char* const_val = get_constant_quad(C2[cid - 1], 2);
                generate_assign_quad(const_val, name);
            }
            else if (strstr(current_token, "(I ")) {  // ����
                int vid;
                sscanf(current_token, "(I %d)", &vid);
                generate_assign_quad(identifiers[vid - 1], name);
            }
            // �Ƴ�char���͵�CT�����֧
            set_initialized(name);  // ���Ϊ�ѳ�ʼ��
            consume();  // ���ı��ʽToken
        }

        match("(P 13)");  // ���ķֺ�";"
    }
    else {
        semantic_error("��������ȱ�ٱ�ʶ��");
    }
}
// ������ֵ���
void analyze_assignment() {
    char name[50];
    sscanf(current_token, "(I %s)", name);
    check_variable(name);  // �������Ƿ�������ѳ�ʼ��
    int var_type = get_type(name);
    consume();  // ������ֵ��ʶ��

    match("(P 11)");  // ���ĸ�ֵ�����"="

    // ������ֵ���ʽ
    if (strstr(current_token, "(C1 ")) {  // ���ͳ���
        int cid;
        sscanf(current_token, "(C1 %d)", &cid);
        char* const_val = get_constant_quad(C1[cid - 1], 1);
        generate_assign_quad(const_val, name);
    }
    else if (strstr(current_token, "(I ")) {  // ����
        int vid;
        sscanf(current_token, "(I %d)", &vid);
        generate_assign_quad(identifiers[vid - 1], name);
    }
    else {
        semantic_error("��֧�ֵ���ֵ���ʽ����");
    }

    set_initialized(name);  // ���Ϊ�ѳ�ʼ��
    consume();  // ���ı��ʽ
    match("(P 13)");  // ���ķֺ�";"
}

// �������
void analyze_statement() {
    if (current_token && strstr(current_token, "(I ")) {
        char* next = lookahead(1);
        if (next && strstr(next, "(P 11)")) {  // ��ֵ���
            analyze_assignment();
        }
        else {
            semantic_error("δ֪���������");
        }
    }
}

// ����if���
void analyze_if_stmt() {
    match("(K 12)");  // ����"if"�ؼ���
    match("(P 3)");   // ����������"("

    // ����������ʽ����
    int cond_type = check_expression_type();
    if (cond_type != 1) {  // Ҫ������Ϊint���ͣ��򻯴���
        semantic_error("�������ʽ����Ϊ��������");
    }

    // ����������ת��Ԫʽ
    char cond[50];
    if (strstr(current_token, "(I ")) {
        int id;
        sscanf(current_token, "(I %d)", &id);
        strcpy(cond, identifiers[id - 1]);
    }
    else if (strstr(current_token, "(C1 ")) {
        int cid;
        sscanf(current_token, "(C1 %d)", &cid);
        sprintf(cond, "C%d", add_constant(C1[cid - 1], 1));
    }
    consume();  // �����������ʽ

    match("(P 4)");  // ����������")"

    // ����������ת����������ת��Ŀ����������
    char target_true[10], target_false[10];
    sprintf(target_true, "L%d", quad_count + 1);
    generate_if_quad(cond, target_true);
    sprintf(target_false, "L%d", quad_count + 2);
    generate_goto_quad(target_false);

    // ����if�����
    enter_scope();
    analyze_block();
    exit_scope();

    // ����else���֣�����У�
    if (current_token && strstr(current_token, "(K 14)")) {
        // ������ת��else������ָ��
        char target_end[10];
        sprintf(target_end, "L%d", quad_count + 1);
        generate_goto_quad(target_end);

        // ����else��֧��Ŀ��
        strcpy(quad[quad_count - 2].result, target_end);

        consume();  // ����"else"�ؼ���
        enter_scope();
        analyze_block();
        exit_scope();
    }
    else {
        // ����else��֧��Ŀ�꣨���û��else������ת��if��֮��
        strcpy(quad[quad_count - 1].result, target_false);
    }
}

// ����while���
void analyze_while_stmt() {
    match("(K 5)");  // ����"while"�ؼ���
    match("(P 3)");  // ����������"("

    // ��¼ѭ����ʼλ��
    char loop_start[10];
    sprintf(loop_start, "L%d", quad_count);

    // ����������ʽ����
    int cond_type = check_expression_type();
    if (cond_type != 1) {
        semantic_error("�������ʽ����Ϊ��������");
    }

    // ����������ת��Ԫʽ
    char cond[50];
    if (strstr(current_token, "(I ")) {
        int id;
        sscanf(current_token, "(I %d)", &id);
        strcpy(cond, identifiers[id - 1]);
    }
    else if (strstr(current_token, "(C1 ")) {
        int cid;
        sscanf(current_token, "(C1 %d)", &cid);
        sprintf(cond, "C%d", add_constant(C1[cid - 1], 1));
    }
    consume();  // �����������ʽ

    match("(P 4)");  // ����������")"

    // ����������ת��ѭ������λ��
    char loop_end[10];
    sprintf(loop_end, "L%d", quad_count + 1);
    generate_if_quad(cond, loop_end);

    // ����ѭ����
    enter_scope();
    analyze_block();
    exit_scope();

    // ������������ת�ص�ѭ����ʼ
    generate_goto_quad(loop_start);

    // ����ѭ������λ��
    strcpy(quad[quad_count - 2].result, loop_end);
}

// ���������
void analyze_block() {
    match("(P 15)");  // ����������"{"
    enter_scope();    // ������������
    analyze_declaration_list();
    exit_scope();     // �˳�������
    match("(P 16)");  // �����һ�����"}"
}

// ��ӡ���ű�ʹ��typetrans������
void print_symbol_table() {
    printf("\n���ű�����:\n");
    printf("����\t����\t����\t������\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("%s\t", symbol_table[i].name);
        typetrans(symbol_table[i].type);  // ʹ��ԭtypetrans�����������
        printf("\t%d\t%d\n",
            symbol_table[i].initialized,
            symbol_table[i].scope);
    }
}

// ��ӡ��Ԫʽ��
void print_quadruples() {
    printf("\n��Ԫʽ�б�:\n");
    printf("���\t������\t������1\t������2\t���\n");
    for (int i = 0; i < quad_count; i++) {
        printf("%d\t(%s\t%s\t%s\t%s)\n",
            i + 1, quad[i].op, quad[i].arg1, quad[i].arg2, quad[i].result);
    }
}

// ��ӡ���¼��
void print_activation_records() {
    printf("\n���¼��:\n");
    printf("������\t��С\tƫ����\t\n");
    for (int i = 0; i < activ_count; i++) {
        printf("%s",activ_rec[i].name);
        printf("\t%d\t%d\t\n",
            activ_rec[i].size,
            activ_rec[i].offset);
    }
}

// ��ӡ������
void print_constant_table() {
    printf("\n������:\n");
    printf("���\tֵ\t\n");
    for (int i = 0; i < const_count; i++) {
        printf("%d\t%s\t",
            i + 1, const_table[i].value);
        printf("\n");
    }
}
//�ܴ�ӡ����