import sys

def strip_lst(lst):
    if lst == '{}':
        return []
    return lst[1:len(lst)-1].strip().split(',')

for line in sys.stdin:
    v, w, lst = line.strip().split("\t")
    print('{0}\t{1}\t{2}'.format(v, w, lst))
    
    for ve in strip_lst(lst):
        print('{0}\t{1:.3f}\t{{}}'.format(ve, float(w) / len(strip_lst(lst))))
