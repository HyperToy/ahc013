/**
 * 1種類だけを格子状に集めようとしてボツ
 */
#include <bits/stdc++.h>
using namespace std;
#define PR(x) cerr << #x << " = " << x << endl

const double TIME_LIMIT = 2.85;
struct Timer {
    clock_t start;
    Timer() { reset(); }
    void reset() { start = clock(); }
    double get() { return (double)(clock() - start) / CLOCKS_PER_SEC; }
};
Timer timer;

struct UnionFind {
    int h, w;
    int N;
    vector<int> parent;
    vector<int> size;
    UnionFind(int h, int w) : h(h), w(w) {
        N = h * w;
        parent.resize(N);
        size.resize(N);
        for (int i = 0; i < N; i++) {
            parent[i] = i;
            size[i] = 1;
        }
    }

    int find(int x) {
        if (parent[x] == x) {
            return x;
        } else {
            parent[x] = find(parent[x]);
            return parent[x];
        }
    }
    pair<int, int> find(pair<int, int> p) {
        int x = p.first * w + p.second;
        x = find(x);
        return {x / w, x % w};
    }

    void unite(int x, int y) {
        if (same(x, y)) return;
        x = find(x);
        y = find(y);
        parent[x] = y;
        size[y] += size[x];
    }
    void unite(pair<int, int> p, pair<int, int> q) {
        int x = p.first * w + p.second;
        int y = q.first * w + q.second;
        unite(x, y);
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }
    bool same(pair<int, int> p, pair<int, int> q) {
        int x = p.first * w + p.second;
        int y = q.first * w + q.second;
        return same(x, y);
    }

    int get_size(int x) {
        return size[find(x)];
    }
    int get_size(pair<int, int> p) {
        int x = p.first * w + p.second;
        return get_size(x);
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
    vector<MoveAction> moves;
    vector<ConnectAction> connects;
    int score;
    Result(int score, const vector<MoveAction> &move, const vector<ConnectAction> &con) : score(score), moves(move), connects(con) {}
};
struct Solver {
    static constexpr int dx[4] = {1, 0, -1, 0};
    static constexpr int dy[4] = {0, 1, 0, -1};
    vector<int> random_dir = {0, 1, 2, 3};

    int N, K;
    int action_count_limit;
    vector<vector<int>> field;
    vector<MoveAction> moves;
    vector<ConnectAction> connects;
    vector<vector<pair<int, int>>> computers;
    vector<vector<int>> index;

    int score;
    UnionFind uf;

    int p_score;
    int n_score;
    vector<vector<int>> p_map;
    vector<vector<int>> n_map;

    random_device rd;
    mt19937 engine{rd()};

    Solver(int N, int K, vector<vector<int>> field, vector<vector<int>> index, vector<vector<pair<int, int>>> computers) :
        N(N), K(K), action_count_limit(K * 100), field(field), index(index), computers(computers), uf(UnionFind(N, N))
    {
        score = 0;
        p_score = 0;
        n_score = 0;
        p_map.resize(N, vector<int>(N));
        n_map.resize(N, vector<int>(N));
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (i % K == 0 || j % K == 0) {
                    p_map[i][j] = K * 4 - (min(j % K, K - j % K) + min(i % K, K - i % K)) * 2; 
                    n_map[i][j] = K * 2 - (min(j % K, K - j % K) + min(i % K, K - i % K)); 
                } else {
                    p_map[i][j] = K - (min(i % K, K - i % K) + min(j % K, K - j % K));
                    n_map[i][j] = (K - (min(i % K, K - i % K) + min(j % K, K - j % K))) * 2;
                }
            }
        }

