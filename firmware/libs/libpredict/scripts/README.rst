libpredict scripts
==================

Converting TLE lines to compact format
--------------------------------------

Libpredict provides a python script that converts the conventional TLE lines 
to a custom binary format. This format was named as "compact" and has the bare minimum 
information to predict the position of a satellite. The idea is to generate the 
binary representation of the TLE lines beforehand and update the predictor at runtime.
An example of its usage using the ISS TLE lines can be seen below:

.. code-block:: bash

   convert_to_compact_tle --print-hex "1 25544U 98067A   25061.57006296  .00014645  00000+0  26707-3 0  9993" "2 25544  51.6363 115.1019 0006087 335.9132 188.1830 15.49668581498622"

The output of the command is the binary representation of the bytes in "compact" format seen below.

.. code-block:: bash

    ASCII Hex: 0019404EC8F7D2B4EE34000017C7433C2ED943A7F4E4398C05874049D172474538EF405CC6858793DD98402EFE4D9A3CACAF

More options format options are available, use the "-h" flag to see the complete usage.
