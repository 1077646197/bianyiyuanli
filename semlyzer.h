#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ������Щ�����ͺ����ڴʷ����������Ѷ���
extern char identifiers[100][50];    // �洢��ʶ���Ķ�ά���飬ÿ�д���һ����ʶ��
extern int id_count;                 // ��ʶ������������¼��ǰ��ʶ������
extern char C1[100][50];             // �洢���ͳ����Ķ�ά���飬ÿ�д���һ�����ͳ���
extern int C1_count;                 // ���ͳ���������
extern char C2[100][50];             // �洢�����ͳ����Ķ�ά����
extern int C2_count;                 // �����ͳ���������
extern char Token[100][20];          // �洢���дʷ���Ԫ�Ķ�ά���飬ÿ���ʷ���Ԫ��ʽ��"(K 1)"
extern int Token_count;              // �ʷ���Ԫ����
extern int token_index;              // ��ǰ����Ĵʷ���Ԫ����
extern char* current_token;          // ��ǰ�ʷ���Ԫָ��
extern char* lookahead(int n);       // ��ǰ�鿴n���ʷ���Ԫ������Ԥ��
extern void consume();               // ���ĵ�ǰ�ʷ���Ԫ��ָ����һ��
extern void match(const char* token_type);  // ƥ�䲢����ָ�����͵Ĵʷ���Ԫ����ƥ���򱨴�

// ���ű�ṹ - �洢�����ж���ı�����Ϣ
typedef struct {
    char name[50];      // ������
    int type;           // ���ͣ�1-int, 2-float, 3-char
    int initialized;    // �Ƿ��ѳ�ʼ����0-δ��ʼ����1-�ѳ�ʼ����
    int scope;          // ������㼶��0-ȫ�֣�1-��һ��飬�������ƣ�
} Symbol;

void typetrans(int type)
{
    switch (type)
    {
        case 1:
            printf("i\t");break;
        case 2:
            printf("r\t");break;
        case 3:
            printf("c\t");break;
        case 4:
            printf("b\t");break;
        case 5:
            printf("a\t");break;
        case 6:
            printf("d\t");break;
    default:
        break;
    }
}

Symbol symbol_table[100];  // ���ű����洢100������
int symbol_count = 0;      // ��ǰ���ű��еķ�������
int current_scope = 0;     // ��ǰ������㼶��0��ʾȫ��������

// ��������
void semantic_error(const char* message);        // ������������������Ϣ����ֹ����
int find_symbol(const char* name, int scope);    // �ڷ��ű��в��ҷ��ţ�����������-1
void add_symbol(const char* name, int type);     // ����ű���ӷ��ţ�����ظ�����
void check_variable(const char* name);           // �������Ƿ�������ѳ�ʼ�������ڶ�ȡ������
void set_initialized(const char* name);          // ���ñ���Ϊ�ѳ�ʼ��״̬
int get_type(const char* name);                  // ��ȡ�������ͣ��������򱨴�
void analyze_program();                          // ���������������������ڵ�
void analyze_declaration_list();                 // ���������б��������������ȣ�
void analyze_var_decl();                         // ���������������
void analyze_statement();                        // ����������䣨��ֵ��if��while�ȣ�
void analyze_assignment();                       // ������ֵ���
void analyze_if_stmt();                          // ����if���
void analyze_while_stmt();                       // ����while���
void analyze_block();                            // ��������飨��{}��Χ������飩
int check_expression_type();                     // �����ʽ���ͣ�����������


// ���������� - ��ʼ������ʼ������������
void analyze_program() {
    token_index = 0;               // ����Token����
    current_token = lookahead(1);  // ��ȡ��һ��Token
    current_scope = 0;             // ��ȫ��������ʼ

    // ������������
    analyze_declaration_list();

    // ����Ƿ���δ�����Token
    if (token_index < Token_count) {
        semantic_error("����ĩβ�ж���Token");
    }

    printf("��������ɹ�\n");
}


