# DTL Type Submodule TODO List

This document tracks identified architectural enhancements, planned features, and API improvements across the `dtl_type` library for future branches/releases.

---

## 1. Implement Key-Based Array Sorting (Schwartzian Transform / DSU)

### Background
[dtl_av_sort](file:///home/cogu/repo/dtl_type/include/dtl_av.h) accepts a `key` parameter of type [dtl_key_func_t](file:///home/cogu/repo/dtl_type/include/dtl_av.h):
```c
typedef dtl_dv_t *(dtl_key_func_t)(const dtl_dv_t *dv);

dtl_error_t dtl_av_sort(dtl_av_t *self, dtl_key_func_t *key, bool reverse);
```
Currently, passing a non-NULL `key` returns `DTL_NOT_IMPLEMENTED_ERROR`. The parameter was designed after Python's `list.sort(key=...)`, where `key` is a 1-argument function extracting a comparable sort key from each array element (for instance, sorting an array of `dtl_hv_t` hashes by extracting a specific field like `"age"` or `"name"`).

### The Challenge in C
`dtl_av_sort` delegates sorting to `adt_ary_sort` in the `adt` library, which expects a 2-argument comparator (`int (*)(const void *a, const void *b)`). Because standard C function pointers cannot capture state (closures), the comparator cannot directly access the user-provided `key` function pointer without using global/thread-local state or changing `adt`'s API to support a `void *user_data` context.

Furthermore, calling `key()` inside a comparator would invoke it $O(N \log N)$ times during comparisons, which is inefficient when `key` performs lookups or string manipulations.

### Proposed Solution: Decorate-Sort-Undecorate (Schwartzian Transform)
Use an intermediate array of tuples/pairs to cache the extracted keys:

```
[ elem0, elem1, elem2 ]
         ↓  1. Decorate: evaluate key(elem) exactly N times
[ (key0, elem0), (key1, elem1), (key2, elem2) ]
         ↓  2. Sort: sort pairs by comparing precomputed keys
[ (key1, elem1), (key0, elem0), (key2, elem2) ]
         ↓  3. Undecorate: write sorted elements back
[ elem1, elem0, elem2 ]
```

#### Architecture:
1. **Intermediate Struct**:
   ```c
   typedef struct dtl_sort_pair_tag {
      dtl_dv_t *key;   /**< Precomputed sort key (extracted via key(elem)) */
      dtl_dv_t *elem;  /**< Original array element */
   } dtl_sort_pair_t;
   ```
2. **Phase 1: Decorate ($O(N)$)**:
   - Allocate a temporary array of `dtl_sort_pair_t` with size $N$.
   - Iterate through `self->pAny`, assigning `pairs[i].elem = elem` and `pairs[i].key = key(elem)`.
   - If `key(elem)` returns `NULL` or fails, handle error and clean up.
3. **Phase 2: Sort ($O(N \log N)$)**:
   - Define a static pair comparator:
     ```c
     static int dtl_sort_pair_vlt(const void *a, const void *b)
     {
        const dtl_sort_pair_t *p_a = (const dtl_sort_pair_t *) a;
        const dtl_sort_pair_t *p_b = (const dtl_sort_pair_t *) b;
        return dtl_sv_vlt(p_a->key, p_b->key);
     }
     ```
   - Sort the pairs array.
4. **Phase 3: Undecorate ($O(N)$)**:
   - Copy the sorted elements back into `self->pAny`:
     ```c
     for (int32_t i = 0; i < n; i++)
     {
        self->pAny->pFirst[i] = pairs[i].elem;
     }
     ```
   - Free the temporary pairs array.

### Tasks
- [ ] Define internal `dtl_sort_pair_t` and `dtl_sort_pair_vlt` comparator in `src/dtl_av.c`.
- [ ] Implement Schwartzian transform path in `dtl_av_sort` when `key != NULL`.
- [ ] Add unit tests in `test/testsuite_dtl_av.c`:
  - Sorting an array of hashes by string key (e.g. `"name"`).
  - Sorting an array of hashes by integer key (e.g. `"age"`).
  - Key function returning `reverse = true` vs `reverse = false`.
  - Error handling when a key function produces an uncomparable value or fails.

---

## 2. Deep Copy / Cloning (`dtl_dv_clone`)

### Background
Currently, sharing dynamic value structures relies on reference counting (`dtl_inc_ref`). In scenarios where a completely independent, mutable clone of a dynamic tree is needed (e.g., copying template configurations or deep-duplicating JSON documents), callers must manually traverse and reconstruct the structure.

### Tasks
- [ ] **`dtl_dv_clone(const dtl_dv_t *dv) -> dtl_dv_t *`** in `include/dtl_dv.h` & `src/dtl_dv.c`:
  - Base dynamic value clone dispatching to type-specific clone functions.
- [ ] **`dtl_sv_clone(const dtl_sv_t *sv) -> dtl_sv_t *`** in `include/dtl_sv.h` & `src/dtl_sv.c`:
  - Clones scalars, duplicating underlying strings (`adt_str_t`), byte arrays (`adt_bytearray_t`), or bytes (`adt_bytes_t`).
- [ ] **`dtl_av_clone(const dtl_av_t *av) -> dtl_av_t *`** in `include/dtl_av.h` & `src/dtl_av.c`:
  - Recursively clones all child elements.
- [ ] **`dtl_hv_clone(const dtl_hv_t *hv) -> dtl_hv_t *`** in `include/dtl_hv.h` & `src/dtl_hv.c`:
  - Recursively clones all key-value pairs.
- [ ] Add comprehensive unit tests covering deep cloning of nested structures.
