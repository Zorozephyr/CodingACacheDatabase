# Error Log

---

## Error #1: Union Access & #define Semicolons
**Date:** Dec 7, 2025

**Error:** `'Tree' has no member named 'tag'` and `expected ')' before ';'`

**Cause:** Accessed union directly (`.tag`) instead of through member (`.n.tag`). Also had semicolons in `#define TagRoot 1;`

**Fix:**
```c
#define TagRoot 1              // No semicolon!
.n.tag = (TagRoot | TagNode)   // Access through .n
```

---

## Warning #1: Printf %p Expects Pointer
**Date:** Dec 7, 2025

**Warning:** `format '%p' expects 'void *', but argument has type 'Tree'`

**Cause:** `%p` needs pointer, not the actual struct.

**Fix:** `printf("%p\n", &root);` — add `&` to get address.

---

## Warning #2: Pointer Signedness Mismatch
**Date:** Dec 7, 2025

**Warning:** `pointer targets differ in signedness`

**Cause:** `int8` = `unsigned char`, but string literals are `char *` (signed).

**Fix:** Cast strings: `(int8 *)"/Users"`

---

## Error #2: Segfault - Casting Int to Pointer
**Date:** Dec 7, 2025

**Error:** `Segmentation fault (core dumped)`

**Cause:** `sport = (char *)PORT;` — PORT is number 12049, cast makes it memory address 12049!

**Fix:** Use number directly: `port = PORT;` or make PORT a string: `#define PORT "12049"`

---

## Error #3: Address Already in Use (Socket)
**Date:** Dec 7, 2025

**Error:** `Address already in use`

**Cause:** Binding to same port multiple times in a loop, or socket in TIME_WAIT state.

**Fix:** Bind ONCE (outside loop). Add `SO_REUSEADDR`:
```c
int opt = 1;
setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
// Then bind()
```

---

## Error #4: Variadic Macro Missing ##
**Date:** Dec 8, 2025

**Error:** Log not printing or trailing comma errors.

**Cause:** `#define log(x, args...) printf(x, args)` — missing `##` causes trailing comma when no args.

**Fix:** `#define log(x, ...) printf(x, ##__VA_ARGS__)`

---

## Error #5: Server Connection Closes Immediately
**Date:** Dec 8, 2025

**Error:** `Connection closed by foreign host` (telnet)

**Cause:** `mainloop()` accepts client then returns. No loop = program exits immediately.

**Fix:**
```c
while(scontinuation){
    mainloop(s);      // Loop to keep accepting
}
// In mainloop: close(s2) after handling client
```

---

## Error #6: SO_REUSEADDR for Quick Restart
**Date:** Dec 8, 2025

**Error:** `Address already in use` when restarting server quickly.

**Cause:** OS holds port in TIME_WAIT (~60 sec) after close.

**Fix:** Add before `bind()`:
```c
int opt = 1;
setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
```

---
