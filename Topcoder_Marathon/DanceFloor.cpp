#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <bits/stdc++.h>
#include <algorithm>
//#pragma GCC optimize "O3"
//#pragma GCC target "sse4.2"

using namespace std;
#define INF 1e9
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
    uint32_t operator() () {
        uint32_t t = (a ^ (a << 11));
        a = b; b = c; c = d;
        return d = (d ^ (d >> 19)) ^ (t ^ (t >> 8));
    }
    static constexpr uint32_t max() { return numeric_limits<result_type>::max(); }
    static constexpr uint32_t min() { return numeric_limits<result_type>::min(); }
private:
    uint32_t a, b, c, d;
};

constexpr double ticks_per_sec = 2800000000;
constexpr double sec_per_ticks = 1.0 / ticks_per_sec;
inline double rdtsc() {  // seconds
    uint32_t lo, hi;
    asm volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo) * sec_per_ticks;
}


class DanceFloor 
{
	int N;
	int C;
	int D;
	int S;
	vector<vector<string>> tileColors; // For each N*N tiles, changing of colors.
	vector<vector<int>> marks; // Each Dancer position at end of each time segment 'x' 'y' 't'
	vector<vector<pair<int, int>>> board; // index in tileColor and current Color.
	vector<pair<int, int>> pos; // Position of each dancer
	//Solution:
	pair<long long, vector<string>>  best;
	// For Union-Find Connected Label
	int color;
	set<int> s;
	vector<int> component;
public:
    int getColor(int x, int y)
	{
		board[x][y].first = (board[x][y].first+1) % C;
		return tileColors[x][y].at(board[x][y].first)-'0';
	}
	void doUnion(int a, int b)
	{
		// get the root component of a and b, and set the one's parent to the other
		while (component[a] != a)
			a = component[a];
		while (component[b] != b)
			b = component[b];
		
		if(b==a)return;
		if(b>a)
		{
			s.erase(b);
			component[b] = a;
		}
		else
		{
			s.erase(a);
			component[a] = b;
		}
	}
	void unionCoords(int x, int y, int x2, int y2)
	{
		if (y2 < N && x2 < N && (board[x][y].second==color) && (board[x2][y2].second==color))
			doUnion(x*N + y, x2*N + y2);
	}
	long long getScore()
	{
		component.resize(N*N);
		long long result =0;
		
		for (color = 0 ; color <C; ++color)
		{
			for (int i = 0; i < N*N; i++)
			{
				s.insert(i);
				component[i] = i;
			}
			for (int x = 0; x < N; x++)
				for (int y = 0; y < N; y++)
				{
					if(board[x][y].second!=color){
						s.erase(x*N + y);
						continue;
					}
					unionCoords(x, y, x+1, y);
					unionCoords(x, y, x, y+1);
				}
			result += (s.size()	*s.size());
			s.clear();	
		}
		return result;
	}
	void updateBoard(int d, string moveSet)
	{
		for (auto &move: moveSet)
		{
			if(move=='D')
				++pos[d].first;
			else if(move=='U')
				--pos[d].first;
			else if(move=='R')
				++pos[d].second;
			else if(move=='L')
				--pos[d].second;
			else
				return;
			board[pos[d].first][pos[d].second].second = getColor(pos[d].first, pos[d].second);
		}
	}
	void updateBoard(int d, char move)
	{
		{
			if(move=='D')
				++pos[d].first;
			else if(move=='U')
				--pos[d].first;
			else if(move=='R')
				++pos[d].second;
			else if(move=='L')
				--pos[d].second;
			else
				return;
			board[pos[d].first][pos[d].second].second = getColor(pos[d].first, pos[d].second);
		}
	}
	vector<string> updateMoves(vector<vector<string>> &moves)
	{
		vector<string> ret(S);
		vector<vector<int>> index(D , vector<int>(2, 0)); // For each dance , which segment is active and in that segment what's the index in moves.
		for (int i =0; i<S; ++i)
			for (int j = 0; j< D; ++j)
			{
				updateBoard(j, moves[j][index[j][0]][index[j][1]]);
				ret[i].push_back(moves[j][index[j][0]][index[j][1]++]);
				
				if(moves[j][index[j][0]].size() == index[j][1])
				{
					++index[j][0]; ///Move to next segment
					index[j][1] = 0;
				}
			}
		return ret;
	}
	vector<vector<string>> calDancerPath()
	{
		vector<vector<string>> d(D);
		for (int i=0; i<D; ++i)
		{
			d[i].resize((marks[i].size()/3 ) -1);
			int p_x=marks[i][0], p_y=marks[i][1], p_t=marks[i][2];
			for (int j =3; j<marks[i].size(); j = j+3)
			{
				int m = abs(marks[i][j]-p_x) + abs(marks[i][j+1]-p_y);
				int t = marks[i][j+2] - p_t;
				
				string move;
				
				if(marks[i][j] > p_x)
					move.append(marks[i][j] - p_x, 'R');
				else
					move.append(p_x - marks[i][j], 'L' );
				
				if(marks[i][j+1] < p_y)
					move.append(p_y- marks[i][j+1] , 'U');
				else
					move.append(marks[i][j+1]-p_y, 'D' );
				
				if(t-m)
					move.append(t-m, '-' );
				
				next_permutation(move.begin(), move.end());
				
				p_t = marks[i][j+2];
				p_x = marks[i][j];
				p_y = marks[i][j+1];
				
				
				d[i][(j/3) -1] = move;
			}
		}
		return d;
	}
	void dumpBoard()
	{
		for (int i=0; i< N; ++i, cout <<"\n")
			for (int j=0; j< N; ++j)
				cerr << board[i][j].second<<" ";
	}
	void initializeBoard()
	{
		this->pos.resize(D);
		for (int i = 0; i <D; ++i)
			pos[i] = make_pair(marks[i][1], marks[i][0]); // x,y in statement to r,c in array
		
		board.resize(N);
		for (int i= 0; i<N; ++i)
		{
			board[i].resize(N);
			for (int j=0; j<N; ++j)
			{
				board[i][j].first  = -1;
				board[i][j].second = getColor(i, j);
			}
		}
	}
	void resetBoard()
	{
		for (int i = 0; i <D; ++i)
		{
			 // x,y in statement to r,c in array
			pos[i].first  = marks[i][1];
			pos[i].second = marks[i][0];
		}
		for (int i= 0; i<N; ++i)
			for (int j=0; j<N; ++j)
			{
				board[i][j].first  = -1;
				board[i][j].second = getColor(i, j);
			}
	}
    vector<string> findSolution(int N, int C, int D, int S, vector<vector<string>> &tileColors, vector<vector<int>> marks) 
    {
		vector<string> ret(S);
		this->best.first = INF;
		this->N = N;this->C = C;this->D = D;this->S = S;
		this->tileColors = tileColors;
		this->marks = marks;
		initializeBoard();
		long long score = getScore();
		
		// Calculate each Dancer path for each step
		vector<vector<string>> moves = calDancerPath();
		score = getScore();
		ret = updateMoves(moves);
		score = getScore();
		if(score < best.first)
		{
			best.first = score;
			best.second = ret;
		}
		
		// Simulated Annealing
		double clock_begin = rdtsc();
		constexpr int TIME_LIMIT = 9;  // seconds
		double clock_end =  clock_begin + TIME_LIMIT;
		//Used for randomness
		random_device device;
		xorshift_128 gen(device());
		double temperature;
		
		for (unsigned iteration = 0; ; ++ iteration) 
		{
			if (iteration % 32 == 0) 
			{
				temperature = (clock_end - rdtsc()) / (clock_end - clock_begin);
				if (temperature <= 0.0) 
					break;
			}
			// Select a random dancer, then random segment, then next permutation	
			int dancer = uniform_int_distribution<int>(0, D - 1)(gen);
			int segment = uniform_int_distribution<int>(0, (marks[dancer].size()/3)-2)(gen);
			string oldMove = moves[dancer][segment];
			random_shuffle(moves[dancer][segment].begin(), moves[dancer][segment].end());
			resetBoard();
			ret = updateMoves(moves);
			score = getScore();
			
			float delta = score - best.first;
			auto probability = [&]() {
            constexpr double boltzmann = -0.05;
            return exp(boltzmann * delta / temperature);
			};
			
			if((delta < 0) || bernoulli_distribution(probability())(gen))
			{
				best.first = score;
				best.second = ret;
			}
			else
				moves[dancer][segment] = oldMove;
		}
		//cerr << "My score is : "<< score<<endl;
        return best.second;
    }
};

int main() 
{
    int N, C, D, S;
    cin >> N >> C >> D >> S;

    vector<vector<string>> tileColors(N, vector<string>(N));
    for (int y = 0; y < N; y++) 
    {
        for (int x = 0; x < N; x++) 
        {
            cin >> tileColors[y][x];
        }
    }

    vector<vector<int>> marks(D);
    for (int i = 0; i < D; i++) 
    {
        int numMarks;
        cin >> numMarks;
        marks[i].resize(3 * numMarks);
        for (int j = 0; j < 3 * numMarks; j++) 
        {
            cin >> marks[i][j];
        }
    }

    DanceFloor sol;
    vector<string> ret = sol.findSolution(N, C, D, S, tileColors, marks);

    cout << ret.size() << endl;
    for (int i = 0; i < ret.size(); i++) 
    {
        cout << ret[i] << endl;
    }
    cout.flush();
    return 0;
}
