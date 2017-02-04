import random
import sys

m = int(sys.argv[1])
a = int(sys.argv[2])
p = float(sys.argv[3])
T = 20000
acc = 0
for _ in xrange(T):
    c = a
    while c != 0 and c != m:
        if random.random() > p:
            if c > m / 2:
                c += 1
            else:
                c -= 1
        else:
            c += random.randint(0, 1) * 2 - 1
        acc += 1
print float(acc) / T

