// C++11
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <queue>
#include <bits/stdc++.h>
#include <algorithm>

#pragma GCC optimize "O3"
#pragma GCC target "sse4.2"
#ifdef DEBUG
int debug =1;
#else
int debug = 0;
#endif
using namespace std;
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
	uint32_t operator()() {
		uint32_t t = (a ^ (a << 11));
		a = b;
		b = c;
		c = d;
		return d = (d ^ (d >> 19)) ^ (t ^ (t >> 8));
	}
	static constexpr uint32_t max() {
		return numeric_limits < result_type > ::max();
	}
	static constexpr uint32_t min() {
		return numeric_limits < result_type > ::min();
	}
private:
	uint32_t a, b, c, d;
};

constexpr double ticks_per_sec = 2800000000;
constexpr double sec_per_ticks = 1.0 / ticks_per_sec;
inline double rdtsc() {  // seconds
	uint32_t lo, hi;
	asm volatile ("rdtsc" : "=a" (lo), "=d" (hi));
	return (((uint64_t) hi << 32) | lo) * sec_per_ticks;
}

typedef pair<int, int> PII;

typedef enum {
	CLOCKWISE = 0, ANTICLOCKWISE, MAX = 2,
} Direction;

//state
typedef enum{
	HIGH_ENERGY=0,
	MEDIUM_ENERGY,
	LOW_ENERGY,
} State;

//penalty threshold


int  HIGH_PENALTY_THRESHOLD = 25;
int  LOW_PENALTY_THRESHOLD = 5;


//Move threshold
typedef enum {
	HIGH_MOVE_THRESHOLD = 2,
	MEDIUM_MOVE_THRESHOLD = 5,
	LOW_MOVE_THRESHOLD = 8
}T;

//Information of each cell.
typedef struct {
	int value;   // value 
	int m_dist; // manhattan distance of the current cell
} C;
int rowSum(int res, C &a) {
	return res + a.m_dist;
}
// Move
typedef struct {
	int x; // row
	int c; // column 
	int s; // size of square
	Direction d; // direction 
	int cost;  // cost saving  
} M;

class RotatingNumbers {
	int locPenalty;
	int bestPenalty;
	int N;
	int P;
	State state;
	vector<int> G;
	vector<int> bestG;
	int moves;
	vector<C> c; // information for each cell
    vector < string > out;
	vector < string > bestMoves;
public:

	// Calculate Manhattan distance
	int getManhattanDistance(PII source, PII target) {
		return pow(abs(source.first - target.first),2)+ pow(abs(source.second - target.second),2);
		//return abs(source.first - target.first)	+ abs(source.second - target.second);
	}

	//Calculate the target coordinate of a given value.
	PII getTarget(int value) {
		int x = value % N;
		int q = value / N;
		if (x) {
			return make_pair(q, x - 1);
		} else
			return make_pair(q - 1, N - 1);
	}

	void rotateMatrixAnti(int **mat, int S) {
		// Consider all squares one by one
		for (int x = 0; x < S / 2; x++) {
			// Consider elements in group of 4 in
			// current square
			for (int y = x; y < S - x - 1; y++) {
				// store current cell in temp variable
				int temp = mat[x][y];

				// move values from right to top
				mat[x][y] = mat[y][S - 1 - x];

				// move values from bottom to right
				mat[y][S - 1 - x] = mat[S - 1 - x][S - 1 - y];

				// move values from left to bottom
				mat[S - 1 - x][S - 1 - y] = mat[S - 1 - y][x];

				// assign temp to left
				mat[S - 1 - y][x] = temp;
			}
		}
	}

	void rotateMatrix(int **mat, int S) {
		// Consider all squares one by one
		for (int x = 0; x < S / 2; x++) {
			// Consider elements in group of 4 in
			// current square
			for (int y = x; y < S - x - 1; y++) {
				// store current cell in temp variable
				int temp = mat[x][y];

				// move values from left to top
				mat[x][y] = mat[S - 1 - y][x];

				// move values from bottom to left
				mat[S - 1 - y][x] = mat[S - 1 - x][S - 1 - y];

				// move values from right to bottom
				mat[S - 1 - x][S - 1 - y] = mat[y][S - 1 - x];

				// assign temp to right
				mat[y][S - 1 - x] = temp;
			}
		}
	}

