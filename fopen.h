#pragma once
#define MAX_SOURCE_SIZE 1000
char source[MAX_SOURCE_SIZE];



void read_file(const char* filename) {
    FILE* file = fopen("program.txt", "r"); // 以只读模式打开文件
    if (file == NULL) { // 检查文件是否打开失败
        perror("Error opening file"); // 输出错误信息
        return; // 退出程序
    }
    else  printf("打开文件 ");

    // 安全读取文件内容到source数组
    if (fgets(source, sizeof(source), file)) { // 读取一行（可能不完整）
        // 去除行末的换行符（如果有的话）
        size_t len = strlen(source);
        if (len > 0 && source[len - 1] == '\n') {
            source[len - 1] = '\0';
        }
    }
    else {
        // 如果文件为空或读取失败，清空source
        source[0] = '\0';
    }

    // 处理文件中可能存在的多行内容（补充完整读取逻辑）
    char tempBuffer[MAX_SOURCE_SIZE] = { 0 };
    while (!feof(file) && !ferror(file)) { // 循环读取直到文件结束或出错
        if (fgets(tempBuffer, sizeof(tempBuffer), file)) {
            size_t sourceLen = strlen(source);
            size_t tempLen = strlen(tempBuffer);

            // 确保总长度不超过MAX_SOURCE_SIZE
            if (sourceLen + tempLen < MAX_SOURCE_SIZE) {
                strcat(source, tempBuffer);

                // 去除追加内容后的换行符（可选）
                if (tempBuffer[tempLen - 1] == '\n') {
                    source[sourceLen + tempLen - 1] = '\0';
                }
            }
            else {
                // 处理超出缓冲区的情况（示例中简单截断）
                strncat(source, tempBuffer, MAX_SOURCE_SIZE - sourceLen - 1);
                source[MAX_SOURCE_SIZE - 1] = '\0';
                break; // 缓冲区已满，停止读取
            }
        }
    }

    fclose(file); // 关闭文件
    

}
