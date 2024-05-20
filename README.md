- 程序参数解析
- 文档

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

![img](https://diangroup.feishu.cn/space/api/box/stream/download/asynccode/?code=M2UxNmQ0YjdhNjJhMGE5ODQyNmUxM2M3NTJkY2E0YTFfOVc1SVZSaGdYNUtvU1JBUFpQVFJMeVNHd0hrRXdVSVJfVG9rZW46RkdMYWJZR1Vnb3B2cVV4TGFycGNsWXRObkpPXzE3MTYxODQ1MDA6MTcxNjE4ODEwMF9WNA)

然后你可以配置并构建库：

```Shell
./configure [configuration options]
make
```

成功构建库：

![img](https://diangroup.feishu.cn/space/api/box/stream/download/asynccode/?code=OWI4NmYyYTUwYmZjOGJkYTI4MmIyYjc0YjZhN2I0NzRfU1BSamVROXJQdzBaV1VQbEJVbGZoYkVmTEtZRE5uNUNfVG9rZW46TUVBWWI4MnJJb01OSHp4dTBNaWNBU2VmbmhnXzE3MTYxODQ1MDA6MTcxNjE4ODEwMF9WNA)

![img](https://diangroup.feishu.cn/space/api/box/stream/download/asynccode/?code=NTgwNmNkNzIyMzNhNTA3OTVkMDI4ZTgyODFhMDcxMWNfV2phU1h2REszMjB2SVdSbTkyWGZEZmRWQldpYzU5d3VfVG9rZW46RU5OUmJYcmhwb3Bwa1Z4Tk5jZ2NEaWdQbkNmXzE3MTYxODQ1MDA6MTcxNjE4ODEwMF9WNA)

现在你可以运行测试套件来检查正确性：

```Shell
make check
```

![img](https://diangroup.feishu.cn/space/api/box/stream/download/asynccode/?code=MzVjMmE2NDlmZjcxOTI4NGZhNDNkN2JlMDQzOTVhN2JfTVdZV29xS2o2WjF2S2tqWWlwUEFxSTFjc3ppMnQ0RlNfVG9rZW46Vjdra2JnRXd6bzY2NDl4NGdUWWNYNkJQbmVjXzE3MTYxODQ1MDA6MTcxNjE4ODEwMF9WNA)

没有报错的话可以安装库：

```Shell
make install
```

# 基于哈希表的同步算法

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


