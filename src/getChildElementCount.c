#include "../include/xml_synchronize.h"
#include "../include/hash_map.h"

// 函数：获取子元素的个数
int getChildElementCount(xmlNodePtr node) {
    int count = 0;
    for (xmlNodePtr child = node->children; child; child = child->next) {
        if (child->type == XML_ELEMENT_NODE) {
            count++;
        }
    }
    return count;
}