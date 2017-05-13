/*************/
/*  CAUTION  */
/* UNFINISHED*/
/*************/

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
class queue{ //キューを自作

private:
	T data[fact(N*N)*4];//定数式なのでなんと要素数にぶち込める。サイズはクソ適当なのでNがデカいと壊れる可能性は非常に高い。
	T* head = &data[0];
	T* tail = &data[0];

public:
	void push(T a) { data[tail++] = a; }
	void pop(T a) { head++; }
	T front{ return data[head]; }
};

int main() {
	
	int puzzle[(N + 2)*(N + 2)], ans[(N + 2)*(N + 2)];
	rep(i, N + 2) {
		rep(j, N + 2) {
			if (i == 0 || j == 0 || i == N - 1 || j == N - 1)
				puzzle[i*(N + 2) + j] = -1;//番兵埋め
			else cin >> puzzle[i*(N + 2) + j];//入力
		}
	}

	rep(i, N + 2) {//答えを作成
		int cnt=1;
		rep(j, N + 2) {
			if (i == 0 || j == 0 || i == N - 1 || j == N - 1)
				ans[i*(N + 2) + j] = -1;
			else if (cnt == N*N)ans[i*(N + 2) + j] = 0;
			else ans[i*(N + 2) + j] = cnt++;
		}
	}

	queue<int[(N+2)*(N+2)]> que;

	int sp;
	que.push(puzzle);
	while (true) {
		
		bool isSolved = true;//答えと比較
		for (int i = 1; i < N; i++)
			for (int j = 1; j < N; j++)
				if (que.front[i*(N + 2) + j] != ans[i*(N + 2) + j])
					isSolved = false;
		if (isSolved)
			break;
		

		sp = slidepoint(que.front);
		int y = sp / (N + 2), x = sp - y*(N + 2);



	}

}
