import sys

prev_v = None
prev_w = 0.0
prev_adj = '{}'

for line in sys.stdin:
    v, w, lst = line.strip().split("\t")                
        
    if prev_v and v != prev_v:
        print('{0}\t{1:.3f}\t{2}'.format(prev_v, prev_w, prev_adj));
        prev_w = 0.0
        prev_adj = '{}'
    
    if lst != '{}':
        prev_adj = lst;
    else:
        prev_w += float(w)
    prev_v =  v
        
print('{0}\t{1:.3f}\t{2}'.format(prev_v, prev_w, prev_adj));
