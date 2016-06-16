"""
Implement the reducer second mapreduce task to calculate the TF-IDF using Hadoop Streaming.
Input: key - word meaning - triple: document number, of tf words in the document and 1 (separated by ';').
Imprint: the key - pair: the word number of the document (separated by '#'), the value of - couple: of tf word in the document, the n - the number of documents with the word (separated by tabs).
-------------------------------------------------------------------------------------------------------
Sample Input: 
aut		1;4;1
aut		2;2;1
bene	2;1;1
de		2;1;1
mortuis	2;1;1
nihil	1;1;1
nihil	2;1;1
Caesar	1;1;1
-------------------------------------------------------------------------------------------------------
Sample Output: 
aut#1	4	2
aut#2	2	2
bene#2	1	1
de#2	1	1
mortuis#2	1	1
nihil#1	1	2
nihil#2	1	2
Caesar#1	1	1
-------------------------------------------------------------------------------------------------------
"""
import sys

last_key = None
terms = []

def print_terms(terms):
    for id, tf in terms:
        print(id, tf, len(terms), sep = '\t')

for line in sys.stdin:
    key, val = line.strip().split("\t")
    n, tf, cnt = val.strip().split(';')
    if last_key and last_key != key:
        print_terms(terms)
        terms.clear()
    terms.append(('{0}#{1}'.format(key, n), tf))
    last_key = key
    
if last_key:
    print_terms(terms)