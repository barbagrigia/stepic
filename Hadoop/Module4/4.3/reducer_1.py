"""
Implement reducer mapreduce first task to calculate the TF-IDF using Hadoop Streaming.
The key component of the input data: it contains the word and number through the "#".
The key in the output is the word, and the value consists of two elements separated by tabs: document number, and tf (how many times the word is met in this document).
-------------------------------------------------------------------------------------------------------
Sample Input: 
aut#1	1
aut#1	1
aut#1	1
aut#1	1
aut#2	1
aut#2	1
bene#2	1
de#2	1
mortuis#2	1
nihil#1	1
nihil#2	1
Caesar#1	1
-------------------------------------------------------------------------------------------------------
Sample Output: 
aut	1	4
aut	2	2
bene	2	1
de	2	1
mortuis	2	1
nihil	1	1
nihil	2	1
Caesar	1	1
-------------------------------------------------------------------------------------------------------
"""
import sys

last_key = None
key_count = 0

def print_key_count(key, count):
    k,v = key.split('#', maxsplit = 1)
    print(k, v, count, sep = '\t')

for line in sys.stdin:
    key, val = line.strip().split("\t")
    if last_key and last_key != key:
        print_key_count(last_key, key_count)
        key_count = 1
    else:
        key_count += 1
    last_key = key

if last_key:
    print_key_count(last_key, key_count)