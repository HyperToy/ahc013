# AHC013 RECRUIT 日本橋ハーフマラソン 2022夏

## ツールのコマンド
- 入力データの生成（tools/ にて実行）
    ```
    cargo run --release --bin gen seeds.txt # 入力ファイルの生成, in/ に出力される 
    ```
- ビジュアライザの生成（tools/ にて実行）  
入力ファイルを in.txt、出力ファイルを out.txt とする
    ```
    cargo run --release --bin vis in.txt out.txt
    ```
## やること
- アイデアの実装（繋がってないコンピュータを適当な方向に移動して、ケーブルにあたれば繋ぐ。
- 結果計算・集計のシェルスクリプトを書く

## 2022.08.10 Wed (2日目)
- 正の点数を達成。各セルに対して下と右を見て、同じ種類であればつなげる。単純なもの。
    - 7,555
- 下と右とを見て、進めるだけ進んで、同じ種類であれば繋げる。
    - クロスするのはダメなので、繋いでいるセルには適当な数値を書き込むことにした。
    - 実装で少し手間取ったが、なんとかなった。
    - 提出待ち。
    - 26,263
        - 263位, トップのスコアの10%
- アイデア
    - 繋がっていないコンピュータで、いずれかの方向に伸ばしてみて同じ種類のコンピュータの「線」に当たれば、そこまで移動して繋げる
    - もしくは1つの方向にしか伸びていないコンピュータは、その方向に移動して合流させる
    - 繋ぐ方の出力用配列を初期化して、繋ぎ直す。
## 2022.08.11 Thu (3日目、祝日)
- シェルスクリプトの調整
    - 指定した回数分、ループでプログラムを回すようにできた。
    - 0000 ~ 9999 までの出力ファイルを生成。
    - 数字を0埋めするのは、 seq コマンドの -f オプションが使えた。下のコマンドをバッククォート (`) で囲って、for 文に渡せばよい。
        ```
        seq -f '%04g' 0 99
        for i in `seq -f '%04g' 0 99`
        do 
            echo $i
        done
        ```
    - 入力ファイルがデフォルトで 0099 までしかなかったので、 0999 までやろうとしたらエラー吐いた。
- アイデア：セルごとに縦横やるのではなく、縦全て、横全てみたいにやったらどうなる？
    - 特に変わらず。点数も同じ。
- ローカル用テスターの調整
    - 入力データの生成
        - README を見ながら、入力データの追加生成を試みた。
        - 途中でエラーが出て、入力データが生成されなかったので、その対応をやった。
            ```
            error[E0658]: use of unstable library feature 'int_abs_diff'
                --> src/lib.rs:199:15
                |
            199 |         if r0.abs_diff(r1) + c0.abs_diff(c1) != 1 {
                |               ^^^^^^^^
                |
                = note: see issue #89492 <https://github.com/rust-lang/rust/issues/89492> for more information

            error[E0658]: use of unstable library feature 'int_abs_diff'
                --> src/lib.rs:199:33
                |
            199 |         if r0.abs_diff(r1) + c0.abs_diff(c1) != 1 {
                |                                 ^^^^^^^^
                |
                = note: see issue #89492 <https://github.com/rust-lang/rust/issues/89492> for more information

            For more information about this error, try `rustc --explain E0658`.
            error: could not compile `tools` due to 2 previous errors
            ```
        - "use of unstable library feature" の部分で検索して、日本語のページで1番上に出たやつを参考に、以下のコマンドで Rust のバージョンをあげた。
            ```
            $ rustup install stable
            ..(中略)
                stable-x86_64-apple-darwin updated - rustc 1.62.1 (e092d0b6b 2022-07-16) (from rustc 1.58.1 (db9d1b20b 2022-01-20))
            $ rustc --version
            rustc 1.62.1 (e092d0b6b 2022-07-16)
            ```
        - これで、まったく同じコマンドをそのまま打ち直したら、今度は何事もなく終了して、無事入力ファイルが生成された。
    - ビジュアライザの生成
        - シェルスクリプトに追加。