import math

def merge5(S, T, U, V, W):
	R = list()
	s = t = u = v = w = 0
	nlen = len(S) + len(T) + len(U) + len(V) + len(W)
	S.append(float("inf"))
	T.append(float("inf"))
	U.append(float("inf"))
	V.append(float("inf"))
	W.append(float("inf"))
	for x in range(nlen):
		if S[s] < T[t]:
			if S[s] < U[u]:
				if S[s] < V[v]:
					if S[s] < W[w]:
						R.append(S[s])
						s += 1
					else:
						R.append(W[w])
						w += 1
				else:
					if V[v] < W[w]:
						R.append(V[v])
						v += 1
					else:
						R.append(W[w])
						w += 1
			else:
				if U[u] < V[v]:
					if U[u] < W[w]:
						R.append(U[u])
						u += 1
					else:
						R.append(W[w])
						w += 1
				else:
					if V[v] < W[w]:
						R.append(V[v])
						v += 1
					else:
						R.append(W[w])
						w += 1
		else:
			if T[t] < U[u]:
				if T[t] < V[v]:
					if T[t] < W[w]:
						R.append(T[t])
						t += 1
					else:
						R.append(W[w])
						w += 1
				else:
					if V[v] < W[w]:
						R.append(V[v])
						v += 1
					else:
						R.append(W[w])
						w += 1
			else:
				if U[u] < V[v]:
					if U[u] < W[w]:
						R.append(U[u])
						u += 1
					else:
						R.append(W[w])
						w += 1
				else:
					if V[v] < W[w]:
						R.append(V[v])
						v += 1
					else:
						R.append(W[w])
						w += 1
	return R
		

def mergesort5(A, n):
	if n <= 1:
		return A
	else:
		p = 0
		gap = math.ceil(n / 5)
		B = A[p : p + gap] ; p += gap
		C = A[p : p + gap] ; p += gap
		D = A[p : p + gap] ; p += gap
		E = A[p : p + gap] ; p += gap
		F = A[p : p + gap]
		return merge5(
			mergesort5(B, len(B)),
			mergesort5(C, len(C)),
			mergesort5(D, len(D)),
			mergesort5(E, len(E)),
			mergesort5(F, len(F)))


# TEST
a = [11, 3, 4, 7, 2, 9, 10, 8, 1, 5, 6]
mergesort5(a, len(a));
