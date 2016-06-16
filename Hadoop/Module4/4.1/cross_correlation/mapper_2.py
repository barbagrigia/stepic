"""
Implement a mapper for Cross-Correlation tasks that each object from the tuple creates a stripe.

Mapper takes a tuple - row consisting of objects, separated by a space.

Mapper writes data in the form of key / value, where key - object, value - corresponding stripe (example: a: 1, b: 2, c: 3)
-------------------------------------------------------------------------------------------------------
Sample Input: 
a b
a b a c
-------------------------------------------------------------------------------------------------------
Sample Output: 
a	b:1
b	a:1
a	b:1,c:1
b	a:2,c:1
a	b:1,c:1
c	b:1,a:2
-------------------------------------------------------------------------------------------------------
"""
import sys
from collections import defaultdict

for line in sys.stdin:
    words = line.strip().split(" ")
    for i in words:
        d = defaultdict(int)
        for j in words:
            if i != j:
                d[j] += 1
        stripe = ','.join(['{0}:{1}'.format(k, v) for (k, v) in d.items()])
        print (i, stripe, sep = '\t')