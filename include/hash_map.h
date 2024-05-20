#ifndef HASH_TABLE
#define HASH_TABLE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/xmlexports.h>
#include <libxml2/libxml/xmlversion.h>

#define MAX_SIZE 1000

/* 键值对 int->string */
typedef struct {
    xmlChar* key;
    xmlNodePtr val;
} Pair;

/* 链表节点 */
typedef struct Node {
    Pair *pair;
    struct Node *next;
} Node;

/* 链式地址哈希表 */
typedef struct {
    int size;         // 键值对数量
    int capacity;     // 哈希表容量
    double loadThres; // 触发扩容的负载因子阈值
    int extendRatio;  // 扩容倍数
    Node **buckets;   // 桶数组
} HashMapChaining;

/* 构造函数 */
HashMapChaining* newHashMapChaining();
/* 析构函数 */
void delHashMapChaining(HashMapChaining *hashMap);
/* 哈希函数 */
int hashFunc(const HashMapChaining *hashMap, int key);
/* 负载因子 */
double loadFactor(const HashMapChaining *hashMap);
/* 查询操作 */
xmlNodePtr get(const HashMapChaining *hashMap, const xmlChar* const key);
/* 添加操作 */
void put(HashMapChaining *hashMap, xmlChar* key, xmlNodePtr val);
/* 扩容哈希表 */
void extend(HashMapChaining *hashMap);
/* 删除操作 */
void removeItem(HashMapChaining *hashMap, xmlChar* key);
/* 打印哈希表 */
void print(const HashMapChaining *hashMap);
/* 加法哈希 */
int addHash(const char *key);
/* 乘法哈希 */
int mulHash(const char *key);
/* 异或哈希 */
int xorHash(const char *key);
/* 旋转哈希 */
int rotHash(const char *key);

#endif
