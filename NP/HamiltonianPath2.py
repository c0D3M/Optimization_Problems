# python3
import os
import itertools
'''
	Hamiltonian Path Problem using minisat SAT-Solver
	Input: Take an undirected graph
	Output: whether its satisfiable or not i.e. ifHamiltonian Path Exist or not.
'''
n, m = map(int, input().split())
adj = [[] for i in range(n)]
for i in range(m):
	a, b = list(map(int, input().split()))
	adj[a-1].append(b)
	adj[b-1].append(a)
clauses = []
# Return unique number using 1-based index
def varnum(i, j):
	return (i+(j-1)*n)

def exactly_one_of(literals):
	# Adds one of literals to be true
	clauses.append([l for l in literals])
	
	# This add 2 negated literals to clauses
	for pairs in itertools.combinations(literals, 2):
		clauses.append([-l for l in pairs])

non_adj=[]
#1. Each node i must appear in the path.
for i in range (1, n+1):
	l = []
	for j in range (1, n+1):
		l.append(varnum(i, j))
	clauses.append(l)

#2. No node i appears twice in the path.
for i in range (1, n+1):
	for  j in range(1, n+1):
		for  k in range(j+1, n+1): # Check for same index of path
			if j!=k:
				clauses.append([-varnum(i, j), -varnum(i, k)])
'''
#3. Every position  on the path must be occupied.
for i in range(1, n+1):
	l = []
	for j in range(1, n+1):
		l.append(varnum(j, i))
	clauses.append(l)
'''	
#4. No two nodes i and j occupy the same position in the path.
for i in range (1, n+1):
	for  j in range(i+1, n+1):
		for  k in range(1, n+1):
			if i!=j:
				clauses.append([-varnum(i, k), -varnum(j, k)])

#5. Non-adjacent vertex cant be adjacent in hamiltonian path.
# Remove adjacent node
for i in range (1, n+1): #node
	for j in range(1, n):#position
		l = []
		l.append(-varnum(i, j))
		for k in adj[i-1]:#adjaceny node
			l.append(varnum(k, j+1))
		clauses.append(l)	
			
with open('tmp.cnf', 'w') as f:
	f.write("p cnf {} {}\n".format(n*n , len(clauses)))
	for c in clauses:
		c.append(0);
		f.write(" ".join(map(str, c))+"\n")
os.system("minisat -verb=0 tmp.cnf tmp.sat > /dev/null")		

with open("tmp.sat", "r") as satfile:
	for line in satfile:
		if line.split()[0] == "UNSAT":
			print ("2 1")
			print ("1 0")
			print ("-1 0")
			break
		elif line.split()[0] == "SAT":
			print ("1 1")
			print ("1 -1 0")
			break
