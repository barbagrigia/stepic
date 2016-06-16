"""
Write a program that implements the In-mapper combining v.1 for WordCount problem in Hadoop Streaming.
-------------------------------------------------------------------------------------------------------
Sample Input: 
aut Caesar aut nihil
aut aut
de mortuis aut bene aut nihil
-------------------------------------------------------------------------------------------------------
Sample Output: 
nihil	1
aut		2
Caesar	1
aut		2
nihil	1
aut		2
de		1
bene	1
mortuis	1
-------------------------------------------------------------------------------------------------------
"""

import sys
from collections import defaultdict

for line in sys.stdin:
    d = defaultdict(int)
    for word in line.rstrip().split(" "):
        d[word] += 1
    for k, v in d.items():
        print(k, "\t", v, sep = "")