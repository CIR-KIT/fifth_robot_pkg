# CIRKIT-5
CIRKIT所有ロボット5号機のリポジトリ

## 仕様
- 開発環境は ROS Kinetic Kameを推奨
- ハードウェア要件は i-cart mini に準ずるものとする: [公式](http://t-frog.com/products/icart_mini/)

## テスト記録
- 旧ハードについてはROS Indigo にて動作確認済み
- 新ハード(本番用)については開発中(以降は, Kinetic Kameによって開発を行う)

## hokuyoの使い方
ipアドレスをスタティックに振り分ける必要があるために次を実行します. `enp7s0` は環境によって変えてください.

```bash
rosrun fifth_robot_launcher openEth.sh enp7s0
# You should change enp7s0 to fit your machine. you can see it on `ifconfig`
```

これは ubuntu16.04 版についてなので, 14.04で走らせたいときは eth0 / wlan0 と言った名前の振り分けにしてください.

## ソフト構成
- navigation
- gmappping
- yp-spur(公式)
- urg_node(LRFドライバ)

## 各ノードの提携図
Updated  10/8

走行モード
![runnning](https://github.com/CIR-KIT/fifth_robot_pkg/blob/images/images/new_pkgs_drafting20161005.jpg)
 
地図モード
![mapmaking](https://github.com/CIR-KIT/fifth_robot_pkg/blob/images/images/new_pkgs_drafting20161005-mapmaker.jpg)

## 詳細
- urg_node
 + subscribing : none
 + publishing  : /Laserscan Sensor_msgs/Laserscan
- map-saver
 + subscribing : none(reading map bags)
 + publishing  : /map nav_smgs/Getmap
- yp-spur
 + subscribing : /cmd_vel geometry_msgs/Twist
 + publishing  : /odom nav-msgs/Odometory
- move_base
 + subscribing : /Laserscan Sensor_msgs/Laserscan
 + subscribing : /map nav_smgs/Getmap
 + subscribing : /odom nav-msgs/Odometory
 + publishing  : /cmd_vel geometry_msgs/twist

我々は,goal/waypoint提供者を実装する必要があります.

## メモ
- セットアップについて,**yp-spur,ssmのインストール**を忘れずに行ってください: [公式](http://www.roboken.iit.tsukuba.ac.jp/platform/wiki/yp-spur/how-to-install)
- contributer を募集しています

~~Note : モータがなんか変だこれ...極性逆っぽいけれどソフト的に対応してるから注意.(可搬性はないのだ)~~
モータはパラメタファイルによって修正済み

## Installation
`git clone` する際に `--recursive` を付ければsubmoduleごと引っ張ってこれます。

catkin workspace のソース内(`src`)にクローンした場合はそのまま。
それ以外のところにクローンした場合は`src`ディレクトリ上で`catkin_init_workspace`を行えばリポジトリをcatkin workspaceにできます。

まずは必要な packeage を確保してください.

初めに,筑波大学の公式から `ssm`, `yp-spur` を各自で自分のシステムにインストールしてください.

次に,third_party内のインストールシェルより `joy`, `joy_teleop`, `navigation` がaptでインストールできます.

必要に応じて `tf` `urg_node` などを更にapt(手作業)でインストールします.

そうすれば `catkin_make` が通るはずです.

## 起動
1. 接続を確認します.(PC <-> spur, PC <-> PS3コン, PC <-> Hokuyo)
2. ロボットの電源を入れます.
3. ネットの接続を全て切断します.右上のアイコンを開いて切断を押してください.
4. `rosrun fifth_robot_launch openEth.sh enp0s25` を実行します.ただし `enp0s25` は環境によって変えます

ここまでが全過程共通の工程です。

### マップ作成(ラジコン操作)

```bash
roslaunch fifth_robot_launch mapping.launch
```

これで, path が適切に通っていればドライバ起動・通信開始・入力受付をやってくれます.
困ったことがあるときは 2 回生とかに質問くれてもいいですし, issue 飛ばしてくれることを期待します.
