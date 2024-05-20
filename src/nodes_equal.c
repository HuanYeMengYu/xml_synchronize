#include "../include/xml_synchronize.h"
#include "../include/hash_map.h"

// 检查两个节点结构是否相等
int are_nodes_equal(xmlNode *node1, xmlNode *node2) {
    if(node1==NULL && node2==NULL)
        return 1;
    if(node1==NULL && node2!=NULL)
        return 0;
    if(node1!=NULL && node2==NULL)
        return 0;
    if(xmlStrcmp(node1->name, node2->name) != 0)
        return 0;

    /* 属性名称不相同判断为两节点结构不相等
    xmlAttr *attr1 = node1->properties;
    xmlAttr *attr2 = node2->properties;
    while (attr1 != NULL && attr2 != NULL) {
        if (xmlStrcmp(attr1->name, attr2->name) != 0)
            return 0;
        attr1 = attr1->next;
        attr2 = attr2->next;
    }
    if (attr1 != NULL || attr2 != NULL)
        return 0;
    */
    return 1;
}

