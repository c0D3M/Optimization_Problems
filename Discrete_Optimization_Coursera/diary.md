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
