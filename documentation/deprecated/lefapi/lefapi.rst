.. -*- Mode: rst -*-

.. include:: ../etc/definitions.rst


=================
LEF API Reference
=================

The LEF API reference is copyrighted by |Cadence| (http://www.cadence.com)
under the `Apache License, Version 2.0`_ and is available here:
`LEF API <../../lefdef/lefapi/ch1Intro.html>`_


Implementation Notes
====================

Understanding Units
~~~~~~~~~~~~~~~~~~~

In the ``UNITS`` section, the statement: ::

    DATABASE MICRONS LEFconvertFactor

Means that:

#. The distances in the |LEF| file are expresseds (always, it seems) in
   micrometers.

#. The distances are multiples of :math:`\frac{1}{LEFconvertFactor}` which is
   the smallest amount of distance we can represent. By default ``LEFconvertFactor``
   is set to ``100``, which means steps of ``0.01µm``.

   This is the equivalent of the **precision** factor in the |Hurricane| database.


Callback Calling Order
~~~~~~~~~~~~~~~~~~~~~~

The callback for the ``PIN`` section is called *before* the callback of the whole
``MACRO`` it belongs to.

As a consequence, we have to create ``Net`` before the ``Cell`` can be known.
To avoid transient storage of lots of informations while we are reading the
``PIN``, when we encounter one **and** no temporary ``Cell`` exists, we
create one named ``"LEFImportTmpCell"``. This temporary ``Cell`` will be renamed
and configured when the subsequent ``MACRO`` callback is called.
