# Week 2
## Knapsack 
So you are given number of items *n* and knapsack capacity *K* and we have to maximize knapsack value without exceeding capacity.  
Started with dynamic programming, it did give correct solution by as *n* grows, execution time degrades.  
Next in order to save memory in dynamic one can only save current state and previous state.  
After going to notes and video lecture, it appears **branch and branch** has to be used to solve this problem efficiently.  
Approach:  
* Sort the items in descending order by Value/Weight
* Calculate UpperBound , this can take fractional value for item index *i* which isnt fitting.  
* Start recursive procedure from *i*  
* upperBound take calculated bound , unused capapcity and return score ,next index to branch on and solutionMap.  
* Recursive take where to branch on, calculated bound , unused capapcity.  
* In this recursive procedure, block *i*<sup>th</sup> item
    - calculate upperBound without this item. (state = 0)  
    - calculate upperBound with this item.  (state = 1) 
    - For either of them if next branch index is -1 and bound > best_solution, save the solution.  
    - For both of above if their upperBound score is greater than *best_solution* , recurse again.

Above procedure passed all test cases  

***Learning:*** Dynamic Programming, Branch & Bound.  

# Week 3
## Graph Coloring
Simple heuristics are used, first sort the nodes in descending order of degree.  
For first node assign color and keep an array of used color.  
Next pick each node and loop in used color if it is present in any of the neghbour.  
If this used color is present in any of the neoghbors, we cant use it and we have to use a new color.  
Otherwise re-use the same color.  
Next store nodes as per color group i.e. group the node as per their color, sort the array and 

***Learning:*** Dynamic Programming, Branch & Bound.  

# Week 4
## Travelling Salesman Problem
Assignment assumption is that TSP follow triangle inequality(d(a-c) < d(a-b) +d(b-c)) and is symmetric (a-b = b-c)  
TSP can be solved by first constructing an initial soluton and then using heuristic & meta-heuristic to improve further on this.  

### Tour Construction
Nearest Neighbor  
Double Tree  
* Construct a MST (used Kruskal algorithm).  
* Double the edges.
* Perform Euler Tour i.e. no edges can repeat itself.  
* Whenever encounter a node which is already visited(TSP we can visit any city only once)  
  connect prev and next of that node.  
  For example in aroute a-b -c -b -a if *b* is found to be already visited, connect c to a (prev(b) to next(b))  
  
Tour cost obtained with this method would be bouned by 2 * OPT  , if OPT is some optimal solution.  
Prrof:  Lets say optimal solution is OPT , which is actually a Hamiltonian cycle.  
    Remove an edge from this cycle , it gives a spanning tree(ST).
	MST <= ST <= OPT   => MST <= OPT  
	After we double edge MST and remove duplicated E <= 2 * MST 
	E <= 2 * OPT  
    
Christofide Algorithm give a more tighter bound of 1.5 * OPT and it is based on Minimum weight perfect matching of odd-degree node.  
Their will always be even number of them.  Add those edge to the MST, and then remove duplicates.  

### Tour Construction
#### Heuristic
Problem specific methods.  
Studies many heuristic and most popular seems to be LK & LKH.  
Implemeted 2-opt & 3-opt, which gives a good insight about the problem, still solution obtaned wasnt good enough.  
#### Meta-Heuristic
Problem independent method like SA, Tabu Search, ACO, Guided Local Search.
Guided Local Search + Fast Local Search  http://csp.bracil.net/gls.html   
  


### Other method.
TSP using integer programming http://opensourc.es/blog/mip-tsp  
Ant-Colony Optimization  https://ac.els-cdn.com/S1002007108002736/1-s2.0-S1002007108002736-main.pdf  
Gomory Cut  http://www.math.uwaterloo.ca/tsp/methods/dfj/index.html  
Summary https://www2.seas.gwu.edu/~simhaweb/champalg/tsp/tsp.html




***Learning:*** Metaheuristic method: SA , GLS & FLS.  
