NP: Solution can be verified in polynomial time.  
NP-Hard:A problem is NP hard if is is as difficult as any NP problem.  
NP-Complete: If the problem is both NP & NP Hard.  
https://www.ibm.com/developerworks/community/blogs/jfp/entry/no_the_tsp_isn_t_np_complete?lang=en  
http://graham-kendall.com/blog/2013/12/what-is-operations-research/  

TSP is NP: need to verify in polynomial time that given solution is optimal? which isnt possible in polynomial time.  
TSP is NP-Hard: All other problem in NP class can be reduced to it in polynomial time.  
TSP is NP_Complete?: Is the given tour shorter than L 

#### Tour Construction Algorithm  
##### Branch & Bound  
##### Nearest Neighbour  
##### Double Tree  
##### Christofide Algorithm  
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
##### TSP using Integer Programming  
    http://opensourc.es/blog/mip-tsp  
   *Gomory Cut*  
       http://www.math.uwaterloo.ca/tsp/methods/dfj/index.html  
##### TSP using Guided Local Search/ Fast Local Search
    http://csp.bracil.net/gls.html   
#### Tour Improvement methods  
##### Lin Kernighan  
##### Lin Kernighan Helsgaun  

**References**
1. Summary of various way to solve TSP. https://www2.seas.gwu.edu/~simhaweb/champalg/tsp/tsp.html  
