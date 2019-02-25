#include <bits/stdc++.h>
#include <algorithm>

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
float bestCost = 0.0; // best found so far

// FLS/GLS
int **penalty;
bool *isActive;
float alpha = 0.125;
#define MAX_NO_IMPROVEMENT 100000
bool logPrint=0;
// Prototype
float calculateAugCost(float cost);
float getDistance (int a, int b);
float getDelta(int a, int b, int c, int d);
void updatePenalty();
void clear();
int FastIteration();
float DeltaC, DeltaP;
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
	if((ac > ab) && (bd > cd))
		return 1;
		if((ac==ab) && (bd==cd)) // Sometime it is observed that ab & ac , bd & cd are both same, so no need
		return 1;
	if((bd==ab) && (ac==cd)) // Sometime it is observed that ab & ac , bd & cd are both same, so no need
		return 1;
	DeltaC = (ac + bd - ab - cd);
	if(fabs(DeltaC)<=numeric_limits<double>::epsilon())
		return 1;
	DeltaP = (alpha *(penalty[Tour[a]][Tour[c]]+penalty[Tour[b]][Tour[d]]-penalty[Tour[a]][Tour[b]]-penalty[Tour[c]][Tour[d]]));
	return DeltaC+DeltaP ;
}
void SaveBestTour()
{
	bestCost = cost2;
	BestTour.clear();
	BestTour.insert(BestTour.begin(), Tour.begin(), Tour.end());
}
bool TwoOpt(int a, int b)
{
	//Case 1 : (prevA, A) with (b, bnext)
	int prev_a = prev(a);
	int next_b = next(b);
	int next_a = next(a);
	if(getDelta(prev_a, a, b, next_b) < 0)
	{
		// Found a better tour
		cost2 += DeltaC;
		
		// Reverse shorter tour
		if(getLength(a, b) < getLength(next_b, prev_a))
			reverseTour(a, b);
		else
			reverseTour(next_b, prev_a);
		// Activate the nodes
		isActive[Tour[prev_a]] = 1;
		isActive[Tour[a]] = 1;
		isActive[Tour[b]] = 1;
		isActive[Tour[next_b]] = 1;
		if(cost2 < bestCost) SaveBestTour();
		return 1;
	}
	//Case 2:  (A, Anext) with (b, bnext)
	if(getDelta(a, next_a, b, next_b) < 0)
	{
		// Found a better tour
		cost2 += DeltaC;
		
		// Reverse shorter tour
		if(getLength(next_a, b) < getLength(next_b, a))
			reverseTour(next_a, b);
		else
			reverseTour(next_b, a);
		// Activate the nodes
		isActive[Tour[a]] = 1;
		isActive[Tour[next_a]] = 1;
		isActive[Tour[b]] = 1;
		isActive[Tour[next_b]] = 1;
		if(cost2 < bestCost) SaveBestTour();
		return 1;
	}
	return 0;
}
void FastLocalSearch()
{
	int visited =0;
	int current = 0; //current node;
	while(visited<n) // If a full loop run without improvement exit
	{
		LOG("BestCost is "<< bestCost<< " current "<< cost2<<" calculated " <<getCost()<<" "<<current<<" "<<visited<<endl);
		bool modified = false;
		if(isActive[Tour[current]])// For each active node test with (0, n-1) nodes and see if any improvement can be made.
		{
			int next_node = 0;
			
			 // For each current node , try with all 'n' nodes if move exchange result in better tour
			while(!modified && (next_node < n))
			{
				if((next_node != current) && (next_node != next(current)) && (next_node != prev(current)) && (next(next_node)!=prev(current)))
					modified = TwoOpt(current, next_node);
				next_node ++;
			}
		}
		if(modified)// we found a better move, so go a previous node and reset loop
		{
			visited =-1;
			current = prev(current);
		}
		else
		{
			// since we tested this current node with all other node and still didnt find anything good
			// mark it as inactive
			isActive[Tour[current]] = 0;
			current = next(current);
		}
		visited++;
	}
}

void updatePenalty()
{
	vector<int> U;
	float maxUtil =0.0, util;
	int next;
	alpha = (float)(0.3*(float)(bestCost/n));
	for (int i=0; i< Tour.size(); i++)
	{
		next = ((i+1)==Tour.size()) ?0: i+1;
		util = (adjMatrix[Tour[i]][Tour[next]] / (1.0 + penalty [Tour[i]][Tour[next]]));
		if(util>maxUtil)
		{
			U.clear(); // We got a new maximum, clear out everything we saved earlier.
			U.push_back(i);
			maxUtil = util;
		}
		else if (util==maxUtil)
			U.push_back(i);
	}
	
	for (int i=0; i< U.size(); i++)
	{
		next = ((U[i]+1)==Tour.size()) ?0: U[i]+1;
		penalty[Tour[U[i]]][Tour[next]]++;
		penalty[Tour[next]][Tour[U[i]]]++;
		//Activate neighborhood of this penalized edge.
		isActive[Tour[U[i]]] =1;
		isActive [Tour[next]] =1;
	}
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
	isActive = new bool[n];
	penalty = new int*[n];
	adjMatrix = new float* [n];
	for (int i=0; i<n; i++)
	{
		isActive[i] =1;
		adjMatrix[i] = new float[n];
		penalty[i] =   new int [n];
		cin >> points[i][0] >> points[i][1];
	}
	vector<pair<int, int> > G; // Graphs containing edges edges (u,v), edges are stored in ascending order.
	vector<pair<int, int> > MST; 
	for (int i=0; i<n; i++)
	{
		adjMatrix[i][i]=0;
		penalty[i][i]=0;
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
	// Guided Local Search 
	while(max_no_improvement < MAX_NO_IMPROVEMENT)
	{
		float oldCost =bestCost;
		//cout<<"best cost is"<< bestCost<<" "<<max_no_improvement<<endl;
		FastLocalSearch();
		if((oldCost-bestCost) > 1)
		    max_no_improvement=0;
		else
		    max_no_improvement++;
		updatePenalty();
	}
	cout <<bestCost<<" 0"<<endl;
	for (auto & i:BestTour)
		cout << i<<" ";
	cout <<endl;
}
