

#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <string>
#include <utility>

#define rep(i,n) for (int i=0 ; i<n;i++)
#define y que.front().first
#define x que.front().second 
using namespace std;

pair<int, int> returnStart(const vector<vector<char> > str,int h,int w) {
	rep(i, h)
		rep(j, w)
		if (str[i][j] == 'S') return pair<int,int>(i, j);
}

int main() {
	int h, w;
	cin >> h >> w; //縦横のサイズを入力
	h += 2; w += 2;//番兵
	vector<vector<pair<int, int> > > from(h, vector<pair<int, int> >(w)); //.f=y .s=x
	vector<pair<int, int> >restore; restore.reserve(h*w);
	vector<vector<bool> > flag(h, vector<bool>(w)); //visited is false 
	rep(i, h)
		rep(j, w)
		flag[i][j] = true;

	queue<pair<int, int> >que;

	vector<vector<char>> grid;
	grid.resize(h);	rep(i, h)	grid[i].resize(w); //h*wのグリッド

	vector<string> grid_tmp(h-2,string(w-2,'*'));

	cin.get(); //縦 (スペース) 横 (改行) 迷路のデータ の入力に対応させる(横の後の改行を一文字とするのを防ぐ)、キモい。

	rep(i, h-2) 
		getline(cin, grid_tmp[i]); //cinではスペースの入力を受け付けられない。
	
	rep(i, h)
		rep(j, w) 
		if (i == 0 || j == 0 || i == h - 1 || j == w - 1) //端の番兵に'*'を配置
			grid[i][j] = '*';
		else
			grid[i][j] = grid_tmp[i-1][j-1]; //端でなければコピー
	
	pair<int, int> start = returnStart(grid, h, w);
	que.push(start);
	from[y][x] = start;
	flag[y][x] = false;

	while (true) { //迷路解く
	
		//cout << y << "," << x<<endl;
		

		if (grid[y - 1][x] != '*' && flag[y - 1][x]) {
			from[y - 1][x] = que.front();
			flag[y-1][x] = false;
			que.push(pair<int, int>(y - 1, x));	
		}
			
		if (grid[y][x + 1] != '*'&& flag[y][x + 1]) {
			from[y][x+1] = que.front();
			flag[y][x+1] = false;
			que.push(pair<int, int>(y, x + 1));

		}
			
		if (grid[y][x - 1] != '*'&& flag[y][x - 1]) {
			from[y][x-1] = que.front();
			flag[y][x-1] = false;
			que.push(pair<int, int>(y, x - 1));
	
		}
		if (grid[y + 1][x] != '*'&& flag[y + 1][x]) {
			from[y + 1][x] = que.front();
			flag[y+1][x] = false;
			que.push(pair<int, int>(y + 1, x));
		}
		if (grid[y][x] == 'G')
			break;
		que.pop();

	}


	int l = y, r = x;

	while (true) {
		restore.push_back(from[l][r]);
		int tmp = l;
		l = from[l][r].first;
		r = from[tmp][r].second;
		if (from[l][r] == start)
			break;
	}

	restore.push_back(from[l][r]); //ここもキモい。

	for (auto itr = restore.rbegin(); itr != restore.rend(); ++itr)
		cout << "(" << (*itr).first << "," << (*itr).second << ")" << endl;

	cout << "(" << y << "," << x << ")" << endl;//ここもキモい。
}
