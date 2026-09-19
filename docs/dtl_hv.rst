Hash Value (dtl_hv)
===================

The ``dtl_hv`` module provides a string-keyed hash table (dictionary/map) container for dynamic values (``dtl_dv_t``).

It maps null-terminated C string keys to arbitrary dynamic values. Like arrays, hash tables manage the reference counts and lifetime of stored values and support full dictionary iteration and key extraction.

API Reference
-------------

Data Types
~~~~~~~~~~

.. doxygenstruct:: dtl_hv_tag
   :members:

.. doxygentypedef:: dtl_hv_t

Lifecycle Functions
~~~~~~~~~~~~~~~~~~~

.. doxygenfunction:: dtl_hv_new
.. doxygenfunction:: dtl_hv_delete
.. doxygenfunction:: dtl_hv_create
.. doxygenfunction:: dtl_hv_destroy

Key-Value Access and Mutation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. doxygenfunction:: dtl_hv_set_cstr
.. doxygenfunction:: dtl_hv_get_cstr
.. doxygenfunction:: dtl_hv_remove_cstr
.. doxygenfunction:: dtl_hv_exists_cstr

Iteration
~~~~~~~~~

.. doxygenfunction:: dtl_hv_iter_init
.. doxygenfunction:: dtl_hv_iter_next_cstr

Container Utilities
~~~~~~~~~~~~~~~~~~~

.. doxygenfunction:: dtl_hv_length
.. doxygenfunction:: dtl_hv_keys
