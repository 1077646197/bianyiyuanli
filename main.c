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

 // �ؼ��ֱ���һ���ǹؼ��֣��ڶ����Ǳ��
char* keywords[][2] = {
    {"int", "1"}, {"void", "2"}, {"break", "3"}, {"float", "4"}, {"while", "5"},
    {"do", "6"}, {"struct", "7"}, {"const", "8"}, {"case", "9"}, {"for", "10"},
    {"return", "11"}, {"if", "12"}, {"default", "13"}, {"else", "14"},{"main", "15"}
};
int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

// �������һ���ǽ�����ڶ����Ǳ��
char* delimiters[][2] = {
    {"-", "1"}, {"/", "2"}, {"(", "3"}, {")", "4"}, {"==", "5"}, {"<=", "6"},
    {"<", "7"}, {"+", "8"}, {"*", "9"}, {">", "10"}, {"=", "11"}, {",", "12"},
    {";", "13"}, {"++", "14"}, {"{", "15"}, {"}", "16"}
};
int delimiter_count = sizeof(delimiters) / sizeof(delimiters[0]);

char identifiers[100][50]; // ��ʶ����
int id_count = 0;

char C1[100][50]; // ��������
int C1_count = 0;

char C2[100][50]; // ��ʵ����
int C2_count = 0;

char CT[100][50]; // �ַ�������
int CT_count = 0;

char ST[100][50]; // �ַ���������
int ST_count = 0;

char Token[100][50]; // Token����
int Token_count = 0;

int iscorret = 1;//�Ƿ���ȷ

// ���ʶ�������Ԫ��
void add_to_identifiers(const char* str) {
    for (int i = 0; i < id_count; i++) {
        if (strcmp(identifiers[i], str) == 0) return;
    }
    strcpy(identifiers[id_count++], str);
}

// �����������Ԫ��
void add_to_C1(const char* str) {
    for (int i = 0; i < C1_count; i++) {
        if (strcmp(C1[i], str) == 0) return;
    }
    strcpy(C1[C1_count++], str);
}

// ��ʵ�������Ԫ��
void add_to_C2(const char* str) {
    for (int i = 0; i < C2_count; i++) {
        if (strcmp(C2[i], str) == 0) return;
    }
    strcpy(C2[C2_count++], str);
}

// ���ַ����������Ԫ��
void add_to_CT(const char* str) {
    for (int i = 0; i < CT_count; i++) {
        if (strcmp(CT[i], str) == 0) return;
    }
    strcpy(CT[CT_count++], str);
}

// ���ַ������������Ԫ��
void add_to_ST(const char* str) {
    for (int i = 0; i < ST_count; i++) {
        if (strcmp(ST[i], str) == 0) return;
    }
    strcpy(ST[ST_count++], str);
}

// ����Ƿ��ǹؼ��֣����򷵻ر�ţ����򷵻�0
int is_keyword(const char* str) {
    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(keywords[i][0], str) == 0) return atoi(keywords[i][1]);
    }
    return 0;
}

// ����Ƿ��ǽ�������򷵻ر�ţ����򷵻�0
int is_delimiter(const char* str) {
    for (int i = 0; i < delimiter_count; i++) {
        if (strcmp(delimiters[i][0], str) == 0) return atoi(delimiters[i][1]);
    }
    return 0;
}

