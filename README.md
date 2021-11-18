# Earthquake_Data
Earthquake_AI用のデータリポジトリ by Haruki Mitarai(U-TOKYO)<br>
課題提出用リポジトリ : https://github.com/H-Mitarai-eeic/Earthquaker <br>
開発用リポジトリ：https://github.com/H-Mitarai-eeic/Earthquake_AI
# ディレクトリに関して
data/ : 気象庁から取得したデータ<br>
data_shaped : 気象庁から取得したデータの内、必要部分を取り出して .csvにしたもの<br>
data_reshaped_honshu6464/ : 学習用データ（本州周辺のデータ, 64 x 64）<br>
data_reshaped_honshu6464/ : 学習用データ（本州周辺のデータでマグニチュード5以上のもの 64 x 64）<br>
data_reshaped_japan256256/ : 学習用データ（日本周辺すべてのデータ 256 x 256）<br>
# ソースコード
## mkdata.c
気象庁から取得したデータの内、必要部分を取り出して .csvにするプログラム。data/ ディレクトリ内のデータを data_shaped/ に出力
## data_reshape.c
mkdata.c で .csvにしたデータをさらに学習用データに成形。
data_shaped/ ディレクトリ内のデータを data_reshaped_*/ に出力
## ObsevationPoints.c
各メッシュ内の観測地点数をcsvで出力。
# データ
## データソース
気象庁 > 地震月報（カタログ編）> 震度データ<br>
https://www.data.jma.go.jp/svd/eqev/data/bulletin/shindo.html
## 除外しているデータ
マグニチュードあるいは深さのいずれかが欠損したデータはすべて除外している。
## データ形式
### data_shaped/kansoku[year].csv
```
地震ID(int), 緯度(float), 経度(float), 計測震度(float), 震度階級(int, 0 ~ 9)
```
### data_shaped/shingen[year].csv
```
地震ID(int), 観測点数(int), 緯度(float), 経度(float), 深さ(float), 気象庁マグニチュード(float)
```
### data_reshaped*/[EarthQuake ID].csv
data_reshaped_honshu256256 ディレクトリは18GBほどあるため、取り扱い注意。
```
震源のx座標(int, 0〜63など), 震源のy座標(int, 0〜64など), 震源の深さ(float, km), マグニチュード\n
正解ラベル(64x64などの二次元int配列，値はその地点の震度)
```
### ObservationPointsMap*.csv
```
各メッシュ内の観測地点数(int型の配列)
```
## data_reshapeでの日本の範囲などの設定
### 日本全体（data_reshaped_japan256256/）
緯度 20 ~ 46<br>
経度 122 ~ 154
### 本州のみ（data_reshaped_honshu6464/）
緯度 30 ~ 46<br>
経度 128 ~ 146

## 顕著な地震データ
    1. 東北地方太平洋沖地震 : EarthQuake ID = 40278