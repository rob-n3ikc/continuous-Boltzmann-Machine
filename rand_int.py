
class mitchel_moore:

 def __init__(me, modulus, seed=10101):
   me.ip = 23
   me.jp = 54 
   me.kp = 0
   me.mod = modulus
   me.buff = []
   for i in range(0,55):
      seed = (seed *2349+14867)%32767
      me.buff.append(  int( modulus * float(seed)/32767.))

 def next(me):
   retv = (me.buff[me.ip] + me.buff[me.jp])%me.mod
   me.buff[me.kp] = retv
   me.ip = (me.ip+1)%55
   me.jp = (me.jp+1)%55
   me.kp = (me.kp+1)%55
   return retv

if __name__ == '__main__':
   s = mitchel_moore(26)
   x = {}
   for i in range(0,100000):
#      print( str(i)+' '+str(s.next()))
       j = s.next()
       try:
            x[j] += 1
       except:
            x[j] = 1

   print(x)
