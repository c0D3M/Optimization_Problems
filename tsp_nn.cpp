#include <bits/stdc++.h>
#include <algorithm>

using namespace std;
float **adjMatrix;
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
	int n;
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
	// Sort Graph in ascending order of edge length.
	sort(G.begin(), G.end(), sortByEdgeLen);	
	#ifdef DEBUG
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
	
	cout <<"Minimum Spanning Tree cost ="<<cost<<endl;
	
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
	#ifdef DEBUG
	for (auto & i:tour)
		cout << i <<" "<<endl;
	#endif
	//cout <<endl;
	
	cost *= (2.0);
	cout <<cost<<endl;;
	//4. Cut short , remove double visited edges
	bool used[n] = {false};
	for (int i=0; i<tour.size(); ++i)
		if(!used[tour[i]]){
			used[tour[i]]=true;
			continue;
		}
		else
			tour[i] = -1;//delete ith item
	
	float cost2 =0;
	int pre =-1;
	for (auto & i:tour)
		if(i!=-1)
		{
		if(pre!=-1)
			cost2 += adjMatrix[i][pre];
		pre =i;
		}
	cout<<cost2+adjMatrix[0][pre]<<" "<<0<<endl;
	for (auto & i:tour)
		if(i!=-1)
		cout << i <<" ";
	cout<<endl;
}