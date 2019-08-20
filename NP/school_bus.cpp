#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
using std::vector;

/*
 * Travelling Salesman problem using Held-Karp Dynamic Programming Algorithm.
 * Time Complexity: O(n^2 * 2^n)
 * Space : O(n * 2^n)
 */


typedef vector<vector<int> > Matrix;
#define IS_SET(a, x) ((a>>x) & 1)
const int INF = 1e9;
#if 1
Matrix read_data() {
    int vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    Matrix graph(vertex_count, vector<int>(vertex_count, INF));
    for (int i = 0; i < edge_count; ++i) {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        --from, --to;
        graph[from][to] = graph[to][from] = weight;
    }
    return graph;
}
#else
Matrix read_data() {
    int vertex_count, edge_count;
    std::cin >> vertex_count ;
    Matrix graph(vertex_count, vector<int>(vertex_count, INF));
    for (int from = 0; from < vertex_count; ++from)
        for (int to = 0; to < vertex_count; ++to)
    {
        int weight;
        std::cin >>weight;
        graph[from][to] = graph[to][from] = weight;
    }
    return graph;
}
#endif
std::pair<int, vector<int> > optimal_path(const Matrix& graph)
{
    std::pair<int, vector<int> > res;
    res.first = -1;
    int n  = graph.size();
    int s = (1<<(n-1)) -1;
    // Space complexity comes from here i.e. for each node (except start) we have find how it is reachel via all possible subset i.e 2^n.
    std::vector<std::vector<std::pair<double, int>>> dp(n, std::vector<std::pair<double, int>> (s+1, std::make_pair(INF, -1)) );  //2-d vector pair init

    // Generate sub-set with 0 to n-1 bits set
    std::vector<int> subset;
    for (int i=0; i<n-1 ;i++)
        for (int j =0; j< (1<<(n-1)); j++ )
            if(__builtin_popcount(j)==i)
                subset.push_back(j);

    //Iterate subset
    // Outer most loop runs for 2^n times , j loop runs for each dest and k loop look for adjancet node which also rn for n making time complexity O(2^n * n^2);
    for (auto & i:subset) // subset (via)
    {
    	for (int j = 1; j < n ;j++) // destination , source is always vertex -0
    	{
    	    //Handle Empty set differently
    	    if( i==0)
    	    {
    	    	dp[j][i].first = graph[j][0];
    	    	dp[j][i].second = 0;
    	    }
    	    else
    	    {
    	        // skip if j-1 bit set in i ?
    	        // for e.g. if subset 'i' is dest node'j' 1 cant be visited via 011 as 1st bit is already set
    	        if(!IS_SET(i, j-1))
    	        {
    	            for (int k=1; k < n; k++)
    	            {
    	                // Here we are checking which set bit'k' can be adjacent to destination 'j' and then removing that bit from subset (xor)
    	            	if(IS_SET(i, k-1))
    	            	{
    	            	    int adj = k; // to get exact which node
    	            	    int other = i ^ (1<<(k-1));
    	            	    if(dp[j][i].first> (graph[j][adj] + dp[adj][other].first))
    	            	    {
    	            	    	dp[j][i].first = (dp[j][i].first, graph[j][adj] + dp[adj][other].first);
    	            	    	dp[j][i].second = k;
    	            	    }
    	            	}
    	            }
    	        }
    	    }
    	}
    }

    //Final Answer is minimum of all subset
    double cost = INF;
    int node;
    for (int i=0; i< (n-1) ; i++)
    {
        int adj = __builtin_ffs(1 << i); // to get exact which node
        int other = s ^ (1 << i);
        if(cost>  graph[0][adj] + dp[adj][other].first)
        {
            cost = graph[0][adj] + dp[adj][other].first;
            node = adj;
        }
    }
    res.first = cost;

    //Solution trace
    res.second.push_back(1);
    int left = s;
    std::vector<bool> isVisited(n , false);
    isVisited[0] = true;
    while(left && (cost!=INF))
    {
        if(isVisited[node]) // If same node is visited again that means we dont have solution
            break;
        isVisited[node] = true;
    	res.second.push_back(node+1);
    	left ^= (1<<(node-1));
    	node = dp[node][left].second;
    }
    // Solution should have exactly n cities and also first and last vertex in solution space should be connected
    if((res.second.size()!=n) || graph[0][res.second[res.second.size()-1]-1]==INF)
        res.first = -1;
    return res;
}

void print_answer(const std::pair<int, vector<int> >& answer) {

    std::cout << answer.first << "\n";
    if (answer.first == -1)
        return;
    const vector<int>& path = answer.second;
    for (size_t i = 0; i < path.size(); ++i)
        std::cout << path[i] << " ";
    std::cout << "\n";
}
int main() {
    print_answer(optimal_path(read_data()));
    return 0;
}
