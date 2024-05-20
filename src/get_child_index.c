#include "../include/xml_synchronize.h"
#include "../include/hash_map.h"

// 获取元素在其父元素的子元素列表中的序号
int get_child_index(xmlNodePtr node){
    if(!node)
        return -1;

    xmlNodePtr parent = node->parent;
    if(!parent)
        return -1;

    xmlNodePtr cur = parent->children;
    int num = 0;
    while(cur){
        if(cur->type == XML_ELEMENT_NODE){
            num++;
            if(xmlStrcmp(cur->name, node->name)==0)
                return num;
        }
        cur = cur->next;
    }
}
