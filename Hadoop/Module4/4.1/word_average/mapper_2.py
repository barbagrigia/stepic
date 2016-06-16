"""
Write a program that implements the In-mapper combining v.2 for WordCount problem in Hadoop Streaming.
-------------------------------------------------------------------------------------------------------
Sample Input: 
aut Caesar aut nihil
aut aut
de mortuis aut bene aut nihil
-------------------------------------------------------------------------------------------------------
Sample Output: 
aut		6
mortuis	1
bene	1
Caesar	1
de		1
nihil	2
-------------------------------------------------------------------------------------------------------
"""

import sys
from collections import defaultdict

d = defaultdict(int)

for line in sys.stdin:
    for word in line.rstrip().split(" "):
        d[word] += 1
for k, v in d.items():
    print(k, "\t", v, sep = "")