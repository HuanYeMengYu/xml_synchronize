#include "../include/xml_synchronize.h"
#include "../include/hash_map.h"

// 获取元素的第一个子元素，没有子元素返回NULL
xmlNodePtr get_first_child(xmlNodePtr node){
    if(!node)
        return NULL;
    xmlNodePtr child = node->children;
    while(child){
        if(child->type == XML_ELEMENT_NODE)
            return child;
        child = child->next;
    }
    return NULL;
}
