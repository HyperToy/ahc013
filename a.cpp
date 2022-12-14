#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define rep(i,n) for (int i = 0; i < (n); ++i)
const int dx[] = {1, 0};
const int dy[] = {0, 1};
const int dX[] = {1, 0, -1, 0};
const int dY[] = {0, 1, 0, -1};

struct UnionFind {
private:
    vector<int> par;
public:
    UnionFind(int n) {
        par.resize(n);
        for (int i = 0; i < n; i++) par[i] = i;
    }
    bool same(int x, int y) {
        return root(x) == root(y);
    }
    void unite(int x, int y) {
        x = root(x);
        y = root(y);
        if (x == y) return;
        par[x] = y;
    }
private:
    int root(int x) {
        if (par[x] == x) return x;
        else return par[x] = root(par[x]);
    }
};


int N, K;
vector<vector<int>> board;

int X;
struct mov {int a, b, c, d; };
vector<mov> movs;
int Y;
struct con {int e, f, g, h; };
vector<con> cons;

void input() {
    cin >> N >> K;
    board = vector<vector<int>>(N, vector<int>(N));
    rep(i,N) {
        string s;
        cin >> s;
        rep(j,N) board[i][j] = s[j] - '0';
    }
}

void eval() {

}

void init() {
    X = 0;
    Y = 0;
    rep(i,N) rep(j,N) {
        if (board[i][j] == 0) continue;
        rep(k,2) {
            int x = i + dx[k];
            int y = j + dy[k];
            bool connect = false;
            while (x < N && y < N) {
                if (board[x][y] < 0) break;
                if (board[x][y] != 0 && board[x][y] != board[i][j]) break;
                if (board[i][j] == board[x][y]) {
                    Y++;
                    cons.push_back(con{i, j, x, y});
                    connect = true;
                    break;
                }
                x += dx[k];
                y += dy[k];
            }
            if (connect) for (x -= dx[k], y -= dy[k]; x > i || y > j; x -= dx[k], y -= dy[k]) {
                board[x][y] = -board[i][j];
            }
        }
    }
    if (Y > K * 100) {
        cons.resize(K * 100);
        Y = K * 100;
    }
}

void simulate() {

}

void output() {
    cout << X << endl;
    for (mov m : movs) printf("%d %d %d %d\n", m.a, m.b, m.c, m.d);
    cout << Y << endl;
    for (con c : cons) printf("%d %d %d %d\n", c.e, c.f, c.g, c.h);
}

void solve() {
    input();
    init();
    // simulate();
    output();
}

int main(){
    solve();
} 