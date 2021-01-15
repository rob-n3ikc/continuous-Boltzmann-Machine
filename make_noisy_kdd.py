#!/usr/bin/python
# nominally python2, but python3 should work as it isn't anything fancy

import math
import sys
import rand_float

def square( nsample, nwide, start):
    retval = []
    for i in range( start, nsample+start): 
        if (i/nwide)%2 == 0 :
            retval.append(1.)
        else: 
            retval.append(-1.)
    return retval

def sin_wave( nsample, nwide, start):
    retval = []
    for i in range(start, nsample+start):
        x = math.pi/nwide*i
        retval.append(math.sin(x))
    return retval

def to_str( numeric_list):
    retval = []
    for i in numeric_list:
        retval.append(str(i))
    return retval
# TBD  add_noise

randf = rand_float.mitchel_moore()

def add_noise(numeric_list):
   for i in  range(0, len(numeric_list)):
       x = randf.next()
       if  x < 0.05:
             numeric_list[i] += (2.*randf.next()-1.)




#MAIN function here
if len(sys.argv) < 4 :
   print('USAGE kdd_gen length width ntodo')
   sys.exit(0)

nsample = int(sys.argv[1])
nwid = int(sys.argv[2]) 
ntodo = int(sys.argv[3]) 
sys.stdout.write(str(nsample+1))
sys.stdout.write('\n')
for i in range(0, nsample):
    o = '@attribute '+ str(i) + '\n'
    sys.stdout.write(o) 
o = '@attribute class category 0 \n@data \n'
sys.stdout.write(o) 

for i in range(0,ntodo):
    o = square( nsample, nwid,i%nwid)
    add_noise(o)
#    o = sin_wave( nsample, nwid,i)
    o.append(0)
    sys.stdout.write( ','.join(to_str(o)))
    sys.stdout.write('\n')
#for i in range(0,nwid):
#    o = square( nsample, nwid,i)
#    o = sin_wave( nsample, nwid,i)
#    o.append(0)
#    sys.stdout.write( ','.join(to_str(o)))
#    sys.stdout.write('\n')
