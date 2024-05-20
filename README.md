# 基于libxml2库tree模块的xml文件同步工具

[github仓库地址](https://github.com/HuanYeMengYu/xml_synchronize)

此工具使用了c语言libxml2库中的tree模块对xml文件进行解析，并同步xml文件的格式，包括同步元素的数据、同步增删元素。

# libxml2库的安装

## 下载

[Github官网](https://github.com/GNOME/libxml2)

[发行版下载](https://gitlab.gnome.org/GNOME/libxml2/-/releases)（对版本无要求）

## 构建指导

libxml2 可以使用 GNU Autotools、CMake、meson 或平台特定子目录中的其他几个构建系统进行构建。

本文只针对此工具进行库的构建，更详细的构建选项请[参考官网](https://github.com/GNOME/libxml2)

### Autotools (for POSIX systems like Linux, BSD, macOS)

如果您从源 tarball 构建，请使用以下命令提取存档：

```Shell
tar xf libxml2-xxx.tar.gz
cd libxml2-xxx
```

如果您从 Git 树构建，则必须安装 Autotools 并运行如下指令生成配置文件：

```Shell
./autogen.sh [configuration options]
```

成功生成配置文件：

![img](https://diangroup.feishu.cn/space/api/box/stream/download/asynccode/?code=YThlODdiNGU0MDNjYTM1MTEyYjc0NmJmYTQwM2M5N2ZfVjJtZDdUMzB1dUhWWThnSlQ2QjRhaG0zVXZZUmJQUFlfVG9rZW46RkdMYWJZR1Vnb3B2cVV4TGFycGNsWXRObkpPXzE3MTYxODg1MjA6MTcxNjE5MjEyMF9WNA)

然后你可以配置并构建库：

```Shell
./configure [configuration options]
make
```

成功构建库：

![img](https://diangroup.feishu.cn/space/api/box/stream/download/asynccode/?code=YjYyNDUwZmMwY2VhMzkzZDUxMGI4NGM2MDE3MWQzZjZfRHMyelZkeGRtZzMyVEFnVERHbUJuRTIxYTdPMFhBa0lfVG9rZW46TUVBWWI4MnJJb01OSHp4dTBNaWNBU2VmbmhnXzE3MTYxODg1MjA6MTcxNjE5MjEyMF9WNA)

![img](https://diangroup.feishu.cn/space/api/box/stream/download/asynccode/?code=NTYzZGQzZmY0MjBmOTc3MjUwODhkOWQ0MzBmOGQwMGRfVVowa1o4TEFYSFdhOTN0T0hYRnBMRTdBdDVHVElrOWlfVG9rZW46RU5OUmJYcmhwb3Bwa1Z4Tk5jZ2NEaWdQbkNmXzE3MTYxODg1MjA6MTcxNjE5MjEyMF9WNA)

现在你可以运行测试套件来检查正确性：

```Shell
make check
```

![img](https://diangroup.feishu.cn/space/api/box/stream/download/asynccode/?code=ZWIyOTBkOGU0OGM0NGVlMjE2ZjQxNjUzNmI5OWU2Y2RfUHBqMjdCdHBzeVlHUExOa0lqWjVla1RJaGxzTnFqbmxfVG9rZW46Vjdra2JnRXd6bzY2NDl4NGdUWWNYNkJQbmVjXzE3MTYxODg1MjA6MTcxNjE5MjEyMF9WNA)

没有报错的话可以安装库：

```Shell
make install
```

# 基于哈希表的同步算法

## 设计思路

libxml2库解析后得到的是树状结构（多叉树）的数据，无法直接根据元素的name找到元素的地址，因此修改、增删时需要先后遍历模版xml和目标xml，时间复杂度为O(n^2)，性能较差，因此考虑使用哈希表，建立从元素name到元素地址的映射，这样就可以根据元素name直接找到元素地址，同步算法的时间复杂度为O(n)（实际性能略低，因为使用了链式地址来解决哈希冲突的问题）

```C
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
```

通过遍历xml的树状结构来构建哈希表：

```C
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
```

## 算法实现

### 元素同步数据

由于需要同步的参数是少数，因此将需要同步参数的元素的name写在指定文件中，同步时只需要读取这些name即可，不需要遍历xml，时间复杂度可以近似看做O(1)

```C
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
```

### 元素同步删除

遍历目标xml，获取每个元素的name，检查模版xml是否有对应的元素，没有则删除目标xml中的元素。遍历操作时间复杂度为O(n)，检查操作时间复杂度为O(1)，因此同步删除的时间复杂度为O(n)

```C
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
```

### 元素同步增加

遍历模版xml，获取每个元素的name，检查目标xml是否有对应的元素，没有则在目标xml中添加对应的元素。遍历操作时间复杂度为O(n)，检查操作时间复杂度为O(1)，因此同步增加的时间复杂度为O(n)

```C
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
```

# 使用方法

静态编译(库的安装位置具体看自己的配置)：

```Shell
cd xml_synchronize
gcc -static ./include/* ./src/* -o ./bin/synchronize -L/usr/local/lib -lxml2 -I/usr/local/include/libxml2/ -lm
```

运行：

sync_value.txt文件中指定了需要同步数据的元素名称

sample.xml文件为已经修改的模版xml

之后的参数（可以是多个）是指定所有要同步的xml文件

```Shell
./synchronize ../resource/sync_value.txt ../resource/sample.xml ../resource/*.xml
```
