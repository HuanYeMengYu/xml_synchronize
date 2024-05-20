#include "../include/xml_synchronize.h"
#include "../include/hash_map.h"

// 同步增加元素
void sync_add(HashMapChaining* src_hashMap, HashMapChaining* dst_hashMap){
    if(!src_hashMap || !dst_hashMap)
        return;

    for (int i = 0; i < src_hashMap->capacity; i++) {
        Node *cur = src_hashMap->buckets[i];
        while (cur) {
            if(cur->pair->val->parent->type == XML_DOCUMENT_NODE){
                cur = cur->next;
                continue;
            }
            xmlNodePtr dst_parent = get(dst_hashMap, cur->pair->val->parent->name);
            // 只需要找到src树中新加元素子树的根元素，然后复制整个子树即可
            if(get(dst_hashMap, cur->pair->key)==NULL && dst_parent!=NULL){
                int child_index = get_child_index(cur->pair->val);
                if(child_index == 1){
                    if(getChildElementCount(cur->pair->val->parent)==1){
                        xmlAddChild(dst_parent, xmlCopyNode(cur->pair->val, 1));
                    }else{
                        xmlAddPrevSibling(get_first_child(dst_parent), xmlCopyNode(cur->pair->val, 1));
                    }
                }else{
                    xmlNodePtr prev_child = dst_parent->children;
                    for(int i=0;i<child_index-1;i++){
                        while(prev_child && prev_child->type!=XML_ELEMENT_NODE)
                            prev_child = prev_child->next;
                        prev_child = prev_child->next;
                    }
                    prev_child = prev_child->prev;
                    xmlAddNextSibling(prev_child, xmlCopyNode(cur->pair->val, 1));
                }
            }
            cur = cur->next;
        }
    }
}
