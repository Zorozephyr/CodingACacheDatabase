# Binary Tree Database Storage Project

## 📋 Project Overview
**Goal:** Build a hierarchical database using binary tree structure  
**Architecture:** Unix-like file system with directories and files  
**Language:** C (C2x standard)

---

## 🌳 Tree Structure Rules

### Core Principles:
- **Root node**: Always starts at path = '/'
- **Root constraint**: Has exactly one child
- **Growth pattern**: Nodes grow downwards to the left
- **Left-downward bias**: Primary path structure
- **Right branches**: Subdirectories/files at same level

### Visual Diagram:
```
                    [Root: /]
                        |
                    [child]
                    /      \
            [left child]   [right child]
                /              /      \
          [node]         [node]      [node]
            /              /            /
        [node]        [node]        [node]
```

### Key Concepts:
- Each node = directory or file
- Parent-child relationships = directory hierarchy

---

## 💾 Data Structures

### Union: U_Tree
```c
union U_Tree {
    struct s_node n;  // For directories/nodes
    struct s_leaf l;  // For data storage
}
```
**Note:** Union holds either a node OR a leaf at any time (not both)

### Struct: s_node (Directory/Folder)
```c
struct s_node {
    struct s_node *up;     // Parent node pointer
    struct s_node *left;   // Connects to subfolder
    struct s_node *right;  // Sibling node pointer
    Tag tag;               // Identifies type
    char path[255];        // Full path string
}
```

### Struct: s_leaf (Data/File)
```c
struct s_leaf {
    struct s_tree *left;   // Connection pointer
    char key[255];         // Key name
    char *value;           // Stored data
    int count;             // Number of characters
    Tag tag;               // Identifies type
}
```

---

## 🔧 Build System (Makefile)

### What is a Makefile?
Special file containing instructions for the `make` build automation tool.

### Compiler Flags:
- `-Wall` → Enable all warnings (helps catch bugs)
- `-O2` → Optimization level 2 (faster code)
- `-std=c2x` → Use C2x language standard

### Linker Flags:
- `ldflags` → Links external libraries (empty for now)

---

## 🎯 Design Decisions

### Path Storage Strategy:
**Decision:** Store full path string at every level

**Example:**
```
    /Users
        /Users/login/  
            /Users/login/wait
```
Each node maintains complete path from root.

---

## 📚 C Fundamentals

### Pointers:
A pointer stores a memory address. `&variable` gets the address, `*pointer` gets the value at that address.

### Union:
A container that can hold different types but only ONE at a time. All members share the same memory space. Size = largest member.

### Bit Flags (TagRoot, TagNode, TagLeaf):
Values 1, 2, 4 use different bit positions. Combine with `|` (OR), check with `&` (AND). Used to identify what type is in the union.

### Integer Sizes:
`int8` = 1 byte (0-255), `int16` = 2 bytes (0-65535), `int32` = 4 bytes (0-4 billion). Use smallest size needed.

### Type Casting `(Node *)&root`:
`&root` gives address of union. `(Node *)` tells compiler to treat it as a Node pointer. Safe because Node starts at same memory location as union.

### Struct vs Union:
Struct holds ALL members at once (size = sum). Union holds ONE member at a time (size = largest).

### Function Returning Pointer `Node *func()`:
Means function returns a memory address. Used when creating new nodes with malloc - returns where the new node lives.

### Pointer Parameters `Node *parent`:
Pass address instead of copying entire struct. Efficient and allows modifying original data. Inside function, parameter holds an address.

### malloc (Memory Allocation):
`malloc(size)` requests bytes from the heap. Returns `void *` (generic pointer). Cast to your type: `(Node *)malloc(size)`. Memory contains garbage until cleared.

### zero() Function:
Custom function to set all bytes to 0. Needed because malloc'd memory contains random garbage. Ensures clean state before using the node.

### Arrow Operator `->`:
Use when you have a POINTER to a struct: `ptr->member`. Shorthand for `(*ptr).member`. Use dot `.` when you have the actual struct, arrow `->` when you have a pointer.

### Strings in C:
A string is an array of `char` ending with `\0` (null terminator, value 0). Functions scan until `\0` to find the end. Without `\0`, functions read garbage! Example: `"Hi"` = `['H']['i']['\0']`

### strncpy (String N Copy):
`strncpy(dest, src, max)` copies string safely with limit. Prevents buffer overflow by limiting characters copied. Always leave room for null terminator.

### String Splitting Trick:
Insert `\0` to split one string into two. Example: `"select /Users"` → set space to `\0` → `"select"` and `"/Users"` become separate strings. Use `*p = 0;` where p points to the space.

### fork() - Clone Process:
Creates identical copy of your program. Returns child's PID (positive) in parent, returns 0 in child. Use `if(pid)` for parent code, `else` for child code. Used in servers to handle multiple clients concurrently.

