#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
int V, E;
typedef pair<int, int> PII;
int *color; int next_color_available=0;
vector<int> usedColor;
void addEdge(vector<int> G[], PII degree[], int u, int v)
{
	G[u].push_back(v);
	G[v].push_back(u);
	degree[u].first =u; degree[u].second++;
	degree[v].first =v; degree[v].second++;
}
bool myFunc(PII a, PII b)
{
	return a.second > b.second;
}
void assignColor(vector<int> G[], int index)
{
	// Check if color_available is  present to any of the neighborer nodes
	int j=0;
	for (; j< usedColor.size(); j++)
	{
		int i=0;
		for (; i< G[index].size(); i++)
			if(color[G[index][i]] ==usedColor[j])
				break;
		if(i==G[index].size()) // This color is not present in any of it neighbors and can be re-used
			break;
	}
	if(j==usedColor.size()) // scanned entire usedColor array and cant found anything to be reused
	{
		// a new color is used
		usedColor.push_back(next_color_available);
		color[index] = next_color_available;
		++next_color_available;
	}
	else
		color[index] = usedColor[j];// same color can be re-used
}
void initGreedy(int index)
{
	memset(color, -1, sizeof(int)*V);
	next_color_available=0;
	//Assign first color unconditionally
	color[index] = next_color_available;
	usedColor.push_back(next_color_available);
	next_color_available++;
}
int main (int argc, char*argv[])
{
	ifstream cin(argv[1]);
	cin >> V >>E;
	vector<int> G[V];
	PII degree[V] ; //index, degree
	for (int i =0 ; i<V; i++)
		degree[i].second =0;// degree
	for (int i =0 ; i<E; i++)
	{
		int u, v;
		cin >> u>>v;
		addEdge(G, degree, u, v);
	}
	// sort the nodes in order of degree i.e. nodes which has highest edges , we should assign color first
	sort(degree, degree+V, myFunc);
	color = new int[V];
	
	initGreedy(degree[0].first);
	#ifdef DEBUG
	for (int i =0 ; i<V; i++)
		cout << degree[i].first<<"->"<<degree[i].second<<" "<<color[i]<<endl;
	#endif
	/* sort vertexes by their degrees and then try to color them in the minimum applicable color, 
	if can't -- add new color to the set of applicable colors*/
	for (int i=1; i<V; i++)
		assignColor(G, degree[i].first);
	#ifdef DEBUG
	cout <<usedColor.size()<<" "<<0<<endl;
	for (int i=0; i<V; i++)
		cout <<color[i]<<" ";
	cout<<endl;
	#endif
	int bestColorCount  = usedColor.size();
	//Reverse permute i.e. 
	for (int ii=0; ii< 50; ii++)
	{
	vector<int> colorGroup[usedColor.size()]; // vertex grouped on basis of color
	for (int i=0; i<usedColor.size(); i++)
		colorGroup[i].clear();
	for (int i=0; i<V; i++)
		colorGroup[color[i]].push_back(i);
	for (int i=0; i<usedColor.size()/2; i++)
		swap(colorGroup[i], colorGroup[usedColor.size()-i-1]);
	#ifdef DEBUG
	cout<<"color group:"<<endl;
	for (int i=0; i<usedColor.size(); i++)
	{
		cout <<color[i]<<" :";
		for (int j=0; j<colorGroup[i].size(); j++)
			cout<<colorGroup[i][j]<<" ";
		cout <<endl;
	}
	#endif
	usedColor.clear();
	initGreedy(colorGroup[0][0]);
	for (int i=0; i<bestColorCount; i++)
		for (int j=0; j<colorGroup[i].size(); j++)
			assignColor(G, colorGroup[i][j]);
	bestColorCount  = usedColor.size();	
	}
	cout <<usedColor.size()<<" "<<0<<endl;
	for (int i=0; i<V; i++)
		cout <<color[i]<<" ";
	cout<<endl;
}
