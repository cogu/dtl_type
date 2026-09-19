Aggregate Header (dtl_type)
============================

The ``dtl_type.h`` header is the primary aggregate header for the ``dtl_type`` library. Including this file automatically includes all public component headers:

.. code-block:: c

   #include "dtl_type.h"

Included Submodules
-------------------

Including ``dtl_type.h`` provides full access to:

* :doc:`dtl_error`: Error codes and standard return types (``dtl_error_t``).
* :doc:`dtl_dv`: Base polymorphic dynamic value type (``dtl_dv_t``) and reference counting.
* :doc:`dtl_sv`: Polymorphic scalar value variants (``dtl_sv_t``).
* :doc:`dtl_av`: Dynamic array container (``dtl_av_t``).
* :doc:`dtl_hv`: Dynamic hash table container (``dtl_hv_t``).

API Reference
-------------

.. doxygenfile:: dtl_type.h
   :project: dtl_type
