#include "CameraCapture.h"
#include <opencv4/opencv2/core/mat.hpp>

CameraCapture::CameraCapture() {
    this->frameWidth_ = FRAME_WIDTH_DEFAULT;
    this->frameHeight_ = FRAME_HEIGHT_DEFAULT;
    this->isHD_ = false;
    this->is60FPS_ = false;
    this->cameraId_ = -1;
    this->frameRate_ = 0;
    this->isFinished_ = false;
}

CameraCapture::~CameraCapture() {
    this->isFinished_ = true;
    if (this->capture_.isOpened())
        this->capture_.release();
}

bool CameraCapture::init(int cameraId) {
    this->frameWidth_ = FRAME_WIDTH_HD_DEFAULT;
    this->frameHeight_ = FRAME_HEIGHT_HD_DEFAULT;
    
    if(this->capture_.isOpened()) {
        this->capture_.release();
    }

    this->cameraId_ = cameraId;

    this->capture_.open(this->cameraId_, cv::CAP_V4L2);
    if (!this->capture_.isOpened()) {
        throw std::runtime_error("Could not open camera.");
    }

    try {
        this->capture_ >> this->frame_;
    } catch (cv::Exception& e) {
        std::cerr << "Exception occurred. Ignoring frame." << std::endl;
        return false;
    }

    this->capture_.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));

    if(this->frameHeight_ != this->capture_.get(cv::CAP_PROP_FRAME_HEIGHT) || this->frameWidth_ != this->capture_.get(cv::CAP_PROP_FRAME_WIDTH)) {
        this->is60FPS_ = false;
    }

    double fps = this->capture_.get(cv::CAP_PROP_FPS);

    if(static_cast<int>(fps) >= 60)
        this->is60FPS_ = true;
    else
        this->is60FPS_ = false;
    
    this->frameRate_ = static_cast<int>(fps);

    return true;
}

void CameraCapture::update(){
    cv::Mat frame;
    if(!this->capture_.isOpened()) {
        std::cerr << "Camera is not opened." << std::endl;
        frame = cv::Mat::zeros(this->frameHeight_, this->frameWidth_, CV_8UC3) * 255;
    } else {
        if(!this->isFinished_)
            this->capture_ >> frame;
    }

    this->frameMutex_.lock();
    this->frame_ = frame;
    this->frameMutex_.unlock();

    this->isFrameAvailable_ = true;
}

cv::Mat CameraCapture::getFrame() {
    return this->frame_;
}

