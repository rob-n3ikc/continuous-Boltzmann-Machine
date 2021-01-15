
class mitchel_moore:

 def __init__(me,  seed=10101):
   me.ip = 23
   me.jp = 54 
   me.kp = 0
   me.buff = []
   for i in range(0,55):
      seed = (seed *2349+14867)%32767
      me.buff.append(   float(seed)/32767.)

 def next(me):
   retv = (me.buff[me.ip] + me.buff[me.jp])
   if retv > 1.:
      retv -= 1.
   me.buff[me.kp] = retv
   me.ip = (me.ip+1)%55
   me.jp = (me.jp+1)%55
   me.kp = (me.kp+1)%55
   return retv

if __name__ == '__main__':
   s = mitchel_moore()
   x = {}
   for i in range(0,100000):
#       print( str(i)+' '+str(s.next()))
       j = int(s.next()*100)
       try:
            x[j] += 1.
       except:
            x[j] = 1.

   print(x)
