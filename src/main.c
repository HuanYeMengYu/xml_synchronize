#include "../include/xml_synchronize.h"
#include <libxml2/libxml/tree.h>

int main(int argc, char* argv[]) {
    xmlDocPtr src_doc, dst_doc;
    xmlNodePtr src_root, dst_root;

    unsigned names[MAX_LINES][MAX_NAME_LENGTH];
    unsigned contents[MAX_LINES][MAX_CONTENT_LENGTH];
    memset(names, 0, sizeof(names));
    memset(contents, 0, sizeof(contents));
    get_new_value(names, contents, argv[1]);

    // 加载第一个XML文件
    src_doc = xmlReadFile(argv[4], NULL, 0);
    if (src_doc == NULL) {
        printf("Error loading XML file 1\n");
        return 1;
    }
    src_root = xmlDocGetRootElement(src_doc);
    // 更新第一个xml文件的数据
    sync_value(src_root, names, contents);

    for(int i=5;i<argc;i++){
        // 加载其他XML文件
        dst_doc = xmlReadFile(argv[i], NULL, 0);
        if (dst_doc == NULL) {
            printf("Error loading XML file %d\n", i-3);
            continue;
        }
        dst_root = xmlDocGetRootElement(dst_doc);
        // 同步删除元素
        sync_delete(dst_root, argv[3]);
        // 同步元素数据
        sync_value(dst_root, names, contents);
        // 保存修改后的XML文件
        xmlSaveFormatFile(argv[i], dst_doc, 1);
        // 释放目标xml资源
        xmlFreeDoc(dst_doc);
    }
    // 释放源xml资源
    xmlFreeDoc(src_doc);
    xmlCleanupParser();
    return 0;
}

