
.. -*- Mode: rst -*-

.. role:: raw-latex(raw)
   :format: latex

.. role:: ul
.. role:: cb
.. role:: sc


=========================
Katabatic 3 Modifications
=========================


General Structuration
=====================

* Short-circuit the usage of the Hurricane DataBase. Now AutoContacts
  & AutoSegments are cached directly in the relevant objects. This is
  way bigger, but should be faster. And anyway simpler to write by
  suppressing Ring walkthrough and Session::lookup() calls.


AutoContact
===========

* Now splitted in four sub-classes:

  * AutoContactTerminal
  * AutoContactTurn
  * AutoContactHTee
  * AutoContactVTee

* isCorner() renamed isTurn().
* isTerminal() is now ambiguous. It may be flag an AutoContact which *is*
  an AutoContactTurn indeed or an AutoContactTurn leading uniquely toward
  a Terminal.


AutoHorizontal/AutoVertical
===========================

* New AutoSegment::makeTopologyDogLeg() creates dogleg needed by topological
  adjustments on AutoContact. They are not counted (nor signaled) as explicit
  dogleg request. **To Implement**.

* The local ``slacken()`` methods are, in fact AutoSegment::makeTopologyDogLeg().

* We now must be able to create AutoHorizontal/AutoContact whithout suppling
  the anchor AutoContacts. Allows more supple building constructions.
