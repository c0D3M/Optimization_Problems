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
template<typename IT>
void swap_ranges(IT start_a, IT end_a, IT start_b, IT end_b) {
    //Will return the position after the moved elements
    auto it = std::rotate(start_a, start_b, end_b);
    //Will determine the point where the new range needs to be moved from
    auto new_start_a = (end_a - start_a) + it;
    std::rotate(it, new_start_a, end_b);
}

bool getDelta3Opt(int a, int b, int c, int d, int e, int f)
{
	bool rc = false;
	float ab = getDistance(a, b);
	float cd = getDistance(c, d);
	float ef = getDistance(e, f);
	
	float ac = getDistance(a, c);
	float ad = getDistance(a, d);
	float ae = getDistance(a, e);
	float bd = getDistance(b ,d);
	float be = getDistance(b ,e);
	float bf = getDistance(b ,f);
	float ce = getDistance(c, e);
	float cf = getDistance(c, f);
	float df = getDistance(d, f);
	
	// Four cases possible , choose biggest delta
	float delta[4];
	
	delta[0] = -(ab+cd+ef) + (ac+be+df);
	delta[1] = -(ab+cd+ef) + (ad+ce+bf);
	delta[2] = -(ab+cd+ef) + (ad+be+cf);
	delta[3] = -(ab+cd+ef) + (ae+bd+cf);
	int index =-1;
	if((delta[0] <0) && (delta[0]<delta[1]) && 	(delta[0]<delta[2]) && (delta[0]<delta[3]))
		index = 0;
	if((delta[1] <0) && (delta[1]<delta[0]) && 	(delta[1]<delta[2]) && (delta[1]<delta[3]))
		index = 1;
	if((delta[2] <0) && (delta[2]<delta[0]) && 	(delta[2]<delta[1]) && (delta[2]<delta[3]))
		index = 2;
	if((delta[3] <0) && (delta[3]<delta[0]) && 	(delta[3]<delta[1]) && (delta[3]<delta[2]))
		index = 3;
	/*
	While doing 3-opt moves we have swap the routes, for example
	0  1  2  3  4  5  6  -> Tour array
	0->1->2->3->4->5->6
	If we have  to rotate [2,4] we can do that with ease but if we have to rotate something like
	[5,1] , we cant do that using C++ rotate API, as the left index > right index.
	To do that we first left shift , rotate and then do right shift [ normalize flag tell if we have to
	do this or not]
	So in above example it will become
	[5,1]  become [3,6] after left shift [move it to origin i.e. index 0]
	Do rotate and thenright shift to again make [5,1]
	
	*/
	bool normalize=false;
	int shifter;
	switch (index)
	{
		case 0:
		reverseTour(b, c);
		reverseTour(d, e);
		break;
		case 1:
		if((b>c) || (d > e))
		{
			normalize = true;
			shifter=a;
			rotate(Tour.begin(), Tour.begin()+a, Tour.end());
			
			b = (b- a+n)%n;
			c = (c- a+n)%n;
			d = (d- a+n)%n;
			e = (e- a+n)%n;
			f = (f- a+n)%n;
			
			a -= a;
		}
		reverseTour(b, c);
		swap_ranges(Tour.begin()+b, Tour.begin()+c+1, Tour.begin()+d, Tour.begin()+e+1);
		break;
		case 2:
		if((b>c) || (d > e))
		{
			normalize = true;
			shifter=a;
			rotate(Tour.begin(), Tour.begin()+a, Tour.end());
			
			b = (b- a+n)%n;
			c = (c- a+n)%n;
			d = (d- a+n)%n;
			e = (e- a+n)%n;
			f = (f- a+n)%n;
			
			a -= a;
		}
		swap_ranges(Tour.begin()+b, Tour.begin()+c+1, Tour.begin()+d, Tour.begin()+e+1);
		// No direction change
		break;
		case 3:
		if((b>c) || (d > e))
		{
			normalize = true;
			shifter=a;
			rotate(Tour.begin(), Tour.begin()+a, Tour.end());
			
			b = (b- a+n)%n;
			c = (c- a+n)%n;
			d = (d- a+n)%n;
			e = (e- a+n)%n;
			f = (f- a+n)%n;
			
			a -= a;
		}
		reverseTour(d, e);
		swap_ranges(Tour.begin()+b, Tour.begin()+c+1, Tour.begin()+d, Tour.begin()+e+1);
		break;
	}
	if(index !=-1)
	{
		cost += delta[index];
		rc = true;
	}
	if(normalize)
		rotate(Tour.begin(), Tour.begin()+n-shifter, Tour.end());
	return rc;
}
float getCost()
{
	float tcost =0.0;
	for (int i=0; i<n; i++)
		tcost += adjMatrix[Tour[i]][Tour[(i+1)%n]];
	return tcost;
}
bool ThreeOptSearch()
{
	int i =0;  //current city
	
	while (1)
	{
		int j = next(i, 1);
		bool improve = false;
		for ( int k= next(i, 2), l = next(i, 3) ; !improve && (l!=i); k=l, l=next(l, 1))
			for(int m = next(k, 2), n=next(k, 3); !improve && (m!=i) && (n!=i); m=n, n =next(n,1))
			{
				LOG(i<<" "<<j<<" "<<k<<" "<<l<<" "<<m<<" "<<n);
				improve =getDelta3Opt(i, j, k, l, m, n);
				LOG("Calculated cost is "<< getCost()<<" Cost= "<<cost);
			}
		i = next(i, 1);
	}
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
	ThreeOptSearch();
	clog << "After 3-Opt Cost: "<< cost <<endl;
}
