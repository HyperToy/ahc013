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
    UnionFind() :parent() {}

    pair<int, int> find(pair<int, int> x) {
        if (parent.find(x) == parent.end()) {
            parent[x] = x;
            return x;
        } else if (parent[x] == x) {
            return x;
        } else {
            parent[x] = find(parent[x]);
            return parent[x];
        }
    }

    void unite(pair<int, int> x, pair<int, int> y) {
        x = find(x);
        y = find(y);
        if (x != y) {
            parent[x] = y;
        }
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

struct Result {
    // int N;
    vector<vector<int>> field;
    vector<MoveAction> move;
    vector<ConnectAction> connect;
    int score;
    Result(int N, vector<vector<int>> field, vector<MoveAction> move, vector<ConnectAction> con) : field(field), move(move), connect(con) {
        score = calc_score(N);
    }
    int calc_score(int N) {
        // for (MoveAction m : move) {
        //     assert(field[m.before_row][m.before_col] > 0);
        //     assert(field[m.after_row][m.after_col] == 0);
        //     swap(field[m.before_row][m.before_col], field[m.after_row][m.after_col]);
        // }
        UnionFind uf;
        for (ConnectAction c : connect) {
            pair<int, int> p1(c.c1_row, c.c1_col), p2(c.c2_row, c.c2_col);
            uf.unite(p1, p2);
        }

        vector<pair<int, int>> computers;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (field[i][j] <= 0) continue;
                computers.emplace_back(i, j);
            }
        }

        int score = 0;
        for (int i = 0; i < (int)computers.size(); i++) {
            for (int j = 0; j < i; j++) {
                pair<int, int> c1 = computers[i];
                pair<int, int> c2 = computers[j];
                if (uf.find(c1) == uf.find(c2)) {
                    score += (field[c1.first][c1.second] == field[c2.first][c2.second]) ? 1 : -1;
                }
            }
        }

        return max(score, 0);
    }
};
// struct State {
//     vector<MoveAction> moves;
//     vector<vector<int>> field;
//     int score;
//     State (vector<MoveAction> moves, vector<vector<int>> field) : moves(moves), field(field) {}
// }

bool comp(Result &s, Result &t) {
    return s.score > t.score;
}

// int N, int K, const vector<vector<int>> &field
struct Solver {
    static constexpr int dx[4] = {1, 0, -1, 0};
    static constexpr int dy[4] = {0, 1, 0, -1};

    int N, K;
    int action_count_limit;
    vector<vector<int>> field;
    vector<vector<int>> connected;

    vector<MoveAction> moves;
    vector<ConnectAction> connects;

    random_device rd;
    mt19937 engine{rd()};

    Solver(int N, int K, vector<vector<int>> field) :
        N(N), K(K), action_count_limit(K * 100), field(field) 
    {
        connected.resize(N, vector<int>(N, 0));
    }
    Solver(int N, int K, vector<vector<int>> field, Result &res) : 
        N(N), K(K), action_count_limit(K * 100), field(field), moves(res.move), connects(res.connect) 
    {
        connected.resize(N, vector<int>(N, 0));
    }

    bool inside (int &x, int &y) {
        return 0 <= x && x < N && 0 <= y && y < N;
    }

