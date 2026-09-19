dtl-type
========

**dtl-type** is a lightweight, reference-counted dynamic type and variant library written in C (C99 and later). It provides a clean runtime type system and dynamic container library for C applications.

In C, building heterogeneous and deeply nested data structures (such as parsed JSON, dynamic configurations, or arbitrary object graphs) requires complex manual memory management and ad-hoc union types. ``dtl-type`` simplifies this by providing polymorphic dynamic values, variant scalars, dynamic arrays, and string-keyed hash maps with recursive reference-counted memory management.

Dynamic Value (DV)
------------------

This is the polymorphic base type that all other dynamic value types derive from. (In C, this is achieved using a common header structure ``dtl_dv_t`` and runtime type tags.)

A dynamic value can be any of the following types:

* **Null Value (``dtl_dv_null()``)**: An explicit null object (``DTL_DV_NULL``).
* **Scalar Value (``dtl_sv_t``)**: A single value variant (numbers, booleans, strings, bytes, pointers).
* **Array Value (``dtl_av_t``)**: An ordered, growable collection of dynamic values.
* **Hash Value (``dtl_hv_t``)**: A string-keyed lookup table of dynamic values.

.. mermaid::
   :align: center

   classDiagram
       direction TB
       class `Dynamic Value (DV)`
       class `Scalar Value (SV)`
       class `Array Value (AV)`
       class `Hash Value (HV)`

       `Dynamic Value (DV)` <|-- `Scalar Value (SV)`
       `Dynamic Value (DV)` <|-- `Array Value (AV)`
       `Dynamic Value (DV)` <|-- `Hash Value (HV)`


Scalar Values (SV)
------------------

A scalar contains a single unit of data.

Supported scalar types:

* **None**: Uninitialized / empty scalar (``dtl_sv_none()``, concept borrowed from Python's ``NoneType``)
* **Integers**: Signed and unsigned 32-bit and 64-bit integers (``i32``, ``u32``, ``i64``, ``u64``)
* **Floating-Point**: Single-precision ``float`` and double-precision ``double``
* **Boolean**: ``true`` or ``false``
* **String**: Managed dynamic strings (``adt_str_t``)
* **Bytes & Byte Arrays**: Immutable byte sequences (``adt_bytes_t``) and growable byte buffers (``adt_bytearray_t``)
* **Pointer**: Managed generic pointers with optional destructor callbacks
* **Dynamic Value**: Wrappers for nested dynamic values (``dtl_dv_t``)

Array Values (AV)
-----------------

Array values are managed arrays containing dynamic values (DVs).

Examples:

* Array of scalar values
* Array of array values
* Array of hash values
* Array of mixed values (any of the above)

Hash Values (HV)
----------------

Hash values are key-value lookup tables where the key is a string and the value is any dynamic value (DV).

.. toctree::
   :maxdepth: 2
   :hidden:
   :caption: API Reference

   dtl_dv
   dtl_sv
   dtl_av
   dtl_hv
   dtl_error
   dtl_type


Components Catalog
==================

Below is a summary of all modules provided by the dtl-type library:

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Module
     - Header
     - Description
   * - :doc:`dtl_dv`
     - ``dtl_dv.h``
     - Polymorphic base dynamic value and reference counting
   * - :doc:`dtl_sv`
     - ``dtl_sv.h``
     - Polymorphic scalar value variant and type conversion routines
   * - :doc:`dtl_av`
     - ``dtl_av.h``
     - Ordered growable array container for dynamic values
   * - :doc:`dtl_hv`
     - ``dtl_hv.h``
     - String-keyed dynamic hash map container
   * - :doc:`dtl_error`
     - ``dtl_error.h``
     - Standardized error codes and error types
   * - :doc:`dtl_type`
     - ``dtl_type.h``
     - Master aggregate header including all DTL submodules
