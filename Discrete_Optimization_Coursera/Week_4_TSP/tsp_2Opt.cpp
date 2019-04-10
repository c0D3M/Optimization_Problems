#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#ifdef DEBUG
    #define LOG(x) std::cerr << x << std::endl
#else
    #define LOG(x)
#endif


int n; // # of cities
bool *isActive;
float **adjMatrix;
vector<int> Tour; //Final Tour
float cost;
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
	//printf("[%d:%d] %f [%d:%d] %f [%d:%d] %f [%d:%d] %f\n", Tour[a], Tour[b], ab, Tour[a], Tour[c], ac, Tour[b], Tour[d], bd, Tour[c], Tour[d], cd);
	if((ac > ab) && (bd > cd))
		return 1;
	return ac + bd - ab - cd;
}
bool TwoOptSearch()
{
	// Tour improvement 2-opt
	int i=0, n_visited=0;
	while (n_visited<n)
	{
	    if(!isActive[i])
	    {
	        i = (i+1)%n;
	        n_visited++;
	        continue;
	    }
		// For each point generate (prev, i) & (i, next) 
		int prev = (i -1 +n) % n;
		int next = (i +1 +n) % n;
		
		int c = (prev +2)%n;
		int d = (prev +3)%n;
		// Generate next pair and try each of them one by one.
		// For example  Tour =0, 4, 1, 2, 3
		// i =0, prev =4 , next =1, 
		// For (4,0)...loop generates (1,2) (2,3) (3, 4) 
		// (0,1) ... (2,3) (3,4) (4,0)
	    // (a, b) => (prev, i)
		bool not_found =true;
		while(isActive[i] && not_found && (d!=prev))
		{
			float delta = getDelta(prev, i, c, d); // exchange (a,b) (c,d) edge and check if we can get shorter tour.
			if(delta<0)
			{
				//Found a shorter tour
				cost += delta;
				isActive[prev] = 1;
				isActive[i] = 1;
				isActive[c] = 1;
				isActive[d] = 1;
				//Reverse shorter tour
				// b-c vs a-d 
				/*
				if b-c circuit length is smaller then "a->c-> Rev (c->b) -> b->d"
				if a-d circuit is smaller then "Rev (a-d)-> d -> b -> c
				*/
				if(abs(i-c) < abs(prev-d))
				{
					int start = min(i, c);
					int end = max(i, c);
					int len  = end  - start+1;
					for(int ii = 0; ii< len/2; ii++)
						swap(Tour[start+ii], Tour[end-ii]);
				}
				else
				{
					int start = min(prev, d);
					int end = max(prev, d);
					int len  = end  - start+1;
					for(int ii = 0; ii< len/2; ii++)
						swap(Tour[start+ii], Tour[end-ii]);
				}
				not_found =false;
				
			}
			c= d;
			d = (d +1 +n) % n;
		}
		
		// (a,b) => (i, next)
		c = (i+2)%n;
		d = (i+3)%n;
		//not_found = true;
		while(isActive[i] && not_found && (d!=i))
		{
			float delta = getDelta(i, next, c, d); // exchange (a,b) (c,d) edge and check if we can get shorter tour.
			if(delta<0)
			{
			    isActive[i] = 1;
			    isActive[next] = 1;
                isActive[c] = 1;
                isActive[d] = 1;
				//Found a shorter tour
				cost += delta;
				//Reverse shorter tour
				// b-c vs a-d 
				/*
				if b-c circuit length is smaller then "a->c-> Rev (c->b) -> b->d"
				if a-d circuit is smaller then "Rev (a-d)-> d -> b -> c
				*/
				if(abs(next-c) < abs(i-d))
				{
					int start = min(next, c);
					int end = max(next, c);
					int len  = end  - start+1;
					for(int ii = 0; ii< len/2; ii++)
						swap(Tour[start+ii], Tour[end-ii]);
				}
				else
				{
					int start = min(i, d);
					int end = max(i, d);
					int len  = end  - start+1;
					for(int ii = 0; ii< len/2; ii++)
						swap(Tour[start+ii], Tour[end-ii]);				
				}
				not_found = false;
			}
			c= d;
			d = (d +1 +n) % n;
		}
		if(not_found)
		{
			if(isActive[i])
				isActive[i] = 0;
			i = (i+1)%n;
			
		}
		else
			i = (i-1+n)%n;
	}
}
int next(int city, int jump)
{
	return (city+jump)%n;
}
int prev(int city, int jump)
{
	if(jump)
		return (city-jump+n)%n;
	else
		return city;
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
int main(int argc, char* argv[])
{
	ifstream cin(argv[1]);
	cin >> n;
	float points [n] [2]; // x & y co-ordinates
	isActive = new bool[n];
	adjMatrix = new float* [n];
	
	for (int i=0; i<n; i++)
	{
		isActive[i] =1;
		adjMatrix[i] = new float[n];
		cin >> points[i][0] >> points[i][1];
	}
	for (int i=0; i<n; i++)
	{
		adjMatrix[i][i]=0;
		for (int j=i+1; j<n; j++)
		{
			adjMatrix [i][j] = adjMatrix [j][i] = sqrt(pow(abs(points[i][0]-points[j][0]), 2) + pow(abs(points[i][1]-points[j][1]), 2));
		}
	}
	Tour.reserve(n);
	cost = 0.0;
	// Just join the cities in order to create an initial solution i.e. 0-1-2-3
	for (int i=0; i<n; i++)
	    Tour.push_back(i);
	for (int i=0; i<n; i++)
		cost += adjMatrix[Tour[i]][Tour[(i+1)%n]];
	clog << "Initial Cost: "<< cost <<endl;
	TwoOptSearch();
	clog << "After 2-Opt Cost: "<< cost <<endl;
	
}