/*¡¤³ÌÐò¶¨Òå
35464648489ktutvfkdt
³ÌÐò½á¹¹
<³ÌÐò> ¡ú <º¯Êý¶¨Òå>
<º¯Êý¶¨Òå> ¡ú <·µ»ØÀàÐÍ> main() <º¯ÊýÌå>
<·µ»ØÀàÐÍ> ¡ú int | void
<º¯ÊýÌå> ¡ú { <Óï¾äÁÐ±í> }

Êý¾ÝÀàÐÍÓë±äÁ¿
<Êý¾ÝÀàÐÍ> ¡ú int | float | char
<±äÁ¿ÉùÃ÷> ¡ú <Êý¾ÝÀàÐÍ> <±êÊ¶·û>;
<±êÊ¶·û> ¡ú <×ÖÄ¸><×ÖÄ¸Êý×Ö>*
<×ÖÄ¸> ¡ú a|b|...|z|A|B|...|Z
<×ÖÄ¸Êý×Ö> ¡ú <×ÖÄ¸>|<Êý×Ö>
<Êý×Ö> ¡ú 0|1|...|9

±í´ïÊ½
<±í´ïÊ½> ¡ú <±äÁ¿> | <³£Á¿> | <±í´ïÊ½><ÔËËã·û><±í´ïÊ½>
<±äÁ¿> ¡ú <±êÊ¶·û>
<³£Á¿> ¡ú <ÕûÊý³£Á¿> | <×Ö·û³£Á¿>
<ÕûÊý³£Á¿> ¡ú [0-9]+
<×Ö·û³£Á¿> ¡ú 'a'|'b'|...|'z'
<ÔËËã·û> ¡ú + | - | * | / | =

Óï¾ä
<Óï¾äÁÐ±í> ¡ú <Óï¾ä> | <Óï¾ä><Óï¾äÁÐ±í>
<Óï¾ä> ¡ú <¸³ÖµÓï¾ä> |   <Ìõ¼þÓï¾ä> | <Ñ­»·Óï¾ä> | <ÉùÃ÷Óï¾ä>
<¸³ÖµÓï¾ä> ¡ú <±äÁ¿> = <±í´ïÊ½>;
<×Ö·û´®> ¡ú "..."
<Ìõ¼þÓï¾ä> ¡ú if (<±í´ïÊ½>) <Óï¾ä> | if (<±í´ïÊ½>) <Óï¾ä> else <Óï¾ä>
<Ñ­»·Óï¾ä> ¡ú while (<±í´ïÊ½>) <Óï¾ä>
<ÉùÃ÷Óï¾ä> ¡ú <±äÁ¿ÉùÃ÷>
 */



 //<ÊäÈëÊä³öÓï¾ä> ¡ú printf(<×Ö·û´®>); | scanf(<¸ñÊ½´®>, <±äÁ¿>);
 //<¸ñÊ½´®> ¡ú "%d" | "%f" | "%c"



#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

 // ¹Ø¼ü×Ö±í£¬µÚÒ»ÁÐÊÇ¹Ø¼ü×Ö£¬µÚ¶þÁÐÊÇ±àºÅ
char* keywords[][2] = {
    {"int", "1"}, {"void", "2"}, {"break", "3"}, {"float", "4"}, {"while", "5"},
    {"do", "6"}, {"struct", "7"}, {"const", "8"}, {"case", "9"}, {"for", "10"},
    {"return", "11"}, {"if", "12"}, {"default", "13"}, {"else", "14"},{"main", "15"}
};
int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

// ½ç·û±í£¬µÚÒ»ÁÐÊÇ½ç·û£¬µÚ¶þÁÐÊÇ±àºÅ
char* delimiters[][2] = {
    {"-", "1"}, {"/", "2"}, {"(", "3"}, {")", "4"}, {"==", "5"}, {"<=", "6"},
    {"<", "7"}, {"+", "8"}, {"*", "9"}, {">", "10"}, {"=", "11"}, {",", "12"},
    {";", "13"}, {"++", "14"}, {"{", "15"}, {"}", "16"}
};
int delimiter_count = sizeof(delimiters) / sizeof(delimiters[0]);

char identifiers[100][50]; // ±êÊ¶·û±í
int id_count = 0;

char C1[100][50]; // ³£ÕûÊý±í
int C1_count = 0;

char C2[100][50]; // ³£ÊµÊý±í
int C2_count = 0;

char CT[100][50]; // ×Ö·û³£Á¿±í
int CT_count = 0;

char ST[100][50]; // ×Ö·û´®³£Á¿±í
int ST_count = 0;

char Token[100][50]; // Token´®±í
int Token_count = 0;

int iscorret = 1;//ÊÇ·ñÕýÈ·

// Ïò±êÊ¶·û±íÌí¼ÓÔªËØ
void add_to_identifiers(const char* str) {
    for (int i = 0; i < id_count; i++) {
        if (strcmp(identifiers[i], str) == 0) return;
    }
    strcpy(identifiers[id_count++], str);
}

