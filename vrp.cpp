#include <bits/stdc++.h>
#include <algorithm>

using namespace std;
#ifdef DEBUG
    #define LOG(x) std::cerr << x << std::endl
	#define LOG_S(x) std::cerr << x
#else
    #define LOG(x)
	#define LOG_S(x)
#endif
#define PI 3.14159265

typedef struct Customer
{
	int demand;
	int left;
	float x;
	float y;
	int id;
	double angle; // in radians , from depot
	double coneAngle;// cone angle formed by bisection of ith and i+1 customer.
}C;
typedef struct VehicleCone
{
	double start;
	double end;
	vector<int> customer;
}VC;
typedef struct Cluster
{
	int capacity;
	vector<int> customer;
}Cluster;
int n;// # of customer
int v;// # of vehicle
int q;// capacity of each vehicle
float **adjMatrix; // distance matrix
float **custToSeed;
float **seedToCust;
vector<Cluster> cluster; // cluster for each vehicle which contains which customer each vehicle will visit.
vector<VC> vc;
float total_demand;
float total_capacity;
float perV;
vector<pair<float, float> > seedPoint;
vector<C> c;
bool sortbyAngle(C a, C b)
{
	return a.angle < b.angle;
}
bool sortbyDemand(C a, C b)
{
	return a.demand > b.demand;
}
bool sortbyID(C a, C b)
{
	return a.id < b.id;
}
void calculateSeedDistance();
void assignCustomerToVehicle();
#define next(i) ((i==n-1)?1:i+1)
#define prev(i) ((i==1)?n-1:i-1)
int main(int argc, char* argv[])
{
	/*
		http://www.bernabe.dorronsoro.es/vrp/index.html?/algorithms/ClustRout.html
		Cluster First, Route Second.
		Sweep Algorithm to find cluster
	*/
	ifstream cin(argv[1]);
	
	
	cin >> n;
	cin >> v;
	cin >> q;
	total_capacity = q * v;
	vc.reserve(v);
	cluster.reserve(v);

	for (int i=0; i<n ;i++)
	{
		C t;
		cin >> t.demand;
		cin >> t.x;
		cin >> t.y;
		t.id = i;
		if(i!=0)
		{
			t.angle = atan2 (t.x - c[0].x, t.y-c[0].y);
			if(t.angle<0)
				t.angle += (2*PI);
		}
		t.left = t.demand;
		total_demand += t.demand;
		c.push_back(t);
	}
	v = ceil(total_demand/q);
	for (int i=0; i<v; i++)
	{
		cluster[i].capacity = q;
		cluster[i].customer.reserve(v);
	}
	perV = (total_demand / total_capacity )*q;
	LOG("Total Demand= "<< total_demand<<" Total Capacity "<< total_capacity<<" per Vehicle= "<<perV);
	// Adjacency matrix between customers & depot.
	adjMatrix = new float* [n];
	for (int i=0; i<n; i++)
		adjMatrix[i] = new float[n];
	for (int i=0; i<n; i++)
	{
		
		adjMatrix[i][i]=0;
		for (int j=i+1; j<n; j++)
			adjMatrix [i][j] = adjMatrix [j][i] = sqrt(pow(abs(c[i].x-c[j].x), 2) + pow(abs(c[i].y-c[j].y), 2));
	}
	// Find the angle between customer and depot.
	sort(c.begin()+1, c.end(), sortbyAngle); // c[0] is depot so dont include that in sorting
	
	for (int i=1; i<n; i++)
	{
		
		if((c[next(i)].angle-c[i].angle)<0)
			c[i].coneAngle = c[i].angle + ((2*PI) - c[i].angle)/2;
		else
			c[i].coneAngle = c[i].angle + (c[next(i)].angle-c[i].angle)/2;
	}
	LOG("ID   Demand x y angle coneAngle"); 
	for (int i=1; i<n; i++)
		LOG("i= "<<c[i].id<<c[i].demand<<" x="<<c[i].x<<" y="<<c[i].y<<" "<<c[i].angle* 180 / PI<<" "<<c[i].coneAngle* 180 / PI);
	
	// ith customer cone is defined as  coneAngle of prev(i) & i
	// Calculate seed points
	int prev =-1;
	int index=1;
	double start = c[n-1].coneAngle;
	vector<double> seedAngle;
	for (int i=0; i < v; i++)
	{
		int sum =0, j;
		for (j =index; c[j].left && (sum+c[j].left)<= perV; j = next(j))
		{
			sum += c[j].left;
			c[j].left =0;
			vc[i].customer.push_back(j);
		}
		double end;
		if(perV-sum)
		{
			end = c[prev(j)].coneAngle + (c[j].coneAngle - c[prev(j)].coneAngle)*(perV-sum)/c[j].demand;
			c[j].left -= c[j].left*(perV-sum)/c[j].demand;
		}	
		else
			end = c[prev(j)].coneAngle;
		
		if((j !=1 ) && (end > c[j].coneAngle))
			vc[i].customer.push_back(j);
		LOG("cone angle start ="<<start*180/PI<<" end = " << end*180/PI<<" size= " << (end-start)*180/PI);
		#if 1
		if((end-start)<0) //cross over
			seedAngle.push_back((start + (2*PI + (end-start))/2)-2*PI);
		else
			seedAngle.push_back(start + (end-start)/2);	
		#endif
		vc[i].start = start;
		vc[i].end = end;
		
		start =end;
		index =j;
	}
	for (auto & i: seedAngle)
		LOG("seedAngle "<< i*180/PI);
	/*
	   Customer angle = angle from y-axis, can be calculated as atan2 	
	   Create customer cone : i & i-1 customer angle.
	1. Each vehicle to serve total_demand/total_capacity percentage load, so based on that vehicle cone, 
	   sum of customer cone and some fraction of last customer.
	2. Construct vehicle cone, bisect the vehicle cone and get the seedAngle
	3. Arc so that 75% of vehicle load is covered and that will give seedDistance.
	*/
	// Step 3:
	
	for (int i = 0; i<v ; i++)
	{
		float capacity = (perV*3.0)/4.0;
		float sum = 0;
		float distance = 0;
		for (int j =0; (j< vc[i].customer.size()) && (sum < capacity); j++)
		{
			sum += c[vc[i].customer[j]].demand;
			if(adjMatrix[0][c[vc[i].customer[j]].id] > distance)// store fartherest customer to create arc.
			{
				distance = adjMatrix[0][c[vc[i].customer[j]].id];
			}
		}
		float seedAngle;
		if((vc[i].end-vc[i].start)<0) //cross over
			seedAngle = ((vc[i].start + (2*PI + (vc[i].end-vc[i].start))/2)-2*PI);
		else
			seedAngle = (vc[i].start + (vc[i].end-vc[i].start)/2);	
		seedPoint.push_back(make_pair(c[0].x+(distance * sin(seedAngle)), c[0].y+(distance*cos(seedAngle))));
	}
	for (int i=0; i<v; i++)
		LOG("seedPoint x="<<seedPoint[i].first<<" y="<<seedPoint[i].second);
	calculateSeedDistance();
	assignCustomerToVehicle();
	
}
//calculate adjMatrix for seedpoint;
void calculateSeedDistance()
{
	sort(c.begin()+1, c.end(), sortbyID);
	custToSeed = new float* [n];
	seedToCust = new float* [v];
	for (int i=0; i<n; i++)
		custToSeed[i] = new float[v];
	for (int i=0; i<v; i++)
		seedToCust[i] = new float[n];
		
	for (int i=0; i<n; i++)
		for(int j=0; j<v ;j++)
			custToSeed [i][j] = seedToCust[j][i] = sqrt(pow(abs(seedPoint[j].first-c[i].x), 2) + pow(abs(seedPoint[j].second-c[i].y), 2));
}
// This function assign each customer to vehicle by checking from which seed point the customer is near.
void assignCustomerToVehicle()
{
	sort(c.begin()+1, c.end(), sortbyDemand);
	for (int i =1; i<n ; i++)
	{
		float minFound = numeric_limits<float>::infinity();
		int assign=-1;
		for (int j=0; j<v; j++)
		{
			//if vehicle capacity is larger than customer i demand 
			if(cluster[j].capacity < c[i].demand)
				continue;
			float cost = min(adjMatrix[0][c[i].id]+custToSeed[c[i].id][j]+seedToCust[j][0], custToSeed[0][j]+seedToCust[j][c[i].id]+adjMatrix[c[i].id][0]) - (custToSeed[0][j] + seedToCust[j][0]);
			if(cost < minFound)
			{
				minFound = cost;
				assign = j; // customer assign to jth vehicle
			}
		}
		if(assign==-1)
			LOG("No cluster can be assigned");
		else
		{
			
			cluster[assign].customer.push_back(c[i].id);
			// Decrease capacity of this vehicle;
			cluster[assign].capacity -= c[i].demand;
			LOG("customer "<< c[i].id<<" cluster "<<assign<< " capacity left "<<cluster[assign].capacity);
		}
	}
	// Solve TSP for each cluster
	float total_cost=0;
	ofstream fout2;
	fout2.open("final.txt");
	for (int i =0; i<v;i++)
	{
		ofstream fout;
		fout.open("in.txt"); 
		fout << cluster[i].customer.size()+1<<endl;
		fout << c[0].x<<" "<<c[0].y<<endl;
		vector<int> mapping; // tsp id to vrp id mapping
		mapping.push_back(0);
		for (auto &j: cluster[i].customer)
		{
			//cout <<j<<" ";
			mapping.push_back(j);
			fout <<c[j].x<<" " << c[j].y<<endl;
		}
		//cout <<endl;
		system("./tsp in.txt > out.txt");
		
		// sum up the cluster tour cost
		ifstream fin;
		fin.open("out.txt");
		float cost, opt;
		fin >> cost >> opt;
		total_cost += cost;
		//cout << cost <<endl;
		int t, rotate_index; //used to rotate around depot since all tour should start from depot
		vector<int> result;
		for (int j=0; j<cluster[i].customer.size()+1; j++)
		{
			fin >> t;
			if(t==0)
				rotate_index = j;
			result.push_back(mapping[t]);
			//cout << t<<"->"<< mapping[t]<<" ";
		}
		//cout <<endl;
		//cout <<"done "<<result.size()<<" "<<rotate_index<<endl;
		rotate(result.begin(), result.begin()+rotate_index, result.end() );
		result.push_back(0);
		
		
		for (auto &i:result)
			fout2<<i<<" ";
		fout2<< endl;
		// Map tsp id to real customer id
		// tour should always start with 0 i.e. depot and also end with 0 (explicit)
	}
	cout << total_cost<<" 0"<<endl;
	system("cat final.txt");
	
}		