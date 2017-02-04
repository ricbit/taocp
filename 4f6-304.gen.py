N = 6
print "~ 4f6-304 generator"
for i in xrange(N):
    for j in xrange(N):
        if i != j:
            print "~x%d x%d" % (i, j)
