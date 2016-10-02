# CIRKIT-5

CIRKIT所有ロボット5号機のリポジトリ

## 仕様
- 開発環境は ROS Kinetic Kameを推奨
- ハードウェア要件は i-cart mini に準ずるものとする: [公式](http://t-frog.com/products/icart_mini/)

## テスト記録
- 旧ハードについてはROS Indigo にて動作確認済み
- 新ハード(本番用)については後日調査(以降は, Kinetic Kameによって開発を行う)

##Updated20161002
諸事情あってちょっともう現行の作業内容はぶっ飛ばすことにしました 
運用開始のめどが立たないので 
猛スピードで作りなおそうと思います. 

##新ソフト仕様
 - navigation
 - gmappping
 - yp-spur(公式)
 - hokuyo-node(北陽のLRFドライバ)
を使用. 

各ノードの提携図は 10/2 中にUpload 

## メモ
- セットアップについて,__yp-spur,ssmのインストール__を忘れずに行ってください: [公式](http://www.roboken.iit.tsukuba.ac.jp/platform/wiki/yp-spur/how-to-install)
- contributer を募集しています

## Installation
`git clone` する際に `--recursive` を付ければsubmoduleごと引っ張ってこれます。
そうすれば `catkin_make` が通るはずです。

catkin workspace のソース内(`src`)にクローンした場合はそのまま。
それ以外のところにクローンした場合は`src`ディレクトリ上で`catkin_init_workspace`を行えばリポジトリをcatkin workspaceにできます。

