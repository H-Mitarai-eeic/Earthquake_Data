# Earthquake_Data
Earthquake_AI用のデータリポジトリ
# データ
## データソース
気象庁 > 地震月報（カタログ編）> 震度データ<br>
https://www.data.jma.go.jp/svd/eqev/data/bulletin/shindo.html
## データ形式
### data_shaped/kansoku[year].csv
```
地震ID(int), 緯度(float), 経度(float), 計測震度(float), 震度階級(int, 0 ~ 9)
```
### data_shaped/shingen[year].csv
```
地震ID(int), 観測点数(int), 緯度(float), 経度(float), 深さ(float), 気象庁マグニチュード(float)
```
### data_reshaped/[EarthQuake ID].csv
```
震源のx座標(int, 0〜255), 震源のy座標(int, 0〜255), 震源の深さ(float, km), マグニチュード\n
正解ラベル(256×256の二次元int配列，値はその地点の震度)
```
## 日本の範囲（仮定）
緯度 20 ~ 46<br>
経度122~154

## 顕著な地震データ
    1. 東北地方太平洋沖地震 : EarthQuake ID = 45235