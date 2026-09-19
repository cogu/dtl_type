![unit tests](https://github.com/cogu/dtl_type/workflows/unit%20tests/badge.svg)

# dtl_type

A lightweight, reference-counted dynamic type and variant library for C.

## Overview

In C, building heterogeneous and deeply nested data structures (such as parsed JSON, dynamic configurations, or arbitrary object graphs) requires complex manual memory management and ad-hoc union types.

`dtl_type` provides a clean runtime type system and dynamic container library built around:

- **Polymorphic Dynamic Values (`dtl_dv_t`)**: A unified base handle for all dynamic types.
- **Scalar Variants (`dtl_sv_t`)**: Flexible containers holding numbers (signed/unsigned 32/64-bit integers, floats, doubles), booleans, strings, raw byte arrays, or custom pointers with automatic destructor callbacks. Includes safe runtime type inspection and cross-type conversion routines.
- **Dynamic Arrays (`dtl_av_t`)**: Ordered, growable collections that can contain any combination of dynamic values (scalars, nested arrays, or hashes).
- **Hash Maps (`dtl_hv_t`)**: String-keyed lookup tables storing arbitrary dynamic values.
- **Automatic Lifetime Management**: Pure reference counting across all objects. When a root container is released, all nested children are automatically cleaned up without memory leaks.

## Key Features

- **Recursive Reference Counting**: Simplifies memory management when building complex, shared object trees.
- **Safe Type Conversions**: Built-in conversion between integers, floating-point numbers, booleans, and strings with success flags.
- **Custom Pointer Management**: Scalars can manage external pointers and automatically trigger a user-defined destructor callback when freed.
- **Lightweight & Portable**: Written in standard C99 with zero external dependencies beyond `adt` and `cutil`.

## Where is it used?

* [cogu/dtl_json](https://github.com/cogu/dtl_json)
* [cogu/c-apx](https://github.com/cogu/c-apx)

This repo is a submodule of the [cogu/c-apx](https://github.com/cogu/c-apx) (top-level) project.

## Dependencies

* [cogu/adt](https://github.com/cogu/adt)
* [cogu/cutil](https://github.com/cogu/cutil)

The unit test project(s) assume that the repos are cloned side-by-side to a common directory as seen below.

* adt
* cutil
* dtl_type (this repo)

### Git Example

```bash
cd ~
mkdir repo && cd repo
git clone https://github.com/cogu/adt.git
git clone https://github.com/cogu/cutil.git
git clone https://github.com/cogu/dtl_type.git
cd dtl_type
```

## Related projects

The [cogu/dtl_json](https://github.com/cogu/dtl_json) project provides JSON serialization and deserialization routines based on the dtl_type system.

## Building with CMake

First clone this repo and its dependencies into a common directory (such as ~/repo) as seen above. Alternatively the repos can be submodules of a top-level repo (as seen in [cogu/c-apx](https://github.com/cogu/c-apx)).

For Windows, use a "Native tools command prompt" from your Visual Studio installation. It comes with a cmake binary that
by default chooses the appropriate compiler version.

### Using CMake Presets (Clang 18 + Ninja)

```bash
# Run unit tests
cmake --preset clang-test
cmake --build --preset clang-test
ctest --preset clang-test

# Address and Undefined Behavior Sanitizers (ASan + UBSan)
cmake --preset clang-asan
cmake --build --preset clang-asan
ctest --preset clang-asan

# Static Analysis
cmake --preset clang-tidy
cmake --build --preset clang-tidy
```

### Manual CMake Workflows (Linux and Windows)

For Windows, use a "Native tools command prompt" from your Visual Studio installation. It comes with a cmake binary that
by default chooses the appropriate compiler version.

#### Running unit tests

Configure:

```sh
cmake -S . -B build-test -GNinja -DUNIT_TEST=ON
```

Build:

```sh
cmake --build build-test
```

Run test cases:

```sh
ctest --test-dir build-test --output-on-failure
```
