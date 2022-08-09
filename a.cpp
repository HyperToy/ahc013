#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define rep(i,n) for (int i = 0; i < (n); ++i)

int N, K;
vector<vector<int>> c;

void input() {
    cin >> N >> K;
    c = vector<vector<int>>(N, vector<int>(N));
    rep(i,N) rep(j,N) cin >> c[i][j];
}

void eval() {

}

void init() {

}

void simulate() {

}

void output() {

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