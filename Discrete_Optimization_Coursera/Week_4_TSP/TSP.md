NP: Solution can be verified in polynomial time.  
NP-Hard:A problem is NP hard if is is as difficult as any NP problem.  
NP-Complete: If the problem is both NP & NP Hard.  
https://www.ibm.com/developerworks/community/blogs/jfp/entry/no_the_tsp_isn_t_np_complete?lang=en  
http://graham-kendall.com/blog/2013/12/what-is-operations-research/  

TSP is NP: need to verify in polynomial time that given solution is optimal? which isnt possible in polynomial time.  
TSP is NP-Hard: All other problem in NP class can be reduced to it in polynomial time.  TODO put a link to this.
TSP is NP_Complete?: Is the given tour shorter than L 

Lower Bound:    
Upper Bound:  

## Tour Construction Algorithm  
----------------------------------
### Branch & Bound  
### Nearest Neighbour  
### Double Tree  
### Christofide Algorithm  
###### Minimum Weight Perfect Matching  
  **Aho-Hopcraft**  
  **Edmond's Blossom Algorithm**  
  https://www-m9.ma.tum.de/graph-algorithms/matchings-blossom-algorithm/index_en.html  
  **Hall's Theorem**  
  It gives a necessary and sufficient condition  for a matching to be possible.  
  For u,v barpartite graph,  u >= v  
  i.e. any node in u should have >= more choice then any node in v.  
  See https://luckytoilet.wordpress.com/2013/12/21/halls-marriage-theorem-explained-intuitively/  
  **Kőnig's theorem**  
  Maximum matching is equal to minimum vertex cover.  
  Minimum vertex covers =minimum vertex by which every edge is touched upon.  
  https://www.youtube.com/watch?v=r_8eiVRNrNw  
  **Berge's Theorem**  
  Matching is maximum if and only if no augmenting path exists.  
  **Belief Propogation**  
  **Hungarian Method**  
     Row-minima /column minima approach.  
## Tour Improvement Methods  
### Heurestics:  
#### 2 -Opt , 3-Opt & k-Opt.  

### Meta-Heurestics:  
Problem independent methods which are used to escape local minimima/maxima and reach for global optimum.  
Some example of tehse are Tabu Search, Simulated Annealing, Genetic Algorithms.  

#### Simulated Annealing  
#### Guided Local Search and Fast Local Search  
     http://csp.bracil.net/gls.html   
Fast Local Search:  This help in reducing evaluation of large neighbourhood. We will mark some of state of state as inactive which doesnt yield a better solution than the existing one. So in next local search, this state wont be evaluated as this wont lead to any better solution.  

Guided Local Search:  

1. Generate an initial solution.  
2. Set penalty (p)  for each node and mark each of node as active.  
3. Objective function _g_ is sum of all edge cost of the tour.  
   Define another 'Augmented objective function' which is _f_ = _g_ + λ * Σ p  
2. Run 2-Opt, for each of active node only and see if we get a better solution than the current.  Note that delta should be Δf and not Δg.   
3. Once a better solution is found, find the most costly edge and penalize the node's connecting it by 1, also mark them active.  

Run the above process until time exceed or max iterations.



#### Ant Colony Optimization  
     https://www.youtube.com/watch?v=wfD5xlEcmuQ  
    ![ACO Paper!](https://ac.els-cdn.com/S1002007108002736/1-s2.0-S1002007108002736-main.pdf)  
### TSP using Integer Programming  
    http://opensourc.es/blog/mip-tsp  
   *Gomory Cut*  
    http://www.math.uwaterloo.ca/tsp/methods/dfj/index.html  

**References**
1. Summary of various way to solve TSP. https://www2.seas.gwu.edu/~simhaweb/champalg/tsp/tsp.html  
