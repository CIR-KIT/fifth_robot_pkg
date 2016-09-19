# CIRKIT-5

CIRKIT所有ロボット5号機のリポジトリ

## 仕様
- 開発環境は ROS Kinetic Kameを推奨
- ハードウェア要件は i-cart mini に準ずるものとする:[公式](http://t-frog.com/products/icart_mini/)

## テスト記録
- 旧ハードについてはROS Indigo にて動作確認済み
- 新ハード(本番用)については後日調査(以降は, Kinetic Kameによって開発を行う)

##　メモ
- 使うときは各人セットアップ済みの catkin_ws に clone して使ってください
- セットアップについて,__yp-spur,ssmのインストール__を忘れずに行ってください:[公式](http://www.roboken.iit.tsukuba.ac.jp/platform/wiki/yp-spur/how-to-install)
- contributer を募集しています

##　インストレーション注意書き
- ソフトウェア的には 誰かの作った i-cart mini 操作用のソフトを使うことになってます [wiki](http://wiki.ros.org/icart_mini)が, これのインストールについて特記するべきことがあるので記します
- __wikiどうりにはやりません__
- これはindigo環境下で試験的に実行した成功例の一つです, アンチパターンなどの一般化はかけていますので, 適宜実行してください.
- 多分Kineticでもうまく往くと思いますが, 問題の切り分けがうまく行ってないので, 近日実行ということにしておきます.

## インストレーション・マニュアル
- __いまからビルド作業を行うディレクトリが, いままで catkin_make__など__行っていない__ことを確認しつつ,　必要であれば  
mkdir -p catkin_ws/src
- →さもなくば後でcatkin build と catkin_make で競合エラーを吐きます
- wikiで紹介されているやつ __ではなく__ (恐らくは) 開発用のほう, [こちら](https://github.com/DaikiMaekawa/ypspur.git)
- をcloneします  git clone https://github.com/DaikiMaekawa/ypspur.git
- 恐らく今のROS環境では標準でインストールされておらず, 不足を生じるであろうツールがあるのでインストール  sudo apt-get install python-catkin-tools  sudo apt-get install catkin
- cd ../  
-  catkin build ypspur  
- wstool init  
-  rosdep install --from-paths . --ignore-src --rosdistro indigo -y  
-  ここで恐らくypspurが重複していると言われるはずなので,src内の方(もう片方は,catkin buildの生成物のどこか)を削除します
-  rm -rf src/ypspur/  
-  rosdep install --from-paths . --ignore-src --rosdistro indigo -y   
-  catkin build