// ɨ�������ַ�����ʶ����൥�ʲ��洢��Token����
void scan(const char* source) {
    int i = 0;
    int n = strlen(source);
    while (i < n) {
        while (isspace(source[i])) i++; // �����հ��ַ�
        if (i >= n) break;

        // ����˫�ַ����
        if (i + 1 < n) {
            char two[3] = { source[i], source[i + 1], '\0' };
            int delimiter_num = is_delimiter(two);
            if (delimiter_num) {
                if (Token_count < 100) { // ��ֹ����Խ��
                    sprintf(Token[Token_count], "(P %d)", delimiter_num);
                    Token_count++;
                }
                i += 2;
                continue;
            }
        }

        // �����ַ����
        char one[2] = { source[i], '\0' };
        int delimiter_num = is_delimiter(one);
        if (delimiter_num) {
            if (Token_count < 100) {
                sprintf(Token[Token_count], "(P %d)", delimiter_num);
                Token_count++;
            }
            i++;
            continue;
        }

        // ����ؼ��ֻ��ʶ��
        if (isalpha(source[i]) || source[i] == '_') {
            char id[50] = { 0 };
            int j = 0;
            while (i < n && (isalpha(source[i]) || isdigit(source[i]) || source[i] == '_')) {
                id[j++] = source[i++];
            }
            id[j] = '\0';
            int keyword_num = is_keyword(id);
            if (keyword_num) {
                if (Token_count < 100) {
                    sprintf(Token[Token_count], "(K %d)", keyword_num);
                    Token_count++;
                }
            }
            else {
                add_to_identifiers(id);
                if (Token_count < 100) {
                    sprintf(Token[Token_count], "(I %d)", id_count);
                    Token_count++;
                }
            }
            continue;
        }

        // �����ַ�����
        if (source[i] == '\'') {
            i++;
            char ct[2] = { source[i], '\0' };
            i++;
            if (source[i] != '\'') {
                iscorret = 0;
                return;
            }
            i++;
            add_to_CT(ct);
            if (Token_count < 100) {
                sprintf(Token[Token_count], "(CT %d)", CT_count);
                Token_count++;
            }
            continue;
        }

        // �����ַ�������
        if (source[i] == '"') {
            i++;
            char st[50] = { 0 };
            int j = 0;
            while (i < n && source[i] != '"') {
                st[j++] = source[i++];
            }
            st[j] = '\0';
            i++;
            add_to_ST(st);
            if (Token_count < 100) {
                sprintf(Token[Token_count], "(ST %d)", ST_count);
                Token_count++;
            }
            continue;
        }

        // �������֣������򸡵�����
        if (isdigit(source[i]) || source[i] == '0') {
            if (source[i] == '0') {
                i++;
                if (i < n && (source[i] == 'x' || source[i] == 'X')) {
                    i++;
                    char num[50] = "0x";
                    int j = 1;
                    while (i < n && (isdigit(source[i]) || (source[i] >= 'a' && source[i] <= 'f') || (source[i] >= 'A' && source[i] <= 'F'))) {
                        num[j++] = source[i++];
                    }
                    num[j] = '\0';
                    long dec = strtol(num, NULL, 16);
                    char dec_str[50];
                    sprintf(dec_str, "%ld", dec);
                    add_to_C1(dec_str);
                    if (Token_count < 100) {
                        sprintf(Token[Token_count], "(C1 %d)", C1_count);
                        Token_count++;
                    }
                    continue;
                }
                else {
                    char num[50] = "0";
                    int j = 1;
                    while (i < n && isdigit(source[i])) {
                        num[j++] = source[i++];
                    }
                    num[j] = '\0';
                    add_to_C1(num);
                    if (Token_count < 100) {
                        sprintf(Token[Token_count], "(C1 %d)", C1_count);
                        Token_count++;
                    }
                    continue;
                }
            }
            else {
                char num[50] = { 0 };
                int j = 0;
                while (i < n && isdigit(source[i])) {
                    num[j++] = source[i++];
                }
                num[j] = '\0';
                if (i < n && (source[i] == '.' || (source[i] == 'e' || source[i] == 'E'))) {
                    char float_num[50];
                    sprintf(float_num, "%s", num);
                    int float_valid = 1;  // ��Ǹ������Ƿ�Ϸ�
                    if (source[i] == '.') {
                        float_num[j++] = source[i++];
                        if (i < n && isdigit(source[i])) {
                            while (i < n && isdigit(source[i])) {
                                float_num[j++] = source[i++];
                            }
                        }
                        else {
                            float_valid = 0;
                        }
                    }
                    if (i < n && (source[i] == 'e' || source[i] == 'E')) {
                        float_num[j++] = source[i++];
                        int exp_start = j;  // ��¼ָ��������ʼλ��

                        if (i < n && (source[i] == '+' || source[i] == '-')) {
                            float_num[j++] = source[i++];
                        }

                        // ��ȡָ�����ֵ�����
                        int exp_digit_count = 0;  // ������ͳ��ָ���������ָ���
                        while (i < n && isdigit(source[i])) {
                            float_num[j++] = source[i++];
                            exp_digit_count++;
                        }

                        // ���ָ�������Ƿ������֣���"12e+"��exp_digit_count=0��
                        if (exp_digit_count == 0) {
                            float_valid = 0;
                        }
                    }

                    float_num[j] = '\0';

                    // �����������������Ϸ�����Ǵ��󲢷���
                    if (!float_valid) {
                        iscorret = 0;
                        return;
                    }

                    add_to_C2(float_num);
                    if (Token_count < 100) {
                        sprintf(Token[Token_count], "(C2 %d)", C2_count);
                        Token_count++;
                    }
                }
                else {
                    add_to_C1(num);
                    if (Token_count < 100) {
                        sprintf(Token[Token_count], "(C1 %d)", C1_count);
                        Token_count++;
                    }
                }
                continue;
            }
        }

        // ������.��ͷ�ĸ�����
        if (source[i] == '.') {
            char float_num[50] = ".";
            i++;
            if (i < n && isdigit(source[i])) {
                int j = 1;
                while (i < n && isdigit(source[i])) {
                    float_num[j++] = source[i++];
                }
                if (i < n && (source[i] == 'e' || source[i] == 'E')) {
                    float_num[j++] = source[i++];
                    if (i < n && (source[i] == '+' || source[i] == '-')) {
                        float_num[j++] = source[i++];
                    }
                    while (i < n && isdigit(source[i])) {
                        float_num[j++] = source[i++];
                    }
                }
                float_num[j] = '\0';
                add_to_C2(float_num);
                if (Token_count < 100) {
                    sprintf(Token[Token_count], "(C2 %d)", C2_count);
                    Token_count++;
                }
            }
            else {
                if (Token_count < 100) {
                    iscorret = 0;
                }
                return;
            }
            continue;
        }

        // �����������
        if (Token_count < 100) {
            iscorret = 0;
        }
        return;
    }
}