	// Given a co-ordinate , return the target value
	int getTargetValue(PII t) {
		return t.first * N + (t.second + 1);
	}

	// Rotate the square of 'size' in 'direction' at index 'x,y'
	// and update the Grid
	int rotate(int x, int y, int size, Direction d) {
		//create a subarray and rotate
		int **t;
		t = new int *[size];
		for (int i = 0; i < size; ++i) {
			t[i] = new int[size];
			for (int j = 0; j < size; ++j)
				t[i][j] = G[(x + i) * N + y + j];
		}

		//Rotate it
		if (d == CLOCKWISE)
			rotateMatrix(t, size);
		else
			rotateMatrixAnti(t, size);

		//Update the Grid
		for (int i = 0; i < size; ++i)
			for (int j = 0; j < size; ++j)
			{
				G[(x + i) * N + y + j] = t[i][j];
				c[(x + i) * N + y + j].value = G[(x + i) * N + y + j];
				PII t = getTarget(c[(x + i) * N + y + j].value);
				c[(x + i) * N + y + j].m_dist = getManhattanDistance(make_pair(x+i, y+j), t);
			}
	}

	// Setup the Grid :
	// Calculate manhattan distance of for each cell
	void setupBoard(void) {
		int total = 0;
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j) {

				c[i * N + j].value = G[i * N + j];
				PII t = getTarget(c[i * N + j].value);
				c[i * N + j].m_dist = getManhattanDistance(make_pair(i, j), t);

				//////if(debug) cerr << c[i*N+j].value <<" Target: "<< t.first << " "<<t.second <<" Penalty =" <<c[i*N+j].m_dist <<"\n";
				total += c[i * N + j].m_dist;
			}
		locPenalty = total*P;
		bestPenalty = locPenalty;
		bestG.clear();
		bestG = G;
		moves =0;
		if(locPenalty > HIGH_PENALTY_THRESHOLD)
			state = HIGH_ENERGY;
		else if (locPenalty < LOW_PENALTY_THRESHOLD)
			state = LOW_ENERGY;
		else
			state = MEDIUM_ENERGY;
	}
	void restoreBestMove()
	{
		if(debug) cerr << "restoring best S:"<< state <<" M: "<<moves<<" CurrentP: "<< locPenalty<<" BestP: " << bestPenalty;
		locPenalty = bestPenalty;
		out.clear();
		G.clear();
		copy (bestG.begin(), bestG.end(), std::back_inserter(G));
		//G = bestG;
		//clear of #moves from tail of bestMove
		if(bestMoves.size() >= moves)
			bestMoves.erase(bestMoves.end()-moves, bestMoves.end());
		setupBoard();
		if(debug) cerr << " moveSize: "<<bestMoves.size()<<endl;
		moves = 0;
	}

	// Find the cost of moving clockwise and anticlockwise for a given position(x,y) and square-size (s)
	M findBestMove(int x, int y, int s) {
		//Choose rectangle from  2 to N size
		//for (int s = 2; s <=N; ++s) // size of square loop
		{
			//for (int i =0 ; i <= N-s; ++i)
			//for (int j =0 ; j <= N-s; ++j) // this fixed the co-ordinate to start he square
			{
				int loop_s = s;
				int ii = x;
				int jj = y;
				int current_cost = 0, clock_cost = 0, aclock_cost = 0;

				for (int k = 0; k < s; ++k)
					current_cost += accumulate(c.begin() + ((x + k) * N + y),
							c.begin() + ((x + k) * N + y + s), 0, rowSum);
				if(debug) cerr << " | Chosen [" << x << "," << y << "] size: " << s << " Cost=" << current_cost<<" ";
				PII t;
				// Sum and calculate the cost
				while (loop_s) {
					int index = ii * N + jj;
					if (loop_s == 1) {
						clock_cost += c[index].m_dist;
						aclock_cost += c[index].m_dist;
						break;
					}

					// Process corners
					//clockwise
					{
						t = getTarget(c[index].value);
						clock_cost += getManhattanDistance(make_pair(ii, jj + loop_s - 1), t);

						t = getTarget(c[index + loop_s - 1].value);
						clock_cost += getManhattanDistance(make_pair(ii + loop_s - 1, jj + loop_s - 1), t);

						t = getTarget( c[((ii + loop_s - 1) * N) + jj + loop_s - 1].value);
						clock_cost += getManhattanDistance(make_pair(ii + loop_s - 1, jj), t);

						t = getTarget(c[((ii + loop_s - 1) * N) + jj].value);
						clock_cost += getManhattanDistance(make_pair(ii, jj), t);
					}
					//anti-clockwise
					{
						t = getTarget(c[index].value);
						aclock_cost += getManhattanDistance(make_pair(ii + loop_s - 1, jj), t);

						t = getTarget(c[index + loop_s - 1].value);
						aclock_cost += getManhattanDistance(make_pair(ii, jj),	t);

						t = getTarget(c[((ii + loop_s - 1) * N) + jj + loop_s- 1].value);
						aclock_cost += getManhattanDistance(make_pair(ii, jj + (loop_s - 1)), t);

						t = getTarget(c[((ii + loop_s - 1) * N) + jj].value);
						aclock_cost += getManhattanDistance(make_pair(ii + loop_s - 1, jj + loop_s - 1), t);
					}

					for (int i = 1; i <= loop_s - 2; ++i) //-2 since we processed corners above
							{
						t = getTarget(c[index + i].value);
						clock_cost += getManhattanDistance(make_pair(ii + i, jj + loop_s - 1), t);

						t = getTarget(c[(ii + i) * N + jj + loop_s - 1].value);
						clock_cost += getManhattanDistance(make_pair(ii + loop_s - 1, jj + loop_s - 1 - i),	t);

						t = getTarget(c[(ii + loop_s - 1) * N + jj + loop_s	- 1 - i].value);
						clock_cost += getManhattanDistance(make_pair(ii + loop_s - 1 - i, jj), t);

						t = getTarget(c[(ii + loop_s - 1 - i) * N + jj].value);
						clock_cost += getManhattanDistance(	make_pair(ii, jj + i), t);

						//Anti-Clockwise
						t = getTarget(c[index + i].value);
						aclock_cost += getManhattanDistance(make_pair(ii + loop_s - 1 - i, jj), t);

						t = getTarget(c[(ii + i) * N + jj + loop_s - 1].value);
						aclock_cost += getManhattanDistance(make_pair(ii, jj + i), t);

						t = getTarget(c[(ii + loop_s - 1) * N + jj + loop_s	- 1 - i].value);
						aclock_cost += getManhattanDistance(make_pair(ii + i, jj + loop_s - 1), t);

						t = getTarget(c[(ii + loop_s - 1 - i) * N + jj].value);
						aclock_cost += getManhattanDistance(make_pair(ii + loop_s - 1, jj + loop_s - 1 - i), t);
					}
					// loop and process inner square
					loop_s -= 2;
					ii += 1;
					jj += 1;
				}

				if(debug) cerr << " Clockwise: " << clock_cost<<" ";
				if(debug) cerr << " Anti-Clockwise: " << aclock_cost<<" \n";

				return M { x, y, s,
						clock_cost < aclock_cost ? CLOCKWISE : ANTICLOCKWISE,
						clock_cost < aclock_cost ?
								P *(clock_cost - current_cost) :
								P*(aclock_cost - current_cost) };
			}
		}
	}

	vector<string> findSolution(int N, int P, vector<int> grid) {
		this->N = N;
		this->P = P;
		this->G.reserve(N*N);
		this->G = grid;
		this->c.reserve(N * N);
		this->bestG.reserve(N*N);
		HIGH_PENALTY_THRESHOLD *=P;
		LOW_PENALTY_THRESHOLD *= P;
		

		// Simulated Annealing
		double clock_begin = rdtsc();
		constexpr int TIME_LIMIT = 9;  // seconds
		double clock_end = clock_begin + TIME_LIMIT;

		//Used for randomness
		random_device device;
		xorshift_128 gen(device());
		double temperature;
		setupBoard();
		if(debug) cerr << " Penalty " <<locPenalty<<"\n";
		bestPenalty = locPenalty;
		{
			for (unsigned iteration = 0;; ++iteration) {
				
				if (!locPenalty)
					break;
				
				if(locPenalty >= bestPenalty)
				if((state==HIGH_ENERGY && moves== HIGH_MOVE_THRESHOLD ) || 
					(state==MEDIUM_ENERGY && moves == MEDIUM_MOVE_THRESHOLD ) || 
				     (state == LOW_ENERGY && moves == LOW_MOVE_THRESHOLD))
					 restoreBestMove();
					 
				if(debug) cerr << "S="<<state<<" M="<<moves<<" Penalty is: " << locPenalty<<" ";	 
					 
				if (iteration % 32 == 0) {
					temperature = (clock_end - rdtsc())
							/ (clock_end - clock_begin);
					if (temperature <= 0.0)
						break;
				}
				// First randomly generate x, y co-ordinate
				int x, y, s = -1;
				while (s == -1) {
					x = uniform_int_distribution<int>(0, N - 1)(gen);
					y = uniform_int_distribution<int>(0, N - 1)(gen);

					//Now select a random valid 's' for above x,y
					{
						int x_left_s = x + 2 - 1 < N ? 2 : -1;
						if (x_left_s == -1)
							continue;
						int x_right_s = N;
						while (!((x + x_right_s - 1) < N))
							--x_right_s;

						int y_left_s = y + 2 - 1 < N ? 2 : -1;
						int y_right_s = N;
						while (!((y + y_right_s - 1) < N))
							--y_right_s;

						if (y_left_s == -1)
							continue;

						s = uniform_int_distribution<int>(
								min(x_left_s, y_left_s),
								min(x_right_s, y_right_s))(gen);
					}
				} //while loop of finding a valid 's' ends

				// Find the energy value
				M m = findBestMove(x, y, s);
				/*
				if(!m.cost || ((m.cost+locPenalty) < 0 )) // if there is a move which isnt gaining anything , co
					continue;
					*/
				float delta = m.cost + floor(pow(s-1, 1.5));
				/*
				if(!delta)
					continue;
				*/
				auto probability = [&]() {
					constexpr double boltzmann = -0.05;
					return exp(boltzmann * delta / temperature);
				};
				if (delta < 0 or bernoulli_distribution(probability())(gen)) // Good move , accept it
				{
							
					locPenalty += delta;///m.cost;
					string txt(
							to_string(m.x) + " " + to_string(m.c) + " "
									+ to_string(m.s) + " " + (m.d ? "L " : "R ") /*+ to_string(locPenalty)*/);
					rotate(m.x, m.c, m.s, m.d);
					bestMoves.push_back(txt);
					//out.push_back(txt);
					
					saveBestFound();
					//if(debug) cerr << " ** " << " BP: "<< bestPenalty;
				}
			} //forLoop of SA iteration
		} // while loop locPenalty
		//if(debug) 
			cerr << "\n bestPenalty "<< bestPenalty << " Size: "<<bestMoves.size()<<" N= "<<N<<"\n";
		return bestMoves;
	}
	void saveBestFound()
	{
		if(locPenalty < bestPenalty)
		{
			bestPenalty = locPenalty;
			bestG = G;
			
			//copy (out.begin(), out.end(), std::back_inserter(bestMoves));
			//out.clear();
			moves = 0;
			if(debug) cerr << "Found a good move with penalty "<< bestPenalty <<" moveSize " <<bestMoves.size() <<"\n";
		}
		else
		{
			++moves;
			if(debug) cerr <<"Bad move taken "<< moves <<endl;
		}
	}
};

int main() {
	RotatingNumbers prog;
	int N;
	int P;
	int num;
	vector<int> grid;

	cin >> N;
	cin >> P;
	for (int i = 0; i < N * N; i++) {
		cin >> num;
		grid.push_back(num);
	}

	vector < string > ret = prog.findSolution(N, P, grid);
	cout << ret.size() << endl;
	for (int i = 0; i < (int) ret.size(); ++i)
		cout << ret[i] << endl;

	cout.flush();
}
