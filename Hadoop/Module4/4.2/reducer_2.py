"""
Write reducer, which makes crossing the elements of the sets A and B.
At the entrance to the reducer come pairs key / value, where key - element of the set, value - sets the marker (A or B).
-------------------------------------------------------------------------------------------------------
Sample Input: 
1	A
2	A
2	B
3	B
-------------------------------------------------------------------------------------------------------
Sample Output: 
2
-------------------------------------------------------------------------------------------------------
"""
import sys

last_key = None
key_count = 0

for line in sys.stdin:
    key,val = line.strip().split("\t")
    if last_key != key:
        if key_count > 0: print(last_key)
        key_count = 0
    else:
        key_count += 1
    last_key = key
if key_count > 0: print(last_key)