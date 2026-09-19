Scalar Value (dtl_sv)
======================

The ``dtl_sv`` module implements polymorphic scalar values (single unit variants). A scalar can hold integers, floating-point numbers, booleans, characters, dynamic strings, raw byte buffers, wrapped dynamic values, or managed pointers with user-defined destructor callbacks.

It provides type-safe conversions between different scalar types with success indicators, reference-counted lifetime management, and comparison routines.

Scalar Type IDs
---------------

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Type ID
     - Stored Value Type
   * - ``DTL_SV_NONE``
     - Uninitialized / empty scalar.
   * - ``DTL_SV_I32``
     - 32-bit signed integer (``int32_t``).
   * - ``DTL_SV_U32``
     - 32-bit unsigned integer (``uint32_t``).
   * - ``DTL_SV_I64``
     - 64-bit signed integer (``int64_t``).
   * - ``DTL_SV_U64``
     - 64-bit unsigned integer (``uint64_t``).
   * - ``DTL_SV_FLT``
     - Single-precision float (``float``).
   * - ``DTL_SV_DBL``
     - Double-precision float (``double``).
   * - ``DTL_SV_CHAR``
     - Single character (``char``).
   * - ``DTL_SV_BOOL``
     - Boolean flag (``bool``).
   * - ``DTL_SV_STR``
     - Dynamic string (``adt_str_t``).
   * - ``DTL_SV_PTR``
     - Pointer with optional destructor callback (``dtl_pv_t``).
   * - ``DTL_SV_DV``
     - Wrapped dynamic value (``dtl_dv_t``).
   * - ``DTL_SV_BYTES``
     - Immutable byte sequence (``adt_bytes_t``).
   * - ``DTL_SV_BYTEARRAY``
     - Growable byte buffer (``adt_bytearray_t``).

API Reference
-------------

Data Types
~~~~~~~~~~

.. doxygenenum:: dtl_sv_type_tag

.. doxygenstruct:: dtl_pv_tag
   :members:

.. doxygenstruct:: dtl_sv_tag
   :members:

.. doxygentypedef:: dtl_sv_t

Lifecycle Functions
~~~~~~~~~~~~~~~~~~~

.. doxygenfunction:: dtl_sv_new
.. doxygenfunction:: dtl_sv_delete
.. doxygenfunction:: dtl_sv_create
.. doxygenfunction:: dtl_sv_destroy

Constructors and Factories
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. doxygenfunction:: dtl_sv_make_i32
.. doxygenfunction:: dtl_sv_make_u32
.. doxygenfunction:: dtl_sv_make_i64
.. doxygenfunction:: dtl_sv_make_u64
.. doxygenfunction:: dtl_sv_make_flt
.. doxygenfunction:: dtl_sv_make_dbl
.. doxygenfunction:: dtl_sv_make_bool
.. doxygenfunction:: dtl_sv_make_char
.. doxygenfunction:: dtl_sv_make_ptr
.. doxygenfunction:: dtl_sv_make_str
.. doxygenfunction:: dtl_sv_make_cstr
.. doxygenfunction:: dtl_sv_make_dv
.. doxygenfunction:: dtl_sv_make_bytes
.. doxygenfunction:: dtl_sv_make_bytes_raw
.. doxygenfunction:: dtl_sv_make_bytearray
.. doxygenfunction:: dtl_sv_make_bytearray_raw

Type Inspection and Getters
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. doxygenfunction:: dtl_sv_type
.. doxygenfunction:: dtl_sv_dv_type
.. doxygenfunction:: dtl_sv_get_bytes
.. doxygenfunction:: dtl_sv_get_bytearray

Setters
~~~~~~~

.. doxygenfunction:: dtl_sv_set_i32
.. doxygenfunction:: dtl_sv_set_u32
.. doxygenfunction:: dtl_sv_set_i64
.. doxygenfunction:: dtl_sv_set_u64
.. doxygenfunction:: dtl_sv_set_flt
.. doxygenfunction:: dtl_sv_set_dbl
.. doxygenfunction:: dtl_sv_set_bool
.. doxygenfunction:: dtl_sv_set_char
.. doxygenfunction:: dtl_sv_set_ptr
.. doxygenfunction:: dtl_sv_set_str
.. doxygenfunction:: dtl_sv_set_cstr
.. doxygenfunction:: dtl_sv_set_bstr
.. doxygenfunction:: dtl_sv_set_dv
.. doxygenfunction:: dtl_sv_set_bytes
.. doxygenfunction:: dtl_sv_set_bytes_raw
.. doxygenfunction:: dtl_sv_set_bytearray
.. doxygenfunction:: dtl_sv_set_bytearray_raw
.. doxygenfunction:: dtl_sv_take_bytes

Conversion Functions
~~~~~~~~~~~~~~~~~~~~

.. doxygenfunction:: dtl_sv_to_i32
.. doxygenfunction:: dtl_sv_to_u32
.. doxygenfunction:: dtl_sv_to_i64
.. doxygenfunction:: dtl_sv_to_u64
.. doxygenfunction:: dtl_sv_to_flt
.. doxygenfunction:: dtl_sv_to_dbl
.. doxygenfunction:: dtl_sv_to_bool
.. doxygenfunction:: dtl_sv_to_char
.. doxygenfunction:: dtl_sv_to_ptr
.. doxygenfunction:: dtl_sv_to_str
.. doxygenfunction:: dtl_sv_to_cstr
.. doxygenfunction:: dtl_sv_to_dv
.. doxygenfunction:: dtl_sv_to_sv
.. doxygenfunction:: dtl_sv_to_av
.. doxygenfunction:: dtl_sv_to_hv

Comparison and Sorting
~~~~~~~~~~~~~~~~~~~~~~

.. doxygenfunction:: dtl_sv_lt
.. doxygenfunction:: dtl_sv_vlt

Macros
~~~~~~

.. doxygendefine:: dtl_sv_none