void print_token_sequence() {
    printf("Token :");
    for (int i = 0; i < Token_count; i++) {
        printf("%s", Token[i]);
    }
}

// �����������
void print_tables() {
    printf("\nI :");
    for (int i = 0; i < id_count; i++) {
        if (i > 0) printf(" ");
        printf("%s", identifiers[i]);
    }
    printf("\nC1 :");
    for (int i = 0; i < C1_count; i++) {
        if (i > 0) printf(" ");
        printf("%s", C1[i]);
    }
    printf("\nC2 :");
    for (int i = 0; i < C2_count; i++) {
        if (i > 0) printf(" ");
        printf("%s", C2[i]);
    }
    printf("\nCT :");
    for (int i = 0; i < CT_count; i++) {
        if (i > 0) printf(" ");
        printf("%s", CT[i]);
    }
    printf("\nST :");
    for (int i = 0; i < ST_count; i++) {
        if (i > 0) printf(" ");
        printf("%s", ST[i]);
    }
}











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
void parse_declaration() {
    if (strstr(current_token, "(K 1)") || strstr(current_token, "(K 2)")) {
        // ��������
        parse_var_decl();
    }
    else if (strstr(current_token, "(I ")) {
        // �����һ��Token�����ָ�ֵ���/if/while
        char* next_token = lookahead(2);
        if (next_token && strstr(next_token, "(P 11)")) { // ��ֵ�����"="
            parse_assign_stmt();
        }
        else if (lookahead(2) && strstr(lookahead(2), "(P 3)")) { // "("
            if (strstr(lookahead(1), "(K 12)")) { // if
                parse_if_stmt();
            }
            else if (strstr(lookahead(1), "(K 5)")) { // while
                parse_while_stmt();
            }
            else {
                syntax_error("�Ƿ�����俪ͷ");
            }
        }
        else {
            syntax_error("�޷�ʶ�����������");
        }
    }
    else {
        syntax_error("ȱ������");
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
    while (current_token && (strstr(current_token, "(P 8)") || strstr(current_token, "(P 1)"))){
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






int main() {
    char source[1000];
    fgets(source, sizeof(source), stdin);
    source[strcspn(source, "\n")] = '\0'; // ȥ�����з�
    scan(source);
    if (iscorret == 1) {
        print_token_sequence();
        print_tables();
        printf("\n");
    }
    else {
        printf("�ʷ�����ERROR");
        return 1;
    }

    // ��ʼ���﷨����
    token_index = 0;
    current_token = lookahead(1);

    // �﷨����
    parse_program();
    if (!iscorret) {
        printf("\n�﷨����ʧ��\n");
        return 1;
    }

    // ������
    printf("�﷨�����ɹ�\n");
    print_tables();
    return 0;
}
