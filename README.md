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
 - urg_node(LRFドライバ)
を使用. 
Note. hokuyo_node は古くて非推奨なのですが, laserprocなどの必要な部品がkineticにおいて揃っておらず,現行使えないということでhokuyo_nodeを使うことにしました.
必要なdriverを追加で導入することにしましたので, ひょっとしたらurg_nodeでも稼働しそうですが, 使ったことのある方を使うようにします.

##各ノードの提携図
Uploaded 10/2  
Updated  10/5  

走行モード 
![runnning](https://github.com/CIR-KIT/fifth_robot_pkg/blob/images/images/new_pkgs_drafting20161005.jpg)
 
地図モード 
![mapmaking](https://github.com/CIR-KIT/fifth_robot_pkg/blob/images/images/new_pkgs_drafting20161005-mapmaker.jpg)

##詳細
- hokuyo_node
   - subscribing : none
   - publishing  : /Laserscan Sensor_msgs/Laserscan
- map-saver
   - subscribing : none(reading map bags)
   - publishing  : /map nav_smgs/Getmap
- yp-spur
   - subscribing : /cmd_vel geometry_msgs/twist
   - publishing  : /odom nav-msgs/Odometory

- move_base
   - subscribing : /Laserscan Sensor_msgs/Laserscan
   - subscribing : /map nav_smgs/Getmap
   - subscribing : /odom nav-msgs/Odometory
   - publishing  : //cmd_vel geometry_msgs/twist

## メモ
- セットアップについて,__yp-spur,ssmのインストール__を忘れずに行ってください: [公式](http://www.roboken.iit.tsukuba.ac.jp/platform/wiki/yp-spur/how-to-install)  
Note : もう必要ないかもしれませんがインストール環境で開発が進んでいます. 使ってない状況での挙動報告は歓迎.  

- contributer を募集しています

## Installation
`git clone` する際に `--recursive` を付ければsubmoduleごと引っ張ってこれます。
そうすれば `catkin_make` が通るはずです。

catkin workspace のソース内(`src`)にクローンした場合はそのまま。
それ以外のところにクローンした場合は`src`ディレクトリ上で`catkin_init_workspace`を行えばリポジトリをcatkin workspaceにできます。

