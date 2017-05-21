#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <unordered_set>
using namespace std;

typedef unsigned long long uint64;

struct Node {
    int parent;
    char operation;

    Node() {}
    Node(int p, char o) : parent(p), operation(o) {}
};

class Solver {
public:
    Solver(int b[]) {
        answer = Node(-1, 0);
        memcpy(initial_board, b, sizeof(initial_board));

        for (int i = 0; i < 9; i++) {
            if (initial_board[i] == 0) initial_op = i;
        }
    }

    int direction(char dir) {
        switch (dir) {
            case 'u': return -3;
            case 'd': return  3;
            case 'l': return -1;
            case 'r': return  1;
        };
        return 0;
    }

    // 盤面の復元
    void restore(Node node) {
        string operation;

        op = initial_op;
        memcpy(board, initial_board, sizeof(initial_board));
        while (node.parent != -1) {
            operation += node.operation;
            node = visited_nodes[node.parent];
        }

        reverse(operation.begin(), operation.end());

        for (int i = 0; i < operation.size(); i++) {
            int _op = op + direction(operation[i]);
            swap(board[op], board[_op]);
            op = _op;
        }
    }

    // 既に訪れたことのある盤面か調べる
    bool checkVisited() {
        uint64 b = 0;
        for (int i = 0; i < 9; i++) {
            b |= uint64(board[i] << (i * 4));
        }
        bool flag = visited.find(b) != visited.end();
        if (!flag) visited.insert(b);
        return flag;
    }

    // 実行部
    void run() {
        search_count = 0;
        queue<Node> que;
        que.push(Node(-1, 0));

        while (!que.empty()) {
            Node node = que.front();
            que.pop();

            restore(node);
            if (checkVisited()) continue;

            search_count++;

            // ゴールか調べる
            if (equal(begin(board), end(board), begin(goal))) {
                answer = node;
                break;
            }

            visited_nodes.push_back(node);

            // 展開
            if ((op % 3) >= 1) {
                Node l = Node(visited_nodes.size() - 1, 'l');
                que.push(l);
            }
            if ((op % 3) <= 1) {
                Node r = Node(visited_nodes.size() - 1, 'r');
                que.push(r);
            }
            if ((op / 3) >= 1) {
                Node u = Node(visited_nodes.size() - 1, 'u');
                que.push(u);
            }
            if ((op / 3) <= 1) {
                Node d = Node(visited_nodes.size() - 1, 'd');
                que.push(d);
            }
        }
    }

    void printBoard(int b[]) {
        for (int i = 0; i < 9; i++) {
            cout << b[i];
            if (i % 3 == 2) cout << endl;
        }
        cout << endl;
    }

    void result() {
        Node node = answer;
        string operation;
        op = initial_op;
        memcpy(board, initial_board, sizeof(initial_board));
        while (node.parent != -1) {
            operation += node.operation;
            node = visited_nodes[node.parent];
        }
        reverse(operation.begin(), operation.end());

        printBoard(initial_board);
        for (int i = 0; i < operation.size(); i++) {
            int _op = op + direction(operation[i]);
            swap(board[op], board[_op]);
            op = _op;
            printBoard(board);
        }

        cout << "number of search nodes: " << search_count << endl;
        cout << "number of moves       : " << operation.size() << endl;
        cout << "direction             : " << operation << endl;
    }

private:
    Node answer;
    int board[9];
    int initial_board[9];
    int goal[9] = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    int op;
    int initial_op;
    int search_count;
    vector<Node> visited_nodes;
    unordered_set<uint64> visited;
};

int main(void) {
    int board[9] = {8, 6, 7, 2, 5, 4, 3, 0, 1};
    Solver solver(board);

    solver.run();
    solver.result();

    return 0;
}