        for (int k = 0; k < K; k++) {
            for (int l = 0; l < 100; l++) {
                int x = computers[k][l].first;
                int y = computers[k][l].second;
                if (k == 0) {
                    p_score += p_map[x][y];
                } else {
                    n_score += n_map[x][y]; // > 0
                }
            }
        }
        // move(K * 5);
        // connect();
    }

    bool inside (int &x, int &y) {
        return 0 <= x && x < N && 0 <= y && y < N;
    }

    bool can_move(int &x, int &y, int &dir, bool is_grid_cal) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        if (inside(nx, ny)) {
            bool togo = false;
            if (field[x][y] == 1) {
                togo = p_map[x][y] < p_map[nx][ny];
            } else {
                togo = n_map[x][y] > n_map[nx][ny];
            }
            togo = togo || ((int)engine() % 1000 < 400);
            if (!togo) {
                return false;
            }
            return field[nx][ny] == 0 || field[nx][ny] == -field[x][y];
        }
        return false;
    }
    void move(int move_limit = -1) {
        if (move_limit < 0) {
            move_limit = K * 50;
        }
        int move_cnt = 0;
        while (move_cnt < move_limit) {
            int k = engine() % K;
            int l = engine() % 100;
            int x = computers[k][l].first;
            int y = computers[k][l].second;
            shuffle(random_dir.begin(), random_dir.end(), engine);
            for (int dir = 0; dir < 4; dir++) {
                if (field[x][y] > 0 && can_move(x, y, random_dir[dir])) {
                    int nx = x + dx[random_dir[dir]];
                    int ny = y + dy[random_dir[dir]];
                    swap(field[x][y], field[nx][ny]);
                    swap(index[x][y], index[nx][ny]);
                    computers[k][l] = {nx, ny};
                    field[x][y] = max(field[x][y], 0); // 移動元がマイナス（ケーブル）だった場合は 0にする
                    moves.emplace_back(x, y, nx, ny);
                    action_count_limit--;
                    move_cnt++;

                    if (k == 0) {
                        p_score -= p_map[x][y];
                        p_score += p_map[nx][ny];
                    } else {
                        n_score -= n_map[x][y];
                        n_score += n_map[nx][ny];
                    }
                    break;
                }
            }
        }
    }

    bool can_connect(int &x, int &y, int &dir) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        while (inside(nx, ny)) {
            if (field[nx][ny] == field[x][y]) {
                if (uf.same({x, y}, {nx, ny})) {
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
                score += uf.get_size({x, y}) * uf.get_size({nx, ny});
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
        vector<int> perm(100);
        for (int i = 0; i < 100; i++) {
            perm[i] = i;
        }
        for (int k = 0; k < K; k++) {
            shuffle(perm.begin(), perm.end(), engine);
            for (int i = 0; i < 100; i++) {
                int x = computers[k][perm[i]].first;
                int y = computers[k][perm[i]].second;
                for (int dir = 0; dir < 4; dir++) {
                    if (can_connect(x, y, dir)) {
                        connects.push_back(line_fill(x, y, dir));
                        action_count_limit--;
                        if (action_count_limit <= 0) {
                            return;
                        }
                    }
                }
            }
        }
    }

    void reconnect() {
        score = 0;
        uf = UnionFind(N, N);
        action_count_limit += connects.size();
        connects = vector<ConnectAction>();
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                field[i][j] = max(field[i][j], 0);
            }
        }
        connect();
    }

    Result solve() {
        while (action_count_limit > 100 + (K - 1) * 20 && timer.get() < TIME_LIMIT) {
            int prep_score = p_score;
            int pren_score = n_score;

            int backup_action_count_limit = action_count_limit;
            vector<vector<int>> backup_field = field;
            vector<MoveAction> backup_moves = moves;
            vector<ConnectAction> backup_connects = connects;
            vector<vector<pair<int, int>>> backup_computers = computers;
            vector<vector<int>> backup_index = index;

            move(5);
            // reconnect();

            if (p_score - n_score < prep_score - pren_score) {
                p_score = prep_score;
                n_score = pren_score;

                action_count_limit = backup_action_count_limit;
                field = backup_field;
                moves = backup_moves;
                connects = backup_connects;
                computers = backup_computers;
                index = backup_index;
            }
        }
        connect();
        return Result(score, moves, connects);
    }
};

void input(int &N, int &K, vector<vector<int>> &field, vector<vector<int>> &index, vector<vector<pair<int, int>>> &computers) {
    cin >> N >> K;
    field.resize(N, vector<int>(N));
    index.resize(N, vector<int>(N));
    computers.resize(K);
    for (int i = 0; i < N; i++) {
        string s;
        cin >> s;
        for (int j = 0; j < N; j++) {
            field[i][j] = s[j] - '0';
            if (field[i][j] > 0) {
                int k = field[i][j] - 1;
                index[i][j] = computers[k].size();
                computers[k].emplace_back(i, j);
            }
        }
    }
}
void print_answer(Result &res) {
    cout << res.moves.size() << endl;
    for (MoveAction m : res.moves) {
        cout << m.before_row << " " << m.before_col << " "
            << m.after_row << " " << m.after_col << endl;
    }
    cout << res.connects.size() << endl;
    for (ConnectAction c : res.connects) {
        cout << c.c1_row << " " << c.c1_col << " "
            << c.c2_row << " " << c.c2_col << endl;
    }
}

void solve() {
    int N, K;
    vector<vector<int>> field;
    vector<vector<pair<int, int>>> computers;
    vector<vector<int>> index;
    input(N, K, field, index, computers);

    Solver best_sol(N, K, field, index, computers);
    Result best_res = best_sol.solve();

    timer.reset();
    int loop_count = 0;
    int update_count = 0;
    while (timer.get() < TIME_LIMIT) {
        loop_count++;
        Solver sol(N, K, field, index, computers);
        Result res = sol.solve();
        if (res.score > best_res.score) {
            update_count++;
            best_res = res;
        }
    }

    print_answer(best_res);
    cerr << "N, K   : " << N << ", " << K << endl;
    cerr << "Score  : " << best_res.score << endl;
    cerr << "Loop   : " << loop_count << endl;
    cerr << "Update : " << update_count << endl;
    cerr << "Time   : " << timer.get() << endl;
}

int main(){
    solve();
}