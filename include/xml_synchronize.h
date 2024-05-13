#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/xmlexports.h>
#include <libxml2/libxml/xmlversion.h>

#define MAX_LENGTH 100
#define MAX_LINES 1000
#define MAX_NAME_LENGTH 50
#define MAX_CONTENT_LENGTH 100

// 同步元素数据
void sync_value(xmlNode *node1, xmlNode *node2, char* special_enum);
// 同步删除元素
void sync_delete(xmlNode *node, char* special_enum);
// 判断两个元素的名称和属性名称是否相同
int are_nodes_equal(xmlNode *node1, xmlNode *node2);
// 判断该元素是否为特殊元素
bool is_special_enum(const xmlChar* name, const char* special_enum);