// Ïò³£ÕûÊý±íÌí¼ÓÔªËØ
void add_to_C1(const char* str) {
    for (int i = 0; i < C1_count; i++) {
        if (strcmp(C1[i], str) == 0) return;
    }
    strcpy(C1[C1_count++], str);
}

// Ïò³£ÊµÊý±íÌí¼ÓÔªËØ
void add_to_C2(const char* str) {
    for (int i = 0; i < C2_count; i++) {
        if (strcmp(C2[i], str) == 0) return;
    }
    strcpy(C2[C2_count++], str);
}

// Ïò×Ö·û³£Á¿±íÌí¼ÓÔªËØ
void add_to_CT(const char* str) {
    for (int i = 0; i < CT_count; i++) {
        if (strcmp(CT[i], str) == 0) return;
    }
    strcpy(CT[CT_count++], str);
}

// Ïò×Ö·û´®³£Á¿±íÌí¼ÓÔªËØ
void add_to_ST(const char* str) {
    for (int i = 0; i < ST_count; i++) {
        if (strcmp(ST[i], str) == 0) return;
    }
    strcpy(ST[ST_count++], str);
}

// ¼ì²éÊÇ·ñÊÇ¹Ø¼ü×Ö£¬ÊÇÔò·µ»Ø±àºÅ£¬·ñÔò·µ»Ø0
int is_keyword(const char* str) {
    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(keywords[i][0], str) == 0) return atoi(keywords[i][1]);
    }
    return 0;
}

// ¼ì²éÊÇ·ñÊÇ½ç·û£¬ÊÇÔò·µ»Ø±àºÅ£¬·ñÔò·µ»Ø0
int is_delimiter(const char* str) {
    for (int i = 0; i < delimiter_count; i++) {
        if (strcmp(delimiters[i][0], str) == 0) return atoi(delimiters[i][1]);
    }
    return 0;
}

// É¨ÃèÊäÈë×Ö·û´®£¬Ê¶±ð¸÷Ààµ¥´Ê²¢´æ´¢µ½TokenÊý×é
void scan(const char* source) {
    int i = 0;
    int n = strlen(source);
    while (i < n) {
        while (isspace(source[i])) i++; // Ìø¹ý¿Õ°××Ö·û
        if (i >= n) break;

        // ´¦ÀíË«×Ö·û½ç·û
        if (i + 1 < n) {
            char two[3] = { source[i], source[i + 1], '\0' };
            int delimiter_num = is_delimiter(two);
            if (delimiter_num) {
                if (Token_count < 100) { // ·ÀÖ¹Êý×éÔ½½ç
                    sprintf(Token[Token_count], "(P %d)", delimiter_num);
                    Token_count++;
                }
                i += 2;
                continue;
            }
        }

        // ´¦Àíµ¥×Ö·û½ç·û
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

        // ´¦Àí¹Ø¼ü×Ö»ò±êÊ¶·û
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

        // ´¦Àí×Ö·û³£Á¿
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

        // ´¦Àí×Ö·û´®³£Á¿
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

        // ´¦ÀíÊý×Ö£¨ÕûÊý»ò¸¡µãÊý£©
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
                    int float_valid = 1;  // ±ê¼Ç¸¡µãÊýÊÇ·ñºÏ·¨
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
                        int exp_start = j;  // ¼ÇÂ¼Ö¸Êý²¿·ÖÆðÊ¼Î»ÖÃ

                        if (i < n && (source[i] == '+' || source[i] == '-')) {
                            float_num[j++] = source[i++];
                        }

                        // ¶ÁÈ¡Ö¸Êý²¿·ÖµÄÊý×Ö
                        int exp_digit_count = 0;  // ÐÂÔö£ºÍ³¼ÆÖ¸Êý²¿·ÖÊý×Ö¸öÊý
                        while (i < n && isdigit(source[i])) {
                            float_num[j++] = source[i++];
                            exp_digit_count++;
                        }

                        // ¼ì²éÖ¸Êý²¿·ÖÊÇ·ñÓÐÊý×Ö£¨Èç"12e+"ÖÐexp_digit_count=0£©
                        if (exp_digit_count == 0) {
                            float_valid = 0;
                        }
                    }

                    float_num[j] = '\0';

                    // ÐÂÔö£ºÈô¸¡µãÊý²»ºÏ·¨£¬±ê¼Ç´íÎó²¢·µ»Ø
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

        // ´¦ÀíÒÔ.¿ªÍ·µÄ¸¡µãÊý
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

        // ÆäËû´íÎóÇé¿ö
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

// Êä³ö¸÷±íÄÚÈÝ
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

int main() {
    char source[1000];
    fgets(source, sizeof(source), stdin);
    source[strcspn(source, "\n")] = '\0'; // È¥³ý»»ÐÐ·û
    scan(source);
    if (iscorret == 1) {
        print_token_sequence();
        print_tables();
    }
    else {
        printf("ERROR");
    }
    return 0;
}
