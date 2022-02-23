All levels in levels.txt are from [this database](https://www.michaelfogleman.com/rush/#DatabaseDownload).

I made a Python script to convert them from the database format to 7 3-Byte and 1 1-Byte base 18 numbers each for compression.

The game reads that format and converts them back to 36-byte char arrays. The numbers inside levels.txt can be ignored.

Only the 36-character string is the actual level. I won't publish the conversion script since it's a mess.
