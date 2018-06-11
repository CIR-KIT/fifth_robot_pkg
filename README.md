# CIRKIT-5
CIRKIT所有ロボット5号機のリポジトリ

## 目次

 1.  [ 特記事項 ](https://github.com/CIR-KIT/fifth_robot_pkg#特記事項)
 2.  [ 仕様と寸法 ](https://github.com/CIR-KIT/fifth_robot_pkg#仕様と寸法)
 3.  [北陽電機製レーザ式二次元測域センサの使い方 ](https://github.com/CIR-KIT/fifth_robot_pkg#北陽電機製レーザ式二次元測域センサの使い方)
 4.  [ ソフト構成 ](https://github.com/CIR-KIT/fifth_robot_pkg#ソフト構成)
 5.  [ 最小構成の各ノード提携図 ](https://github.com/CIR-KIT/fifth_robot_pkg#最小構成の各ノード提携図)
 6.  [ 詳細 ](https://github.com/CIR-KIT/fifth_robot_pkg#詳細)
 7.  [ インストールとセットアップ ](https://github.com/CIR-KIT/fifth_robot_pkg#インストールとセットアップ)
 8.  [ パラメタファイル ](https://github.com/CIR-KIT/fifth_robot_pkg#パラメタファイル)
 9.  [ マップ作成とラジコン操作](https://github.com/CIR-KIT/fifth_robot_pkg#マップ作成とラジコン操作)
 10. [ bagfileの取り扱い ](https://github.com/CIR-KIT/fifth_robot_pkg#bagfileの取り扱い)
 11. [ GazeboSimulation ](https://github.com/CIR-KIT/fifth_robot_pkg#gazebosimulation)
 12. [ GazeboSimulatedMap ](https://github.com/CIR-KIT/fifth_robot_pkg#gazebosimulatedmap)
 13. [ GazeboSimulatedNavigation ](https://github.com/CIR-KIT/fifth_robot_pkg#gazebosimulatednavigation)
 14. [ よくある障害 ](https://github.com/CIR-KIT/fifth_robot_pkg#よくある障害)
 15. [新・起動方法 (2017/4/29 追記)](http://github.com/CIR-KIT/fifth_robot_pkg#新・起動方法(2017/4/29追記))
 17. [ 補遺 ](https://github.com/CIR-KIT/fifth_robot_pkg#補遺)
 18. [ 参考 ](https://github.com/CIR-KIT/fifth_robot_pkg#参考)


起動(実機)
## 特記事項
- LRFを通信規格がUSBケーブル式のものに変更したため, ブランチを分けた.
- バスの電流不足による障害が発生している. バスパワーハブを使うか, 配線を工夫.
- Github (https://github.com/CIR-KIT/fifth_robot_pkg )上の[wiki](https://github.com/CIR-KIT/fifth_robot_pkg/wiki) も参照してください

## 仕様と寸法
- 開発環境は ROS Kinetic Kameを推奨
- ハードウェア要件は i-cart mini に準ずるものとする: [公式](http://t-frog.com/products/icart_mini/)
  - 乾燥重量 8kg
  - 車輪φ 140mm
  - フレーム地上高 140mm
  - フレーム構成 x, y, z : 300, 250, 180mm
  - 車軸位置 フレーム前縁 30mm
  - 車軸長 400mm

## 北陽電機製レーザ式二次元測域センサの使い方
ipアドレスをスタティックに振り分ける必要があるために次を実行します. `enp7s0` は環境によって変えてください.

```bash
rosrun fifth_robot_launcher openEth.sh enp7s0
# You should change enp7s0 to fit your machine. you can see it on `ifconfig`
```

これは ubuntu16.04 版についてなので, 14.04で走らせたいときは eth0 / wlan0 と言った名前の振り分けにしてください.

## ソフト構成
- 実機
  - navigation
    - trajectry_planner
      -  move_base 
        - local_planner 
        - local_costmap 
        - glabal_costmap 
        - glabal_planner 
        - recovery_behavior 
  - gmappping
  - yp-spur(公式)
  - urg\_node(LRFドライバ)

  - nmea_navsat_driver(予定)
  - GPS-Odometory 結合者 (未実装)
  - 高機能マップ提供者(未実装)
  - 高機能マップ記録者(未実装)

- シミュレータ
  - navigation
    - 上に同じ
  - gmapping
  - Gazebo
   - diff\_drive\_controller
   - gazebo_ros
  - Gazebo_Plugin(LRFドライバ, diffdriveドライバなどのエミュレーションを担当)

## 最小構成の各ノード提携図
基礎的な部分は RO\S_wikiのmove_base(url)[http://wiki.ros.org/move_base] か Quittaの当該記事(url)[https://qiita.com/MoriKen/items/0b75ab291ab0d95c37c2]を参照してください
Updated  10/8

走行モード
![runnning](https://github.com/CIR-KIT/fifth_robot_pkg/blob/images/images/new_pkgs_drafting20161005.jpg)
 
地図モード
![mapmaking](https://github.com/CIR-KIT/fifth_robot_pkg/blob/images/images/new_pkgs_drafting20161005-mapmaker.jpg)

## 詳細

最小構成では, 
ノード `amcl` が トピック `map`  `scan` `odometory` によって事故位置を推定し,
何らかの形で与えられたGoalに向けて `move_base` が経路決定を行い, ドライバ `yp-spur`がその信号たる`cmd_vel` を解釈し実行, `odom` を報告

move_base 内の costmap はパラメータによって挙動が変わる(要調整)
yp-spurも挙動が変わる(要調整)

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

goal/waypoint提供者は外部にあり, third_partyにある
`ros_waypoint_generator`と`goal_sender`がそれに当たります.
構成に合わせてlaunchに登録してください.

※更新の予定があります

## インストールとセットアップ
- セットアップについて, まずインストールシェルより **yp-spur,ssmのインストール**を忘れずに行ってください:さもなければmakeが通りません [公式](http://www.roboken.iit.tsukuba.ac.jp/platform/wiki/yp-spur/how-to-install)
- contributer を募集しています

モータはパラメタファイルによって修正済み

当該レポジトリを `git clone` する際に `--recursive` を付けてsubmoduleごと引っ張ってこれます。

catkin workspace のソース内(`src`)にクローンした場合はそのまま。
それ以外のところにクローンした場合は`src`ディレクトリ上で`catkin_init_workspace`を行えばリポジトリをcatkin workspaceにできます。

まずは必要な packeage を確保してください.

初めに,筑波大学の[公式](http://www.roboken.iit.tsukuba.ac.jp/platform/wiki/ssm/index)から `ssm`, `yp-spur` を各自で自分のシステムにインストールしてください.

_注意_ yp\_spur\_ros\_bridge はいりません. インストール先がwikiに乗ってないので自動で落とせるようにシェルってます

次に,third\_party内のインストールシェルより `joy`, `joy_teleop`, `navigation` がaptでインストールできます.

~~必要に応じて `tf` `urg_node` などを更にapt(手作業)でインストールします.~~

ROS フル版なら入っています. Third\_Party内のいらない奴がが喚くようなら消してください. 
(現状, driver\_commonが不要ですがまた更新がかかった時のために残してあります)

そうすれば `catkin_make` が通るはずです.

## 起動(実機)

1. 接続を確認します.(PC <-> spur, PC <-> PS3コン, PC <-> Hokuyo)
2. ロボットの電源を入れます.
3. ネットの接続を全て切断します.右上のアイコンを開いて切断を押してください.
4. `roslaunch fifth_robot_launcher connect.launch ethernet_device:=enp025` を実行します.  
   ただし `enp0s25` は環境によって変えます.
   再接続する際はもう一度これを実行するだけでよいです。

ここまで実機に共通の工程です。

## パラメタファイル

マシンによってちょいちょいモータの回転が悪いことはありますので, 適当に調整.
fifth\_robot\_launcher内部のfifth\_paramにある.
公式のガイダンスもあるが, RADIUSは右が0で左が1
標準のパラメータファイルは足が遅くなる, おそらく相当に安全側に寄せてある.

### マップ作成とラジコン操作

```bash
roslaunch fifth_robot_launch teleop.launch
```

これで, path が適切に通っていればドライバ起動・通信開始・入力受付をやってくれます.
困ったことがあるときは ~~2~~ 今は3 回生とかに質問くれてもいいですし, issue 飛ばしてくれることを期待します.

### bagfileの取り扱い

rosが走ってる時に, 
```bash
rosbag record --all
```
で, 記録. 満足したら Ctrl + C で中止.
トピックは各個別に指定することもできますが, 面倒なので--allにしてます. ただ, ファイルサイズとトレードオフなので
レーザ系のトピック + odom のみでいいと思います.

一旦 terminal を落として, 別のところで core を立ち上げて, 
terminal 1 にて
```bash 
rosparam set use_sim_time true
```
terminal 1 にて
```bash 
rosrun gmapping slam_gmapping 
```

現在, マルチエコー機能が使えてないので, ここで読むtopic については scan でよろしい.
ただ, マルチエコーを有効とするUTM-30-LX-EWなどを用いるときは scanを読み替えて

`scan:=<読み替え先のtopic名,同一でも可能>`

とする.

terminal 2 にて

```bash 
rosbag play --clock <さっきのbag>
```

終了したところで,\n 
terminal 1 を __決して閉じず__\n
terminal 2 にて 
```bash 
 rosrun map_server map_saver -f <map_name>
```

これでmapあがり.

### gazebosimulation

`Gazebo` について言うなら `fifth_robot_description/launch` にある `fifth_robot_gazebo.launch` が roslaunch でたち上がります.

`ros_control`越しにnavigationとかと提携するのは ~~まだです~~ できます.

`fifth_robot_2dnav`などをアップデートして使ってください

### gazebosimulatedMap

上に記したマップ作成との差異は, teleopのかわりにgazeboを立ち上げることのみです.

![sample](https://github.com/CIR-KIT/fifth_robot_pkg/blob/images/images/gazebo_sample1.png)

この通りコントローラが出てくるはずです.
controll系のパッケージが不足します.
`fifth_robot_description`内部の`install_simulator.sh`で必要な奴らをインストール

- Gazebo上でのロボットモデル評価 マップの作成テスト

基本的に上で紹介した方法と同一で実行できるようになりました. ただ, map を作る前に gazebo を落としてコアを立ち上げ直してください.

タイムスタンプがおかしくなって, ` Detected jump back in time. Clearing TF buffer.`とか言い出す例がありますが, 
ズレは微小で原因が不明(処理落ち?)ですしmap作成に問題がありませんでした. これは無視してください.

mapを作ったらnavigationできます.
Willowで作った地図をここにおいておきます. 結構綺麗なものが出来上がります.
![map](https://github.com/CIR-KIT/fifth_robot_pkg/blob/images/images/willow_map.jpg)


<b> 重くなるのでマップデータやbagファイルをmasterやらに置かないでください. 現場で_絶対に_後悔します</b>

### gazebosimulatedNavigation

実機なしでもできるシミュレーション. navigation の運用練習やパラメータ調整を迅速化できるので是非身につけてください.

以下wikiと同一

てっとり早いtutorialには, ` /fifth_robot_description/launch ` にある実体から

`roslaunch fifth_robot_descriotion test.launch `  

重いですが willow_garage が立ち上がる.

`roslaunch fifth_robot_description gazebo_nav.launch laser_topic:=scan ` 

ここはgazeboのレーザードライバが/scanに吐き出しているところを実機となるだけ共通で扱うためにそうしています. 実機では ` /most_intense ` をとっています( MultiEcho モード ) 


`rviz` 

ちょっとばかり設定が込み入っています 

当該パッケージにおける Navigation の構成は amcl による自己位置推定( 2D_Pose_Estimate )と move_base によるものですから, 初期位置を与える必要があります. 

Fixed_Frame を /map に固定しなければ amcl が作動しません. 上ベインに標準で出ている 2D Pose Estimate をGazebo上の位置と勘案して設定します(rvizのビジュアル上でマウスを使いD&Dでにゅっとやります)

![gazebo_nav_tuto1](https://github.com/CIR-KIT/fifth_robot_pkg/blob/images/images/gazebo_nav_instruction_1.png)

壁のscanとmapが一致するのが目安となります. ※現実でやる時もmap上で勘案して設定します

![gazebo_nav_tuto2](https://github.com/CIR-KIT/fifth_robot_pkg/blob/images/images/gazebo_nav_instruction_2.png)

左側ベインのPoseをAddし, トピック( /move_base_simple/goal または /move_base/current_goal )を指定 <b> Unreliable オプションを指定(チェックボックス)した上で,</b>  上ベインに標準で出ている 2D Nav Goal を設定すると走り出します. 緑色で表示されているのはpathで, global_planning の結果が表示されています.

現在, local_plannningも表示されはしますがこれでは短すぎ, コストマップを更新する前に障害物に突っ込んでしまいます.

コストマップは標準の状態ではパブリッシュされません. ROS-wikiの記述によると nav\_msgs/Gridcells の local\_costmap/inflated_obstacles トピックに障害物評価が表示されるそうなので, (要検証) 確認調整をおねがいします.

![gazebo_nav_tuto2](https://github.com/CIR-KIT/fifth_robot_pkg/blob/images/images/gazebo_nav_instruction_3.png)

goalについたら矢印の方に向いて止まります. その際, pointcloudが収束(ある程度矢印の向きが揃う)までフラフラしますが, これはデフォルトのパラメータ調整が不適なためです.


### よくある障害

 - gazeboが立ち上がらない
   - gazeboは非常に重たいので少し待ってください.
   - クラッシュしたgazebo-guiやgzclientがゾンビになってることがあります, `ps -A` でPIDを見つけて `ps kill -KILL {PID}`で終了.
   - test.launch に入っているwillow Garage なんかはモデルといい, gazeboを素で立ち上げてからロード(左ベイン→Insert→heep://gazebosim.org/mod...ってかいてるとこから)することができます.
     - 環境によっては初回ロードをlaunchファイルから行うと失敗することがありますが, まずempty_worldを パッケージ ros_gazebo から実行して, そこで一回対象のモデルをロードすると以降うまく行きます.

 - yp-spur制御基板(二軸モータドライバ TF-2MD3-R6) と通信できない
   - 電源ボードとの接触不良
   - マイコンボードがUSB給電を必要とします. バスパワーを使うか, 配線をラップトップの一箇所に集中させないようにしてボードの負荷を分散
   - dev内の認識されたデバイスの権限がrootのみになっている.dialoutに登録するか,  `chmod 777 /dev/serial/by-id/*`
   - 機嫌が悪いとしか言いようのない時があります. 一旦すべてディスコネクトして放電してください.
 - LRFと通信できない
   - 上と同じくUSB給電
   - 上と同じく権限
   - UTM-30−LX-EW はIPが割り振られています. `fifth_robot_launcher` に同梱したテストシェルの中にもありますが, デフォルトのip `192.168.0.10`にピンを打ってください.
      - ネットワーク設定を変更しないとつながりません！ イーサネットデバイスをifconfigで開いてゲートウェイ設定を変更してください(上記hokuyoのつかいかたにもあるが, `openEth.sh`にあり. 動作確認済み)
      - また, dialoutにユーザを登録してください
      - それでもダメならipが変更になっている恐れがありますので, arpで検出してください.

## 新・起動方法(2017/4/29追記) 
### 接続

```bash
sudo -s
roslaunch fifth_robot_launcher connec.launch ethernet_device:=<device>
```
`<device>` の部分は `ifconfig`で取得してください.
再接続の際はこれをキルしてもう一度実行するだけで良いです。
### ラジコン操作
Joyスティックとの通信準備
ロボットとの接続を確認後、    
```bash
roslaunch fifth_robot_launcher telop.launch
```
上で `enable_lrf:=false`と指定するとLRFなしで起動します。

### 地図作成
#### rosbagを利用する場合
Joyスティックとの通信準備  
ロボットとの接続を確認後、  
```bash
roslaunch fifth_robot_launcher telop.launch
```
別ターミナルで、  
```bash
rosbag record --all
```
＊面倒なので --all にしてます。  

走り終わったら、rosbagをキルして、  
```bash
roslaunch fifth_robot_launcher generate_map scan_topic:=<topic_name>
```
`<topic_name>`はLaserScanのトピック名を指定してください。  
別ターミナルで、  
```bash
rosbag play --clock [先ほど録った.bagファイル]
```
再生し終わったら、
```bash
rosrun map_server map_saver -f [地図ファイルの名前]
```
終

改訂版　ただしdevelopブランチ内の当該作業スペースに限る( 2018/5/21 )  
ターミナル1: 
```bash
roslaunch fifth_robot_description teleop_keyboard.launch
```
ターミナル2:
```bash
rosbag record –all
```
気が済んだらターミナル2で Ctr + C をして終了
その後、ターミナル1も Ctr + C で終了
ターミナル2: 
```bash
roslaunch fifth_robot_description generate_map_gazebo.launch  
scan_topic:=front_scan
```
ターミナル3: 
```bash
rosbag play –clock <bag ファイル >
rosrun map_server map_saver -f < マップ名 >
```
終

#### リアルタイムで地図作成する場合
接続を確認後
```bash
roslaunch fifth_robot_launcher telop.launch
```
別ターミナルで、
```bash
roslaunch fifth_robot_launcher generate_map.launch scan_topic:=<topic_name>
```
`<topic_name>`はLaserScanのトピック名を指定してください。

走り終わったら、
```bash
rosrun map_server map_saver -f [地図ファイルの名前]
```
終

### Waypoint 生成
接続を確認後、
```bash
roslaunch fifth_robot_launcher generate_waipoint.launch map:=<map_file.yaml>
```
別ターミナルで、
```bash
rosbag play [bag_file.bag]
```
bagファイル再生後、
```bash
rosrun ros_waypoint_generator ros_waypoint_generator
```
終

### Planner Test
waypointなしで move_baseを走らせる。navigationの動作テストに使います。  
接続を確認後、  
```bash
roslaunch fifth_robot_launcher planner_test.launch map:=<map_file.yaml>
```
終

### Navigation 
```bash
roslaunch fifth_robot_launcher navigation.launch map:=<map_file.yaml> waypoint:=<waypoint.csv>
```
終

### 補遺

 - ROSパッケージによるリモコンの実装(推奨)
 
  1. 側面のwireless スイッチをオンにします.
    オフのままでもwi-fiは起動している場合がありますがおそらく内部配線に起因するもので, Intel 8625 Dual-band は Bluetoothデバイス を内包しますので必要です.

  2. インストールします
     `sudo apt-get install ros-kinetic-ps3joy `
     `sudo apt-get install ros-kinetic-joystick-drivers `
     で、足りると思う

  3. 使い方は同じです
 

 - ROSによらないリモコンPS3コントローラ(JoyStick)の設定(非推奨)
    - sixad sixpair コマンドを使う.
      - インストール(バイナリをおとして使う)

      `sudo add-apt-repository "deb http://ppa.launchpad.net/falk-t-j/qtsixa/ubuntu vivid main"`

      `sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 736E4F0B`

      `sudo apt update`

      `sudo apt install sixad`

      - インストール(ソースを落として自分でコンパイルする)

      `cd`

      `sudo apt-get install pyqt4-dev-tools git `

      `sudo apt-get install libusb-dev libjack-dev libbluetooth-dev pyqt4-dev-tools`

      `cd Downloads`

      `git clone https://github.com/falkTX/qtsixa.git`

      `cd qtsixa`

      `make`

      `sudo make install`

- リモコンの開始
    USB A to Micro を使い, 接続の後
    `sudo sixpair`
    接続を解いて, 
    `sudo sixad -s`
- リモコンの終了
    `sudo sixad --stop`
### 参考
move\_baseの概説. まずはここから<br>
http://wiki.ros.org/move_base

navigation stack を理解する
MoriKen様の非常に有益なアルゴリズムから理解できるページ<br>
https://qiita.com/MoriKen/items/0b75ab291ab0d95c37c2

Using rviz with the Navigation Stack
navigation周りは可視化する要素が多くややこしいので必要<br>
http://wiki.ros.org/navigation/Tutorials/Using%20rviz%20with%20the%20Navigation%20Stack

Basic Navigation Tuning Guide
かなり有益. 挙動の実用的な調整に非常によい. ここからたどるリンクもよい<br>
http://wiki.ros.org/navigation/Tutorials/Navigation%20Tuning%20Guide

Navigation関連の議論と質問はこちら<br>
https://github.com/CIR-KIT/fifth_robot_pkg/issues/79
