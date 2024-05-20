#include "../include/xml_synchronize.h"
#include "../include/hash_map.h"
#include <libxml2/libxml/tree.h>
#include <stdio.h>

// 同步删除元素
void sync_delete(HashMapChaining* src_hashMap, HashMapChaining* dst_hashMap){
    if(!src_hashMap || !dst_hashMap)
        return;

    for (int i = 0; i < dst_hashMap->capacity; i++) {
        Node *cur = dst_hashMap->buckets[i];
        while (cur) {
            if(get(src_hashMap, cur->pair->key) == NULL && get(src_hashMap, cur->pair->val->parent->name)!=NULL){
                xmlUnlinkNode(cur->pair->val);
                xmlFreeNode(cur->pair->val);
            }
            cur = cur->next;
        }
    }
}
