"""
Implement a reducer of tasks Distinct Values v2.

Reducer accepts input lines, each of which consists of a tab-delimited values, and the group name.
-------------------------------------------------------------------------------------------------------
Sample Input: 
1	a
1	b
1	b
2	a
2	d
2	e
3	a
3	b
-------------------------------------------------------------------------------------------------------
Sample Output: 
a	3
d	1
b	2
e	1
-------------------------------------------------------------------------------------------------------
"""
import sys
from collections import defaultdict

d = defaultdict(int)
last = (None, None)

for line in sys.stdin:
    (val, key) = line.strip().split("\t")
    if (val, key) != last:
        d[key] += 1
    last = (val, key)
    
for key, val in d.items():
    print(key, val, sep = "\t")