// ������ - ���������Ϣ����ֹ����
void semantic_error(const char* message) {
    printf("�������: %s\n", message);
    exit(1);  // ��ֹ����ִ��
}

// �ڷ��ű��в��ұ��� - �ӵ�ǰ�����������ϲ���
int find_symbol(const char* name, int scope) {
    for (int i = symbol_count - 1; i >= 0; i--) {
        if (strcmp(symbol_table[i].name, name) == 0 &&
            symbol_table[i].scope <= scope) {
            return i;  // ���ط��ű�����
        }
    }
    return -1;  // δ�ҵ�
}

// ��ӱ��������ű� - ����ظ����岢����·���
void add_symbol(const char* name, int type) {
    if (find_symbol(name, current_scope) != -1) {
        semantic_error("�ظ�����ı���");
    }

    Symbol new_symbol;
    strcpy(new_symbol.name, name);
    new_symbol.type = type;
    new_symbol.initialized = 0;  // ��ʼ��Ϊδ��ʼ��״̬
    new_symbol.scope = current_scope;

    if (symbol_count < 100) {
        symbol_table[symbol_count++] = new_symbol;
    }
    else {
        semantic_error("���ű����");
    }
}

// �������Ƿ�������ѳ�ʼ�� - ���ڶ�ȡ����ֵ֮ǰ�ļ��
void check_variable(const char* name) {
    int index = find_symbol(name, current_scope);
    if (index == -1) {
        semantic_error("δ����ı���");
    }

    // ���ڸ�ֵ����Ҳ�ı���������Ƿ��ѳ�ʼ��
    // ��ֵ������ı�������δ��ʼ������int a = 10;��
    if (current_token && strstr(current_token, "(P 11)") == NULL) {  // �Ǹ�ֵ������
        if (!symbol_table[index].initialized) {
            semantic_error("ʹ��δ��ʼ���ı���");
        }
    }
}

// ���ñ���Ϊ�ѳ�ʼ�� - �ڱ�������ֵ�����
void set_initialized(const char* name) {
    int index = find_symbol(name, current_scope);
    if (index != -1) {
        symbol_table[index].initialized = 1;
    }
}

// ��ȡ�������� - ���ر����������룬�������򱨴�
int get_type(const char* name) {
    int index = find_symbol(name, current_scope);
    if (index == -1) {
        semantic_error("δ����ı���");
    }
    return symbol_table[index].type;
}

// ���ͼ�飺���ر��ʽ���� - ʶ���������������
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
    // �����������չ...
    return 0;
}

// ���������б� - �������������������
void analyze_declaration_list() {
    while (current_token && token_index < Token_count) {
        if (strstr(current_token, "(K 1)") || strstr(current_token, "(K 4)")) {
            // ����������int��float��
            analyze_var_decl();
        }
        else if (strstr(current_token, "(I ")) {
            // ��ֵ���������ã��Ա�ʶ����ͷ��
            analyze_statement();
        }
        else if (strstr(current_token, "(K 12)")) {
            // if���
            analyze_if_stmt();
        }
        else if (strstr(current_token, "(K 5)")) {
            // while���
            analyze_while_stmt();
        }
        else {
            semantic_error("��֧�ֵ��������");
        }
    }
}

// ������������ - �������������ͳ�ʼ��
void analyze_var_decl() {
    int type;
    if (strstr(current_token, "(K 1)")) {  // int����
        type = 1;
        consume();  // �������͹ؼ���
    }
    else if (strstr(current_token, "(K 4)")) {  // float����
        type = 2;
        consume();
    }
    else {
        semantic_error("��Ч������˵����");
    }

    if (current_token && strstr(current_token, "(I ")) {
        int id;
        // 1. ����Token�е�����
        sscanf(current_token, "(I %d)", &id);

        // 3. �ӱ�ʶ�����ȡ���Ʋ����Ƶ�name����
        char name[50];
        strcpy(name, identifiers[id - 1]); // id-1��������������0��ʼ��

        // 4. ��ӵ����ű�����Token
        add_symbol(name, type);
        consume();
    

        // �����ʼ��������У�
        if (current_token && strstr(current_token, "(P 11)")) {  // ��ֵ�����"="
            consume();
            int expr_type = check_expression_type();
            if (expr_type != type && expr_type != 0) {
                semantic_error("���Ͳ�ƥ��");
            }
            set_initialized(name);  // ���Ϊ�ѳ�ʼ��
            consume();  // ���ı��ʽ
        }

        match("(P 13)");  // ���ķֺ�";"
    }
    else {
        semantic_error("��������ȱ�ٱ�ʶ��");
    }
}

