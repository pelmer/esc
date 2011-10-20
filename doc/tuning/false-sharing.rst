False sharing
=============

*The goal of this exercise is to demonstrate a technique for detection
of false sharing.*

In this example, the phenomenon of false sharing is illustrated.  The
program runs several independent threads, each of which is writing to
different places in the same array.  Use ``pfmon`` (not Pfmon Deluxe)
to monitor the following event, which is related to false sharing:
**MEM_LOAD_RETIRED:OTHER_CORE_L2_HIT_HITM**.

Example::

  ./measure.sh ./falseshare 2

**Fill in Table 3.**

What would you do to minimize false sharing?
