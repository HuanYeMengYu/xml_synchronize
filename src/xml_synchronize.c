#include "../include/xml_synchronize.h"
#include <libxml2/libxml/tree.h>
#include <stdio.h>

// 同步XML文件的函数
void sync_xml(xmlNode *node1, xmlNode *node2, char* special_enum) {
    if (!are_nodes_equal(node1, node2)) {
        printf("xml文件结构不同，同步失败！\n");
    }else if(!is_special_enum(node2->content, special_enum))
        node2->content = xmlStrdup(node1->content);
    // 递归处理子节点
    xmlNode* child1 = node1->children;
    xmlNode* child2 = node2->children;
    if(!are_nodes_equal(child1, child2)){
        printf("xml文件结构不同，同步失败！\n");
        return;
    }
    while (child1 != NULL && child2 != NULL) {
        sync_xml(child1, child2, special_enum);
        child1 = child1->next;
        child2 = child2->next;
    }
}

