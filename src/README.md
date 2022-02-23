All levels in levels.txt are from [this database](https://www.michaelfogleman.com/rush/#DatabaseDownload).

I made a Python script to convert them from the database format to a 1-Byte integer storing the minimum required amount

of moves for the Level (the first number of each line in levels.txt) followed by 7 3-Byte and 1 1-Byte base 18 numbers

for each level. The game reads that format and converts them back to 36-byte char arrays. The numbers at the end of the

lines in levels.txt are not used for the game and can be ignored. The 36-character string is the actual level.

I won't publish the conversion script since it's a mess.
