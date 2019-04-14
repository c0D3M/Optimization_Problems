#include <bits/stdc++.h>
#include <algorithm>
#pragma GCC optimize "O3"
#pragma GCC target "sse4.2"

using namespace std;
#ifdef DEBUG
    #define LOG(x) std::cerr << x << std::endl
#else
    #define LOG(x)
#endif

int n; // Number of cities
float **adjMatrix; // distance matrix
vector<int> Tour; // Final tour
vector<int> BestTour;
float cost2 =0;
float bestCost = INFINITY; // best found so far
/*
xorshift_128 works as same as other PRNGs, but this is fast.
For example, default_random_engine is often a linear congruential 
generators, which uses modulo operation (roughly, ~ 80 clock cycles). 
But, xorshift uses few bit operations (1 ~ 2 clock cycles each).
*/
class xorshift_128 {
public:
    typedef uint32_t result_type;
    xorshift_128(uint32_t seed) {
        set_seed(seed);
    }
    void set_seed(uint32_t seed) {
        a = seed = 1812433253u * (seed ^ (seed >> 30));
        b = seed = 1812433253u * (seed ^ (seed >> 30)) + 1;
        c = seed = 1812433253u * (seed ^ (seed >> 30)) + 2;
        d = seed = 1812433253u * (seed ^ (seed >> 30)) + 3;
    }
    uint32_t operator() () {
        uint32_t t = (a ^ (a << 11));
        a = b; b = c; c = d;
        return d = (d ^ (d >> 19)) ^ (t ^ (t >> 8));
    }
    static constexpr uint32_t max() { return numeric_limits<result_type>::max(); }
    static constexpr uint32_t min() { return numeric_limits<result_type>::min(); }
private:
    uint32_t a, b, c, d;
};

constexpr double ticks_per_sec = 2800000000;
constexpr double sec_per_ticks = 1.0 / ticks_per_sec;
inline double rdtsc() {  // seconds
    uint32_t lo, hi;
    asm volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo) * sec_per_ticks;
}


#define MAX_NO_IMPROVEMENT 100000
// Prototype
float getDistance (int a, int b);
float getDelta(int a, int b, int c, int d);
float DeltaC;
float getCost()
{
	float tcost =0.0;
	for (int i=0; i<n; i++)
		tcost += adjMatrix[Tour[i]][Tour[(i+1)%n]];
	return tcost;
}

int next(int city, int jump=1)
{
	return (city+jump)%n;
}
int prev(int city, int jump=1)
{
	if(jump)
		return (city-jump+n)%n;
	else
		return city;
}
int getLength(int start, int end)
{
	if(start > end) // loop around case i.e. (50, 0) 
		return n + end-start;
	else
		return end-start;
}
void reverseTour(int start, int end)
{
	int count= abs(start-end);
	if(start >end)
		count = n -count+1;
	else
		count += 1;
	count /= 2;
	for (int i=0; i<count; i++)
		std::swap(Tour[next(start, i)], Tour[prev(end, i)]);
}
// a, b, c, d  are indices in Tour array
float getDistance (int a, int b)
{
	return adjMatrix[Tour[a]][Tour[b]];
}
// a, b, c, d  are indices in Tour array
float getDelta(int a, int b, int c, int d)
{
	float ab =  getDistance(a, b);	
	float ac =  getDistance(a, c);
	float bd =  getDistance(b, d);
	float cd =  getDistance(c, d);
	#if 0
	if((ac > ab) && (bd > cd))
		return 1;
	if((ac==ab) && (bd==cd)) // Sometime it is observed that ab & ac , bd & cd are both same, so no need
		return 1;
	if((bd==ab) && (ac==cd)) // Sometime it is observed that ab & ac , bd & cd are both same, so no need
		return 1;
	#endif
	DeltaC = (ac + bd - ab - cd);
	#if 0
	//if(fabs(DeltaC)<=numeric_limits<double>::epsilon())
	//if(almost_equal(DeltaC, (float)0.0, 2))
	if(fabs(DeltaC)<1e-3)
		return 1;
	#endif
	return DeltaC ;
}
void SaveBestTour()
{
	if(cost2 < bestCost)
	{
		bestCost = cost2;
		BestTour.clear();
		BestTour.insert(BestTour.begin(), Tour.begin(), Tour.end());
	}
}
bool isValid(int a, int b)
{
	if(a<b)
		a+=n;
	return (a-b)>2;
}
void applyChanges(int a, int b)
{
	int prev_a = prev(a);
	int next_b = next(b);
	cost2 += DeltaC;
	// Reverse shorter tour
	if(getLength(a, b) < getLength(next_b, prev_a))
		reverseTour(a, b);
	else
		reverseTour(next_b, prev_a);
	#if 0
	float cost = 0;
	for(int i=0; i<Tour.size(); i++)
	    cost += adjMatrix[Tour[i]][Tour[(i+1)%n]];
	if(abs(cost-cost2)> 1.0)
		cout<<"Error: "<<cost<<" "<<cost2<<"a=:"<<a<<" b=: "<<b<<endl;
    #endif
}
float TwoOpt(int a, int b)
{
	
	//Case 1 : (prevA, A) with (b, bnext)
	int prev_a = prev(a);
	int next_b = next(b);
	return getDelta(prev_a, a, b, next_b);
}
class DisjointSet
{
	private:
	void MakeSet(int n);
	
