#include "../include/xml_synchronize.h"

void get_new_value(unsigned char names[MAX_LINES][MAX_NAME_LENGTH], unsigned char contents[MAX_LINES][MAX_CONTENT_LENGTH], char* special_enum){
    FILE *file = fopen(special_enum, "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        exit(1);
    }
    int num_lines = 0;
    char line[MAX_NAME_LENGTH + MAX_CONTENT_LENGTH + 2];
    while (fgets(line, sizeof(line), file) != NULL) {
        // 去除行尾的换行符
        line[strcspn(line, "\n")] = '\0';

        // 使用空格分割 name 和 content
        char *name = strtok(line, " ");
        char *content = strtok(NULL, " ");

        // 检查 name 和 content 是否存在
        if (name != NULL && content != NULL) {
            // 将 name 和 content 复制到数组中
            strncpy(names[num_lines], name, MAX_NAME_LENGTH);
            strncpy(contents[num_lines], content, MAX_CONTENT_LENGTH);
            num_lines++;
        }
    }
}