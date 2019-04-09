#include <bits/stdc++.h>
#include <algorithm>
#define SET_BIT(a, x)   (a |= ((1<<x)))
#define CLEAR_BIT(a, x)   (a &= (~(1<<x)))
using namespace std;
int n, K;
typedef struct
{
	unsigned int v;
	unsigned int w;
	bool blocked; // 1 blocked for solution
	bool state; // 1 taken , 0 no taken,
	int index; // for solution traceback
}I;
vector<I> items;
typedef struct {
	float first; // score
	int second;  // next branch index
	vector<int> index;  // list of indices, can be useful if its integer solution
}tBoundCalc;

bool sortByV(I a, I b)
{
	return ((float)a.v/a.w) > ((float)b.v/b.w);
}
tBoundCalc upperBound(unsigned int initial, unsigned int capacity) //
{
	tBoundCalc ret;
	float bound = initial;
	int i =0;
	for (; i < n; ++i)
	{
		if(items[i].blocked)
			continue;
		if(items[i].w <=capacity) // item can be taken
		{
			bound += items[i].v;
			capacity 	-= items[i].w;
			ret.index.push_back(i);
		}
		else
			break;
	}
	if(i==n) // everything consumed , no branching index
		{ret.first = bound;ret.second = -1;}
	else
	{		
		if(items[i].blocked)//blocked
			++i;
		 if(i==n) // all consumed
			{ret.first = bound;ret.second = -1;}
		else
			{bound += ((items[i].v*1.0*capacity)/items[i].w);ret.first = bound;ret.second = i;}
	}
	return ret;
}
int best_solution =0;
bool *solutionMap;
void solution(vector<int> index)
{
	memset(solutionMap, 0, n*sizeof(bool));
	for (int i =0; i< n; i++)
		if(items[i].blocked && items[i].state)
			solutionMap [ items[i].index] = 1;
		
	for (int i =0; i< index.size(); i++)	
		solutionMap [ items[index[i]].index] = 1;
}
void dfs (int i, unsigned int bound , unsigned int capacity)
{
	// left child take it, right child not take it.
	items[i].blocked = 1;// item i is blocked
	tBoundCalc decision [2] = { {-1, 0}, {-1, 0}};
	
	
	if(capacity >= items[i].w) 
		decision[0]     = upperBound(bound+items[i].v, capacity-items[i].w);//taken
		
	decision[1] = upperBound(bound, capacity); //not taken
	unsigned int weight[2], value[2];
	weight[0] = items[i].w;
	weight[1] = 0;
	value[0] = items[i].v;
	value[1] = 0;
	// select which index to include for further branching?
	vector<bool> nodes;
	for (int j=0 ; j<2; j++)
	{
		if(decision[j].first !=0 && decision[j].second!=-1) //reject infeasible or integer solution index
	        if(decision[j].first > decision[!j].first)
				if(nodes.size())nodes.push_back(!nodes[0]);else nodes.push_back(j);
			else
				if(nodes.size())nodes.push_back(!nodes[0]);else nodes.push_back(!j);
		if((decision[j].second ==-1)  && (decision[j].first > best_solution))// integer solution
		{
			best_solution = decision[j].first;
			solution(decision[j].index);
		}
	}	
			
	for (int j=0 ; j<nodes.size(); j++)
	{
		bool index = nodes[j];		
		if((int)decision[index].first > best_solution)
		{
			items[i].state =!index;//1 is counted as taken while calculating solutionMap
			dfs(decision[index].second, bound+value[index], capacity-weight[index]); 
		}
	}
	items[i].blocked = 0;// item i is unblocked
	items[i].state = 0;
}	
int main (int argc, char*argv[])
{
	ifstream cin(argv[1]);I t;
	cin >> n>>K;
	solutionMap = new bool[n];
	for (int i=0; i< n; i++)
	{
		unsigned int v, w;
		cin >> t.v >> t.w;
		t.index = i;
		t.blocked = 0;
		t.state =0;
		items.push_back(t);
	}
	sort(items.begin(), items.end(), sortByV); //Sort as per v/w
	#ifdef DEBUG
	cout <<"Capacity: "<<K<<endl;
	for (int i=0; i < n; ++i)
		cout <<i<<":"<<items[i].index <<" | " <<items[i].v <<"  "<< items[i].w<<"  "<< (float)items[i].v/items[i].w<<endl; 
	#endif
	tBoundCalc f; // f.first is upperBound and f.second is branch index
	f = upperBound(0, K);
	if(f.second != -1)
		dfs(f.second, 0, K);
	cout << best_solution<<" "<<0<<endl;
	for (int i =0; i< n; i++)
		cout <<solutionMap[i]<<" ";
	cout<<endl;
	return 0;
}
