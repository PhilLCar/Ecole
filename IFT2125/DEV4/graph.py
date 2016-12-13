T=np.array([[0,2,3,3,2,2,1,2,1,1],[2,0,3,2,2,2,3,2,1,2],[3,3,0,1,3,3,2,1,2,1],[3,2,1,0,1,1,2,3,1,1],[2,2,3,1,0,2,1,2,2,1],[2,2,3,1,2,0,3,2,3,3],[1,3,2,2,1,3,0,3,1,3],[2,2,1,3,2,2,3,0,3,1],[1,1,2,1,2,3,1,3,0,1],[1,2,1,1,1,3,3,1,1,0]])

# Fonction utilisée pour déterminer le k maximal
def maxK(T):
	max = 0;
	for x in range(1, 3):
		v = findWay(T, x, [0], 0)
		if v > max:
			max = v
	return max

# Backtracking, explore les solutions
def findWay(G, C, V, lvl):
	max = lvl
	for x in range(V[-1] + 1, len(G)):
		if G[0][x] == C:
			for y in range(len(V)):
				if G[V[y]][x] == C:
					t = findWay(G, C, V + [x], lvl + 1)
					if t > max:
						max = t
				else: return lvl
	return max
