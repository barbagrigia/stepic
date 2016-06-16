"""
Given file logs the user navigates. Each line consists of three fields: the transition time (unix timestamp), user ID, URL, to which the user clicked.

Send mapper using Hadoop Streaming, typing the URL from every line.
-------------------------------------------------------------------------------------------------------
Sample Input: 
1448713968	user2	https://ru.wikipedia.org/
1448764519	user10	https://stepic.org/
1448713968	user5	http://google.com/
1448773411	user10	https://stepic.org/explore/courses
1448709864	user3	http://vk.com/
-------------------------------------------------------------------------------------------------------
Sample Output: 
https://ru.wikipedia.org/
https://stepic.org/
http://google.com/
https://stepic.org/explore/courses
http://vk.com/
-------------------------------------------------------------------------------------------------------
"""
import sys

for line in sys.stdin:
    timestamp, user_id, url = line.strip().split("\t")
    print(url)