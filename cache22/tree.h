/*tree.h*/
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#pragma GCC diagnostic push
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <assert.h>
#include <errno.h>

#define TagRoot 1
#define TagNode 2
#define TagLeaf 4

#define find_last(x) find_last_linear(x)

#define find_leaf(x,y) find_leaf_linear(x,y)

#define find_node(x) find_node_linear(x)

#define lookup(x,y) lookup_linear(x,y)

#define reterr(x) \
    errno = (x); \
    return null_ptr;

#define NoError 0
#define ExampleFile "wl.txt"
#define ExampleSearchFile "wl.50k.txt"

#define Print(x) \
        zero(buf,256); \
        strncpy((char *)buf, (char*)(x),255); \
        size = (int16)strlen((char *)buf); \
        if(size!=0) \
            if(write(fd, (char *)buf,size)!=size){ \
                perror("Write failed"); \
            }

typedef void* Nullptr;
Nullptr null_ptr = 0;

typedef unsigned int int32;
typedef unsigned short int int16;
typedef unsigned char int8;
typedef unsigned char Tag;


struct s_node{
    Tag tag;
    struct s_node *north;
    struct s_node *west;
    struct s_leaf *east;
    int8 path[256];
};

struct s_leaf{
    Tag tag;
    union u_tree *west;
    struct s_leaf *east;
    int8 key[128];
    int8 *value;
    int16 size;
};

typedef struct s_leaf Leaf;
typedef struct s_node Node;

union u_tree{
    Node n;
    Leaf l;
};

typedef union u_tree Tree;
/*
north will point to itself if it is the root node.
*/

int8 *indent(int16);
void zero(int8*, int16);
Leaf *find_leaf_linear(int8 *, int8 *);
Node *find_node_linear(int8 *);
int8 *lookup_linear(int8 *, int8 *);
Node *create_node(Node *, int8 *);
Leaf *find_last_linear(Node *);
Leaf *create_leaf(Node *, int8*, int8*, int16);
void print_tree(int, Tree *);
    int8 *example_path(int8);
    Tree * example_tree(void);
    int32 example_leaves(void);
    int8 *example_duplicate(int8*);
    int32 example_searches(int8*);
int main(int,char**);
//int main(void);
