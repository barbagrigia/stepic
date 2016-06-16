"""
Implement the first mapper mapreduce task to calculate the TF-IDF using Hadoop Streaming.
The format of the input data is the following: each line contains the number of the document and a string from it, separated by ":".
The key output is a composite: it contains a word document and its number, separated by "#".
The word in the document - a sequence of characters (letters or numbers) that does not contain whitespace characters and punctuation marks.
-------------------------------------------------------------------------------------------------------
Sample Input: 
1:aut Caesar aut nihil
1:aut aut
2:de mortuis aut bene aut nihil
-------------------------------------------------------------------------------------------------------
Sample Output: 
aut#1	1
Caesar#1	1
aut#1	1
nihil#1	1
aut#1	1
aut#1	1
de#2	1
mortuis#2	1
aut#2	1
bene#2	1
aut#2	1
nihil#2	1
-------------------------------------------------------------------------------------------------------
"""
import sys
import re

rWords = re.compile(u'\w+', re.UNICODE)

for line in sys.stdin:
    key, val = line.strip().split(':', maxsplit = 1)
    for word in re.findall(rWords, val):
        print('{0}#{1}'.format(word, key), 1, sep = '\t')