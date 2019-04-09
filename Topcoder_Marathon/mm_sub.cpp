// C++11
#include <bits/stdc++.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <string>

using namespace std;
bool myFunc(pair<int, int> s1, pair<int, int> s2)
    {
        return s1.first < s2.first;
    }
class Tile {
public:
    Tile(int rank, int row, int column) : rank(rank),row(row), column(column) {}
    Tile(){}
    int rank;
    int row;
    int column;
    vector<pair<int, int>> block;
};

bool operator<(const Tile& a, const Tile& b) {
    return a.rank < b.rank;
}
class SameColorPairs {
private:
    int H;
    int W;
    vector<pair<int , int> > left[6];
    vector<pair<int, int> > holes[100];
    vector<string> ret;
    priority_queue<Tile> pq;

    void init(vector <string> &board)
    {
        for (int i = 0; i < H; ++i)
        	for (int j = 0; j < W; ++j)
        	    left[board[i][j]-'0'].push_back(make_pair(i, j));

    }
    void remove(vector<pair<int, int> > &h)
    {
        for(int k= h.size()-1; k>=0;k--)
                    if(h[k].first==-1)
                        h.erase(h.begin()+k);
    }
    // Merge hole at i-th index
    void merge_hole(int j)
    {
        std::sort(holes[j].begin(), holes[j].end(), myFunc);

        for(int k= 0;k<holes[j].size()-1; k++)
            for(int l= k+1; l<holes[j].size(); l++)
                if((holes[j][k].second+1)==holes[j][l].first) // consecutive merges
                {
#ifdef DEBUG
                    printf("[Merge]j=%d [%d, %d] [%d, %d]\n", j,  holes[j][k].first, holes[j][k].second, holes[j][l].first, holes[j][l].second);
#endif
                    holes[j][k].second = holes[j][l].second;
                    holes[j][l].first=-1; // No finding a better way, will check
                }
                else if( (holes[j][k].first<=holes[j][l].first) && (holes[j][k].second>= holes[j][l].second)) //2nd lies inside 1st
                    holes[j][l].first=-1; // No finding a better way, will check

        //Remove unused
        remove(holes[j]);
    }
	void debug_u(vector<string> board, vector<string> ret = vector<string>()) {
		cout <<"Holes === "<<endl;
		for (int j=0; j<H; j++)
		{
			cout << "********Size of "<<j<<holes[j].size()<<endl;
			for(auto &i:holes[j])
			cout << i.first << i.second<<endl;
		}
		cout <<"Left"<<endl;
		for (int j=0; j<=5; j++)
		for(auto &i:left[j])
		cout << j <<"--->"<< i.first<<"," << i.second<<endl;
		cout<<"Board====="<<endl;
		for (int ii=0; ii<H; ii++)
		cout <<board[ii]<<endl;
		for (int i = 0; i < (int)ret.size(); ++i)
		cout <<"["<<i<<"]"<< ret[i] << endl;

	}
    void debug(vector<string> board,  vector<string> ret= vector<string>())
    {
#ifdef DEBUG

        cout <<"Holes === "<<endl;
        for (int j=0; j<H; j++)
        {
            cout << "********Size of "<<j<<holes[j].size()<<endl;
            for(auto &i:holes[j])
                cout << i.first << i.second<<endl;
        }
        cout <<"Left"<<endl;
        for (int j=0; j<=5; j++)
            for(auto &i:left[j])
                    cout << j <<"--->"<< i.first<<"," << i.second<<endl;
        cout<<"Board====="<<endl;
        for (int ii=0; ii<H; ii++)
               cout <<board[ii]<<endl;
        for (int i = 0; i < (int)ret.size(); ++i)
                cout <<"["<<i<<"]"<< ret[i] << endl;

#endif
    }
    void removeTile(vector<string> &board, int r, int c, int o_r, int o_c)
    {
        ret.push_back(to_string(r) + " " + to_string(c) + " " + to_string(o_r) + " " + to_string(o_c));
        board[r][c] = ' ';
        board[o_r][o_c] = ' ';

        if(r==o_r) // row
        {
            if(o_c>c)
                holes[r].push_back(make_pair(c, o_c));
            else
                holes[r].push_back(make_pair(o_c, c));
        }
        else//column
        {
            holes[r].push_back(make_pair(c, c));
            holes[o_r].push_back(make_pair(o_c, o_c));
        }

    }
    void deleteCorners(vector<string> &board)
    {
        //left up
        if((board[0][1] !=' ')&& board[0][0]==board[0][1])
            removeTile(board, 0, 0, 0, 1);
        if(H>1)
        if((board[1][0] !=' ')&& board[0][0]==board[1][0])
                    removeTile(board, 0, 0, 1, 0);

         //Right up corner
        {
        if(W>1)
        if((board[0][W-1] !=' ')&& board[0][W-2]==board[0][W-1])
            removeTile(board, 0, W-2, 0, W-1);
        if(H>1)
        if((board[0][W-1] !=' ')&& board[1][W-1]==board[0][W-1])
            removeTile(board, 1, W-1, 0, W-1);
        }

        //Left down corner
        if(H>1 && W>2)
        if((board[H-1][1] !=' ')&& board[H-1][0]==board[H-1][1])
            removeTile(board, H-1, 0, H-1, 1);

        if(H>1)
        if((board[H-2][0] !=' ')&& board[H-1][0]==board[H-2][0])
                    removeTile(board, H-1, 0, H-2, 0);

        // down right
        if(H>1 && W>2)
        if((board[H-1][W-1] !=' ')&& board[H-1][W-2]==board[H-1][W-1])
            removeTile(board, H-1, W-2, H-1, W-1);

        if(H>2 && W>1)
        if((board[H-2][W-1] !=' ')&& board[H-1][W-1]==board[H-2][W-1])
            removeTile(board, H-1, W-1, H-2, W-1);

    }
    bool atemptTile(vector<string> &board, int r, int c)
    {
        //Find another tile of this color and remove , if possible
        //basic version
#if 0
    	if((c+1<W) && (board[r][c+1] !=' ')&& board[r][c]==board[r][c+1])
    	    removeTile(board, r, c, r, c+1);
    	else if((c-1 >= 0) && (board[r][c-1] !=' ') && board[r][c]==board[r][c-1])
            removeTile(board, r, c, r, c-1);
    	else if ((r+1<H) && (board[r+1][c] !=' ') &&board[r+1][c]==board[r][c])
                    removeTile(board, r, c, r+1, c);
    	else if ((r-1>=0) && (board[r-1][c] !=' ') &&board[r-1][c]==board[r][c])
    	            removeTile(board, r, c, r-1, c);
#endif
    	// Forward [r,c+1]
    	bool found = false;
    	int start=0;
    	for (int i = c+1; i<W; ++i)
    	    if(board[r][i]!=' ')
    	        if(board[r][c+start]==board[r][i])//Found a matching tile
    	        {
    	            removeTile(board, r, c+start, r, i);
    				start += 2;
    				++i;
    				found = true;
    	        }
    	        else
    	            break;
        // Backward [r,c-1]
        start = 0;
        for (int i = c - 1; i >= 0; --i)
            if (board[r][i] != ' ')
                if (board[r][c + start] == board[r][i])  //Found a matching tile
                {
                    removeTile(board, r,  c+start, r, i);
                    start -=2;
                    --i;
                    found = true;
                }
                else
                    break;
        // Downward [r+1,c]
        start = 0;
        for (int i = r + 1; i < H; ++i)
            if (board[i][c] != ' ')
                if (board[r+start][c] == board[i][c])  //Found a matching tile
                {
                    removeTile(board, r+start, c, i, c);
                    start +=2;
                    ++i;
                    found = true;
                }
                else
                    break;
        // upward [r,c-1]
        start = 0;
        for (int i = r - 1; i >= 0; --i)
            if (board[i][c] != ' ')
                if (board[r+start][c] == board[i][c])  //Found a matching tile
                {
                    removeTile(board, r+start, c , i, c);
                    start -=2;
                    --i;
                    found = true;
                }
                else
                    break;
        if(!found)
        {
            int i = board[r][c]-'0';
            for (int k=0; k <left[i].size() ; k++)
            {
                // left pieces is now been removed, skip that
            	if((board[left[i][k].first][left[i][k].second]==' '))
            	    continue;
            	//Not self
            	if((r==left[i][k].first) && (c==left[i][k].second))
            	    continue;
            	int n_x = left[i][k].first;
#ifdef DEBUG
            	printf("Found a piece that can be removed [%d, %d]\n", left[i][k].first, left[i][k].second);
#endif
            	{
                    int x, y;
                    if(c< left[i][k].second)
                        {x =c; y=left[i][k].second;}
                    else
                        {x =left[i][k].second; y=c;}

                    //handle x=y case
                    //First row [x+1,y]
                    if(x==y)
                        continue;
                    if(x!=y)
                    {
                    bool found = true;
                    if(c< left[i][k].second)
                    {
                        int h;
                        for (h=0; h<holes[r].size(); h++)
                               if((holes[r][h].first <= x+1) && (holes[r][h].second >= y))
                                   break;
                        if(h==holes[r].size())
                            continue;

                    }
                    else
                    {
                        int h;
                        for (h=0; h<holes[r].size(); h++)
                            if((holes[r][h].first <= x) && (holes[r][h].second >= y-1))
                                break;
                        if(h==holes[r].size())
                            continue;
                    }
                    //Last row  [x, y-1]
                    if(c> left[i][k].second)
                    {
                        int h;
                        for (h=0; h<holes[n_x].size(); h++)
                            if((holes[n_x][h].first <= x+1) && (holes[n_x][h].second >= y))
                                break;
                        if(h==holes[n_x].size())
                            continue;
                    }
                    else
                    {
                        int h;
                        for (h=0; h<holes[n_x].size(); h++)
                            if((holes[n_x][h].first <= x) && (holes[n_x][h].second >= y-1))
                                break;
                        if(h==holes[n_x].size())
                            continue;
                    }
                    }
#if 0
                    if(!(x==0 && y==0))
                        x++;y--;
                    if(x>y)
                        swap(x, y);
#endif
                    int ii, jj;
                    if(abs(r-n_x)>1)
                    {
                    for (ii=min(r, n_x)+1; ii<max(r, n_x); ii++ )
                    {
                        for (jj=0; jj<holes[ii].size(); jj++)
                            if((holes[ii][jj].first <= x) && (holes[ii][jj].second >= y))
                                break;
                        if(jj==holes[ii].size())// not found
                            break;
                    }
                    if(ii!=max(r, n_x))// not found
                        continue;
                    }
                    found = true;
                    removeTile(board, r,  c, left[i][k].first, left[i][k].second);
                    break;
            	}
            }
        }
        // Look for leftover TODO
        return found;
    }
    int getRank(vector<string> &board, int r, int c , vector<pair<int, int>> &block)
    {
        int rank=0;
    	// scan row forward
        for (int i = c+1; i<W; i++)
            if(board[r][i]!= ' ')
            {
                if(board[r][c]!=board[r][i])
                	{++rank;block.push_back(make_pair(r, i));}
                break;
            }

        // scan row back
        for (int i = c-1; i>=0; i--)
            if(board[r][i]!= ' ')
            {
                if(board[r][c]!=board[r][i])
                    {++rank;block.push_back(make_pair(r, i));}
                break;
            }
        // scan column down
        for (int i = r+1; i<H; i++)
        	if(board[i][c]!= ' ')
        	{
        	    if(board[r][c]!=board[i][c])
        	        {++rank;block.push_back(make_pair(i, c));}
        	    break;
        	}
        // scan column up
        for (int i = r-1; i>=0; i--)
            if(board[i][c]!= ' ')
            {
                if(board[r][c]!=board[i][c])
                    {++rank;block.push_back(make_pair(i, c));}
                break;
            }
        // Tile is surrounded by similar color , do nothing
        return rank;
    }
    void iter_pq()
    {
    	while(!pq.empty())
    	{
    	    Tile t = pq.top();
    	    for (auto &i: t.block)
    	        printf("rank=%d [%d, %d] block[%d, %d]", t.rank, t.row, t.column, i.first, i.second);
    	    pq.pop();
    	}
    }
    void removeByRank(vector<string> &board)
    {
        // Scan board and assign rank for each color  i.e. ab in any direction
        for (int i = 0; i < H; ++i)
        	for (int j = 0; j < W; ++j)
        	{
        		// Find the rank for this tile
        	    int r=0;
        	    Tile t;
        	    t.row = i;
        	    t.column = j;
        		if((board[i][j]!=' ') && (t.rank=getRank(board, i, j, t.block)))
        		    	pq.push(t);
        	}
        	//debug(board);
        // Attempt to decrease rank
        bool removed = true;
        int fail_count =0;
        bool test = false;
        while(pq.size())
        {
            if(fail_count==pq.size())
                break;
            Tile t = pq.top();
            if(board[t.row][t.column]==' ')
                {pq.pop();continue;}
            removed =false;
            for (auto &i: t.block)
            {
#ifdef DEBUG
            	printf("Attempt to remove rank=%d [%d, %d] block[%d, %d]\n", t.rank, t.row, t.column, i.first, i.second);
#endif
            	if( (board[i.first][i.second]==' '))
            	    continue;
            	 if(atemptTile(board, i.first, i.second))
            	    removed = true;
            	 if(test)
            	debug_u(board);
            }
            pq.pop();
            if(!removed) // Not able to remove any of the blocker
                fail_count++;
            else
            {
                // Rank to be re-evaluated
            //debug(board);
            t.block.clear();

            if((t.rank=getRank(board, t.row, t.column, t.block)))
                pq.push(t);
            }
        }
        //printf("removed=%d pq.size()=%d \n", fail_count, pq.size());
        if(test)
        debug_u(board);
        return;


    }
public:

