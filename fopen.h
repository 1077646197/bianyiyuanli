#pragma once
#define MAX_SOURCE_SIZE 1000
char source[MAX_SOURCE_SIZE];



void read_file(const char* filename) {
    FILE* file = fopen("program.txt", "r"); // ��ֻ��ģʽ���ļ�
    if (file == NULL) { // ����ļ��Ƿ��ʧ��
        perror("Error opening file"); // ���������Ϣ
        return; // �˳�����
    }
    else  printf("���ļ� ");

    // ��ȫ��ȡ�ļ����ݵ�source����
    if (fgets(source, sizeof(source), file)) { // ��ȡһ�У����ܲ�������
        // ȥ����ĩ�Ļ��з�������еĻ���
        size_t len = strlen(source);
        if (len > 0 && source[len - 1] == '\n') {
            source[len - 1] = '\0';
        }
    }
    else {
        // ����ļ�Ϊ�ջ��ȡʧ�ܣ����source
        source[0] = '\0';
    }

    // �����ļ��п��ܴ��ڵĶ������ݣ�����������ȡ�߼���
    char tempBuffer[MAX_SOURCE_SIZE] = { 0 };
    while (!feof(file) && !ferror(file)) { // ѭ����ȡֱ���ļ����������
        if (fgets(tempBuffer, sizeof(tempBuffer), file)) {
            size_t sourceLen = strlen(source);
            size_t tempLen = strlen(tempBuffer);

            // ȷ���ܳ��Ȳ�����MAX_SOURCE_SIZE
            if (sourceLen + tempLen < MAX_SOURCE_SIZE) {
                strcat(source, tempBuffer);

                // ȥ��׷�����ݺ�Ļ��з�����ѡ��
                if (tempBuffer[tempLen - 1] == '\n') {
                    source[sourceLen + tempLen - 1] = '\0';
                }
            }
            else {
                // �������������������ʾ���м򵥽ضϣ�
                strncat(source, tempBuffer, MAX_SOURCE_SIZE - sourceLen - 1);
                source[MAX_SOURCE_SIZE - 1] = '\0';
                break; // ������������ֹͣ��ȡ
            }
        }
    }

    fclose(file); // �ر��ļ�
    

}
