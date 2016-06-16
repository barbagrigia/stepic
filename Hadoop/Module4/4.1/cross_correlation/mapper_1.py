"""
Implement a mapper for Cross-Correlation tasks that for each tuple generates all pairs of elements included in it.

Mapper takes a tuple - row consisting of objects, separated by a space.

Mapper writes data in the form of key / value, where key - pair of objects, separated by a comma, value - unit.
-------------------------------------------------------------------------------------------------------
Sample Input: 
a b
a b a c
-------------------------------------------------------------------------------------------------------
Sample Output: 
a,b		1
b,a		1
a,b		1
a,c		1
b,a		1
b,a		1
b,c		1
a,b		1
a,c		1
c,a		1
c,b		1
c,a		1
-------------------------------------------------------------------------------------------------------
"""
import sys

for line in sys.stdin:
    words = line.strip().split(" ")
    for i in words:
        for j in words:
            if i != j:
                print(i, ",", j, "\t", 1, sep = "")