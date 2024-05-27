#include "../include/xml_synchronize.h"
#include "../include/hash_map.h"
#include "../include/find_files.h"

// 根据指定的模版文件和指定要同步的根目录获取需要同步的所有文件的路径
void find_files(const char *sample_filename, const char *base_path, char found_files[MAX_FILES][MAX_PATH_LENGTH], int *file_count) {
    DIR* dir = opendir(base_path[0] == '\0' ? "." : base_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // 忽略 . 和 ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // 构建完整的相对路径
        char path[MAX_PATH_LENGTH];
        if (base_path[0] == '\0') {
            snprintf(path, sizeof(path), "%s", entry->d_name);
        } else {
            snprintf(path, sizeof(path), "%s/%s", base_path, entry->d_name);
        }

        // 构建完整的绝对路径
        char full_path[MAX_PATH_LENGTH];
        if (base_path[0] == '\0') {
            snprintf(full_path, sizeof(full_path), "./%s", entry->d_name);
        } else {
            snprintf(full_path, sizeof(full_path), "%s/%s", base_path, entry->d_name);
        }

        struct stat statbuf;
        if (stat(full_path, &statbuf) == 0) {
            if (S_ISDIR(statbuf.st_mode)) {
                // 如果是目录，递归调用
                find_files(sample_filename, path, found_files, file_count);
            } else if (S_ISREG(statbuf.st_mode)) {
                // 如果是常规文件，检查文件名
                if (strcmp(entry->d_name, sample_filename) == 0) {
                    if (*file_count < MAX_FILES) {
                        strncpy(found_files[*file_count], path, MAX_PATH_LENGTH);
                        (*file_count)++;
                    } else {
                        fprintf(stderr, "Exceeded maximum file count\n");
                        closedir(dir);
                        return;
                    }
                }
            }
        } else {
            perror("stat");
        }
    }

    closedir(dir);
}
