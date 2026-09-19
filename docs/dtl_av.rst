Array Value (dtl_av)
====================

The ``dtl_av`` module provides an ordered, growable array container for dynamic values (``dtl_dv_t``). Arrays can contain any mixture of dynamic values including scalars, nested arrays, and hashes.

When dynamic values are added to the array with reference incrementing enabled, the array manages their lifetime and automatically decrements reference counts when elements are removed or when the array itself is destroyed.

API Reference
-------------

Data Types
~~~~~~~~~~

.. doxygenstruct:: dtl_av_tag
   :members:

.. doxygentypedef:: dtl_av_t

.. c:type:: dtl_key_func_t

   .. code-block:: c

      typedef dtl_dv_t *(dtl_key_func_t)(const dtl_dv_t *dv);

   Key extraction function type for sorting arrays.

Lifecycle Functions
~~~~~~~~~~~~~~~~~~~

.. doxygenfunction:: dtl_av_new
.. doxygenfunction:: dtl_av_make
.. doxygenfunction:: dtl_av_delete
.. doxygenfunction:: dtl_av_create
.. doxygenfunction:: dtl_av_destroy

Element Access and Modification
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. doxygenfunction:: dtl_av_set
.. doxygenfunction:: dtl_av_get
.. doxygenfunction:: dtl_av_value
.. doxygenfunction:: dtl_av_push
.. doxygenfunction:: dtl_av_pop
.. doxygenfunction:: dtl_av_shift
.. doxygenfunction:: dtl_av_unshift

Container Capacity and Utilities
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. doxygenfunction:: dtl_av_length
.. doxygenfunction:: dtl_av_is_empty
.. doxygenfunction:: dtl_av_exists
.. doxygenfunction:: dtl_av_extend
.. doxygenfunction:: dtl_av_fill
.. doxygenfunction:: dtl_av_clear
.. doxygenfunction:: dtl_av_sort
