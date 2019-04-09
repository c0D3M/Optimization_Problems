#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#ifdef DEBUG
    #define LOG(x) std::cerr << x << std::endl
#else
    #define LOG(x)
#endif
typedef struct
{
	int cache_id;
	int latency;
}C;
typedef struct
{
	int size_left;
	vector<int> v; //assigned list of video to this cache server
}R; // Result
typedef struct
{
	int latency; // latency to data center
	int n_c; // # of cache server connected
	int id;
	vector<C> c;
	// Request# & video_id
	vector<pair<int, int> > r;
}E;

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
void reset()	
{
	best_solution =0;
	delete []solutionMap;
	items.clear();
}
// n = number of items
// K -capacity of knapsack
// vector of pair(value, weight)
bool* knapsack(vector<pair<int, int> > item)
{
	I t;
	solutionMap = new bool[n];
	for (int i=0; i< n; i++)
	{
		unsigned int v, w;
		t.v = item[i].first;
		t.w = item[i].second;
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
	//cout <<"hhh "<< f.first <<" "<<f.second<<endl;
	if(f.second != -1)
		dfs(f.second, 0, K);
	else
	{
		best_solution = f.first;
		solution(f.index);
	}
	#ifdef DEBUG
	cout << best_solution<<" "<<0<<endl;
	for (int i =0; i< n; i++)
		cout <<solutionMap[i]<<" ";
	cout<<endl;
	#endif
	return solutionMap;
}
bool sortByActiveEndpoint (E a, E b)
{
	return a.r.size() > b.r.size();
}
bool sortByNearestCacheServer (C a, C b)
{
	return a.latency < b.latency;
}

int main(int argc, char* argv[])
{
	ifstream cin(argv[1]);
	int n_v, n_e, n_r, cache_count, cache_size;
	cin >>  n_v >> n_e>> n_r>> cache_count>> cache_size;
	vector<R> r(cache_count);
	for (int i=0; i<cache_count; i++)
		r[i].size_left = cache_size;
	
	vector<int> v(n_v) ; //size of each video
	for (int i =0 ; i<n_v; i++)
		cin >> v[i];
	vector<E> e(n_e);
	for (int i =0 ; i<n_e; i++)
	{
		e[i].id =i;
		cin >> e[i].latency;
		cin >> e[i].n_c;
		for (int j =0 ; j<e[i].n_c; j++)
		{
			C t;
			cin >> t.cache_id;
			cin >> t.latency;
			e[i].c.push_back(t);
		}
	}
	for (int i =0 ; i<n_r; i++)	
	{
		int id, r, v;
		cin >> v>>id>>r;
		e[id].r.push_back(make_pair(r, v));
	}
	
	//Model as knapsack , cache server act as knapsack.
	// Items are videos , weight = size , value = # of requests.
	
	// Most active Endpoint 
    sort(e.begin(), e.end(), sortByActiveEndpoint);	
	
	// Use that as knapsack and associated req_vid as items.
	for (int i=0; i<e.size(); i++)
	{
		// Sort by it's nearest cache server.
		sort(e[i].c.begin(), e[i].c.end(), sortByNearestCacheServer);
		vector<pair<int, int> > items;
		for(int j=0; j<e[i].r.size(); j++)// # of request & video-id
			items.push_back(make_pair(e[i].r[j].first, v[e[i].r[j].second]));
		for(int k=0; (k < e[i].c.size())&& items.size(); k++)
		{
			K =r[e[i].c[k].cache_id].size_left;
			n = items.size();//e[i].r.size();
			bool *result = knapsack(items);
			//reset knapsack algorithm
			
			items.clear();
			
			int used = 0; 
			for( int j =0; j < n; j++)
			{
				if(result[j])
				{
					used += v[e[i].r[j].second]; // decrease cache capacity
					r[e[i].c[k].cache_id].v.push_back(e[i].r[j].second);  /// Assign the video-id to the cache
				}
				else
					items.push_back(make_pair(e[i].r[j].first, v[e[i].r[j].second])); // Unable to fit this vide, so try in next cache
			}
			reset();
			// decrease the capacity of this cache by assigned video size.
			r[e[i].c[k].cache_id].size_left -= used;
				
			// Any more video which are left unassigned , use another cache if available
		}
	}
	int used =0;
	for (int i=0; i<cache_count; i++)
		if(r[i].size_left!=cache_size)
			used++;
	cout << used<<endl;
	for (int i=0; i<cache_count; i++)
	{
		if(r[i].v.size())
		{
			cout << i;
			for (auto & j: r[i].v)
				cout <<" "<<j;
			cout <<endl;
		}
	}
}
