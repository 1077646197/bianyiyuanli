#pragma once
/*

���� �� �����б�
�����б� �� ���� �����б� | ��

// �ؼ��޸ģ�����struct��ͷ�Ĳ���ʽ
���� �� ��������
      | if���
      | while���
      | ��ʶ�����
      | struct��ͷ������  

struct��ͷ������ �� struct ��ʶ�� struct��׺����
// ������һ���������ֽṹ�������ͱ�������
struct��׺���� �� { �ṹ���Ա�б� } ;    // �ṹ������
                | ��ʶ�� ( = �ṹ���ʼ�� )? ;  // �ṹ���������

//�����������
�������� �� �������� ��ʶ�� ( = ���ʽ )? ;
�������� �� int | void

// �ϲ���ʶ����ͷ�����
��ʶ����� �� ��ʶ�� ( ��ֵ��׺ | ������׺ | �Լ���׺ )
��ֵ��׺ �� = ���ʽ ;    // ��ֵ���
������׺ �� ++ ;         // �������
�Լ���׺ �� -- ;         // �Լ����

// �ṹ�嶨��
�ṹ���Ա�б� �� �ṹ���Ա �ṹ���Ա�б� | ��
�ṹ���Ա �� ��Ա���� ��ʶ�� ( = ���ʽ )? ;
��Ա���� �� int | void | struct ��ʶ��

// �ṹ���ʼ��
�ṹ���ʼ�� �� { ���ʽ�б� }
���ʽ�б� �� ���ʽ ���ʽ�б��׺ | ��
���ʽ�б��׺ �� , ���ʽ ���ʽ�б��׺ | ��

// ���ʽ
���ʽ �� �ӷ����ʽ
�ӷ����ʽ �� �˷����ʽ ( (+|-) �˷����ʽ )*
�˷����ʽ �� �������ʽ ( (*|/) �������ʽ )*
�������ʽ �� ��ʶ�� | ���� | ( ���ʽ )

// if���
if��� �� if ( ���� ) ����� (else����)?

// while���
while��� �� while ( ���� ) �����

// ����
���� �� ���ʽ ��ϵ����� ���ʽ
��ϵ����� �� > | < | == | != | >= | <=

// �����
����� �� { �����б� }

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



// ������������
void parse_program();                                 //���� �� �����б�
void parse_declaration_list();                        //�����б� �� ���� �����б� | ��

void parse_declaration();                             /*���� �� ��������
                                                         | if���
                                                         | while���
                                                         | ��ʶ�����
                                                         | struct ��ʶ�� struct��׺���� */

void parse_struct_decl();                             //struct��ͷ������ �� struct ��ʶ�� struct��׺����
void parse_struct_suffix_decl();                      /*struct��׺���� �� { �ṹ���Ա�б� } ;    // �ṹ������
                                                                      | ��ʶ�� ( = �ṹ���ʼ�� )? ;  // �ṹ���������*/

void parse_var_decl();                                //�������� �� �������� ��ʶ�� ( = ���ʽ )? ;
void parse_basic_type();                              //�������� �� int | void

void parse_identifier_stmt();                         //��ʶ����� �� ��ʶ�� ( ��ֵ��׺ | ������׺ | �Լ���׺ )
void parse_assign_suffix();                           //��ֵ��׺ �� = ���ʽ;    ��ֵ���
void parse_incr_suffix();                             //������׺ ��++;          �������
void parse_decr_suffix();                             //�Լ���׺ ��--;          �Լ����

void parse_struct_member_list();                      //�ṹ���Ա�б� �� �ṹ���Ա �ṹ���Ա�б� | ��
void parse_struct_member();                           //�ṹ���Ա �� ��Ա���� ��ʶ��(= ���ʽ) ? ;
void parse_member_type();                             //��Ա���� �� int | void | struct ��ʶ��

void parse_struct_init();                             //�ṹ���ʼ�� �� { ���ʽ�б� }
void parse_expr_list();                               //���ʽ�б� �� ���ʽ ���ʽ�б��׺ | ��
void parse_expr_list_suffix();                        //���ʽ�б��׺ ��, ���ʽ ���ʽ�б��׺ | ��

