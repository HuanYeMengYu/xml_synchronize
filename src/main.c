#include "../include/xml_synchronize.h"
#include <libxml2/libxml/tree.h>

int main(int argc, char* argv[]) {
    xmlDocPtr src_doc, dst_doc;
    xmlNodePtr src_root, dst_root;

    // 加载第一个XML文件
    src_doc = xmlReadFile(argv[2], NULL, 0);
    if (src_doc == NULL) {
        printf("Error loading XML file 1\n");
        return 1;
    }
    src_root = xmlDocGetRootElement(src_doc);

    for(int i=3;i<argc;i++){
        // 加载其他XML文件
        dst_doc = xmlReadFile(argv[i], NULL, 0);
        if (dst_doc == NULL) {
            printf("Error loading XML file %d\n", i-1);
            continue;
        }
        dst_root = xmlDocGetRootElement(dst_doc);
        // 同步XML文件
        sync_xml(src_root, dst_root, argv[1]);
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

