"""
Write reducer, which implements the symmetric difference of the sets A and B (ie, leaves only those elements that are only in one of the sets).
At the entrance to the reducer come pairs key / value, where key - element of the set, value - sets the marker (A or B).
-------------------------------------------------------------------------------------------------------
Sample Input: 
1	A
2	A
2	B
3	B
-------------------------------------------------------------------------------------------------------
Sample Output: 
1
3
-------------------------------------------------------------------------------------------------------
"""
import sys

last_key = None
last_mark = None
key_count = 0

for line in sys.stdin:
    key, mark = line.strip().split("\t")
    if last_key != key:
        if key_count == 1: print(last_key)
        key_count = 1
    else:
        key_count += 1
    last_key = key
    last_mark = mark
if key_count == 1: print(last_key)