#pragma once
/*��
//������
���� �� �����б�
�����б� �� ���� �����б� | ��
���� �� �������� | ��ֵ��� | if��� | while��� | ������� | �Լ���� | �ṹ������ | �ṹ���������

// �������ṹ������
�ṹ������ �� struct ��ʶ�� { �ṹ���Ա�б� } ;
�ṹ���Ա�б� �� �ṹ���Ա �ṹ���Ա�б� | ��
�ṹ���Ա �� ����˵���� ��ʶ�� ;

// �������ṹ���������
�ṹ��������� �� struct ��ʶ�� ��ʶ�� (��ֵ����� �ṹ���ʼ�����ʽ)? ;
�ṹ���ʼ�����ʽ �� { ���ʽ�б� }
���ʽ�б� �� ���ʽ (, ���ʽ)*

// ��������
�������� �� ����˵���� ��ʶ�� (��ֵ����� ���ʽ)? ;
����˵���� �� int | void | struct ��ʶ��  // �����ṹ������

// ��ֵ���
��ֵ��� �� ��ʶ�� = ���ʽ ;

// ���ʽ�����ֲ��䣩
���ʽ �� �ӷ����ʽ
�ӷ����ʽ �� �˷����ʽ     (+ �˷����ʽ | - �˷����ʽ)��*�հ�
�˷����ʽ �� �������ʽ     (* �������ʽ | / �������ʽ)��*�հ�
�������ʽ �� ��ʶ�� | ���� | ( ���ʽ )

// if ���
if��� �� if ( ���� ) ����� (else �����)��
���� �� ���ʽ ��ϵ����� ���ʽ
��ϵ����� �� > | < | == | != | >= | <=

// while ѭ��
while��� �� while ( ���� ) �����

//�������
������� �� ��ʶ�� ++ ;

//�Լ����
�Լ���� �� ��ʶ�� -- ;

// �����
����� �� { �����б� }

// �ս�����ʷ���Ԫ��
��ʶ�� : [a-z��A-Z��_]  [a-z��A-Z��0-9��_]��*�հ�
���� : [0-9]��+�հ�

 */



#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
 

 // ȫ�ֱ���
int token_index = 0;           // ��ǰToken����
char* current_token;           // ��ǰToken


//��������
int is_token_type(const char* token, const char* type);// ��ȷƥ��Token����
char* lookahead(int n);// ��ǰ�鿴n��Token
void sync_to_statement_end();// ͬ������������Tokenֱ������ָ�����ͻ�������
void consume();// ���ĵ�ǰToken���ƶ�����һ��
void match(const char* token_type);// ƥ���ض����͵�Token������
void match_prefix(const char* prefix);
void syntax_error(const char* message);// ������


// ����������������Ӧ�ķ����ս����
void parse_program();           // ���� �� �����б�
void parse_declaration_list();  // �����б� �� ���� �����б� | ��
void parse_declaration();       // ���� �� �������� | ��ֵ��� | if��� | while��� | ������� | �Լ���� | �ṹ������ | �ṹ���������
void parse_var_decl();          // �������� �� ����˵���� ��ʶ�� (��ֵ����� ���ʽ)? ;
void parse_type_specifier();    // ����˵���� �� int | void
void parse_assign_stmt();       // ��ֵ��� �� ��ʶ�� = ���ʽ ;
void parse_if_stmt();           // if��� �� if��� �� if ( ���� ) ����� (else �����)��
void parse_while_stmt();        // while��� �� while ( ���� ) �����
void parse_condition();         // ���� �� ���ʽ ��ϵ����� ���ʽ
void parse_relop();             // ��ϵ����� �� > | < | == | != | >= | <=
void parse_block();             // ����� �� { �����б� }
void parse_expr();              // ���ʽ �� �ӷ����ʽ
void parse_additive_expr();     // �ӷ����ʽ �� �˷����ʽ     (+ �˷����ʽ | - �˷����ʽ)��*�հ�
void parse_multiplicative_expr(); // �˷����ʽ �� �������ʽ     (* �������ʽ | / �������ʽ)��*�հ�
void parse_primary_expr();      // �������ʽ �� ��ʶ�� | ���� | ( ���ʽ )
void parse_incr_stmt();         //������� �� ��ʶ��++;
void parse_decr_stmt();         //�Լ���� �� ��ʶ��--;
void parse_struct_decl();         // �ṹ������ �� struct ��ʶ�� { �ṹ���Ա�б� } ;
void parse_struct_member_list();  // �ṹ���Ա�б� �� �ṹ���Ա �ṹ���Ա�б� | ��
void parse_struct_member();       // �ṹ���Ա �� ����˵���� ��ʶ�� ;
void parse_struct_var_decl();     // �ṹ��������� �� struct ��ʶ�� ��ʶ�� (��ֵ����� �ṹ���ʼ�����ʽ)? ;
void parse_struct_init_expr();    // �ṹ���ʼ�����ʽ �� { ���ʽ�б� }
void parse_expr_list();           // ���ʽ�б� �� ���ʽ (, ���ʽ)*