### When to use `&`:
Regular variable (`root`) needs `&` to get address. Pointer variable (`n`, `n2`) already IS an address - use directly. `&n` gives where pointer lives, `n` gives what address it holds.

### NULL / null_ptr:
Value 0 meaning "points to nothing". Used to indicate empty pointers. Check with `if (ptr == NULL)` or `if (!ptr)` before using pointers.

### assert():
Safety check that crashes program if condition is false. `assert(parent)` = crash if parent is NULL. Use to catch bugs during development - shows file and line number on failure.

### errno (Error Number):
Global variable that stores error codes. Set it to indicate what went wrong. `errno = NoError` (0) means success. Check errno after functions return NULL.

### reterr Macro:
Shortcut for `errno = (x); return null_ptr;`. Sets error code and returns NULL in one line. Used when function encounters an error or empty condition.

---

## 🐛 Common Pitfalls

### #define Macros:
Never use semicolons in #define statements. They do text replacement, not declarations.

### Union Member Access:
Unions must be accessed through specific members (.n for Node, .l for Leaf), not directly.

### Printf Format Specifiers:
`%p` expects a pointer (address). Use `&variable` to get the address of a variable. To print union data, access specific members like `root.n.path`.

---

## 🌐 Networking Fundamentals

### Server Port vs Client Port:
**Server port** = You choose (e.g., 12049), fixed, must be available, clients connect TO it.
**Client port** = OS assigns automatically (ephemeral, e.g., 54321), temporary per connection.

### socket() - Create a Socket:
```c
int socket(int domain, int type, int protocol);
// Example: s = socket(AF_INET, SOCK_STREAM, 0);
```
| Arg | Meaning | Common Values |
|-----|---------|---------------|
| domain | Protocol family | `AF_INET` (IPv4) |
| type | Communication type | `SOCK_STREAM` (TCP) |
| protocol | Specific protocol | `0` (default) |

**Returns:** Socket file descriptor (positive int) on success, `-1` on error. **Nothing mutated.**

### bind() - Reserve a Port:
```c
int bind(int sockfd, struct sockaddr *addr, socklen_t addrlen);
// Example: bind(s, (struct sockaddr *)&sock, sizeof(sock));
```
| Arg | Meaning | Mutated? |
|-----|---------|----------|
| sockfd | Socket from socket() | No |
| addr | Address struct with IP/port | No (read only) |
| addrlen | Size of addr struct | No |

**Returns:** `0` on success, `-1` on error. **Nothing mutated** - just associates socket with address.

### listen() - Start Accepting Connections:
```c
int listen(int sockfd, int backlog);
// Example: listen(s, 10);
```
| Arg | Meaning | Mutated? |
|-----|---------|----------|
| sockfd | Bound socket | No |
| backlog | Max pending connections queue | No |

**Returns:** `0` on success, `-1` on error. **Nothing mutated** - marks socket as passive (listening).

### accept() - Accept a Client Connection:
```c
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
// Example: client = accept(s, (struct sockaddr *)&client_addr, &len);
```
| Arg | Meaning | Mutated? |
|-----|---------|----------|
| sockfd | Listening socket | No |
| addr | Struct to store client's address | **YES! Filled with client IP/port** |
| addrlen | Pointer to size | **YES! Updated with actual size** |

**Returns:** NEW socket fd for this client (positive int), `-1` on error.
**IMPORTANT:** `addr` and `addrlen` are **OUTPUT parameters** - function fills them with client info!

### Server Flow Summary:
```
socket() → bind() → listen() → accept() → read/write → close()
   ↓         ↓          ↓           ↓
 Create   Reserve    Start     Wait for    Handle
 socket    port     listening   client     client
```

### Byte Order Conversion Functions:
Network uses big-endian, your PC (Intel) uses little-endian. Must convert!

| Function | Direction | For | Use When |
|----------|-----------|-----|----------|
| `htons(port)` | Host → Network | 16-bit (ports) | Setting `sin_port` |
| `ntohs(port)` | Network → Host | 16-bit (ports) | Reading client port |
| `htonl(addr)` | Host → Network | 32-bit (IPs) | Setting `sin_addr` |
| `ntohl(addr)` | Network → Host | 32-bit (IPs) | Reading IP as number |

### IP Address Conversion Functions:
| Function | Converts | Example |
|----------|----------|---------|
| `inet_addr("127.0.0.1")` | String → Binary | For setting up socket |
| `inet_ntoa(addr.sin_addr)` | Binary → String | For printing IP address |

**Memory trick:** h=host, n=network, s=short(16-bit), l=long(32-bit), a=ascii, ntoa=number-to-ascii

---

## 🔍 Debugging Tools

### strace -f (System Call Tracer):
Traces all system calls your program makes. Use `strace -f ./program` to see what your code is actually doing at the OS level. `-f` flag follows child processes. Helps debug segfaults, file issues, and socket problems by showing exactly which system call failed.