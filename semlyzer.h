#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* parse_expression_for_assignment();
char* parse_term();
char* parse_factor();
void semantic_error(const char* message);        // ������������Ϣ����ֹ����
int find_symbol(const char* name, int scope);    // �ڷ��ű��в��ұ���������������-1��
void add_symbol(const char* name, int type,int kind);     // ����±��������ű�����ظ���
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
char* parse_condition_expression();
void generate_while_quad(const char* cond, const char* target);
void analyze_struct_stmt();
void typetrans(int type);

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
    case 6: printf("d<����"); break;  // �ṹ������
    default: printf("?"); break; // δ֪����
    }
}

// ����ת������������ԭtypetrans�߼���
void kindtrans(int type) {
    switch (type) {
    case 1: printf("v"); break;  // ��������
    case 2: printf("d"); break;  // �ṹ���Ա����
    case 3: printf("t"); break;  // ��������
    case 4: printf("c"); break;  // ��������
    case 5: printf("f"); break;  // ��������
    case 6: printf("vn"); break;  // �ṹ������
    default: printf("?"); break; // δ֪����
    }
}

// ����ת������������ԭtypetrans�߼���
void kindtrans(int type) {
    switch (type) {
    case 1: printf("v"); break;  // ��������
    case 2: printf("d"); break;  // �ṹ���Ա����
    case 3: printf("t"); break;  // ��������
    case 4: printf("c"); break;  // ��������
    case 5: printf("f"); break;  // ��������
    case 6: printf("vn"); break;  // �ṹ������
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
void add_symbol(const char* name, int type, int kind ) {
    if (find_symbol(name, current_scope) != -1) {
        semantic_error("�ظ�����ı���");
    }
    Symbol new_symbol;
    strcpy(new_symbol.name, name);
    new_symbol.type = type;
    new_symbol.initialized = kind;  // ��ʼ��Ϊδ��ʼ��״̬
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
        int id;
        sscanf(current_token, "(I %d)", &id);
        return get_type(identifiers[id-1]);  // ��������
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
    generate_quad("=", src, "_", dest);
}

// ����������ת��Ԫʽ
void generate_if_quad(const char* cond, const char* target) {
    generate_quad("if", target, "_", "_");
}

// ������������ת��Ԫʽ
void generate_goto_quad(const char* target) {
    generate_quad("goto", "_", "_", target);
}

//do��Ԫʽ
void generate_while_quad(const char* cond, const char* target)
{
    generate_quad("do", target, "_", "_");
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
char* get_constant_quad(char* value, int type) {
    int idx = add_constant(value, type);
    char* buf = (char*)malloc(10);
    sprintf(buf, "%d", idx);
    return value;
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
        else if (strstr(current_token, "(K 7)")) {
            analyze_struct_stmt();//struct
        }
        else if (strstr(current_token, "(P 16)")) {
            break;
        }
        else {
            printf("%s", Token[token_index]);
            semantic_error("��֧�ֵ��������1111111");
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

        add_symbol(name, type,1);            // ��ӵ����ű�
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


void analyze_assignment() {
    int id;
    sscanf(current_token, "(I %d)", &id);
    char* lhs_name = identifiers[id - 1];
    consume();  // ������ֵ��ʶ��

    match("(P 11)");  // ���ĸ�ֵ�����"="

    // ������ֵ���ʽ��֧�ּӼ��˳�����ѭ��������ȼ���
    char* expr_quad = parse_expression_for_assignment();

    // ���ɸ�ֵ��Ԫʽ����ֵ��� -> ��ֵ������
   
    generate_assign_quad(expr_quad, lhs_name);
    match("(P 13)");  // ���ķֺ�";"
}

// �������ʽ������Ӽ����㣩
char* parse_expression_for_assignment() {
    char* result = parse_term(); // �˳���������T2=4��

    // ����Ӽ��������+ -��
    while (current_token && (strstr(current_token, "(P 8)") || strstr(current_token, "(P 1)"))) {
        char* op = (strstr(current_token, "(P 8)")) ? "+" : "-";
        consume(); // �������������+��

        char* operand = parse_term(); // ������

        // ���ɼӷ���Ԫʽ����T2+1=T3��
        char temp_result[10];
        sprintf(temp_result, "T%d", quad_count + 1);
        generate_quad(op, result, operand, temp_result);
        strcpy(result, temp_result);
    }

    return result;
}

// ���������˳����㣩
char* parse_term() {
    char* result = parse_factor(); // �������ӣ��糣��2��

    // ����˳��������* /��
    while (current_token && (strstr(current_token, "(P 9)") || strstr(current_token, "(P 2)"))) {
        char* op = (strstr(current_token, "(P 9)")) ? "*" : "/";
        consume(); // �������������*��

        char* operand = parse_factor(); // ������һ�����ӣ�����һ��2��

        // ���ɳ˳���Ԫʽ����2*2=T2��
        char temp_result[10];
        sprintf(temp_result, "T%d", quad_count + 1);
        generate_quad(op, result, operand, temp_result);
        strcpy(result, temp_result);
    }

    return result;
}

// �������ӣ����������������ű��ʽ��
char* parse_factor() {
    char* result = (char*)malloc(50);
    strcpy(result, "");

    if (strstr(current_token, "(I ")) {  // ����
        int vid;
        sscanf(current_token, "(I %d)", &vid);
        strcpy(result, identifiers[vid - 1]);
        check_variable(identifiers[vid - 1]);
        consume();
    }
    else if (strstr(current_token, "(C1 ")) {  // ���ͳ���
        int cid;
        sscanf(current_token, "(C1 %d)", &cid);
        strcpy(result, get_constant_quad(C1[cid - 1], 1));
        consume();
    }
    else if (strstr(current_token, "(P 3)")) {  // ������"(", �������ű��ʽ
        consume(); // ����������
        result = parse_expression_for_assignment(); // �ݹ�������ʽ
        match("(P 4)"); // ����������")"
    }
    else {
        semantic_error("���ʽ������Ч");
    }

    return result;
}




// �������
void analyze_statement() {
    if (current_token && strstr(current_token, "(I ")) {
        char* next = lookahead(2);
        if (next && strstr(next, "(P 11)")) {  // ��ֵ���
            analyze_assignment();
        }
        else {
            semantic_error("δ֪���������");
        }
    }
}


// �����ṹ����䣨������ʼ����
void analyze_struct_stmt() {
    // ����"struct"�ؼ���
    match("(K 7)");  // ����(K 20)��"struct"��Token����

    // ��ȡ�ṹ������
    if (!strstr(current_token, "(I ")) {
        semantic_error(current_token);
    }
    int struct_id;
    sscanf(current_token, "(I %d)", &struct_id);
    char* struct_name = identifiers[struct_id - 1];
    consume();  // ���Ľṹ������

    // ���ṹ���Ƿ��Ѷ���
    int struct_type_id = find_symbol(struct_name,current_scope);
    if (struct_type_id != -1) {
        semantic_error(struct_name);
    }

    add_symbol(struct_name, 6, 3);

    // ��ʼ�����ṹ���Ա
    match("(P 15)");  // ����������"{"

    while (current_token && !strstr(current_token, "(P 16)")) {  // ֱ�������һ�����"}"
        // ������Ա����
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

            add_symbol(name, type,2);            // ��ӵ����ű�
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
                consume();  // ���ı��ʽToken
            }
            match("(P 13)");  // ���ķֺ�";"
        }
        else {
            semantic_error("��������ȱ�ٱ�ʶ��");
        }
    }
    // �����һ�����
    match("(P 16)");  // �һ�����"}"
    match("(P 13)");  // �ֺ�";"
}
// ����if���
void analyze_if_stmt() {
    match("(K 12)");  // ����"if"�ؼ���
    match("(P 3)");   // ����������"("

    // �����������ʽ��֧�ֹ�ϵ�������
    char* cond_result = parse_condition_expression();

    // ����������ת��Ԫʽ
    char target_true[10], target_false[10];
    sprintf(target_true, "T%d", quad_count);
    generate_if_quad(cond_result, target_true);  // �������ʽ�����Ϊ����
    //sprintf(target_false, "L%d", quad_count + 2);
    //generate_goto_quad(target_false);

    // �ͷ��������ʽ������ڴ�
    if (strncmp(cond_result, "T", 1) == 0) {
        free(cond_result);
    }

    if (!strstr(current_token, "(P 4)")) {
        semantic_error("�������ʽȱ��������')'");
    }
    match("(P 4)");  // ����������")"

    // ����if�����
    analyze_block();


    // ����else���֣�����У�
    if (current_token && strstr(current_token, "(K 14)")) {
        generate_quad("el", "_", "_", "_");

        // ������ת��else������ָ��
        char target_end[10];
        sprintf(target_end, "T%d", quad_count + 1);
       // generate_goto_quad(target_end);

        // ����else��֧��Ŀ��
        strcpy(quad[quad_count - 2].result, target_end);

        consume();  // ����"else"�ؼ���
        analyze_block();
    }

   /*else {
        // ����else��֧��Ŀ�꣨���û��else������ת��if��֮��
        strcpy(quad[quad_count - 1].result, target_false);
    }*/ 
    generate_quad("ie", "_", "_", "_");
}

// �����������ʽ��֧�ֹ�ϵ�����������consume���ã�
char* parse_condition_expression() {
    // ���������������"a"��"a + b"��
    char* left_operand = parse_expression_for_assignment();

    // ����Ƿ���ڹ�ϵ�����
    if (current_token &&
        (strstr(current_token, "(P 5)") ||  // ==
         strstr(current_token, "(P 6)") ||  // <=
         strstr(current_token, "(P 7)") ||  // <
         strstr(current_token, "(P 10)")||  //>
         strstr(current_token, "(P 17)") || //!=
         strstr(current_token, "(P 18)") //>=
            ))
    {

        char rel_op[3] = { 0 }; // ��ʼ��Ϊ���ַ�����ȷ���ַ�����ȷ��ֹ

        if (strstr(current_token, "(P 10)")) { // >
            strcpy(rel_op, ">");
            consume();
        }
        else if (strstr(current_token, "(P 7)")) { // <
            strcpy(rel_op, "<");
            consume();
        }
        else if (strstr(current_token, "(P 5)")) { // ==
            strcpy(rel_op, "==");
            consume();
        }
        else if (strstr(current_token, "(P 6)")) { // <=
            strcpy(rel_op, "<=");
            consume();
        }
        else if (strstr(current_token, "(P 17)")) { // !=
            strcpy(rel_op, "!=");
            consume();
        }
        else if (strstr(current_token, "(P 18)")) { // >=
            strcpy(rel_op, ">=");
            consume();
        }
        else {
            semantic_error("��֧�ֵĹ�ϵ�����");
        }

        // �����Ҳ���������"10"��"c"��
        char* right_operand = parse_expression_for_assignment();

        // ���ɹ�ϵ���ʽ����Ԫʽ���� a > 10 �� T1 = a > 10��
        char* temp_result = (char*)malloc(10);
        sprintf(temp_result, "T%d", quad_count + 1);
        generate_quad(rel_op, left_operand, right_operand, temp_result);

        // �ͷ����Ҳ��������ڴ棨�������ʱ������
        if (strncmp(left_operand, "T", 1) == 0) {
            free(left_operand);
        }
        if (strncmp(right_operand, "T", 1) == 0) {
            free(right_operand);
        }

        return temp_result;
    }
    // ���û�й�ϵ�������ֱ�ӷ�������������絥������������
    return left_operand;
}

// ����while���
void analyze_while_stmt() {
    generate_quad("wl", "_", "_", "_");
    match("(K 5)");  // ����"while"�ؼ���
    match("(P 3)");  // ����������"("

    // ��¼ѭ����ʼλ�ã���ǩ��
    char loop_start[10];
    sprintf(loop_start, "T%d", quad_count);

    // �����������ʽ��ʹ����if�����ͬ�ı��ʽ����������
    char* cond_result = parse_condition_expression();

    // ����������ת��Ԫʽ������Ϊ��ʱ��ת��ѭ���壬��������ѭ����
    char loop_exit[10];
    sprintf(loop_exit, "T%d", quad_count);
    generate_while_quad(cond_result, loop_exit);

    // �ͷ��������ʽ������ڴ�
    if (strncmp(cond_result, "T", 1) == 0) {
        free(cond_result);
    }

    match("(P 4)");  // ����������")"

    // ����ѭ����
    enter_scope();
    analyze_block();
    exit_scope();

    // ������������ת�ص�ѭ����ʼ��
    //generate_goto_quad(loop_start);

    // ����������ת��Ŀ���ǩ��ѭ������λ�ã�
    strcpy(quad[quad_count - 2].result, loop_exit);
    generate_quad("we", "_", "_", "_");
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
        printf("\t");
        kindtrans(symbol_table[i].initialized);
        printf("\t%d\n",symbol_table[i].scope);
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