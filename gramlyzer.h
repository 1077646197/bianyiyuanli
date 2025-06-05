#pragma once
/*��������
���� �� �����б�
�����б� �� ���� �����б� | ��
���� �� �������� | ��ֵ��� | if��� | while���

// ��������
�������� �� ����˵���� ��ʶ�� �ֺ�
����˵���� �� ���� | ������

// ��ֵ���
��ֵ��� �� ��ʶ�� ��ֵ����� ���ʽ �ֺ�

// ���ʽ�����ֲ��䣩
���ʽ �� �ӷ����ʽ
�ӷ����ʽ �� �˷����ʽ (�ӷ������ �˷����ʽ | ��������� �˷����ʽ)*
�˷����ʽ �� �������ʽ (�˷������ �������ʽ | ��������� �������ʽ)*
�������ʽ �� ��ʶ�� | ���� | ������ ���ʽ ������

// if ���
if��� �� ��� ������ ���� ������ ����� (���� �����)?
���� �� ���ʽ ��ϵ����� ���ʽ
��ϵ����� �� ���� | С�� | ���� | ������ | ���ڵ��� | С�ڵ���

// while ѭ��
while��� �� �� ������ ���� ������ �����

// �����
����� �� ������ �����б� �һ�����

// �ս�����ʷ���Ԫ��
���� : "int"
������ : "void"
��ʶ�� : [a-zA-Z_][a-zA-Z0-9_]*
���� : [0-9]+
�ֺ� : ";"
��ֵ����� : "="
�ӷ������ : "+"
��������� : "-"
�˷������ : "*"
��������� : "/"
������ : "("
������ : ")"
��� : "if"
���� : "else"
�� : "while"
������ : "{"
�һ����� : "}"
���� : ">"
С�� : "<"
���� : "=="
������ : "!="
���ڵ��� : ">="
С�ڵ��� : "<="
 */




#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
 // ȫ�ֱ���
int token_index = 0;           // ��ǰToken����
char* current_token;           // ��ǰToken

// ��ǰ�鿴n��Token
char* lookahead(int n);
// ���ĵ�ǰToken���ƶ�����һ��
void consume();
// ƥ���ض����͵�Token������
void match(const char* token_type);
// ������
void syntax_error(const char* message);
// ������������Ӧ�ķ����ս����
void parse_program();           // ���� �� �����б�
void parse_declaration_list();  // �����б� �� ���� �����б� | ��
void parse_declaration();       // ���� �� �������� | ��ֵ��� | if��� | while���
void parse_var_decl();          // �������� �� ����˵���� ��ʶ�� �ֺ�
void parse_type_specifier();    // ����˵���� �� ���� | ������
void parse_assign_stmt();       // ��ֵ��� �� ��ʶ�� ��ֵ����� ���ʽ �ֺ�
void parse_if_stmt();           // if��� �� ��� ������ ���� ������ ����� (���� �����)?
void parse_while_stmt();        // while��� �� �� ������ ���� ������ �����
void parse_condition();         // ���� �� ���ʽ ��ϵ����� ���ʽ
void parse_relop();             // ��ϵ����� �� ���� | С�� | ���� | ������ | ���ڵ��� | С�ڵ���
void parse_block();             // ����� �� ������ �����б� �һ�����
void parse_expr();              // ���ʽ �� �ӷ����ʽ
void parse_additive_expr();     // �ӷ����ʽ �� �˷����ʽ (�ӷ������ �˷����ʽ | ��������� �˷����ʽ)*
void parse_multiplicative_expr(); // �˷����ʽ �� �������ʽ (�˷������ �������ʽ | ��������� �������ʽ)*
void parse_primary_expr();      // �������ʽ �� ��ʶ�� | ���� | ������ ���ʽ ������

char* lookahead(int n) {
    if (token_index + n - 1 < Token_count) {
        return Token[token_index + n - 1];
    }
    return NULL; // ����Token��ĩβ
}

