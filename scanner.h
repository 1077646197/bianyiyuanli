

//<����������> �� printf(<�ַ���>); | scanf(<��ʽ��>, <����>);
//<��ʽ��> �� "%d" | "%f" | "%c"



#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
 // �ؼ��ֱ���һ���ǹؼ��֣��ڶ����Ǳ��
char* keywords[][2] = {
    {"int", "1"},
    {"void", "2"},
    {"break", "3"},
    {"float", "4"},
    {"while", "5"},
    {"do", "6"},
    {"struct", "7"},
    {"const", "8"},
    {"case", "9"},
    {"for", "10"},
    {"return", "11"},
    {"if", "12"},
    {"default", "13"},
    {"else", "14"},
    {"main", "15"},
    {"char","16"},
    {"bool","17"}
};
int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

// �������һ���ǽ�����ڶ����Ǳ��
char* delimiters[][2] = {
    {"-", "1"},
    {"/", "2"},
    {"(", "3"},
    {")", "4"},
    {"==", "5"},
    {"<=", "6"},
    {"<", "7"},
    {"+", "8"},
    {"*", "9"},
    {">", "10"},
    {"=", "11"},
    {",", "12"},
    {";", "13"},
    {"++", "14"},
    {"{", "15"},
    {"}", "16"},
    {"!=","17"},
    {">=","18"},
    {"--", "19"}
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

char Token[100][20]; // Token����
int Token_count = 0;

char bool[10][20]; // Token����
int bool_count = 0;

int iscorret = 1;//�Ƿ���ȷ

// ���ʶ�������Ԫ��
int add_to_identifiers(const char* str) {
    for (int i = 0; i < id_count; i++) {
        if (strcmp(identifiers[i], str) == 0) return i+1;
    }
    strcpy(identifiers[id_count], str);
    return ++id_count;
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
        if (strcmp(CT[i], str) == 0)
            return;
    }
    strcpy(CT[CT_count++], str);
}

// ���ַ������������Ԫ��
void add_to_ST(const char* str) {
    for (int i = 0; i < ST_count; i++) {
        if (strcmp(ST[i], str) == 0)
            return;
    }
    strcpy(ST[ST_count++], str);
}

// ����Ƿ��ǹؼ��֣����򷵻ر�ţ����򷵻�0
int is_keyword(const char* str) {
    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(keywords[i][0], str) == 0)
            return atoi(keywords[i][1]);
    }
    return 0;
}

// ����Ƿ��ǽ�������򷵻ر�ţ����򷵻�0
int is_delimiter(const char* str) {
    for (int i = 0; i < delimiter_count; i++) {
        if (strcmp(delimiters[i][0], str) == 0)
            return atoi(delimiters[i][1]);
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
                int index =  add_to_identifiers(id);
                if (Token_count < 100) {
                    sprintf(Token[Token_count], "(I %d)", index);
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
    printf("Token����Ϊ :");
    for (int i = 0; i < Token_count; i++) {
        printf("%s", Token[i]);
    }
}

// �����������
void print_tables() {
    printf("\n��ʶ���� :");
    for (int i = 0; i < id_count; i++) {
        if (i > 0) printf(" |");
        printf("%s", identifiers[i]);
    }
    printf("\n������ :");
    for (int i = 0; i < C1_count; i++) {
        if (i > 0) printf(" |");
        printf("%s", C1[i]);
    }
    printf("\n�������� :");
    for (int i = 0; i < C2_count; i++) {
        if (i > 0) printf(" |");
        printf("%s", C2[i]);
    }
    printf("\n�ַ������� :");
    for (int i = 0; i < CT_count; i++) {
        if (i > 0) printf(" |");
        printf("%s", CT[i]);
    }
    printf("\n�ַ��������� :");
    for (int i = 0; i < ST_count; i++) {
        if (i > 0) printf(" |");
        printf("%s", ST[i]);
    }
    printf("\n");
}