"""
Write reducer, realizes the unification of the two files (Join) for the user id. The first file contains two tabs across the field id and user query into a search engine.
The second file contains the user id and URL, which passed by a search engine.
Mapper transmits data to the reducer in the form of key  value, where the key - id user, value consists of 2 parts source file marker (query or url) and the query or URL.
Each line reducer output should contain 3 fields separated by tabs id user, the request, the URL.
-------------------------------------------------------------------------------------------------------
Sample Input: 
user1	query:гугл
user1	url:google.ru
user2	query:стэпик
user2	query:стэпик курсы
user2	url:stepic.org
user2	url:stepic.org/explore/courses
user3	query:вконтакте
-------------------------------------------------------------------------------------------------------
Sample Output: 
user1	гугл	google.ru
user2	стэпик	stepic.org
user2	стэпик	stepic.org/explore/courses
user2	стэпик курсы	stepic.org
user2	стэпик курсы	stepic.org/explore/courses
-------------------------------------------------------------------------------------------------------
"""
import sys
from collections import defaultdict

urls = defaultdict(list)
queries = defaultdict(list)
users =  set()

for line in sys.stdin:
    key, val = line.strip().split('\t')
    mod, str = val.split(':')
    if (mod == 'url'): urls[key].append(str)
    else: queries[key].append(str)
    users.add(key)
    
for user in users:
    for url in urls[user]:
        for query in queries[user]:
            print(user, query, url, sep = '\t')