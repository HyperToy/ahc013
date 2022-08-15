#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define rep(i,n) for (int i = 0; i < (n); ++i)
#define PR(x) cerr << #x << " = " << x << endl

int main() {
    int n;
    cin >> n;
    int score = 0;
    int sum[10][50];
    int cnt[10][50];
    rep(i,n) {
        int N, K;
        cin >> N >> K;
        string a, b;
        int x;
        cin >> a >> b >> x;

        score += x;
        sum[K][0] += x;
        cnt[K][0] += 1;
        sum[K][N] += x;
        cnt[K][N] += 1;
    }
    for (int K = 2; K <= 5; K++) {
        cout << "K = " << K << " : " << sum[K][0] / cnt[K][0] << " (" << cnt[K][0] << ")" << endl;
        for (int N = K * 3 + 9; N <= K * 3 + 33; N++) {
            if (cnt[K][N] > 0) cout << "   " << N << " : " << sum[K][N] / cnt[K][N] << " (" << cnt[K][N] << ")" << endl;
        }
        cout << endl;
    }
    cout << "total : " << score / n << " (" << score * 50 / n << ")" << endl << endl;
}
