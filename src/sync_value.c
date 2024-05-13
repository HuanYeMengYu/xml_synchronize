#include "../include/xml_synchronize.h"
#include <libxml2/libxml/tree.h>
#include <stdio.h>

// 同步XML文件的函数
void sync_value(xmlNode *node, unsigned char names[MAX_LINES][MAX_NAME_LENGTH], unsigned char contents[MAX_LINES][MAX_CONTENT_LENGTH]) {
    if(!node)
        return;
        
    int i = is_sync_value(node->name, names);
    if(i!=-1 && node->type==XML_ELEMENT_NODE)
        xmlNodeSetContent(node, xmlStrdup((xmlChar*)contents[i]));

    // 递归处理子节点
    xmlNode* child = node->children;
    while (child != NULL) {
        sync_value(child, names, contents);
        child = child->next;
    }
}
