#include "../include/xml_synchronize.h"
#include "../include/hash_map.h"

// 创建xml文件的哈希表
HashMapChaining* xml_to_hash(HashMapChaining* hashMap, const xmlNode* node){
    if(!node || node->type!=XML_ELEMENT_NODE)
        return NULL;

    put(hashMap, node->name, node);

    // 递归处理子节点
    xmlNode* child = node->children;
    while (child != NULL) {
        xml_to_hash(hashMap, child);
        child = child->next;
    }
}
