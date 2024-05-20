#ifndef XML_SYNCHRONIZE
#define XML_SYNCHRONIZE

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/xmlexports.h>
#include <libxml2/libxml/xmlversion.h>
#include "hash_map.h"

#define MAX_LENGTH 100
#define MAX_LINES 1000
#define MAX_NAME_LENGTH 100
#define MAX_CONTENT_LENGTH 200

// 获取元素的第一个子元素，没有子元素返回NULL
xmlNodePtr get_first_child(xmlNodePtr node);
// 函数：获取子元素的个数
int getChildElementCount(xmlNodePtr node);
// 获取元素在其父元素的子元素列表中的序号
int get_child_index(xmlNodePtr node);
// 同步元素数据
void sync_value(HashMapChaining* src_hashMap, HashMapChaining* dst_hashMap, const char* special_enum);
// 同步增加元素
void sync_add(HashMapChaining* src_hashMap, HashMapChaining* dst_hashMap);
// 同步删除元素
void sync_delete(HashMapChaining* src_hashMap, HashMapChaining* dst_hashMap);
// 判断两个元素的名称和属性名称是否相同
int are_nodes_equal(xmlNode *node1, xmlNode *node2);
// 判断该元素是否为特殊元素
bool is_special_enum(const xmlChar* name, const char* special_enum);
// 判断元素的数据是否需要同步
int is_sync_value(const xmlChar* name, unsigned char names[MAX_LINES][MAX_NAME_LENGTH]);
// 获取需用同步的数据
void get_new_value(unsigned char names[MAX_LINES][MAX_NAME_LENGTH], unsigned char contents[MAX_LINES][MAX_CONTENT_LENGTH], char* special_enum);
// 创建xml文件的哈希表
HashMapChaining* xml_to_hash(HashMapChaining* hashMap, const xmlNode* node);

#endif
