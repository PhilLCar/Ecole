p1 = ((99,145),(59,115),(70,174),(66,187),(78,144),(10,135),(3,111),(20,164),(27,110),(55,164),(42,108),(192,199),(58,142),(1,195),(87,169),(35,113),(132,149),(77,132),(37,152),(24,142),(69,105),(110,169),(29,154),(34,157),(84,200),(68,109),(68,160),(90,127),(64,111),(62,139),(161,172),(3,175),(33,198),(25,121),(117,123),(7,167),(170,200),(72,163),(37,166),(83,101),(93,105),(154,162),(89,118),(165,168),(169,189),(155,177),(108,150),(6,113),(31,103),(88,186))

p2 = ((57968,190078),(34971,173764),(24195,114779),(13134,153884),(71527,113005))

p3 = ((9,10),(4,50),(1,3),(28,33),(24,32),(1,15),(1,33),(13,26),(0,40),(28,46),(7,19),(6,39),(0,22),(0,3),(34,43),(1,2),(13,45),(23,29),(2,3),(14,16),(14,33),(7,24),(14,45),(29,50),(0,20))

p4 = ((3,10),(3,20),(3,6),(3,7),(1,13),(0,2),(17,22),(1,3),(6,23),(11,15),(5,6),(15,16))

p5 = ((10,36),(1,5),(2,18),(28,45),(17,29),(17,18),(27,30),(29,36),(1,2),(5,23),(15,18),(2,3),(18,28),(0,3),(22,46),(8,39),(8,39),(4,10),(12,19),(25,38),(9,13),(1,7),(0,33),(8,13),(16,22))

def estCongru(n, t):
	return (n - t[0]) % t[1] == 0

def nonCongru(p):
	k = 1
	l = len(p)
	for i in range(l):
		k = ppcm(k, p[i][1])
	for i in range(1, k):
		for j in range(l):
			if estCongru(i, p[j]):
				break
			if j == l - 1:
				return i
	return 0		

def pgcd(a, b):
	while b:
		a, b = b, a % b
	return a

def ppcm(a, b):
	return a* b // pgcd(a, b)

nonCongru(p1) #2
nonCongru(p2) #1
nonCongru(p3) #?
nonCongru(p4) #0
nonCongru(p5) #40
