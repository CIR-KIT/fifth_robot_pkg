# fifth_robot & cartographer

進捗はコミットログを見るから良いとして、    
使い方とか分かったことがあったら[このページ](https://github.com/CIR-KIT/fifth_robot_pkg/edit/develop/use-cartographer/src/fifth_robot_pkgs/fifth_robot_description/README.md)  
困ったこと・わからないことがあったら 
[Issue](https://github.com/CIR-KIT/fifth_robot_pkg/issues/new) もしくは [Projects](https://github.com/CIR-KIT/fifth_robot_pkg/projects/8)の方に書いてください。  

## 使用方法
[Edit Required](https://github.com/CIR-KIT/fifth_robot_pkg/edit/develop/use-cartographer/src/fifth_robot_pkgs/fifth_robot_description/README.md)  

## 参考文献

公式ドキュメント  
* [Cartographer (TOP)](https://google-cartographer.readthedocs.io/en/latest/index.html)  
* [Cartographer_ROS (TOP)](https://google-cartographer-ros.readthedocs.io/en/latest/)  
* [Cartographer_ROS (ROS API)](https://google-cartographer-ros.readthedocs.io/en/latest/ros_api.html)  
* [Cartographer (configuration)](https://google-cartographer.readthedocs.io/en/latest/configuration.html)  
* [Cartographer_ROS (configuration)](https://google-cartographer-ros.readthedocs.io/en/latest/configuration.html)  
* [read_the_docs (Cartographer)](https://media.readthedocs.org/pdf/google-cartographer/latest/google-cartographer.pdf)
* [read_the_docs (Cartographer_ROS)](https://media.readthedocs.org/pdf/google-cartographer-ros/latest/google-cartographer-ros.pdf)  

Github
* [googlecartographer/cartographer](https://github.com/googlecartographer/cartographer)  
* [googlecartographer/cartographer_ros](https://github.com/googlecartographer/cartographer_ros)  
* [takuyani/SLAM-Robot_Code](https://github.com/takuyani/SLAM-Robot_Code)
* [ros-japan-users/cartographer_example](https://github.com/ros-japan-users/cartographer_example)


Qiita  
* [GoogleのCartographerをシミュレーターで使いたいだけの人生だった[失敗談]](https://qiita.com/syoamakase/items/ea3ad5040cd53507ca5e)  
* [Google Cartographer でSLAMデータを保存する](https://qiita.com/nnn112358/items/f5495f6b9abaf5ceb117)  
* [RaspberryPi3と(中略)_009日目_SLAM_Google Cartographer_LiDAR(A1M8)](https://qiita.com/PINTO/items/4845c438cac05eda4d1e)  
* [RaspberryPi3と(中略)_015日目_SLAM_自律走行の前準備_CartoGrapherによる自宅の地図作成](https://qiita.com/PINTO/items/d8fd7a91ee00df7702b7)  
* [ROSとGazeboによる自律移動シミュレーション 〜SLAMの実装〜](https://qiita.com/Crafty_as_a_Fox/items/bf9ead1056f841f82f6a)  
* [ROS31:Cartographerを動かすまで](https://qiita.com/NextWorld/items/4fc3f62b08b5a455c925)
* [ROS32:cartgrapher(2D)のデータを保存、再生する。](https://qiita.com/NextWorld/items/8f215e98adbff6e98ae1)  
* [ROS33:cartgrapher(3D)を動かす。](https://qiita.com/NextWorld/items/80858bba07940f2c1f42)  

Tips  
* [How to view Pb format file？ #216](https://github.com/googlecartographer/cartographer/issues/216)  
* [Confusion about TF frames used #300](https://github.com/googlecartographer/cartographer_ros/issues/300)

Other
## [takuyani/SLAM-Robot_Code](https://github.com/takuyani/SLAM-Robot_Code)のビルド方法
リポジトリそのものがsrc内にあるべきファイル群なので次のような手順を踏みます  
1.   
  `mkdir slambot_ws ;cd slambot_ws`  
2.   
  `git clone git@github.com:takuyani/SLAM-Robot_Code.git src ;cd src`  
3.   
  `catkin_init_workspace ;cd ../`  
4.   
  `catkin_make`  
  
  これでビルドは通ります。
