import sys
from pyscipopt import Model, quicksum
n = 0 # number of facility
m = 0 # number of customer
class Facility:
    __slots__ = ['cost', 'capacity', 'x', 'y']
    def __init__(self, cost, capacity, x, y):
        self.cost = float(cost)
        self.capacity = int(capacity)
        self.x = float(x)
        self.y = float(y)

class Customer:
	__slots__ = ['demand', 'x', 'y']
	def __init__(self, demand, x, y):
		self.demand = int(demand)
		self.x = float(x)
		self.y = float(y)

facility = []
customer = []
d = []
def process_input():
	file = sys.argv[1]
	with open(file) as fp:
		lines = fp.readlines()
	global n, m, facility, customer
	n = int(lines[0].split()[0])
	m = int(lines[0].split()[1])
	#process facility
	for x in range(n):
		input = lines[1+x].split()
		facility.append(Facility(input[0], input[1], input[2], input[3]))
	for x in range(m):
		input = lines[1+n+x].split()
		customer.append(Customer(input[0], input[1], input[2]))
def calculate_distance():
	# calculate distance of customer from each facility
	global d
	d = [[0 for x in range(m)] for y in range(n)]
	for i in range(n):
		for j in range(m):
			d[i][j] = ((((facility[i].x-customer[j].x)**2)+((facility[i].y-customer[j].y)**2))**0.5)
def isFacilityOpen(a, model):
	for i in range(m):
		return True
	return False		
def optimize():
	global n, m, facility, customer
	model = Model("Facility")
	model.hideOutput()
	# define decision variable
	x = [] #bool flag is facility i is serving customer j
	x = [[0 for x in range(m)] for y in range(n)]
	o = [0 for x in range(n)]
	#x[i][j] jth customer is assigned to ith warehouse
	for i in range(n):
		for j in range(m):
			x[i][j] = model.addVar(vtype="B", name="%s,%s" % (i,j)) # whether facility i is serving customer j
	# each warehouse whether its open or not.
	for i in range(n):
		o[i] = model.addVar(vtype="B", name ="o(%s)" %(i))
	# define constraints
	# Each customer is exactly assigned to 1 facility
	for i in range(m):
		model.addCons(quicksum(x[j][i] for j in range(n)) ==1)
	#Each facility is serving to its demand
	for i in range(n):
		model.addCons(quicksum(x[i][j]*customer[j].demand for j in range(m))<= facility[i].capacity)
	# If facility is closed, no customer is assigned to it
	for i in range(n):
		for j in range(m):
			model.addCons(x[i][j] <= o[i])

	# if a facility is open , it should have at-least 1 customer.
	model.addCons(quicksum(x[i][j] for j in range(m) for i in range(n) if o[i]) >=1)
	
	# define objective function
	model.setObjective(quicksum(o[i]*facility[i].cost + quicksum(x[i][j]*d[i][j] for j in range(m)) for i in range(n)), "minimize");
	model.optimize()
	status = model.getStatus()
	if status == 'optimal':
		print ("%s 1" %(model.getObjVal()))
	else:
		print ("%s 0" %(model.getObjVal()))
	cost = 0
	'''
	if status == 'optimal':
		for v in model.getVars():
			if v.name != "n":
				print("%s: %d" % (v, round(model.getVal(v))))
	else:
		print("* No variable is printed if model status is not optimal")
	'''
	res= []
	res = [0 for x in range(m)]
	for i in range(n):
		for j in range(m):
			if model.getVal(x[i][j]) > 0:
				res[int(str(x[i][j]).split(',')[1])] = i
	
	for j in range(m):
		print(res[j], end =" ")
	
	print("\n")			
def main():
	process_input()
	calculate_distance()
	optimize()
if __name__ == "__main__":
	main()