    vector<string> removePairs(vector<string> board) {

        H = board.size();
        W = board[0].size();
        
        init(board);
        debug(board);

        //deleteCorners(board);
        debug(board);

        int old_s = ret.size();
        int countz=0;
        do{
            old_s = ret.size();
            pq = priority_queue <Tile>(); // reset it
            removeByRank(board);

            ++countz;
#ifdef DEBUG
            cout <<"Total loop"<<countz<<endl;
#endif
            for (int j=0; j<H; j++)
                        if(holes[j].size())
                            merge_hole(j);
            for(int i=0; i<6; i++)
            {
                if(left[i].size())
                {
                    for (int j =0; j <left[i].size(); j++)
                        if(board[left[i][j].first][left[i][j].second]==' ')
                            left[i][j].first = -1;
                    remove(left[i]);
                }
            }
        }while(old_s!=ret.size());
        //debug_u(board, ret);
        //return ret;
        //iter_pq();
#ifdef DEBUG
        Tile t = pq.top();
        printf("Rank of [%d, %d] \n", t.row, t.column );
        for (auto &i:t.block)
        	{printf("Rank of [%d, %d] %c", i.first, i.second);cout<< board[i.first][i.second]<<endl;}
#endif
        //return ret;

        for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
        {
			if(board[i][j] == ' ')
				continue;
			// find the first pair of horizontally adjacent tiles and remove them	
            if (((j+1) <W) &&  board[i][j] == board[i][j+1]) {
                ret.push_back(to_string(i) + " " + to_string(j) + " " + to_string(i) + " " + to_string(j+1));
                board[i][j] = ' ';
				board[i][j+1] = ' ';
				if(holes[i].size() &&  (holes[i].back().second == (j-1)))//Coalescing
				{
				    holes[i].back().second = j+1;
				}
				else
				{
					holes[i].push_back(make_pair(j, j+1));
				}
            }
			else if ((i+1 < H) && board[i][j] == board[i+1][j]) //vertical
			{
				ret.push_back(to_string(i) + " " + to_string(j) + " " + to_string(i+1) + " " + to_string(j));
                board[i][j] = ' ';
				board[i+1][j] = ' ';
				//Add holes
				holes[i].push_back(make_pair(j, j));
				holes[i+1].push_back(make_pair(j, j));

			}
			else
			{
			    left[board[i][j]-'0'].push_back(make_pair(i, j));
			}
				
        }
        //Merge holes
        for (int j=0; j<H; j++)
            if(holes[j].size())
            	merge_hole(j);
        //Go through each 'left' list and see if we can find pattern in holes list
        int old;
        int count =0;
        bool all_empty=false;
        do{
            ++count;
            //Exit Condition , All tiles removed, all colors, stalemate
            if(  ((ret.size()<<1) == (H*W)) || all_empty)
                break;
            all_empty=true;
        for (int i=0; i<6; i++)
        {
            //removeByRank(board);
            if(!left[i].size())
                continue;
#ifdef DEBUG
            cout<<"Test for =========="<<i<<"999"<<++count<<endl;
#endif
            all_empty=false;
            debug(board, ret);
            old = ret.size();

            for (int j=0; left[i].size() && j<left[i].size()-1; j++)
                for (int k=j+1; left[i].size() && k<left[i].size(); k++)
            {
                    if((left[i][j].first==left[i][k].first) &&(left[i][j].second==left[i][k].second))
                        continue;

                    if(left[i][j].first==-1 || left[i][k].first==-1)
                        continue;
                    if((board[left[i][j].first][left[i][j].second]==' '))
                        continue;
                    if((board[left[i][k].first][left[i][k].second]==' '))
                            continue;
                int f_x= left[i][j].first; // X -cordinate
                //Find the next 'left' , if we found
                int n_x = left[i][k].first;
#ifdef DEBUG
                printf("j=%d k=%d [%d, %d] [%d, %d]\n", j, k, left[i][j].first, left[i][j].second, left[i][k].first, left[i][k].second);
#endif
                //Are they in same plane?
                if(f_x == n_x)//same plane
                {
                    //Find a hole of size
                    int f_y = left[i][j].second+1;
                    int n_y = left[i][k].second-1;
                    if(f_y>n_y)
                        swap(f_y, n_y);
                    for (int l=0; holes[f_x].size() && l<holes[f_x].size(); l++)
                        if((holes[f_x][l].first <= f_y) && (holes[f_x][l].second >= n_y))
                        {
                            ret.push_back(to_string(left[i][j].first) + " " + to_string(left[i][j].second) + " " + to_string(left[i][k].first) + " " + to_string(left[i][k].second));
                            //old = ret.size();
                            //Clear boaard
                            board[left[i][j].first][left[i][j].second] = ' ';
                            board[left[i][k].first][left[i][k].second] = ' ';
                            left[i][j].first=-1;
                            left[i][k].first=-1;

                            //Insert hole
                            holes[f_x].push_back(make_pair(left[i][j].second, left[i][j].second));
                            holes[n_x].push_back(make_pair(left[i][k].second, left[i][k].second));
                            merge_hole(f_x);
                            remove(left[i]);
                            if(count>=5)
                            {
                                //cout<<"@@@SAME@@@ size="<<ret.size()<<endl;
                                debug(board, ret);
                                //return ret;
                            }
                            break;

                        }

                }
                else //multi-plane hole finding
                {
                    int x, y;
                	if(left[i][j].second< left[i][k].second)
                		{x =left[i][j].second; y=left[i][k].second;}
                	else
                		{x =left[i][k].second; y=left[i][j].second;}

                	//handle x=y case
                	//First row [x+1,y]
                	if(x!=y)
                	{
                	bool found = true;
                	if(left[i][j].second< left[i][k].second)
                	{
                	    int h;
                	    for (h=0; h<holes[f_x].size(); h++)
                	           if((holes[f_x][h].first <= x+1) && (holes[f_x][h].second >= y))
                	               break;
                	    if(h==holes[f_x].size())
                	    {
                	        found = false;
                	        continue;
                	    }
                	}
                	else
                	{
                	    int h;
                	    for (h=0; h<holes[f_x].size(); h++)
                	    	if((holes[f_x][h].first <= x) && (holes[f_x][h].second >= y-1))
                	    		break;
                	    if(h==holes[f_x].size())
                	    {
                	    	found = false;
                	    	continue;
                	    }
                	}
                	//Last row  [x, y-1]
                	if(left[i][j].second> left[i][k].second)
                	{
                		int h;
                		for (h=0; h<holes[n_x].size(); h++)
                			if((holes[n_x][h].first <= x+1) && (holes[n_x][h].second >= y))
                				break;
                		if(h==holes[n_x].size())
                		{
                			found = false;
                			continue;
                		}
                	}
                	else
                	{
                		int h;
                		for (h=0; h<holes[n_x].size(); h++)
                			if((holes[n_x][h].first <= x) && (holes[n_x][h].second >= y-1))
                				break;
                		if(h==holes[n_x].size())
                		{
                			found = false;
                			continue;
                		}
                	}
                	}
#if 0
                	if(!(x==0 && y==0))
                		x++;y--;
                	if(x>y)
                	    swap(x, y);
#endif
                	int ii, jj;
                	for (ii=f_x+1; ii<n_x; ii++ )
                	{
                	    for (jj=0; jj<holes[ii].size(); jj++)
                	    	if((holes[ii][jj].first <= x) && (holes[ii][jj].second >= y))
                	    		break;
                	    if(jj==holes[ii].size())// not found
                	        break;
                	}
                	if(ii!=n_x)// not found
                		continue;
                	ret.push_back(to_string(left[i][j].first) + " " + to_string(left[i][j].second) + " " + to_string(left[i][k].first) + " " + to_string(left[i][k].second));
                	board[left[i][j].first][left[i][j].second] = ' ';
                	board[left[i][k].first][left[i][k].second] = ' ';
                	left[i][j].first=-1;
                	left[i][k].first=-1;

                	//Add to holes and merge
                	holes[f_x].push_back(make_pair(left[i][j].second, left[i][j].second));
                	holes[n_x].push_back(make_pair(left[i][k].second, left[i][k].second));
                	merge_hole(f_x);
                	merge_hole(n_x);
                	remove(left[i]);
                	debug(board, ret);
#ifdef DEBUG
                	if(x==y)
                	    debug(board, ret);
                	break;
#endif
                }


            }
#if 0
            if(ret.size()==48)
            {
                debug(board, ret);
                return ret;
            }
#endif
        }

        }while(old != ret.size());
        return ret;
    }
};
// -------8<------- end of solution submitted to the website -------8<-------

template<class T> void getVector(vector<T>& v) {
    for (int i = 0; i < v.size(); ++i)
        cin >> v[i];
}

int main() {
    SameColorPairs scp;
    int H;
    cin >> H;
    vector<string> board(H);
    getVector(board);

    vector<string> ret = scp.removePairs(board);
    cout << ret.size() << endl;
    for (int i = 0; i < (int)ret.size(); ++i)
        //cout <<"["<<i<<"]"<< ret[i] << endl;
        cout << ret[i] << endl;
    cout.flush();
}
