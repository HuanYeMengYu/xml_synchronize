#include "../include/xml_synchronize.h"
#include "../include/hash_map.h"

/* 构造函数 */
HashMapChaining* newHashMapChaining() {
    HashMapChaining *hashMap = (HashMapChaining *)malloc(sizeof(HashMapChaining));
    hashMap->size = 0;
    hashMap->capacity = 4;
    hashMap->loadThres = 2.0 / 3.0;
    hashMap->extendRatio = 2;
    hashMap->buckets = (Node **)malloc(hashMap->capacity * sizeof(Node*));
    for (int i = 0; i < hashMap->capacity; i++)
        hashMap->buckets[i] = NULL;
    return hashMap;
}

/* 析构函数 */
void delHashMapChaining(HashMapChaining *hashMap) {
    for (int i = 0; i < hashMap->capacity; i++) {
        Node *cur = hashMap->buckets[i];
        while (cur) {
            Node *tmp = cur;
            cur = cur->next;
            free(tmp->pair);
            free(tmp);
        }
    }
    free(hashMap->buckets);
    free(hashMap);
}

/* 哈希函数 */
int hashFunc(const HashMapChaining *hashMap, int key) {
    return key % hashMap->capacity;
}

/* 负载因子 */
double loadFactor(const HashMapChaining *hashMap) {
    return (double)hashMap->size / (double)hashMap->capacity;
}

/* 查询操作 */
xmlNodePtr get(const HashMapChaining *hashMap, const xmlChar* const key) {
    int index = hashFunc(hashMap, mulHash(key));
    // 遍历桶，若找到 key ，则返回对应 val
    Node *cur = hashMap->buckets[index];
    while (cur) {
        if (xmlStrcmp(cur->pair->key, key) == 0) {
            return cur->pair->val;
        }
        cur = cur->next;
    }
    return NULL; // 若未找到 key ，则返回空字符串
}

/* 添加操作 */
void put(HashMapChaining *hashMap, xmlChar* key, xmlNodePtr val) {
    // 当负载因子超过阈值时，执行扩容
    if (loadFactor(hashMap) > hashMap->loadThres) {
        extend(hashMap);
    }
    int index = hashFunc(hashMap, mulHash(key));
    // 遍历桶，若遇到指定 key ，则更新对应 val并返回
    Node *cur = hashMap->buckets[index];
    while (cur) {
        if (xmlStrcmp(cur->pair->key, key) == 0) {
            cur->pair->val = val;
            return;
        }
        cur = cur->next;
    }
    // 若无该 key ，则将键值对添加至链表头部
    Pair *newPair = (Pair*)malloc(sizeof(Pair));
    newPair->key = key;
    newPair->val = val;
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->pair = newPair;
    newNode->next = hashMap->buckets[index];
    hashMap->buckets[index] = newNode;
    hashMap->size++;
}

/* 扩容哈希表 */
void extend(HashMapChaining *hashMap) {
    // 暂存原哈希表
    int oldCapacity = hashMap->capacity;
    Node **oldBuckets = hashMap->buckets;
    // 初始化扩容后的新哈希表
    hashMap->capacity *= hashMap->extendRatio;
    hashMap->buckets = (Node **)malloc(hashMap->capacity * sizeof(Node *));
    for (int i = 0; i < hashMap->capacity; i++)
        hashMap->buckets[i] = NULL;
    hashMap->size = 0;
    // 将键值对从原哈希表搬运至新哈希表
    for (int i = 0; i < oldCapacity; i++) {
        Node *cur = oldBuckets[i];
        while (cur) {
            put(hashMap, cur->pair->key, cur->pair->val);
            Node *temp = cur;
            cur = cur->next;
            // 释放内存
            free(temp->pair);
            free(temp);
        }
    }
    free(oldBuckets);
}

/* 删除操作 */
void removeItem(HashMapChaining *hashMap, xmlChar* key) {
    int index = hashFunc(hashMap, mulHash(key));
    Node *cur = hashMap->buckets[index];
    Node *pre = NULL;
    while (cur) {
        if (xmlStrcmp(cur->pair->key, key)) {
            // 从中删除键值对
            if (pre) {
                pre->next = cur->next;
            } else {
                hashMap->buckets[index] = cur->next;
            }
            // 释放内存
            free(cur->pair);
            free(cur);
            hashMap->size--;
            return;
        }
        pre = cur;
        cur = cur->next;
    }
}

/* 打印哈希表 */
void print(const HashMapChaining *hashMap) {
    for (int i = 0; i < hashMap->capacity; i++) {
        Node *cur = hashMap->buckets[i];
        printf("[");
        while (cur) {
            printf("%s -> %s,", cur->pair->key, xmlNodeGetContent(cur->pair->val));
            cur = cur->next;
        }
        printf("]\n");
    }
}
