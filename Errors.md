# Error Log

## Error #1: Union Member Access & #define Semicolons

### Date: Dec 7, 2025

### Errors Encountered:
```
tree.c:4:6: error: 'Tree' {aka 'union u_tree'} has no member named 'tag'
    4 |     .tag = (TagRoot | TagNode),
      |      ^~~

tree.h:12:18: error: expected ')' before ';' token
   12 | #define TagRoot 1;
```

### Root Causes:

**1. Union Direct Member Access:**
- Tried to access `.tag` directly on union `Tree`
- Unions don't have direct members - must access through `.n` (Node) or `.l` (Leaf)

**2. Semicolons in #define Macros:**
- Had semicolons after `#define` values: `#define TagRoot 1;`
- `#define` does text replacement, so `TagRoot` became `1;` causing syntax errors
- Example: `(TagRoot | TagNode)` expanded to `(1; | 2;)` ❌

### Solution:

**Fix 1 - Remove semicolons from all #defines in tree.h:**
```c
#define TagRoot 1   // No semicolon!
#define TagNode 2
#define TagLeaf 4
```

**Fix 2 - Access union members correctly in tree.c:**
```c
Tree root = {
    .n.tag = (TagRoot | TagNode),    // Access through .n (Node)
    .n.north = (Tree *)&root,
    .n.west = 0,
    .n.east = 0,
    .n.path = "/"
};
```

### Key Takeaways:
- **#define** = text replacement, never use semicolons
- **Union access** = Always specify which struct member (.n or .l)
- Unions hold ONE type at a time, not both simultaneously

---

## Warning #1: Printf Format Specifier Mismatch

### Date: Dec 7, 2025

### Warning Encountered:
```
tree.c:12:14: warning: format '%p' expects argument of type 'void *', 
but argument 2 has type 'Tree' {aka 'union u_tree'} [-Wformat=]
   12 |     printf("%p\n", root);
      |             ~^     ~~~~
      |              |     |
      |              |     Tree {aka union u_tree}
      |              void *
```

### Root Cause:
- `%p` format specifier expects a **pointer** (memory address)
- `root` is a **union variable** (the actual data), not a pointer
- Tried to print the entire union structure, which doesn't work with `%p`

### Solution Options:

**Option 1 - Print the address of root:**
```c
printf("%p\n", &root);  // Use & to get memory address
```

**Option 2 - Print specific data inside root:**
```c
printf("Path: %s\n", root.n.path);  // Print path string
printf("Tag: %u\n", root.n.tag);    // Print tag value
```

### Key Takeaway:
- **Without &**: `root` = the actual data structure
- **With &**: `&root` = the memory address where root is stored
- `%p` always requires a pointer, so use `&` before non-pointer variables

---

## Warning #2: Pointer Signedness Mismatch

### Date: Dec 7, 2025

### Warning Encountered:
```
tree.c:36:36: warning: pointer targets in passing argument 2 of 'create_node' 
differ in signedness [-Wpointer-sign]
   36 |     n = create_node((Node *)&root, "/Users");
      |                                    ^~~~~~~~
      |                                    char *

tree.c:20:39: note: expected 'int8 *' {aka 'unsigned char *'} 
but argument is of type 'char *'
   20 | Node *create_node(Node *parent, int8 *path){
```

### Root Cause:
- `int8` is defined as `unsigned char` (0 to 255)
- String literals like `"/Users"` are `char *` (signed, -128 to 127)
- Function expects `int8 *` but receives `char *` - different signedness

### Solution:
Cast string literals to `int8 *` when calling the function:
```c
n = create_node((Node *)&root, (int8 *)"/Users");
n2 = create_node(n, (int8 *)"/Users/login");
```

### Key Takeaway:
- `char` and `unsigned char` are both 1 byte but have different signedness
- For ASCII text (0-127), both work identically - this is just a warning
- Cast string literals to `int8 *` to maintain consistency with your codebase design

---