    bool can_move(int &x, int &y, int &dir) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        if (inside(nx, ny)) {
            return field[nx][ny] == 0 || field[nx][ny] == -field[x][y];
        }
        return false;
    }
    void move(int move_limit = -1) {
        if (move_limit == -1) {
            move_limit = K * 50;
        }

        int move_cnt = 0;
        while (move_cnt < move_limit) {
            int x = engine() % N;
            int y = engine() % N;
            int dir = engine() % 4;
            if (field[x][y] > 0 && can_move(x, y, dir)) {
                int nx = x + dx[dir];
                int ny = y + dy[dir];
                swap(field[x][y], field[nx][ny]);
                if (field[x][y] < 0) {
                    field[x][y] = 0;
                }
                moves.emplace_back(x, y, nx, ny);
                action_count_limit--;
                move_cnt++;
            }
        }
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
                connected[x][y]++;
                connected[nx][ny]++;
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
                            return;
                        }
                    }
                }
            }
        }
    }

    bool search_line(int &x, int &y, int &dir) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        while (inside(nx, ny)) {
            if (field[nx][ny] == -field[x][y]) {
                return true;
            } else if (field[nx][ny] != 0) {
                return false;
            }
            nx += dx[dir];
            ny += dy[dir];
        }
        return false;
    }
    vector<MoveAction> move_to(int &x, int &y, int &dir) {
        vector<MoveAction> moves;
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        while (inside(nx, ny)) {
            if (field[nx][ny] == -field[x][y]) {
                field[nx][ny] = field[x][y];
                field[x][y] = 0;
                moves.emplace_back(nx - dx[dir], ny - dy[dir], nx, ny);
                action_count_limit--;
                return moves;
            }
            assert(field[nx][ny] == 0);
            moves.emplace_back(nx - dx[dir], ny - dy[dir], nx, ny);
            action_count_limit--;
            nx += dx[dir];
            ny += dy[dir];
        }
        assert(false);
    }

    void reset_field() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                connected[i][j] = 0;
                if (field[i][j] < 0) {
                    field[i][j] = 0;
                }
            }
        }
    }

    Result solve() {
        // vector<MoveAction> moves;
        move();
        // vector<ConnectAction> connects;
        connect();

        return Result(N, field, moves, connects);
    }
    Result init() {
        // vector<MoveAction> moves;
        // move(moves);
        // vector<ConnectAction> connects;
        connect();

        return Result(N, field, moves, connects);
    }

    Result next_state (int move_limit = -1) {
        if (move_limit == -1) {
            move_limit = 1;
        }
        move(move_limit);
        reset_field();
        connect();
        return Result(N, field, moves, connects);
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

int calc_score(int N, vector<vector<int>> field, const Result &res) {
    for (MoveAction m : res.move) {
        assert(field[m.before_row][m.before_col] > 0);
        assert(field[m.after_row][m.after_col] == 0);
        swap(field[m.before_row][m.before_col], field[m.after_row][m.after_col]);
    }
    UnionFind uf;
    for (ConnectAction c : res.connect) {
        pair<int, int> p1(c.c1_row, c.c1_col), p2(c.c2_row, c.c2_col);
        uf.unite(p1, p2);
    }

    vector<pair<int, int>> computers;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] <= 0) continue;
            computers.emplace_back(i, j);
        }
    }

    int score = 0;
    for (int i = 0; i < (int)computers.size(); i++) {
        for (int j = 0; j < i; j++) {
            pair<int, int> c1 = computers[i];
            pair<int, int> c2 = computers[j];
            if (uf.find(c1) == uf.find(c2)) {
                score += (field[c1.first][c1.second] == field[c2.first][c2.second]) ? 1 : -1;
            }
        }
    }

    return max(score, 0);
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

bool check (int &N, vector<vector<int>> &f, vector<vector<int>> &g) {
    for (int i = 0; i < N; i ++) {
        for (int j = 0; j < N; j++) {
            if (f[i][j] != g[i][j]) {
                return false;
            }
        }
    }
    return true;
}
void solve() {
    int N, K;
    vector<vector<int>> field;
    input(N, K, field);

    vector<vector<int>> FIELD(N, vector<int>(N));
    for (int i = 0; i < N; i ++) {
        for (int j = 0; j < N; j++) {
            FIELD[i][j] = field[i][j];
        }
    }

    Timer timer;
    vector<Result> beam;
    Solver s(N, K, field);
    beam.push_back(s.init());

    PR(check(N, field, FIELD));
    int loop_count = 0;
    int update_count = 0;
    while (timer.get() < TIME_LIMIT) {
        PR(timer.get());
        loop_count++;
        vector<Result> current_beam;
        for (Result res : beam) {
            for (int i = 0; i < 3; i ++) {
                Solver t(N, K, res.field, res);
                current_beam.push_back(t.next_state());
            }
        }
    PR(check(N, field, FIELD));
        sort(current_beam.begin(), current_beam.end(), comp);
        vector<Result> next_beam;
        for (int i = 0; i < min(5, (int)current_beam.size()); i++) {
            next_beam.push_back(current_beam[i]);
        }
        beam = next_beam;
        PR(beam[0].score);
    }

    // output(beam[0]);
    // init();
    // simulate();
    PR(check(N, field, FIELD));

    cerr << "Score  : " << calc_score(N, field, beam[0]) << endl;
    cerr << "Loop   : " << loop_count << endl;
}

int main(){
    solve();
} 