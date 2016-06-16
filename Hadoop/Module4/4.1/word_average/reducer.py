"""
Implement reducer in the problem of calculating the average time spent by a user on the page.

Mapper reducer transmits data in the form of key / value, where key - page address, value - the number of seconds spent by the user on this page.

The average time to bring the output to an integer.
-------------------------------------------------------------------------------------------------------
Sample Input: 
www.facebook.com	100
www.google.com	10
www.google.com	5
www.google.com	15
stepic.org	60
stepic.org	100
-------------------------------------------------------------------------------------------------------
Sample Output: 
www.facebook.com	100
www.google.com	10
stepic.org	80
-------------------------------------------------------------------------------------------------------
"""
import sys

last_key = None
last_val = 0
last_count = 0

for line in sys.stdin:
    (key, val) = line.strip().split("\t")
    if last_key and last_key != key:
        print(last_key, int(last_val / last_count), sep = "\t")
        last_key = key
        last_count = 1
        last_val = int(val)
    else:
        last_key = key
        last_count += 1
        last_val += int(val)

if last_key:
    print(last_key, int(last_val / last_count), sep = "\t")