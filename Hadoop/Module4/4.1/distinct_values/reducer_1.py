"""
Implement a reducer of the phase 1 problem Distinct Values v1.

Reducer receives input data by mapper from the previous step.
-------------------------------------------------------------------------------------------------------
Sample Input: 
1,a		1
1,b		1
1,b		1
2,a		1
2,d		1
2,e		1
3,a		1
3,b		1
-------------------------------------------------------------------------------------------------------
Sample Output: 
1,a
1,b
2,a
2,d
2,e
3,a
3,b
-------------------------------------------------------------------------------------------------------
"""
import sys

last_key = None

for line in sys.stdin:
    (key, value) = line.strip().split("\t")
    if key != last_key:
        print(key)
    last_key = key