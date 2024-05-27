#ifndef FIND_FILES
#define FIND_FILES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#define MAX_FILES 1024
#define MAX_PATH_LENGTH 1024

// 根据指定的模版文件和指定要同步的根目录获取需要同步的所有文件的路径
void find_files(const char *sample_filename, const char *base_path, char found_files[MAX_FILES][MAX_PATH_LENGTH], int *file_count);

#endif
