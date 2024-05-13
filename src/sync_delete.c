#include "../include/xml_synchronize.h"
#include <libxml2/libxml/tree.h>
#include <stdio.h>

// 同步XML文件的函数
void sync_delete(xmlNode *node, char* special_enum) {
    if(is_special_enum(node->name, special_enum) && node->type==XML_ELEMENT_NODE){
        // 解除连接
        xmlUnlinkNode(node);
        // 递归释放
        xmlFreeNode(node);
        return;
    }
    // 递归处理子节点
    xmlNode* child = node->children;
    xmlNode* next_child = NULL;
    if(child != NULL)
        next_child = child->next;
    while (child != NULL) {
        sync_delete(child, special_enum);
        child = next_child;
        if(next_child)
            next_child = next_child->next;
    }
}
