#include "../include/xml_synchronize.h"
#include "../include/find_files.h"
#include <libxml2/libxml/tree.h>

int main(int argc, char* argv[]) {
    xmlDocPtr src_doc, dst_doc;
    xmlNodePtr src_root, dst_root;

    // 获取指定根目录下所有需要同步的xml文件
    char found_files[MAX_FILES][MAX_PATH_LENGTH];
    int file_count = 0;
    // 获取不含路径的文件名
    const char *sample_filename_basename = basename(argv[2]);
    for(int i=3;i<argc;i++)
        // 调用 find_files 函数搜索文件
        find_files(sample_filename_basename, argv[i], found_files, &file_count);

    // 加载第一个XML文件
    src_doc = xmlReadFile(argv[2], NULL, 0);
    if (src_doc == NULL) {
        printf("Error loading XML file 1\n");
        return 1;
    }
    src_root = xmlDocGetRootElement(src_doc);
    // 源xml转换为哈希表
    HashMapChaining* src_hashMap = newHashMapChaining();
    xml_to_hash(src_hashMap, src_root);

    for(int i=0;i<file_count;i++){
        // 加载其他XML文件
        dst_doc = xmlReadFile(found_files[i], NULL, 0);
        if (dst_doc == NULL) {
            printf("Error loading XML file: %s\n", found_files[i]);
            continue;
        }
        dst_root = xmlDocGetRootElement(dst_doc);
        // 目标xml转换为哈希表
        HashMapChaining* dst_hashMap = newHashMapChaining();
        xml_to_hash(dst_hashMap, dst_root);
        // 同步元素数据
        sync_value(src_hashMap, dst_hashMap, argv[1]);
        // 同步删除元素
        sync_delete(src_hashMap, dst_hashMap);
        // 同步增加元素
        sync_add(src_hashMap, dst_hashMap);
        // 保存修改后的XML文件
        xmlSaveFormatFile(found_files[i], dst_doc, 1);
        // 释放目标xml资源
        xmlFreeDoc(dst_doc);
        delHashMapChaining(dst_hashMap);
        // 格式化xml文件
        char command[256];
        snprintf(command, sizeof(command), "xmllint --format %s -o %s", found_files[i], found_files[i]);
        system(command);
    }
    delHashMapChaining(src_hashMap);
    // 释放源xml资源
    xmlFreeDoc(src_doc);
    xmlCleanupParser();
    return 0;
}
