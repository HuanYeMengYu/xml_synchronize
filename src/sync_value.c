#include "../include/xml_synchronize.h"
#include "../include/hash_map.h"
#include <libxml2/libxml/tree.h>
#include <stdio.h>

// 同步指定的XML元素数据
void sync_value(HashMapChaining* src_hashMap, HashMapChaining* dst_hashMap, const char* special_enum) {
    if(!src_hashMap || !dst_hashMap)
        return;
    
    for (int i = 0; i < dst_hashMap->capacity; i++) {
        Node *cur = dst_hashMap->buckets[i];
        while (cur) {
            if(get(src_hashMap, cur->pair->key)!=NULL)
                if(is_special_enum(cur->pair->key, special_enum))
                    xmlNodeSetContent(cur->pair->val, xmlStrdup((xmlChar*)(xmlNodeGetContent(get(src_hashMap, cur->pair->key)))));
            cur = cur->next;
        }
    }
}
