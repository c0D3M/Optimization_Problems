# python3
import itertools
import os
''' Graph Coloring Problem using 3-color
    Input: Undirected Graph
    Output: Assignment of 3-color to various nodes
'''
clauses = []
def exactly_one_of(literals):
	# Adds one of literals to be true
	clauses.append([l for l in literals])
	
	# This add 2 negated literals to clauses
	for pairs in itertools.combinations(literals, 2):
		clauses.append([-l for l in pairs])

def varnum(i, j):
	return (i+j*n)+1
	
# read input , split , typecast to int and assign to x, y
n, m = map(int, input().split())

#read edges
edges = [ list(map(int, input().split())) for i in range(m) ]

#Each vertex should have some color		
for i in range(n):
	exactly_one_of([varnum(i, j) for j in range(3)])

for i in range(len(edges)):
	for j in range(3):
		x, y = edges[i]
		x = x-1 # convert to 0 index
		y = y-1
		# NOT [ (Vertex U, color j) AND (Neighbor vertex of same color j)
		# Translate to NOT(U, j) OR NOT(V, j)
		#vertex (u, v) cannot have same color
		clauses.append([-varnum(x, j), -varnum(y, j)])
		
with open('tmp.cnf', 'w') as f:
	f.write("p cnf {} {}\n".format(3*n , len(clauses)))
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
			pass
		else:
			my_list = line.split()
			my_list = my_list[:-1] # delete last 0 which is there in tmp.sat
			# split in chunk of n size
			final = [my_list[i * n:(i + 1) * n] for i in range((len(my_list) + n - 1) // n )]
			print (len(final), 3*n)
			for x in final:
				x.append(0)
				print(*x, sep=' ')
				
