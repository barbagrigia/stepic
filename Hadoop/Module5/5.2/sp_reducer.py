import sys

def strip_lst(lst):
    if lst == '{}':
        return []
    return lst[1:len(lst)-1].strip().split(',')

prev_v = None
v_min = None
v_set = set()

for line in sys.stdin:
    v, w, lst = line.strip().split("\t")                
        
    if prev_v and v != prev_v:
        wstr = str(v_min) if v_min != None else 'INF'
        print('{0}\t{1}\t{{{2}}}'.format(prev_v, wstr, ','.join(map(str, sorted(map(int, list(v_set)))))));
        v_min = None
        v_set = set()
    
    for e in strip_lst(lst):
        v_set.add(e)
    if w != 'INF' and (not v_min or int(w) < v_min):
        v_min = int(w)
    prev_v =  v
        
wstr = str(v_min) if v_min != None else 'INF'
print('{0}\t{1}\t{{{2}}}'.format(prev_v, wstr, ','.join(map(str, sorted(map(int, list(v_set)))))));
