#include "tree.h"

Tree root = {
    .n.tag = (TagRoot | TagNode),
    .n.north = (Node *)&root,
    .n.west = 0,
    .n.east = 0,
    .n.path = "/"
};

void zero(int8 *str, int16 size){
    int8 *p;
    int16 n;

    for(n=0,p=str; n<size;p++,n++)
        *p=0;
    return;
}

Node *create_node(Node *parent, int8 *path){
    Node *n;
    int16 size;

    errno = NoError;
    assert(parent);
    size = sizeof(struct s_node);
    n = (Node *)malloc((int)size);
    zero((int8 *)n,size);

    parent->west = n;
    n->tag=TagNode;
    n->north=parent;
    strncpy((char *)n->path, (char *)path, 255);
    return n;
}

Leaf *find_last_linear(Node *parent){
    Leaf *l;

    errno = NoError;
    assert(parent);

    if(!parent->east) reterr(NoError);
    for(l=parent->east;l->east;l=l->east);

    assert(l);
    return l;
}

Leaf * create_leaf(Node * parent, int8 *key, int8 *value, int16 count){
    Leaf *l,*new;
    int16 size;

    assert(parent);
    l = find_last(parent);

    size = sizeof(struct s_leaf);

    new = (Leaf*)malloc(size);
    if(!l){
        parent->east = new;
    }
    else{
        l->east = new;
    }
    zero((int8 *)new, size);
    new->tag = TagLeaf;
    new->west = (!l) ? (Tree *)parent : (Tree *)l;
    strncpy((char *)new->key, (char *)key, 127);
    new->value = (int8 *)malloc(count);
    strncpy((char *)new->value, (char *)value, count);
    new->size = count;
    return new;
}


int main(){
    Node * n, *n2;
    Leaf *l1, *l2;
    int8 *key,*value;
    int16 size;

    n = create_node((Node *)&root,(int8 *) "/Users");
    assert(n);
    n2 = create_node(n, (int8 *) "/Users/login");
    assert(n2);

    key = (int8 *)"jonas";
    value = (int8 *)"1234567890";
    size = (int16)strlen((char *)value);

    l1 = create_leaf(n2, key, value, size);
    assert(l1);

    key = (int8 *)"jonas1";
    value = (int8 *)"12345678901";
    size = (int16)strlen((char *)value);
    l2 = create_leaf(n2, key, value, size);
    assert(l2);

    printf("Leaf 1: %p, value: %s\n", (void *)l1, (char *)l1->value);
    printf("Leaf 2: %p, value: %s\n", (void *)l2, (char *)l2->value);
    printf("Root: %p\n", (void *)&root);
    printf("Node 1: %p\n", (void *)n);
    printf("Node 2: %p\n", (void *)n2);
    free(n2);
    free(n);
    return 0;
}