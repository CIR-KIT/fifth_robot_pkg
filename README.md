# CIRKIT-5
CIRKIT所有ロボット5号機のリポジトリ

## 特記事項
- LRFを通信規格がUSBケーブル式のものに変更したため, ブランチを分けた.
- バスの電流不足による障害が発生している. バスパワーハブを使うか, 配線を工夫.

## 仕様
- 開発環境は ROS Kinetic Kameを推奨
- ハードウェア要件は i-cart mini に準ずるものとする: [公式](http://t-frog.com/products/icart_mini/)
  - 空虚重量 8kg
  - 車輪φ 140mm
  - フレーム地上高 140mm
  - フレーム構成 x, y, z : 300, 250, 180mm
  - 車軸位置 フレーム前縁 30mm
  - 車軸長 400mm

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
- urg\_node(LRFドライバ)

## 各ノードの提携図
Updated  10/8

走行モード
![runnning](https://github.com/CIR-KIT/fifth_robot_pkg/blob/images/images/new_pkgs_drafting20161005.jpg)
 
地図モード
![mapmaking](https://github.com/CIR-KIT/fifth_robot_pkg/blob/images/images/new_pkgs_drafting20161005-mapmaker.jpg)

## 詳細
- urg\_node
 + subscribing : none
 + publishing  : /Laserscan Sensor\_msgs/Laserscan
- map-saver
 + subscribing : none(reading map bags)
 + publishing  : /map nav\_smgs/Getmap
- yp-spur
 + subscribing : /cmd\_vel geometry\_msgs/Twist
 + publishing  : /odom nav-msgs/Odometory
- move\_base
 + subscribing : /Laserscan Sensor\_msgs/Laserscan
 + subscribing : /map nav\_smgs/Getmap
 + subscribing : /odom nav-msgs/Odometory
 + publishing  : /cmd\_vel geometry\_msgs/twist

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

_注意_ yp\_spur\_ros\_bridge はいりません. インストール先がwikiに乗ってないので自動で落とせるようにシェルってます

次に,third\_party内のインストールシェルより `joy`, `joy_teleop`, `navigation` がaptでインストールできます.

~~必要に応じて `tf` `urg_node` などを更にapt(手作業)でインストールします.~~

ROS フル版なら入っています. Third\_Party内のいらない奴がが喚くようなら殺してかましません.

そうすれば `catkin_make` が通るはずです.

## 起動
1. 接続を確認します.(PC <-> spur, PC <-> PS3コン, PC <-> Hokuyo)
2. ロボットの電源を入れます.
3. ネットの接続を全て切断します.右上のアイコンを開いて切断を押してください.
4. `rosrun fifth_robot_launch openEth.sh enp0s25` を実行します.ただし `enp0s25` は環境によって変えます

ここまでが全過程共通の工程です。

## パラメタファイル

マシンによってちょいちょいモータの回転が悪いことはありますので, 適当に調整.
fifth\_robot\_launcher内部のfifth\_paramにある.
公式のガイダンスもあるが, RADIUSは右が0で左が1
標準のパラメータファイルは足が遅くなる, おそらく相当に安全側に寄せてある.

### マップ作成(ラジコン操作)

```bash
roslaunch fifth_robot_launch teleop.launch
```

これで, path が適切に通っていればドライバ起動・通信開始・入力受付をやってくれます.
困ったことがあるときは ~~2~~ 今は3 回生とかに質問くれてもいいですし, issue 飛ばしてくれることを期待します.

### bagfileの取り扱い

rosが走ってる時に, 
```bash
rosbag record /target_topics
```

で, 記録. 満足したら Ctrl + C で中止.

一旦 terminal を落として, 別のところで core を立ち上げて, 
terminal 1 にて
```bash 
rosparam set use\_sim\_time true
```
terminal 1 にて
```bash 
rosrun gmapping slam\_gmapping scan:=<読み替え先のtopic名,同一でも可能>
```

現在, マルチエコー機能が使えてないので, ここで読むtopic については scan でよろしい.

terminal 2 にて

```bash 
rosbag play --clock <さっきのbag>
```

終了したところで,\n 
terminal 1 を __決して閉じず__\n
terminal 2 にて 
```bash 
 rosrun map\_server map\_saver -f <map_name>
```

これでmapあがり.

### Simulation

`Gazebo` について言うなら
`fifth_robot_description/launch` にあるgazebo.launch でたち上がります.
`ros_control`越しにnavigationとかと提携するのはまだです
