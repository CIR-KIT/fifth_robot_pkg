# CIRKIT-5

CIRKIT所有ロボット5号機のリポジトリ

## 仕様
- 開発環境は ROS Kinetic Kameを推奨
- ハードウェア要件は i-cart mini に準ずるものとする:[公式](http://t-frog.com/products/icart_mini/)

## テスト記録
- 旧ハードについてはROS Indigo にて動作確認済み
- 新ハード(本番用)については後日調査(以降は, Kinetic Kameによって開発を行う)

## メモ
- 使うときは各人セットアップ済みの catkin_ws に clone して使ってください
- セットアップについて,__yp-spur,ssmのインストール__を忘れずに行ってください:[公式](http://www.roboken.iit.tsukuba.ac.jp/platform/wiki/yp-spur/how-to-install)
- contributer を募集しています

## インストレーション注意書き
- ソフトウェア的には 誰かの作った i-cart mini 操作用のソフトを使うことになってます [wiki](http://wiki.ros.org/icart_mini)が, これのインストールについて特記するべきことがあるので記します
- __wikiどうりにはやりません__

## Installation
`git clone` する際に `--recursive` を付ければsubmoduleごと引っ張ってこれます。

catkin workspace のソース内にクローンした場合はそのまま。
それ以外のところにクローンした場合は`src`ディレクトリ上で`catkin_init_workspace`を行えばリポジトリをcatkin workspaceにできます。