// ���ĵ�ǰToken���ƶ�����һ��
void consume() {
    if (token_index < Token_count) {
        token_index++;
        current_token = lookahead(1);
    }
    else {
        current_token = NULL;
    }
}

// ƥ���ض����͵�Token������
void match(const char* token_type) {
    if (current_token && strstr(current_token, token_type)) {
        consume();
    }
    else {
        char error_msg[100];
        sprintf(error_msg, "���� %s��ȴ���� %s", token_type, current_token ? current_token : "EOF");
        syntax_error(error_msg);
    }
}


// �﷨������
void syntax_error(const char* message) {
    printf("�﷨����: %s\n", message);
    iscorret = 0; // ��ʷ������Ĵ����־ͳһ
}

// ��������program �� �����б�
void parse_program() {
    parse_declaration_list();
    // �������ʱӦ��ʣ��Token
    if (token_index < Token_count) {
        syntax_error("����ĩβ���ڶ���Token");
    }
}

// ���������б�declaration_list �� ���� �����б� | ��
void parse_declaration_list() {
    if (current_token &&
        (strstr(current_token, "(K 1)") ||   // int
            strstr(current_token, "(K 2)") ||   // void
            strstr(current_token, "(I ") ||     // ��ʶ������ֵ���/if/while��
            strstr(current_token, "(K 12)") ||  // if
            strstr(current_token, "(K 5)"))) {  // while
        parse_declaration();
        parse_declaration_list();
    }
    // �ղ���ʽ�������������
}

// ����������declaration �� �������� | ��ֵ��� | if��� | while���
//void parse_declaration() {
//    if (strstr(current_token, "(K 1)") || strstr(current_token, "(K 2)")) {
//        // ��������
//        parse_var_decl();
//    }
//    else if (strncmp(current_token, "(I ", 3) == 0 && current_token[3] != ')') {
//        // ��ȷƥ���ʶ�� (I name)
//        char* next_token = lookahead(1);
//        if (next_token && strstr(next_token, "(P 11)")) { // ��ֵ�����"="
//            parse_assign_stmt();
//        }
//        else {
//            syntax_error("��ʶ����ȱ�ٸ�ֵ�����");
//        }
//    }
//    else if (strstr(current_token, "(K 12)")) { // if
//        parse_if_stmt();
//    }
//    else if (strstr(current_token, "(K 5)")) { // while
//        parse_while_stmt();
//    }
//    else {
//        syntax_error("ȱ�����������Ͳ�֧��");
//    }
//}
void parse_declaration() {
    if (strstr(current_token, "(K 1)") || strstr(current_token, "(K 2)")) {
        // ��������
        parse_var_decl();
    }
    else if (strncmp(current_token, "(I ", 3) == 0 && current_token[3] != ')') {
        // ��ʶ�������Ǹ�ֵ����if/while������
        char* next_token = lookahead(1);
        if (next_token && strstr(next_token, "(P 11)")) { // ��ֵ�����"="
            parse_assign_stmt();
        }
        else if (next_token && strstr(next_token, "(P 3)")) { // "("
            // ����Ƿ�Ϊif/while������
            char* next_next_token = lookahead(2);
            if (next_next_token &&
                (strstr(next_next_token, "(K 12)") || strstr(next_next_token, "(K 5)"))) {
                syntax_error("if/while�ؼ���λ�ô���");
            }
            else {
                syntax_error("��ʶ����ȱ�ٸ�ֵ�����");
            }
        }
        else {
            syntax_error("��ʶ����ȱ�ٸ�ֵ�����");
        }
    }
    else if (strstr(current_token, "(K 12)")) { // if
        parse_if_stmt();
    }
    else if (strstr(current_token, "(K 5)")) { // while
        parse_while_stmt();
    }
    else {
        syntax_error("ȱ�����������Ͳ�֧��");
    }
}
// ��������������var_decl �� ����˵���� ��ʶ�� �ֺ�
void parse_var_decl() {
    parse_type_specifier();
    if (current_token && strstr(current_token, "(I ")) {
        consume(); // ���ı�ʶ��
        if (current_token && strstr(current_token, "(P 13)")) { // �ֺ�";"
            consume(); // ���ķֺ�
        }
        else {
            syntax_error("��������ȱ�ٷֺ�");
        }
    }
    else {
        syntax_error("��������ȱ�ٱ�ʶ��");
    }
}