void parse_expr();                                    //���ʽ �� �ӷ����ʽ
void parse_additive_expr();                           //�ӷ����ʽ �� �˷����ʽ((+| -) �˷����ʽ)*
void parse_multiplicative_expr();                     //�˷����ʽ �� �������ʽ((*|/) �������ʽ)*
void parse_primary_expr();                            //�������ʽ �� ��ʶ�� | ���� | (���ʽ)

void parse_if_stmt();                                 //if��� �� if ( ���� ) ����� (else����)?

void parse_while_stmt();                              //while��� �� while ( ���� ) �����

void parse_condition();                               //���� �� ���ʽ ��ϵ����� ���ʽ
void parse_relop();                                   //��ϵ����� �� > | < | == | != | >= | <=

void parse_block();                                   //����� �� { �����б� }



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

// ��������program �� �����б�
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
            strstr(current_token, "(K 5)") ||   // while
            strstr(current_token, "(K 7)"))) {  // struct 
        parse_declaration();
        parse_declaration_list();
    }
    // �ղ���ʽ�������������
}

// ����������declaration �� �������� | if��� | while��� | ��ʶ����� | struct ��ʶ�� struct��׺����
void parse_declaration() {
    // 1. �������������int/void ��ͷ��
    if (strstr(current_token, "(K 1)") || strstr(current_token, "(K 2)")) {
        parse_var_decl();
        return;
    }

    // 2. �����ʶ����ͷ�����
    if (strncmp(current_token, "(I ", 3) == 0 && current_token[3] != ')') {
        parse_identifier_stmt();
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
         parse_struct_decl();
        return;
    }

    // 6. δʶ�����������
    syntax_error("ȱ�����������Ͳ�֧��");
    if (current_token) consume();  // ��������Token
}

// ��������������var_decl �� �������� ��ʶ�� (= ���ʽ)? ;
void parse_var_decl() {
    parse_basic_type();
    if (current_token && strstr(current_token, "(I ")) {
        match_prefix("(I "); // ���ı�ʶ��
        // ��ѡ�ĸ�ֵ����
        if (current_token && strstr(current_token, "(P 11)")) { // "="
            match("(P 11)");
            parse_expr();
        }
        if (current_token && strstr(current_token, "(P 13)")) { // �ֺ�";"
            match("(P 13)"); // ���ķֺ�";"
        }
        else {
            syntax_error("��������ȱ�ٷֺ�");
        }
    }
    else {
        syntax_error("��������ȱ�ٱ�ʶ��");
    }
}

// �����������ͣ�basic_type �� int | void
void parse_basic_type() {
    if (current_token && (strstr(current_token, "(K 1)") || strstr(current_token, "(K 2)"))) {
        consume(); // ����int��void
    }
    else {
        syntax_error("ȱ�ٻ�������˵������int/void��");
    }
}

// ������ʶ����䣺identifier_stmt �� ��ʶ�� ( ��ֵ��׺ | ������׺ | �Լ���׺ )
void parse_identifier_stmt() {
    match_prefix("(I "); // ƥ���ʶ��

    // �жϺ�����׺����
    if (current_token && strstr(current_token, "(P 11)")) { // "="
        parse_assign_suffix();
    }
    else if (current_token && strstr(current_token, "(P 14)")) { // "++"
        parse_incr_suffix();
    }
    else if (current_token && strstr(current_token, "(P 19)")) { // "--"
        parse_decr_suffix();
    }
    else {
        syntax_error("��ʶ����ȱ�ٸ�ֵ/����/�Լ������");
        sync_to_statement_end();
    }
}

// ������ֵ��׺��assign_suffix �� = ���ʽ ;
void parse_assign_suffix() {
    match("(P 11)"); // "="
    parse_expr();
    match("(P 13)"); // ";"
}

// ����������׺��incr_suffix �� ++ ;
void parse_incr_suffix() {
    match("(P 14)"); // "++"
    match("(P 13)"); // ";"
}

