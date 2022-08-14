#include <bits/stdc++.h>
using namespace std;
#define PR(x) cerr << #x << " = " << x << endl

const double TIME_LIMIT = 2.9;
struct Timer {
    clock_t start;
    Timer() { reset(); }
    void reset() { start = clock(); }
    double get() { return (double)(clock() - start) / CLOCKS_PER_SEC; }
};

struct UnionFind {
    map<pair<int,int>, pair<int, int>> parent;
    map<pair<int,int>, int> size;
    UnionFind() :parent() {}

    pair<int, int> find(pair<int, int> x) {
        if (parent.find(x) == parent.end()) {
            parent[x] = x;
            size[x] = 1;
            return x;
        } else if (parent[x] == x) {
            return x;
        } else {
            parent[x] = find(parent[x]);
            return parent[x];
        }
    }

    void unite(pair<int, int> x, pair<int, int> y) {
        if (!same(x, y)) {
            parent[x] = y;
            size[y] += size[x];
        }
    }

    bool same(pair<int, int> x, pair<int, int> y) {
        return find(x) == find(y);
    }
};

struct MoveAction {
    int before_row, before_col, after_row, after_col;
    MoveAction(int before_row, int before_col, int after_row, int after_col) : 
        before_row(before_row), before_col(before_col), after_row(after_row), after_col(after_col) {}
};
struct ConnectAction {
    int c1_row, c1_col, c2_row, c2_col;
    ConnectAction(int c1_row, int c1_col, int c2_row, int c2_col) : 
        c1_row(c1_row), c1_col(c1_col), c2_row(c2_row), c2_col(c2_col) {}
};

struct Solver {
    static constexpr int dx[4] = {1, 0, -1, 0};
    static constexpr int dy[4] = {0, 1, 0, -1};

    int N, K;
    int action_count_limit;
    vector<vector<int>> field;
    vector<MoveAction> moves;
    vector<ConnectAction> connects;

    int score;
    UnionFind uf;

    random_device rd;
    mt19937 engine{rd()};

    Solver(int N, int K, vector<vector<int>> field) :
        N(N), K(K), action_count_limit(K * 100), field(field) 
    {
        // uf = UnionFind(N * N);
        score = 0;
        connect();
        PR(score);
    }
    
    bool inside (int &x, int &y) {
        return 0 <= x && x < N && 0 <= y && y < N;
    }

    bool can_connect(int &x, int &y, int &dir) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        while (inside(nx, ny)) {
            if (field[nx][ny] == field[x][y]) {
                if (uf.same(pair(x, y), pair(nx, ny))) {
                    return false;
                } else {
                    return true;
                }
            } else if (field[nx][ny] != 0) {
                return false;
            }
            nx += dx[dir];
            ny += dy[dir];
        }
        return false;
    }
    ConnectAction line_fill(int &x, int &y, int &dir) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        while (inside(nx, ny)) {
            if (field[nx][ny] == field[x][y]) {
                assert(!uf.same({x, y}, {nx, ny}));
                score += uf.size[uf.find({x, y})] * uf.size[uf.find({nx, ny})];
                uf.unite({x, y}, {nx, ny});
                return ConnectAction(x, y, nx, ny);
            }
            assert(field[nx][ny] == 0);
            field[nx][ny] = -field[x][y];
            nx += dx[dir];
            ny += dy[dir];
        }
        assert(false);
    }
    void connect() {
        for (int k = 1; k <= K; k++) {
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    if (field[i][j] != k) {
                        continue;
                    }
                    for (int dir = 0; dir < 2; dir++) { // ↓ と → の2方向を見る
                        if (can_connect(i, j, dir)) {
                            connects.push_back(line_fill(i, j, dir));
                            action_count_limit--;
                            if (action_count_limit <= 0) {
                                return;
                            }
                        }
                    }
                }
            }
        }
    }

    int solve() {
        return score;
    }
};

void input(int &N, int &K, vector<vector<int>> &field) {
    cin >> N >> K;
    field.resize(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        string s;
        cin >> s;
        for (int j = 0; j < N; j++) {
            field[i][j] = s[j] - '0';
        }
    }
}

void print_answer(Solver &sol) {
    cout << sol.moves.size() << endl;
    for (MoveAction m : sol.moves) {
        cout << m.before_row << " " << m.before_col << " "
            << m.after_row << " " << m.after_col << endl;
    }
    cout << sol.connects.size() << endl;
    for (ConnectAction c : sol.connects) {
        cout << c.c1_row << " " << c.c1_col << " "
            << c.c2_row << " " << c.c2_col << endl;
    }
}

void solve() {
    int N, K;
    vector<vector<int>> field;
    input(N, K, field);

    Solver s(N, K, field);

    Timer timer;

    // while (timer.get() < TIME_LIMIT) {

    // }

    print_answer(s);
    PR(s.score);
}

int main(){
    solve();
} 