	public:
	typedef struct Node
	{
		int data;
		int rank;
		struct Node*  parent;
	}N;
	vector<N*> map; //uses to find Node from data

	DisjointSet(int n);
	bool Union(int a , int b);
	N* FindSet(N* );
	void display();
};
/*
  * n: number of nodes in graph
 */
DisjointSet::DisjointSet(int n)
{
	for (int i=0; i<n; ++i)
		MakeSet(i);
}
void DisjointSet::MakeSet(int node)
{
	N *n = new N();
	n->data = node;
	n->rank =0;
	n->parent = n;
	map.push_back(n);
}
DisjointSet::N* DisjointSet::FindSet(N* n)
{
	if(n->data == n->parent->data)
		return n;
	FindSet(n->parent);
}
bool DisjointSet::Union(int a , int b)
{
	bool added = true;
	N* pa = FindSet(map[a]); //Find parent
	map[a]->parent = pa; //path compression
	N *pb = FindSet(map[b]); 
	map[b]->parent = pb; //path compression
	
	if(pa->data == pb->data) // Same set
		added =false;
	else if(pa->rank == pb->rank) // same rank , choose any one)
	{
		++pa->rank; // a parent
		pb->parent = pa;
	}
	else if(pa->rank >= pb->rank)
		pb->parent =pa;
	else
		pa->parent = pb;
	return added;
}
void DisjointSet::display()
{
	for (int i=0; i<map.size(); i++)
		cout << i << "--->"<< FindSet(map[i])->data<<endl;
}
bool sortByEdgeLen(pair<int,int> a, pair<int, int> b)
{
	return adjMatrix[a.first][a.second] < adjMatrix[b.first][b.second];
}
bool sortByu(pair<int,int> a, pair<int, int> b)
{
	return a.first < b.first;
}
int main(int argc, char* argv[])
{
	/*
	Algorithm Double Tree:
	1.Find an MST
	2.Double the edges
	3.Find an Euler tour
	4.Cut short solution 'E'
	Lets say optimal solution is OPT , which is actually a Hamiltonian cycle, remove an edge from this cycle
	gives a spanning tree.
	MST <= ST <= OPT   => MST <= OPT
	After we double edge and remove duplicated E <= 2*MST 
	E <= 2*OPT
	*/
	ifstream cin(argv[1]);
	cin >> n;
	float points [n] [2]; // x & y co-ordinates
	adjMatrix = new float* [n];
	for (int i=0; i<n; i++)
	{
		adjMatrix[i] = new float[n];
		cin >> points[i][0] >> points[i][1];
	}
	vector<pair<int, int> > G; // Graphs containing edges edges (u,v), edges are stored in ascending order.
	vector<pair<int, int> > MST; 
	for (int i=0; i<n; i++)
	{
		adjMatrix[i][i]=0;
		for (int j=i+1; j<n; j++)
		{
			adjMatrix [i][j] = adjMatrix [j][i] = sqrt(pow(abs(points[i][0]-points[j][0]), 2) + pow(abs(points[i][1]-points[j][1]), 2));
			//Push this edge
			G.push_back(make_pair(i, j));
		}
	}
	Tour.reserve(n);
	BestTour.reserve(n);
#if 1
	// Sort Graph in ascending order of edge length.
	sort(G.begin(), G.end(), sortByEdgeLen);	
	#ifdef DEBUG2
	cout <<"Adjacency Matrix ==="<<endl;
	for (int i=0; i<n; i++,cout<<endl)
		for (int j=0; j<n; j++)
			cout<<adjMatrix [i][j]<<" ";
	
	cout << "Graph edges in ascending order"<<endl;
	for (auto & i:G)
		cout << i.first<< " " << i.second<<"-->"<< adjMatrix[i.first][i.second]<<endl;
	#endif
	
	// For n city , their will be n*(n-1) / 2 unique edges.
	float cost = 0;
	//Kruskal algorithm, select edge of least weight which doesn't form a loop
	DisjointSet d(n);
	for (auto & i:G)
	{
		if(!d.Union(i.first , i.second)) // if both the vertex are in Forest skip this edge as it create loop.
			continue;
		cost += adjMatrix[i.first][i.second]; // Add cost
		
		MST.push_back(make_pair(i.first, i.second));
		MST.push_back(make_pair(i.second, i.first));//Double the edges
	}
	#ifdef DEBUG
	cout <<"Minimum Spanning Tree cost ="<<cost<<endl;
	#endif
	sort(MST.begin(), MST.end(), sortByu); //sort ascending
	#ifdef DEBUG2
	for (auto & i:MST)
		cout << i.first << " " << i.second<<endl;
	#endif
	//Perform Euler Tour
	stack<int> nodes;
	stack<int> prev;
	nodes.push(MST[0].first);
	int parent =-1;
	bool visited[n] = {false};
	vector<int> tour;
	while(!nodes.empty())
	{
		int top = nodes.top();
		nodes.pop();
		
		tour.push_back(top);
		bool no_neighbours =1;
		prev.push(parent);
		for (auto & i:MST)
			if(!visited[i.second] && (top == i.first))
			{nodes.push(i.second);no_neighbours = 0;}
		visited[top] = true;
		if(no_neighbours)
		{
		tour.push_back(prev.top());
		prev.pop();		
		}
		else
			parent = top;
	}
	while(!prev.empty())
	{tour.push_back(prev.top());;prev.pop();}
	#ifdef DEBUG2
	for (auto & i:tour)
		cout << i <<" "<<endl;
	#endif
	//cout <<endl;
	
	cost *= (2.0);
	#ifdef DEBUG
	cout <<cost<<endl;;
	#endif
	//4. Cut short , remove double visited edges
	bool used[n] = {false};
	for (int i=0; i<tour.size(); ++i)
		if(!used[tour[i]]){
			used[tour[i]]=true;
			continue;
		}
		else
			tour[i] = -1;//delete ith item
	

	int pre =-1;
	for (auto & i:tour)
		if(i!=-1)
		{
		if(pre!=-1)
			cost2 += adjMatrix[i][pre];
		pre =i;
		}
		
	cost2 += adjMatrix[0][pre];	
#ifdef DEBUG2
	cout<<cost2<<" "<<0<<endl;
	for (auto & i:tour)
		if(i!=-1)
		cout << i <<" ";
#endif
	//TODO: find a neat way to get above tour.
	
	for (auto & i:tour)
		if(i!=-1)
			Tour.push_back(i);
#else
	for (int i=0; i<n; i++)
	    Tour.push_back(i);
	cost2 = 0;
	    for(int i=0; i<Tour.size(); i++)
	        cost2 += adjMatrix[Tour[i]][Tour[(i+1)%n]];
#endif
	SaveBestTour();
	int max_no_improvement =0;
	// Simulated Annealing
	double clock_begin = rdtsc();
	constexpr int TIME_LIMIT = 10;  // seconds
	double clock_end =  clock_begin + TIME_LIMIT;
	//Used for randomness
	random_device device;
    xorshift_128 gen(device());
	double temperature;

	for (unsigned iteration = 0; ; ++ iteration) 
	{
        if (iteration % 32 == 0) 
		{
            temperature = (clock_end - rdtsc()) / (clock_end - clock_begin);
            if (temperature <= 0.0) 
                break;
        }
		int a=0, b=0;
		while(!isValid(a,b)) // find distinct 2 cities which are separated by 2 index.
		{
			a = uniform_int_distribution<int>(0, n - 1)(gen);
			b = uniform_int_distribution<int>(0, n - 1)(gen);
		}
		float delta = TwoOpt(a, b);
		
		auto probability = [&]() {
            constexpr double boltzmann = -0.05;
            return exp(boltzmann * delta / temperature);
        };
        
		if (delta < 0 or bernoulli_distribution(probability())(gen)) 
            applyChanges(a, b);
		SaveBestTour();
		
	}
	cout <<bestCost<<" 0"<<endl;
	for (auto & i:BestTour)
		cout << i<<" ";
	cout <<endl;
}
