def encodeBeta(n):
	if n > 1:
		return encodeBeta(n // 2) + str(n % 2)
	else:
		return str(n)

def encodeGamma(n):
	if n == 0:
		return ''
	else:
		l = encodeBeta(n)
		return encodeGamma(len(l) - 1) + l

def encodeOmega(n):
	return encodeGamma(n) + '0'
