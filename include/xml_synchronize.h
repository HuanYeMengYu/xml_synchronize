#include <stdio.h>
#include <stdbool.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/xmlexports.h>
#include <libxml2/libxml/xmlversion.h>

#define MAX_LENGTH 100

// 同步xml文件
void sync_xml(xmlNode *node1, xmlNode *node2, char* special_enum);
// 判断两个元素的名称和属性名称是否相同
int are_nodes_equal(xmlNode *node1, xmlNode *node2);
// 判断该元素是否为特殊元素
bool is_special_enum(const xmlChar* name, const char* special_enum);
