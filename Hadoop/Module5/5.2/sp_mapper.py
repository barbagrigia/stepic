import sys

def strip_lst(lst):
    return lst[1:len(lst)-1].strip().split(',')

for line in sys.stdin:
    v, w, lst = line.strip().split("\t")
    print('{0}\t{1}\t{2}'.format(v, w, lst))
    
    for ve in strip_lst(lst):
        if ve == '':
            continue
        if w != 'INF':
            print('{0}\t{1}\t{2}'.format(ve, int(w)+1, '{}'))
        else:
            print('{0}\tINF\t{1}'.format(ve, '{}'))
