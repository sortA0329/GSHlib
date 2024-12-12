# GSHlib

実行速度を最大限まで最適化したC++用の数値計算ライブラリです。
また、インターフェースはC++の標準ライブラリを模倣して設計しており、標準ライブラリからの移行が容易に行えます。

# インストール

このライブラリはヘッダオンリーであり、また外部ライブラリに依存していないためインストールは簡単です。

1. ページ上部の緑色の`<> Code ▼`ボタンから`Download ZIP`を選択してダウンロードします。
2. ZIPファイルを解凍し、解凍先のディレクトリのパスをコピーします。
3. コピーしたパスをコンパイラのインクルードパスに追加することで使用することができます。

# 動作環境

## コンパイラ

- GCC11以降
- Clang16以降

GSHlibはC++20の機能を使用しているため、`-std=c++20`もしくはそれ以降のC++のバージョンを指定してください。

## OS

- Ubuntu
- Windows

Windowsを使用する場合はMinGW-w64のGCC/Clangがサポートされています。
WSLを用いたコンパイルは検証していません。

# 使用方法の例

```cpp
#include <gsh/InOut.hpp>

gsh::BasicReader rd;
gsh::BasicWriter wt;
int main() {
    int n = rd.read<int>();
    wt.writeln(2 * n);
}
```
