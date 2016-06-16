"""
Implement Combiner problem in calculating the average time spent by a user on the page.

Mapper writes data in the form of key / value, where key - page address, value - a pair of numbers separated by ";".
The first - the number of seconds spent by the user on this page, is equal to 1 second.
-------------------------------------------------------------------------------------------------------
Sample Input: 
www.facebook.com	100;1
www.google.com	10;1
www.google.com	5;1
www.google.com	15;1
stepic.org	60;1
stepic.org	100;1
-------------------------------------------------------------------------------------------------------
Sample Output: 
www.facebook.com	100;1
www.google.com	30;3
stepic.org	160;2
-------------------------------------------------------------------------------------------------------
"""
import sys

last_key = None
last_val = 0
last_count = 0

for line in sys.stdin:
    (key, val_count) = line.strip().split("\t")
    val, count = val_count.split(";")
    if last_key and last_key != key:
        print(last_key, "\t", last_val, ";", last_count, sep = "")
        last_key = key
        last_count = 1
        last_val = int(val)
    else:
        last_key = key
        last_count += 1
        last_val += int(val)

if last_key:
    print(last_key, "\t", last_val, ";", last_count, sep = "")