// ��������˵������type_specifier �� ���� | ������
void parse_type_specifier() {
    if (current_token && (strstr(current_token, "(K 1)") || strstr(current_token, "(K 2)"))) {
        consume(); // ��������˵����
    }
    else {
        syntax_error("ȱ������˵����");
    }
}

// ������ֵ��䣺assign_stmt �� ��ʶ�� ��ֵ����� ���ʽ �ֺ�
void parse_assign_stmt() {
    match("(I "); // ��ʶ��
    match("(P 11)"); // ��ֵ�����"="
    parse_expr();
    match("(P 13)"); // �ֺ�";"
}

// ����if��䣺if_stmt �� ��� ������ ���� ������ ����� (���� �����)?
void parse_if_stmt() {
    match("(K 12)"); // "if"
    match("(P 3)"); // "("
    parse_condition();
    match("(P 4)"); // ")"
    parse_block();

    // ��ѡ��else����
    if (current_token && strstr(current_token, "(K 14)")) { // "else"
        consume();
        parse_block();
    }
}

// ����while��䣺while_stmt �� �� ������ ���� ������ �����
void parse_while_stmt() {
    match("(K 5)"); // "while"
    match("(P 3)"); // "("
    parse_condition();
    match("(P 4)"); // ")"
    parse_block();
}

// ����������condition �� ���ʽ ��ϵ����� ���ʽ
void parse_condition() {
    parse_expr();
    parse_relop();
    parse_expr();
}

// ������ϵ�������relop �� ���� | С�� | ���� | ������ | ���ڵ��� | С�ڵ���
void parse_relop() {
    if (current_token && (strstr(current_token, "(P 10)") ||  // ">"
        strstr(current_token, "(P 7)") ||   // "<"
        strstr(current_token, "(P 5)") ||   // "=="
        strstr(current_token, "(P 11)") ||  // "=" (��ֵ��������)
        strstr(current_token, "(P 6)"))) {  // "<="
        consume();
    }
    else {
        syntax_error("ȱ�ٹ�ϵ�����");
    }
}

// ��������飺block �� ������ �����б� �һ�����
void parse_block() {
    match("(P 15)"); // "{"
    parse_declaration_list();
    match("(P 16)"); // "}"
}

// �������ʽ����ʾ��������ʵ����ֲ㴦��Ӽ��˳���
void parse_expr() {
    parse_additive_expr();
}

//�����ӷ����ʽ
void parse_additive_expr() {
    parse_multiplicative_expr();
    while (current_token && (strstr(current_token, "(P 8)") || strstr(current_token, "(P 1)"))) {
        // �ӷ������"+"����������"-"
        consume();
        parse_multiplicative_expr();
    }
}

// �����˷����ʽ��multiplicative_expr �� �������ʽ (�˷������ �������ʽ | ��������� �������ʽ)*
void parse_multiplicative_expr() {
    parse_primary_expr();
    while (current_token && (strstr(current_token, "(P 9)") || strstr(current_token, "(P 2)"))) {
        // �˷������"*"����������"/"
        consume();
        parse_primary_expr();
    }
}

// �����������ʽ��primary_expr �� ��ʶ�� | ���� | ������ ���ʽ ������
void parse_primary_expr() {
    if (current_token && strstr(current_token, "(I ")) {
        consume(); // ���ı�ʶ��
    }
    else if (current_token && (strstr(current_token, "(C1 ") || strstr(current_token, "(C2 "))) {
        consume(); // ���ĳ���
    }
    else if (current_token && strstr(current_token, "(P 3)")) { // "("
        consume();
        parse_expr();
        match("(P 4)"); // ")"
    }
    else {
        syntax_error("��Ч�ı��ʽ��ʼ");
    }
}
#pragma once
