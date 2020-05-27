# python3
'''
2-Phase Simplex Algorithm
Input p(equation) , q(variable)
The next 𝑝 + 1 lines contain the coefficients of the linear inequalities
in the standard form 𝐴𝑥 ≤ 𝑏. Specifically, 𝑖-th of the next 𝑝 lines contains 𝑞 integers 𝐴𝑖1,𝐴𝑖2, . . . ,𝐴𝑖𝑞,
and the next line after those 𝑝 contains 𝑝 integers 𝑏1, 𝑏2, . . . , 𝑏𝑝. These lines describe 𝑝 inequalities of
the form 𝐴𝑖1 · 𝑥1 + 𝐴𝑖2 · 𝑥2 + · · · + 𝐴𝑖𝑞 · 𝑥𝑞 ≤ 𝑏𝑖. The last line of the input contains 𝑞 integers — the
coefficients 𝑐𝑖 of the objective

Output: 
No Solution for infeasible
Infinity for unbounded
Bounded Solution for valid solution
'''

import math
from sys import stdin
def phase1(n, m, A, b, c):
	a = []
	s = []
	for i in range(0, n):
		if b[i] < 0:
			b[i] *= -1
			for j in range(0, m):
				A[i][j] *= -1
			#add surplus variable
			s.append(i)
			#add artifical variable
			a.append(i)
		else:
			#add slack variable
			s.append(i)
	
	Cj = [0] * (m + len(s))
	for i in range(len(a)):
		Cj.append(-1)
	
	#Append Slack & Artifical in equation
	t = [0] * (len(s) +len(a))
	for i in range(0, n):
		A[i] += t
		
	for i in range(0,len(a)):
		A[a[i]][m+len(s)+i] = 1 #Setting artifical variable
		A[a[i]][m+a[i]] = -1 #Setting surplus variable as -1
	for i in s:
		if A[i][m+i]==0: # Surplus variable with artifical would already been set earlier
			A[i][m+i] = 1 # Set slack variable as 1
	
	Cb = [0] * n
	pos_Cb = [0] * n #Position of basic variable in n
	for i in range(0,len(a)):
		Cb[a[i]] =-1
		pos_Cb[a[i]] = m + len(s) +i

	for i in range (0, n):
		if Cb[i]==0:
			pos_Cb[i] = m+i
	Phase2Started = False		
	unbounded = False
	while True:
		while True: #Phase 1 loop			
			Z = 0;
			for i in range(0, n):
				Z += (Cb[i] * b[i])
				
			total_col = m + len(s) + len(a)	
			
			Zj = []
			Zj_Cj = []
			for i in range (0, total_col):
				t = 0
				for j in range (0, n):
					t += Cb[j] * A[j] [i]
				Zj.append(t)	
				Zj_Cj.append(Zj[i]-Cj[i]) 
			if min(Zj_Cj) < 0:
				key_col = Zj_Cj.index(min(Zj_Cj))
			else:
				break
			
			all_key_col = False
						
			minx = math.inf
			key_row = -1
			for i in range(0,n):
				if A[i] [key_col]>0:
					if (b[i]/A[i] [key_col] < minx) or  (((b[i]/A[i] [key_col] == minx) and  (Phase2Started== False) and (Cb[i]<0))):
						#For phase 1 if we get Xb/Xi same as exisiting prefer artifical variable
						key_row = i
						minx = b[i]/A[i] [key_col]
			
			if key_row ==-1:
				unbounded = True
				break
			key_element  = A[key_row][key_col]
			
			#Divide Key row by key element
			for i in range (0, total_col):
				A[key_row][i] /= key_element
			b[key_row] /= key_element
			# For all other non-zero elements in key_column for all 'n'
			for i in range (0, n):
				if (i!= key_row) and (A[i][key_col]!=0):
					mul = -A[i][key_col]/A[key_row][key_col]
					for j in range (0, total_col):
						A[i][j] += A[key_row][j]*mul
					b[i] += b[key_row]*mul		
			
			#delete column of n which is getting removed
			if pos_Cb[key_row]> m+len(s):
				column_removed = pos_Cb[key_row]
				for i in range (0, n):
					A[i][column_removed] = float("nan")
			
			Cb[key_row] = Cj[key_col]
			pos_Cb[key_row] = key_col
			#print (A)
		if unbounded==True:
			return [1, []]
			break
		if -1 in Cb:
			#Zj-Cj >=0 and we still have artifical variable*b >0 that means no solution
			#Zj-Cj >=0 and if we still have artifical variable*b <= 0 that means solution
			if Phase2Started==False:
				for i in range(0, len(Cb)):
					if Cb[i]==-1 and b[i] >1e-9: 
						return [-1, []]
			l = [0] *m
			for i in range(0, n):
				if pos_Cb[i] < m:
					l[pos_Cb[i]] = b[i]
			return [0, l]	
			
			break
		elif Phase2Started==True:
			l = [0] *m
			for i in range(0, n):
				if pos_Cb[i] < m:
					l[pos_Cb[i]] = b[i]
			return [0, l]
			break
		else:
			Phase2Started = True
			#Modify the Z
			#Cj and Cb
			for i in range(0, m):
				Cj[i] = c[i]
			#Get rid of Artifical variable
			for i in range(m+len(s), m+len(s)+len(a)):
				del(Cj[m+len(s)])
			#Now Cb
			for i in range(0, n):
				if pos_Cb[i] < m:
					Cb[i] = c[pos_Cb[i]]
			a = [] #since all artifical variable are removed set the list as NULL
def allocate_ads(n, m, A, b, c):
	return phase1(n, m, A, b, c)

n, m = list(map(int, stdin.readline().split()))
A = []
for i in range(n):
  A += [list(map(int, stdin.readline().split()))]
b = list(map(int, stdin.readline().split()))
c = list(map(int, stdin.readline().split()))

anst, ansx = allocate_ads(n, m, A, b, c)

if anst == -1:
  print("No solution")
if anst == 0:  
  print("Bounded solution")
  print(' '.join(list(map(lambda x : '%.18f' % x, ansx))))
if anst == 1:
  print("Infinity")
