"""
Implement mapper of the phase 2 task Distinct Values v1.

Mapper takes a string containing the value and group separated by commas.
-------------------------------------------------------------------------------------------------------
Sample Input: 
1,a
2,a
3,a
1,b
3,b
2,d
2,e
-------------------------------------------------------------------------------------------------------
Sample Output: 
a	1
a	1
a	1
b	1
b	1
d	1
e	1
-------------------------------------------------------------------------------------------------------
"""
import sys

for line in sys.stdin:
    (key, value) = line.strip().split(",")
    print(value, 1, sep = "\t")