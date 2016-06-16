"""
Implement mapper from the phase 1 problem Distinct Values v1.

Mapper takes a string containing the value and tab through the list of groups, separated by commas.
-------------------------------------------------------------------------------------------------------
Sample Input: 
1	a,b
2	a,d,e
1	b
3	a,b
-------------------------------------------------------------------------------------------------------
Sample Output: 
1,a		1
1,b		1
2,a		1
2,d		1
2,e		1
1,b		1
3,a		1
3,b		1
-------------------------------------------------------------------------------------------------------
"""
import sys

for line in sys.stdin:
    (key, values_str) = line.strip().split("\t")
    values = values_str.split(",")
    for value in values:
        print(key, ",", value, "\t", 1, sep = "")