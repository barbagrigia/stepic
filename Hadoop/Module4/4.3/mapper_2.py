"""
Implement a second mapper mapreduce task to calculate the TF-IDF using Hadoop Streaming.
The input data is the key word, and the value consists of the document number of tf and, separated by tabs.
The value in the output - a triple: document number, and unit of tf separated by ";".
-------------------------------------------------------------------------------------------------------
Sample Input: 
aut		1	4
aut		2	2
bene	2	1
de		2	1
mortuis	2	1
nihil	1	1
nihil	2	1
Caesar	1	1
-------------------------------------------------------------------------------------------------------
Sample Output: 
aut		1;4;1
aut		2;2;1
bene	2;1;1
de		2;1;1
mortuis	2;1;1
nihil	1;1;1
nihil	2;1;1
Caesar	1;1;1
-------------------------------------------------------------------------------------------------------
"""
import sys

for line in sys.stdin:
    key, n, tf = line.strip().split("\t")
    print('{0}\t{1};{2};1'.format(key, n, tf))