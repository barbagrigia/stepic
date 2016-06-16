"""
Given file logs the user navigates. Each line consists of three fields: the transition time (unix timestamp), user ID, URL, to which the user clicked.

Send mapper using Hadoop Streaming, print only those rows from a file that match the user user10.
-------------------------------------------------------------------------------------------------------
Sample Input: 
1448713968	user2	https://ru.wikipedia.org/
1448764519	user10	https://stepic.org/
1448713968	user5	http://google.com/
1448773411	user10	https://stepic.org/explore/courses
1448709864	user3	http://vk.com/
-------------------------------------------------------------------------------------------------------
Sample Output: 
1448764519	user10	https://stepic.org/
1448773411	user10	https://stepic.org/explore/courses
-------------------------------------------------------------------------------------------------------
"""
import sys

for line in sys.stdin:
    timestamp, user_id, url = line.strip().split("\t")
    if user_id == 'user10':
        print(line.strip())