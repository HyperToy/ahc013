#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define rep(i,n) for (int i = 0; i < (n); ++i)

int main() {
    int N;
    cin >> N;
    int score;
    vector<vector<int>> kind(4, vector<int>(2, 0));
    rep(i,N) {
        int K = i % 4;
        string a, b;
        int x;
        cin >> a >> b >> x;
        score += x;
        kind[K][0]++;
        kind[K][1] += x;
    }
    rep(k,4) {
        printf("K = %d   : %d\n", k + 2, kind[k][1] / kind[k][0]);
    }
    printf("total   : %d\naverage : %d\n", score, score / N);
}