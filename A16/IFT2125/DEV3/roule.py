import math

def nb1(n):
	r = 0
	while n != 0:
		r += n % 2
		n //= 2
	return r

def nb0(n):
	r = 0
	while n != 0:
		r += 1 - n % 2
		n //= 2
	return r

def modif(b, n):
	a = n
	p = 0
	b = math.ceil(b / 2)
	while b != 0:
		if a % 2 == 1:
			n -= 2 ** p
			b -= 1
		p += 1
	return n
		

def roule(a, n):
	if n == 1:
		return a
	else if n == 2:
		return a * a
	else
		b = nb1(n)
		if b == 1:
			z = nb0(n)
			p = 2 ** (z // 2)
			q = 2 ** math.ceil(z / 2)
			return roule(roule(a, p), q)
		else:
			p = modif(b, n)
			return roule(a, p) * roule(a, n - p)
