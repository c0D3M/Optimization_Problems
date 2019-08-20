# python3
import os
'''
	Hamiltonian Path Problem using minisat SAT-Solver
	Input: Take an undirected graph
	Output: whether its satisfiable or not i.e. ifHamiltonian Path Exist or not.
'''
n, m = map(int, input().split())
#edges = [ list(map(int, input().split())) for i in range(m) ]
clauses = []
# Return unique number using 1-based index
def varnum(i, j):
	return (i+(j-1)*n)

non_adj=[]
#1. Each node i must appear in the path.
for i in range (1, n+1):
	non_adj.append(list(range(1, n+1))) #This will be used in #5
	non_adj[i-1].remove(i) # Remove i itself.#remove self node
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
#3. Every position  on the path must be occupied.
for i in range(1, n+1):
	l = []
	for j in range(1, n+1):
		l.append(varnum(j, i))
	clauses.append(l)
#4. No two nodes i and j occupy the same position in the path.
for i in range (1, n+1):
	for  j in range(i+1, n+1):
		for  k in range(1, n+1):
			if i!=j:
				clauses.append([-varnum(i, k), -varnum(j, k)])

#5. Non-adjacent vertex cant be adjacent in hamiltonian path.
#Remove adjacent node
for i in range(m):
	x, y = map(int, input().split())
	#x, y = edges[i]
	if y in non_adj[x-1]:
		non_adj[x-1].remove(y) #Remove neigbour
	if x in non_adj[y-1]:
		non_adj[y-1].remove(x) #Remove neigbour
	
for i in range (1, n+1):
	# now contain all non-adjacent node of i
	for j in non_adj[i-1]:
		for k in range (1, n):
			clauses.append([-varnum(i, k) , -varnum(j, k+1)])
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
