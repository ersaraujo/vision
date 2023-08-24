#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include <mutex>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <thread>
#include <atomic>

#define FRAME_WIDTH_DEFAULT 640
#define FRAME_HEIGHT_DEFAULT 480
#define FRAME_WIDTH_HD_DEFAULT 1280
#define FRAME_HEIGHT_HD_DEFAULT 720

#if (CV_VERSION_MAJOR < 4)
#error "The OpenCV version is incompatible. Version 4 or higher is required."
#endif

class CameraCapture {
public:
    CameraCapture();
    ~CameraCapture();
    
    bool init(int cameraId);
    void update();
    cv::Mat getFrame();
    
    int getFPS() const {
        return this->frameRate_;
    }

private:  
    cv::VideoCapture capture_;

    int frameWidth_;
    int frameHeight_;
    bool isHD_;
    bool is60FPS_;  
    int frameRate_;

    int cameraId_;

    cv::Mat frame_;
    std::atomic<bool> isFinished_;
    std::atomic<bool> isFrameAvailable_;

    std::mutex frameMutex_;


};


#endif /* CAMERACAPTURE_H */