# -*- coding: cp936 -*-
import random, os

def test():
	f=file("test.txt","w")
	n=random.randint(2,5)
	x=[]
	f.write(str(n)+"\n")
	for i in range(n):
		l=random.randint(2,10);
		a=random.sample(range(100),l);
		x.extend(a)
		a.sort()
		f.write(str(l)+"\n")
		for j in a:
			f.write(str(j)+" ")
		f.write("\n")
	f.close()
	os.system("N个有序数组求中位数.exe < test.txt")
	x.sort()
	res=open("result.txt","r")
	ans=int(res.read())
	print x[(len(x)-1)/2],ans,
	if ans!=x[(len(x)-1)/2]:
		print "Error"
		res.close()
		exit(0)
	else:
		print ""
	res.close()

for i in range(1000):
	test()