// ��ȷƥ��Token���ͣ���"(K 5)"��
int is_token_type(const char* token, const char* type) {
    return strcmp(token, type) == 0;
}

// ���鿴n��Token
char* lookahead(int n) {
    if (token_index + n - 1 < Token_count) {
        return Token[token_index + n - 1];
    }
    return NULL; // ����Token��ĩβ
}

// ͬ������������Tokenֱ������ָ�����ͻ�������
void sync_to_statement_end() {
    while (current_token &&
        !strstr(current_token, "(P 13)") &&  // �ֺ�
        !strstr(current_token, "(P 16)") &&  // �һ�����
        !strstr(current_token, "(K 5)") &&   // while
        !strstr(current_token, "(K 12)")) {  // if
        consume();
    }
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

// ��ǿ��match�������ṩ����ȷ��ƥ��ʹ���ָ�
void match(const char* token_type) {
    if (current_token && is_token_type(current_token, token_type)) {
        consume();
    }
    else {
        char error_msg[100];
        sprintf(error_msg, "���� %s��ȴ���� %s", token_type, current_token ? current_token : "EOF");
        syntax_error(error_msg);
        sync_to_statement_end(); // ��������һ�����
    }
}

//ʶ��ǰ׺���match����
void match_prefix(const char* prefix) {
    if (current_token && strncmp(current_token, prefix, strlen(prefix)) == 0) {
        consume();
    }
    else {
        char error_msg[100];
        sprintf(error_msg, "������ %s ��ͷ��Token��ȴ���� %s", prefix, current_token ? current_token : "EOF");
        syntax_error(error_msg);
        sync_to_statement_end();
    }
}

// �﷨������
void syntax_error(const char* message) {
    printf("�﷨����: %s\n", message);
    iscorret = 0; // ��ʷ������Ĵ����־ͳһ
}

//// ��������program �� �����б�
//void parse_program() {
//    parse_declaration_list();
//    // �������ʱӦ��ʣ��Token
//    if (token_index < Token_count) {
//        syntax_error("����ĩβ���ڶ���Token");
//    }
//}
void parse_program() {
    printf("��ʼ��������Token����: %d\n", Token_count);
    for (int i = 0; i < Token_count; i++) {
        printf("Token[%d]: %s\n", i, Token[i]);
    }

    parse_declaration_list();

    printf("������ɺ󣬵�ǰToken����: %d\n", token_index);
    if (token_index < Token_count) {
        printf("δ�����Token: ");
        for (int i = token_index; i < Token_count; i++) {
            printf("%s ", Token[i]);
        }
        printf("\n");
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

void parse_declaration() {
    // 1. �������������int/void ��ͷ��
    if (strstr(current_token, "(K 1)") || strstr(current_token, "(K 2)")) {
        parse_var_decl();
        return;
    }

    // 2. �����ʶ����ͷ�����
    if (strncmp(current_token, "(I ", 3) == 0 && current_token[3] != ')') {
        char* next_token = lookahead(2);

        // 2.1 ��ֵ��䣨��ʶ�� = ���ʽ��
        if (next_token && strstr(next_token, "(P 11)")) {
            parse_assign_stmt();
            return;
        }

        // 2.2 ������䣨��ʶ�� ++ ;��
        if (next_token && strstr(next_token, "(P 14)")) {
            parse_incr_stmt();
            return;
        }

        // 2.3 �Լ���䣨��ʶ�� -- ;��
        if (next_token && strstr(next_token, "(P 19)")) {
            parse_decr_stmt();
            return;
        }

        // 2.4 �����ŵı��ʽ�������������������ã�
        if (next_token && strstr(next_token, "(P 3)")) {
            // ���浱ǰ״̬���ڴ���ָ�
            char* prev_token = current_token;
            int prev_index = token_index;

            // ���ı�ʶ����������
            consume();  // ���ı�ʶ��
            consume();  // ����������

            // ���������ڵı��ʽ
            parse_expr();

            // ����Ƿ�ƥ��������
            if (current_token && strstr(current_token, "(P 4)")) {
                consume();  // ����������

                // ����Ƿ�Ϊ����鿪ͷ����������Ϊ��if/while���
                if (current_token && strstr(current_token, "(P 15)")) {
                    parse_block();
                    return;
                }

                // �����Ǵ���飬�����Ǻ������û���ʽ���
                syntax_error("ȱ�ٴ���飬������δ֧�ֵĺ�������");
            }
            else {
                syntax_error("���Ų�ƥ��");
            }

            // ����ָ������˵�����ǰ��״̬
            token_index = prev_index;
            current_token = prev_token;
            consume();  // ��������ı�ʶ��������������������
            return;
        }

        // 2.5 �����������ʶ����ȷ�=Ҳ��(Ҳ��++/--
        syntax_error("��ʶ����ȱ�ٸ�ֵ�����/����/�Լ������/������");
        consume();  // ��������Token
        return;
    }

    // 3. ����while���
    if (strstr(current_token, "(K 5)")) {
        parse_while_stmt();
        return;
    }

    // 4. ����if���
    if (strstr(current_token, "(K 12)")) {
        parse_if_stmt();
        return;
    }

    // 5. ����ṹ������������ṹ��������ṹ�����������
    if (current_token && strstr(current_token, "(K 7)")) {
        // ���浱ǰ״̬���ڻ���
        char* prev_token = current_token;
        int prev_index = token_index;

        // ����struct�ؼ���
        consume();

        // ����Ƿ�Ϊ�ṹ��������struct ��ʶ�� { ... }��
        if (current_token && strstr(current_token, "(I ") && lookahead(2) && strstr(lookahead(2), "(P 15)")) {
            token_index = prev_index;
            current_token = prev_token;
            parse_struct_decl();
            return;
        }

        // ����Ƿ�Ϊ�ṹ�����������struct ������ ��������
        else if (current_token && strstr(current_token, "(I ") && lookahead(2) && strstr(lookahead(2), "(I ")) {
            token_index = prev_index;
            current_token = prev_token;
            parse_struct_var_decl();
            return;
        }

        // �����ǣ�����ָ�
        else {
            token_index = prev_index;
            current_token = prev_token;
            syntax_error("��Ч�Ľṹ���������������");
            sync_to_statement_end();
            return;
        }
    }

    // 7. δʶ�����������
    syntax_error("ȱ�����������Ͳ�֧��");
    if (current_token) consume();  // ��������Token
}

// ��������������var_decl �� ����˵���� ��ʶ�� (��ֵ����� ���ʽ)? ;
void parse_var_decl() {
    parse_type_specifier();
    if (current_token && strstr(current_token, "(I ")) {
        consume(); // ���ı�ʶ��
        // ��ѡ�ĸ�ֵ����
        if (current_token && strstr(current_token, "(P 11)")) { // "="
            consume();
            parse_expr();
        }
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

// ��������˵������type_specifier �� ���� | ������ | struct ��ʶ��
void parse_type_specifier() {
    // ֧�ֻ������ͺͽṹ������
    if (current_token &&
        (strstr(current_token, "(K 1)") ||   // int
            strstr(current_token, "(K 2)") ||   // void
            strstr(current_token, "(K 7)")))    // struct
    {
        // �����struct���ͣ���Ҫ���⴦��
        if (strstr(current_token, "(K 7)")) {
            consume(); // ����"struct"
            match_prefix("(I "); // ���Ľṹ��������
        }
        else {
            consume(); // ���Ļ�������(int/void)
        }
    }
    else {
        syntax_error("ȱ������˵������int/void/struct��");
    }
}

// ������ֵ��䣺assign_stmt �� ��ʶ�� ��ֵ����� ���ʽ �ֺ�
void parse_assign_stmt() {
    match_prefix("(I "); // ��ʶ��
    match("(P 11)"); // ��ֵ�����"="
    parse_expr();//
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
        strstr(current_token, "(P 6)") ||   // "<="
        strstr(current_token, "(P 17") ||   // "!="
        strstr(current_token, "(P 18"))) {  // ">="
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

// ����������������� �� ��ʶ�� ++ ;
void parse_incr_stmt() {
    match_prefix("(I ");  // ƥ���ʶ��
    match("(P 14)");     // ƥ��"++"
    match("(P 13)");     // ƥ��";"
}

// �����������Լ���� �� ��ʶ�� -- ;
void parse_decr_stmt() {
    match_prefix("(I ");  // ƥ���ʶ��
    match("(P 19)");     // ƥ��"--"
    match("(P 13)");     // ƥ��";"
}

// �����ṹ��������struct_decl �� struct ��ʶ�� { �ṹ���Ա�б� } ;
void parse_struct_decl() {
    match("(K 7)");              // ƥ��"struct"�ؼ���
    match_prefix("(I ");         // ƥ��ṹ�����ƣ���ʶ����
    match("(P 15)");             // ƥ��"{"
    parse_struct_member_list();  // �����ṹ���Ա�б�
    match("(P 16)");             // ƥ��"}"
    match("(P 13)");             // ƥ��";"
}

// �����ṹ���Ա�б�struct_member_list �� �ṹ���Ա �ṹ���Ա�б� | ��
void parse_struct_member_list() {
    if (current_token && (strstr(current_token, "(K 1)") || strstr(current_token, "(K 2)"))) {
        parse_struct_member();
        parse_struct_member_list();
    }
    // �ղ���ʽ�������������
}

// �����ṹ���Ա��struct_member �� ����˵���� ��ʶ�� ;
void parse_struct_member() {
    parse_type_specifier();       // ��������˵������int/void��
    match_prefix("(I ");          // ƥ���Ա����������ʶ����
    match("(P 13)");              // ƥ��";"
}

// �����ṹ�����������struct_var_decl �� struct ��ʶ�� ��ʶ�� (��ֵ����� �ṹ���ʼ�����ʽ)? ;
void parse_struct_var_decl() {
    match("(K 7)");              // ƥ��"struct"�ؼ���
    match_prefix("(I ");         // ƥ��ṹ��������
    match_prefix("(I ");         // ƥ�������
    // ��ѡ�ĳ�ʼ������
    if (current_token && strstr(current_token, "(P 11)")) { // "="
        match("(P 11)");
        parse_struct_init_expr();
    }
    match("(P 13)");             // ƥ��";"
}

// �����ṹ���ʼ�����ʽ��struct_init_expr �� { ���ʽ�б� }
void parse_struct_init_expr() {
    match("(P 15)");             // ƥ��"{"
    parse_expr_list();            // �������ʽ�б�
    match("(P 16)");             // ƥ��"}"
}

// �������ʽ�б�expr_list �� ���ʽ (, ���ʽ)*
void parse_expr_list() {
    parse_expr();                 // ������һ�����ʽ
    while (current_token && strstr(current_token, "(P 12)")) { // ƥ��","
        match("(P 12)");
        parse_expr();             // �����������ʽ
    }
}