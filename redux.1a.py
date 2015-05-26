A = [5, 1, 5, 5, 1, 5]
B = [4, 3, 4, 4, 3, 6]
C = [3, 2, 3, 6, 2, 6]

# 1 1 5 5 5 5 
# 3 3 4 4 4 6
# 4/6*(5/6) + (2/6)*(0)
# 20 / 36 = 5/9

def gcd(a, b):
  if b == 0:
    return a
  else:
    return gcd(b, a % b)
   
def compare(a, b):
  greater, total = 0, 0
  for i in a:
    for j in b:
      total += 1
      if i > j:
        greater += 1
  d = gcd(greater, total)
  print "%d/%d" % (greater / d, total / d)

compare(A, B)
compare(B, C)
compare(C, A)