// ������ֵ��� - �����ֵ����ֵ����ƥ��
void analyze_assignment() {
    char name[50];
    sscanf(current_token, "(I %s)", name);
    check_variable(name);  // �������Ƿ�������ѳ�ʼ��������Ƕ�ȡ������
    int var_type = get_type(name);
    consume();  // ������ֵ��ʶ��

    match("(P 11)");  // ���ĸ�ֵ�����"="

    int expr_type = check_expression_type();
    if (expr_type != var_type && expr_type != 0) {
        semantic_error("��ֵ���Ͳ�ƥ��");
    }

    set_initialized(name);  // ���Ϊ�ѳ�ʼ������ʹ֮ǰ�ѳ�ʼ����
    consume();  // ���ı��ʽ
    match("(P 13)");  // ���ķֺ�";"
}

// ������� - ���ֲ�ͬ���͵���䣨Ŀǰ��֧�ָ�ֵ��
void analyze_statement() {
    if (current_token && strstr(current_token, "(I ")) {
        char* next = lookahead(1);
        if (next && strstr(next, "(P 11)")) {  // ��ֵ��䣨��ʶ�����"="��
            analyze_assignment();
        }
        else {
            semantic_error("δ֪���������");
        }
    }
}

// ����if��� - �����������ʽ�ʹ����
void analyze_if_stmt() {
    match("(K 12)");  // ����"if"�ؼ���
    match("(P 3)");   // ����������"("

    // ����������ʽ���ͣ�����Ϊint���ͣ�
    int cond_type = check_expression_type();
    if (cond_type != 1) {  // Ҫ������Ϊint���ͣ��򻯴���
        semantic_error("�������ʽ����Ϊ��������");
    }
    consume();  // �����������ʽ

    match("(P 4)");  // ����������")"

    // ������������if����飩
    current_scope++;

    // ����if�����
    analyze_block();

    // �˳�������
    current_scope--;

    // ����else���֣�����У�
    if (current_token && strstr(current_token, "(K 14)")) {
        consume();  // ����"else"�ؼ���
        current_scope++;  // ����else�����������
        analyze_block();
        current_scope--;  // �˳�else�����������
    }
}

// ����while��� - ����ѭ��������ѭ����
void analyze_while_stmt() {
    match("(K 5)");  // ����"while"�ؼ���
    match("(P 3)");  // ����������"("

    // ����������ʽ���ͣ�����Ϊint���ͣ�
    int cond_type = check_expression_type();
    if (cond_type != 1) {  // Ҫ������Ϊint����
        semantic_error("�������ʽ����Ϊ��������");
    }
    consume();  // �����������ʽ

    match("(P 4)");  // ����������")"

    // ������������ѭ���壩
    current_scope++;

    // ����ѭ����
    analyze_block();

    // �˳�������
    current_scope--;
}

// ��������� - ������{}��Χ������飬����������
void analyze_block() {
    match("(P 15)");  // ����������"{"

    // ������������
    current_scope++;

    // �������������б�
    analyze_declaration_list();

    // �˳�������
    current_scope--;

    match("(P 16)");  // �����һ�����"}"
}
void printsem()
{
    printf("\n���ű�����:\n");
    printf("����\t����\t��ʼ��\t������\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("%s\t", symbol_table[i].name);
        typetrans(symbol_table[i].type);
        printf("%d\t%d\n", symbol_table[i].initialized, symbol_table[i].scope);
    }
}