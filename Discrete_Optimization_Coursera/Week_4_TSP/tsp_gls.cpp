#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#ifdef DEBUG
    #define LOG(x) std::cerr << x << std::endl
#else
    #define LOG(x)
#endif

#define MAX_LOOP 100000

int n; // Number of cities
float **adjMatrix; // distance matrix
vector<int> Tour; // Final tour
vector<int> BestTour;
float cost =0.0;
float bestCost = 0.0; // best found so far
float DeltaC, DeltaP;
// FLS/GLS
int **penalty;
bool *isActive;
float alpha = 0.125;
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
	DeltaC = (ac + bd - ab - cd);
	DeltaP = (alpha *(penalty[Tour[a]][Tour[c]]+penalty[Tour[b]][Tour[d]]-penalty[Tour[a]][Tour[b]]-penalty[Tour[c]][Tour[d]]));
	return DeltaC+DeltaP ;
}
void SaveBestTour()
{
	bestCost = cost;
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
		cost += DeltaC;
		
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
		if(cost < bestCost) SaveBestTour();
		return 1;
	}
	//Case 2:  (A, Anext) with (b, bnext)
	if(getDelta(a, next_a, b, next_b) < 0)
	{
		// Found a better tour
		cost += DeltaC;
		
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
		if(cost < bestCost) SaveBestTour();
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
		LOG("BestCost is "<< bestCost<< " current "<< cost<<" calculated " <<getCost()<<" "<<current<<" "<<visited<<endl);
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
int main(int argc, char* argv[])
{
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
	
	//Setup adjacency Matrix
	for (int i=0; i<n; i++)
	{
		adjMatrix[i][i]=0;
		penalty[i][i]=0;
		for (int j=i+1; j<n; j++)
			adjMatrix [i][j] = adjMatrix [j][i] = sqrt(pow(abs(points[i][0]-points[j][0]), 2) + pow(abs(points[i][1]-points[j][1]), 2));
	}
	Tour.reserve(n);
	BestTour.reserve(n);
	
	// Initialize a random tour by connecting city next to each other.
	for (int i=0; i<n; i++)
	    Tour.push_back(i);
	for(int i=0; i<Tour.size(); i++)
	    cost += adjMatrix[Tour[i]][Tour[(i+1)%n]];
	SaveBestTour();
	LOG("Initial Cost"<<bestCost);
	int max_no_improvement=0;
	while(max_no_improvement < MAX_LOOP)
	{
		float oldCost= bestCost;
		FastLocalSearch();
		LOG("Cost is "<< bestCost);
		updatePenalty();
		/*
		If tour improve > 1 then reset the loop counter, it is seen that sometime tour improve < 1 , so for those cases ignore
		else the loop very long to exit.
		*/
		if((oldCost-bestCost) > 1)
			max_no_improvement =0;
		else
			max_no_improvement++;
	}
	cout <<bestCost<<" 0"<<endl;
	for (auto & i:BestTour)
		cout << i<<" ";
	cout<<endl;
}
