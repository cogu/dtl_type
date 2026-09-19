Error Handling (dtl_error)
===========================

The ``dtl_error`` module defines standard error constants and types returned by functions throughout the ``dtl_type`` library.

Error Constants
---------------

.. list-table::
   :header-rows: 1
   :widths: 35 15 50

   * - Constant
     - Value
     - Description
   * - ``DTL_NO_ERROR``
     - 0
     - Operation completed successfully.
   * - ``DTL_INVALID_ARGUMENT_ERROR``
     - 1
     - Invalid argument passed to a function.
   * - ``DTL_MEM_ERROR``
     - 2
     - Memory allocation failure.
   * - ``DTL_NOT_IMPLEMENTED_ERROR``
     - 3
     - Feature or method is not implemented.
   * - ``DTL_TYPE_ERROR``
     - 4
     - Type mismatch encountered.
   * - ``DTL_CONVERSION_ERROR``
     - 5
     - Type conversion failed.

API Reference
-------------

Data Types
~~~~~~~~~~

.. doxygentypedef:: dtl_error_t

Error Defines
~~~~~~~~~~~~~

.. doxygendefine:: DTL_NO_ERROR
.. doxygendefine:: DTL_INVALID_ARGUMENT_ERROR
.. doxygendefine:: DTL_MEM_ERROR
.. doxygendefine:: DTL_NOT_IMPLEMENTED_ERROR
.. doxygendefine:: DTL_TYPE_ERROR
.. doxygendefine:: DTL_CONVERSION_ERROR
