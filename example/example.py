import launcher

d=launcher.Launcher('tree.txt')
a=[-0.891781167209,-0.652142545264,-0.178550459359]
b=[]
d.get_response(a,b)
print "XXXXXXXXXXXXXX",b
a=[2.1547068889,1.34706907631,-4.09817552007]
b=[]
d.get_response(a,b)
print "YYYYYYYYYYYYYY",b
a=[-4.96547877523,-5.79105552388,0.471001524926	]
b=[]
d.get_response(a,b)
print "ZZZZZZZZZZZZZZ",b
d=None #delete from memory