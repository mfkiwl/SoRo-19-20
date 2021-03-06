#include <vector>
#include <iostream>
#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <aruco/markerdetector.h>
#include <opencv2/flann.hpp>
#include <opencv2/aruco.hpp>
#include <unistd.h>

/*NOTE: Certain settings need adjusted before code is run. To do this run the code below:
    v4l2-ctl --list-devices #to find the device you want. Below I assume /dev/video1
    v4l2-ctl -d /dev/video1 --set-ctrl=focus_auto=0
    v4l2-ctl -d /dev/video1 --set-ctrl=focus_absolute=0
    v4l2-ctl -d /dev/video1 --set-ctrl=contrast=255
    v4l2-ctl -d /dev/video1 --set-ctrl=sharpness=255
    
    This can also be done with v4l2ucp for the GUI lovers
*/

//this program takes a picture of an artag at 100cm away that is 20cm wide and returns the focal length for cm
int main()
{   
    aruco::MarkerDetector MDetector; 
    std::vector<aruco::Marker> Markers;
    MDetector.setDictionary("../urc.dict");
    
    cv::VideoCapture cap("/dev/video0"); 
    cap.set(cv::CAP_PROP_FRAME_WIDTH,1920); //resolution set at 640x480
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);
    
    cv::Mat image;
    
    while(true)
    {
        cap >> image;
        Markers = MDetector.detect(image);
        
        double widthOfTag = 0;
        if(Markers.size() > 0)
            widthOfTag = Markers[0][1].x - Markers[0][0].x;
        else
            std::cout << "nothing found" << std::endl;
            
        std::cout << "Focal Length: " << ((widthOfTag * 100.0) / 20.0) << std::endl;
        cv::imshow("win", image);
        cv::waitKey(100);
    }
}
