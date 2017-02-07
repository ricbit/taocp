from fractions import Fraction as F

def binomial(a, b):
    ans = 1
    for i in xrange(1, b + 1):
        ans = ans * (a - i + 1) / i
    return ans

def ga(m, a, p, q):
    return F(a, q - p) - F(
        2 * p * q * F(p, q) ** m * (F(q , p) ** a - 1), (q - p) ** 2)

def g(m, pl):
    p = F(pl, 2)
    q = 1 - p
    acc = 0
    for a in xrange(0, m):
        acc += 2 * binomial(2 * m + 2, a + 1) * (1 + ga(m, a, p, q))
    acc += binomial(2 * m + 2, m + 1) * (1 + ga(m, m, p, q))
    return F(1, 2 ** (2 * m + 2)) * acc

def ga_odd(m, a, p, q):
    return F(a, q - p) - F(
        q * F(p, q) ** m * (F(q , p) ** a - 1), (q - p) ** 2)

def g_odd(m, pl):
    p = F(pl, 2)
    q = 1 - p
    acc = 0
    for a in xrange(0, m):
        acc += 2 * binomial(2 * m + 1, a + 1) * (1 + ga_odd(m, a, p, q))
    return F(1, 2 ** (2 * m + 1)) * acc

print float(g_odd(3, F(50, 100)))

for pl in [F(1, 1000), F(1, 100), F(1, 10), F(5, 10), F(9, 10), F(99, 100), F(999, 1000)]:
    print float(g(499, pl))
