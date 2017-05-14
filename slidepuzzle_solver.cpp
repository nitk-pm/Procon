/*************|
|*  CAUTION  *|
|* UNFINISHED*|
|*************/

#include <iostream>
#include <algorithm>
//#include <queue>
#include <cmath>
#include <vector>
#include <string>
#include <utility>

#define rep(i,n) for (int i=0 ; i<n;i++)
#define N 3//(N^2)-1パズル
using namespace std;

int slidepoint(int p[(N + 2)*(N + 2)]) {//動かせる座標を探す。
	for (int i = 1; i < N; i++)
		for (int j = 1; j < N; j++)
			if (p[i*(N + 2) + j] == 0)
				return i*(N+2) + j;
}

constexpr int fact(int n) {return ((n > 1) ? n*fact(n - 1) : n);}//中三女子

template<typename T>
class queue { //キューを自作

private:
	T data[fact(N*N) * 4];//定数式なのでなんと要素数にぶち込める。サイズはクソ適当なのでNがデカいと壊れる可能性は非常に高い。
	T* head = &data[0];
	T* tail = &data[0];

public:
	void push(T a) { data[tail++] = a; }
	void pop() { head++; }
	T front{ return data[head]; }
};

struct Node {
	char oparetion; //どういう操作で今の状態になったか
	Node* parent; //操作前の状態
	int puzzle[(N + 2)*(N + 2)]; //現在の盤面
	int sp;//動かせる座標
};

int main() {
	bool isSolved;
	Node initial;
	rep(i, N + 2) {
		rep(j, N + 2) {
			if (i == 0 || j == 0 || i == N - 1 || j == N - 1)
				initial.puzzle[i*(N + 2) + j] = -1;//番兵埋め
			else cin >> initial.puzzle[i*(N + 2) + j];//入力
		}
	}

	initial.oparetion = -1;
	initial.sp = slidepoint(initial.puzzle);

	int ans[(N + 2)*(N + 2)];
	rep(i, N + 2) {//答えを作成
		int cnt=1;
		rep(j, N + 2) {
			if (i == 0 || j == 0 || i == N - 1 || j == N - 1)
				ans[i*(N + 2) + j] = -1;
			else if (cnt == N*N)ans[i*(N + 2) + j] = 0;
			else ans[i*(N + 2) + j] = cnt++;
		}
	}

	queue<Node> que;

	int sp;
	que.push(initial);
	while (true) {

		isSolved = true;//答えと比較
		for (int i = 1; i < N; i++)
			for (int j = 1; j < N; j++)
				if (que.front.puzzle[i*(N + 2) + j] != ans[i*(N + 2) + j])
					isSolved = false;
		if (isSolved)
			break;


		sp = que.front.sp;
		int y = sp / (N + 2), x = sp - y*(N + 2);

		/*
		0:↑
		1:→
		2:↓
		3:←
		*/

		{
			Node tmp;
			tmp.parent = &que.front;
			rep(i, N + 2)
				rep(j, N + 2)
				tmp.puzzle[i*(N + 2) + j] = que.front.puzzle[i*(N + 2) + j];

			if (que.front.puzzle[(y - 1)*(N + 2) + (x)] != -1 || que.front.oparetion == 2) {
				tmp.oparetion = 0;
				swap(tmp.puzzle[(y - 1)*(N + 2) + (x)], tmp.puzzle[y*(N + 2) + x]);//シュッ
				que.push(tmp);
				swap(tmp.puzzle[(y - 1)*(N + 2) + (x)], tmp.puzzle[y*(N + 2) + x]);//元に戻して再利用
			}

			if (que.front.puzzle[(y)*(N + 2) + (x + 1)] != -1 || que.front.oparetion == 3) {
				tmp.oparetion = 1;
				swap(tmp.puzzle[(y)*(N + 2) + (x + 1)], tmp.puzzle[y*(N + 2) + x]);
				que.push(tmp);
				swap(tmp.puzzle[(y)*(N + 2) + (x + 1)], tmp.puzzle[y*(N + 2) + x]);
			}

			if (que.front.puzzle[(y + 1)*(N + 2) + (x)] != -1 || que.front.oparetion == 0) {
				tmp.oparetion = 2;
				swap(tmp.puzzle[(y + 1)*(N + 2) + (x)], tmp.puzzle[y*(N + 2) + x]);
				que.push(tmp);
				swap(tmp.puzzle[(y + 1)*(N + 2) + (x)], tmp.puzzle[y*(N + 2) + x]);
			}

			if (que.front.puzzle[(y)*(N + 2) + (x - 1)] != -1 || que.front.oparetion == 1) {
				tmp.oparetion = 3;
				swap(tmp.puzzle[(y)*(N + 2) + (x - 1)], tmp.puzzle[y*(N + 2) + x]);
				que.push(tmp);
				swap(tmp.puzzle[(y)*(N + 2) + (x - 1)], tmp.puzzle[y*(N + 2) + x]);
			}

		}
		que.pop();
	}
	vector<Node> restore;
		{
			Node res_tmp = que.front;

			while (res_tmp.oparetion != -1)
			restore.push_back(res_tmp);
			res_tmp = *res_tmp.parent;
		}

		//reverse(restore.begin(),restore.end());
		for (auto itr = restore.rbegin(); itr != restore.rend(); ++itr)
			switch (itr->oparetion) {
			case 0:cout << "↑" << endl; break;
			case 1:cout << "→" << endl; break;
			case 2:cout << "↓" << endl; break;
			case 3:cout << "←" << endl; break;
			default: break;
			}
	}