// �����Լ���׺��decr_suffix �� -- ;
void parse_decr_suffix() {
    match("(P 19)"); // "--"
    match("(P 13)"); // ";"
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
        match("(K 14)");
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

// �������ʽ�б��׺��expr_list_suffix �� , ���ʽ ���ʽ�б��׺ | ��
void parse_expr_list_suffix() {
    if (current_token && strstr(current_token, "(P 12)")) { // ","
        match("(P 12)");
        parse_expr();
        parse_expr_list_suffix();
    }
    // �ղ���ʽ���
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

// ����struct������struct_decl �� struct ��ʶ�� struct��׺����
void parse_struct_decl() {
    match("(K 7)"); // "struct"
    match_prefix("(I "); // �ṹ������

    // ����struct��׺���������ֽṹ�������ͱ���������
    parse_struct_suffix_decl();
}

// ����struct��׺������struct_suffix_decl �� { �ṹ���Ա�б� } ; | ��ʶ�� ( = �ṹ���ʼ�� )? ;
void parse_struct_suffix_decl() {
    // ���浱ǰ״̬���ڻ���
    char* prev_token = current_token;
    int prev_index = token_index;

    // ����ƥ��ṹ��������{ �ṹ���Ա�б� } ;
    if (current_token && strstr(current_token, "(P 15)")) { // "{"
        match("(P 15)"); // "{"
        parse_struct_member_list();
        match("(P 16)"); // "}"
        match("(P 13)"); // ";"
        return;
    }

    // ���ݲ�����ƥ��ṹ�������������ʶ�� ( = �ṹ���ʼ�� )? ;
    token_index = prev_index;
    current_token = prev_token;

    if (current_token && strstr(current_token, "(I ")) { // ������
        match_prefix("(I "); // ������
        // ��ѡ�ĳ�ʼ������
        if (current_token && strstr(current_token, "(P 11)")) { // "="
            match("(P 11)");
            parse_struct_init();
        }
        match("(P 13)"); // ";"
        return;
    }

    // ƥ��ʧ��
    syntax_error("��Ч�Ľṹ���������������");
    sync_to_statement_end();
}

// �����ṹ���Ա�б�struct_member_list �� �ṹ���Ա �ṹ���Ա�б� | ��
void parse_struct_member_list() {
    if (current_token && (strstr(current_token, "(K 1)") || strstr(current_token, "(K 2)") || strstr(current_token, "(K 7)"))) {
        parse_struct_member();
        parse_struct_member_list();
    }
    // �ղ���ʽ���
}

// �����ṹ���Ա��struct_member �� ��Ա���� ��ʶ�� ( = ���ʽ )? ;
void parse_struct_member() {
    parse_member_type(); // ������Ա����
    match_prefix("(I "); // ��Ա������

    // ��ѡ�ĳ�ʼ������
    if (current_token && strstr(current_token, "(P 11)")) { // "="
        match("(P 11)");
        parse_expr();
    }

    match("(P 13)"); // ";"
}

// ������Ա���ͣ�member_type �� int | void | struct ��ʶ��
void parse_member_type() {
    if (current_token && (strstr(current_token, "(K 1)") || strstr(current_token, "(K 2)"))) {
        consume(); // int��void
    }
    else if (current_token && strstr(current_token, "(K 7)")) {
        match("(K 7)"); // "struct"
        match_prefix("(I "); // �ṹ��������
    }
    else {
        syntax_error("ȱ�ٳ�Ա����˵����");
    }
}

// �����ṹ���ʼ����struct_init �� { ���ʽ�б� }
void parse_struct_init() {
    match("(P 15)"); // "{"
    parse_expr_list();
    match("(P 16)"); // "}"
}

// �������ʽ�б�expr_list �� ���ʽ (, ���ʽ)*
void parse_expr_list() {
    parse_expr();                 // ������һ�����ʽ
    while (current_token && strstr(current_token, "(P 12)")) { // ƥ��","
        match("(P 12)");
        parse_expr();             // �����������ʽ
    }
}