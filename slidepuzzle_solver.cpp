
#include <iostream>
#include <algorithm>
//#include <queue>
#include <cmath>
#include <vector>
#include <string>
#include <utility>
#include <set>

#define rep(i,n) for (int i=0 ; i<n;i++)
#define N 3//(N^2)-1パズル
using namespace std;

typedef unsigned long long uint64;

constexpr int fact(int n) { return ((n > 1) ? n*fact(n - 1) : n); }//中三女子
constexpr int sq(int n) { return n*n; }

int slidepoint(int p[sq(N)]) {//動かせる座標を探す。
	rep(i, sq(N))
		if (p[i] == 0)
			return i;
	return -1;
}
template<typename T>
class queue { //キュー

private:
	T data[fact(sq(N))/4];
	int head = 0;
	int tail = 0;
	
	~queue() {
		delete [] data;
	}

public:
	void push(T a) { data[tail++] = a; }
	void pop() { head++; }
	T front(){ return data[head]; }
	bool empty(){ return head == tail ? true : false; }
	
};

struct Node {
	char operation;
	int parentIndex;
};
uint64 tohash(int b[]) {
	uint64 tmp=0;
	rep(i, sq(N))
		tmp |= b[i] << (i*4);
	return tmp;
}

int direction(char c) {
	switch (c) {

	/*
	0：→
	1：↓
	2：←
	3：↑
	*/

	case 0: return 1;
	case 1: return 3;
	case 2: return -1;
	case 3: return -3;
	default:return 0;
	
	
	}
}
void printBoard(int b[]) {
	rep(i, sq(N)) {
		cout << b[i];
		if (i % 3 == 2)cout << endl;
	}
	cout << endl;
}

int main() {
	
	int initial_board[] = { 2,5,3,0,1,6,4,7,8 };//2,5,3,0,1,6,4,7,8
	int answer_board[] = { 1,2,3,4,5,6,7,8,0 };
	uint64 answer_hash = tohash(answer_board);
	Node answer;
	int initial_sp=slidepoint(initial_board);
	Node initial={ -1,-1};
	set<uint64>visited;
	vector<Node>usednode;
	queue<Node> que;
	que.push(initial);

	while (!que.empty()) {/*変更点　:毎回、最初の状態から現時点までの盤面の復元を行う。*/
		Node current = que.front();
		que.pop();

		vector<char>op;
		int sp = initial_sp;
		Node cur_node = current;
		int board[sq(N)];
		memcpy(board, initial_board, sizeof(initial_board));

		while (cur_node.operation!=-1) {
			op.push_back(cur_node.operation);
			cur_node = usednode[cur_node.parentIndex];

		}

		reverse(op.begin(), op.end());
		rep(i,op.size()) {
			int _sp = sp + direction(op[i]);
			swap(board[sp], board[_sp]);
			sp = _sp;
		}

		usednode.push_back(current);

		uint64 nowhash = tohash(board);

		if (nowhash == answer_hash) {
			answer = current;
			break;
		}


		if (visited.find(nowhash) == visited.end())
			visited.insert(nowhash);
		else continue;

		if (sp % 3 <= 1) {//左から1または2番目
			Node tmp = { 0,usednode.size() - 1 };
			que.push(tmp);
		}
		if (sp / 3 <= 1) {//上から1または2番目
			Node tmp = { 1,usednode.size() - 1 };
			que.push(tmp);
			}
		if (sp % 3 >= 1) {//左から2または3番目
			Node tmp = { 2,usednode.size() - 1 };
			que.push(tmp);

		}
		if (sp / 3 >= 1) {
			Node tmp = { 3,usednode.size() - 1 };
			que.push(tmp);

		}
	}

	vector<char>op;
	int sp = initial_sp;
	Node cur_node = answer;
	int board[sq(N)];
	memcpy(board, initial_board, sizeof(initial_board));
	
	while (cur_node.operation != -1) {
		op.push_back(cur_node.operation);
		cur_node = usednode[cur_node.parentIndex];
	}
	printBoard(board);

	for (auto itr = op.rbegin(); itr != op.rend(); ++itr) {
		int _sp = sp + direction(*itr);
		swap(board[sp], board[_sp]);
		sp = _sp;
		printBoard(board);
	}
	
}
