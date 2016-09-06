#include <ros/ros.h>
#include <opencv2/opencv.hpp>

int main(){
cv::VideoCapture cap(0);
cv::Mat frame;

if(!cap.isOpened()) return 999;

while(1){
   cap >> frame;
   cv::imshow("WEB camera", frame);

   int key = cv::waitKey(1);
   if(key == 'q'){
      break;
   }else if(key == 's'){
      cv::imwrite("WEB_cam.png", frame);
   }
   }
   cv::destroyAllWindows();
   return 0;
}
