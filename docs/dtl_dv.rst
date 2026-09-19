Dynamic Value Base (dtl_dv)
============================

The ``dtl_dv`` module defines the polymorphic base structure and reference counting mechanism for all dynamic types in the ``dtl_type`` library.

Every dynamic value (scalars, arrays, hashes) shares a common header (defined by ``DTL_DV_HEAD``) containing an internal pointer, reference count, and type flags. This enables unified memory management, runtime type identification, and automatic recursive destruction.

Type Enumeration
----------------

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Type ID
     - Description
   * - ``DTL_DV_NULL``
     - Represents an explicit null value.
   * - ``DTL_DV_SCALAR``
     - Scalar value holding numbers, booleans, strings, bytes, or pointers (:doc:`dtl_sv`).
   * - ``DTL_DV_ARRAY``
     - Growable array of dynamic values (:doc:`dtl_av`).
   * - ``DTL_DV_HASH``
     - String-keyed hash map of dynamic values (:doc:`dtl_hv`).

API Reference
-------------

Data Types and Constants
~~~~~~~~~~~~~~~~~~~~~~~~

.. doxygenstruct:: dtl_dv_tag
   :members:

.. doxygentypedef:: dtl_dv_t

.. doxygenenum:: dtl_dv_type_id

Lifecycle and Factory Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. doxygenfunction:: dtl_dv_null
.. doxygenfunction:: dtl_dv_delete
.. doxygenfunction:: dtl_dv_vdelete

Reference Counting
~~~~~~~~~~~~~~~~~~

.. doxygenfunction:: dtl_dv_inc_ref
.. doxygenfunction:: dtl_dv_dec_ref
.. doxygenfunction:: dtl_dv_dec_ref_void

Type Inspection
~~~~~~~~~~~~~~~

.. doxygenfunction:: dtl_dv_type

Convenience Macros
~~~~~~~~~~~~~~~~~~

.. doxygendefine:: dtl_ref_cnt
.. doxygendefine:: dtl_inc_ref
.. doxygendefine:: dtl_dec_ref
