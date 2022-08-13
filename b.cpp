#include <bits/stdc++.h>
using namespace std;

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

struct Result {
    vector<MoveAction> move;
    vector<ConnectAction> connect;
    Result(const vector<MoveAction> &move, const vector<ConnectAction> &con) : move(move), connect(con) {}
};

struct Solver {
    static constexpr char USED = 'x';
    static constexpr int dx[4] = {1, 0, -1, 0};
    static constexpr int dy[4] = {0, 1, 0, -1};

    int N, K;
    int action_count_limit;
    // mt19937 engine;
    vector<vector<int>> field;

    Solver(int N, int K, const vector<vector<int>> &field, int seed = 0) :
        N(N), K(K), action_count_limit(K * 100), field(field) 
    {
        // engine.seed(seed);
    }

    vector<MoveAction> move(int move_limit = -1) {
        vector<MoveAction> moves;
        if (move_limit == -1) move_limit = K * 50;

        // for (int i = 0; i < move_limit; i++) {

        // }

        return moves;
    }

    bool inside (int &x, int &y) {
        return 0 <= x && x < N && 0 <= y && y < N;
    }

    bool can_connect(int &x, int &y, int &dir) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        while (inside(nx, ny)) {
            if (field[nx][ny] == field[x][y]) {
                return true;
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
                return ConnectAction(x, y, nx, ny);
            }
            assert(field[nx][ny] == 0);
            field[nx][ny] = -field[x][y];
            nx += dx[dir];
            ny += dy[dir];
        }
        assert(false);
    }

    vector<ConnectAction> connect() {
        vector<ConnectAction> connects;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (field[i][j] <= 0) {
                    continue;
                }
                for (int dir = 0; dir < 2; dir++) { // ↓ と → の2方向を見る
                    if (can_connect(i, j, dir)) {
                        connects.push_back(line_fill(i, j, dir));
                        action_count_limit--;
                        if (action_count_limit <= 0) {
                            return connects;
                        }
                    }
                }
            }
        }
        return connects;
    }

    Result solve() {
        vector<MoveAction> moves = move();
        vector<ConnectAction> connexts = connect();
        return Result(moves, connexts);
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

void eval() {
}

void init() {
}

void simulate() {
}

void output(const Result &res) {
    cout << res.move.size() << endl;
    for (MoveAction m : res.move) {
        cout << m.before_row << " " << m.before_col << " "
            << m.after_row << " " << m.after_col << endl;
    }
    cout << res.connect.size() << endl;
    for (ConnectAction c : res.connect) {
        cout << c.c1_row << " " << c.c1_col << " "
            << c.c2_row << " " << c.c2_col << endl;
    }
}

void solve() {
    int N, K;
    vector<vector<int>> field;
    input(N, K, field);

    Solver s(N, K, field);
    Result res = s.solve();
    // init();
    // simulate();
    output(res);
}

int main(){
    solve();
} 