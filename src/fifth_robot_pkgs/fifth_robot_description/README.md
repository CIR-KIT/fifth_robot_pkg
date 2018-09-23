# fifth robot simulator
***
## シミュレーション上でラジコン操作 
ラジコン操作でロボットを操作し、データを取得する。
1. `roslaunch fifth_robot_description teleop_keyboard.launch`  
   シミュレータを立ち上げ、キーボード入力で操作できる。  
   末尾に`world_name:=willowgarage`を付加すると gazeboの`world` を変更できる。  
   デフォルトで `clearpath_playpen`が読み込まれる。
  
2. 別ターミナルで  
   `rosbag record --all -O rosbag_raw`  
   bag データを録る。`rosbag_raw`は bag ファイルの名前になる。
***
## bag ファイルのフィルタリング
上で録れた bag ファイルをフィルタリングする必要がある。  
#### 2D Navigation 用
`roslaunch fifth_robot_description bag_filter_2d.launch bagfile:="rosbag_rawへの絶対パス"` 

#### 3D Navigation 用
`roslaunch fifth_robot_description bag_filter_3d.launch bagfile:="rosbag_rawへの絶対パス"` 

デフォルトで `filtered_(2d|3d).bag`が `~/.ros/` 内に作成される。  
どちらも、末尾に`output_name="hoge"`を付加すると出力される bag ファイルの名前を変更できる。
***
## 地図作成 (2D)
1. gmapping を走らせる。  
   `roslaunch fifth_robot_description gmapping.launch bagfile:="filtered_2d.bag"への絶対パス`
   
2. 上の bag play が終了したら別のターミナルで  
   `rosrun map_server map_saver -f 2d_map`  
   とすると 2D のマップが出来る。`2d_map`の部分はマップファイルの名前になる。  
***
## 地図作成 (3D)（調整中）
1. cartographer_offline_node を走らせる。  
   `roslaunch fifth_robot_description offline_cartographer.launch bagfile:="fiftered_3d.bag"への絶対パス`
***
## waypoint 作成 (2D)
1. `roslaunch fifth_robot_description generate_waypoint.launch  
   bagfile:="filtered_3d.bagへの絶対パス"  
   mapfile:="2d_map.yamlへの絶対パス"`  
 
2. 上の bag play が終了したら別のターミナルで  
   `rosrun ros_waypoint_generator ros_waypoint_saver`
   とすると waypoint (csv) ファイルが出来る。
***
 ## Navigation (2D)
 * テスト  
   `roslaunch fifth_robot_description planning_test.launch mapfile:="2d_map.yamlへの絶対パス"`  
   
 * waypoint 使用  
   `roslaunch fifth_robot_description navigation.launch mapfile:="2d_map.yamlへの絶対パス" waypoint=`  
  
 末尾に`world_name:=willowgarage`を付加すると gazeboの`world` を変更できる。  
 デフォルトで `clearpath_playpen`が読み込まれる。  
 また `mapfile`、`waypoint`を省略するとサンプルが読み込まれる。  

## Navigation (3D)
edit required
