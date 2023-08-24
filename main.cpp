#include <iostream>
#include <opencv2/opencv.hpp>
#include "CameraCapture/CameraCapture.h"

int main() {
    CameraCapture camera;

    camera.init(1);
    
    while (true) {
        camera.update();
        cv::Mat frame = camera.getFrame();

        if (frame.empty()) {
            std::cerr << "Quadro vazio recebido." << std::endl;
            break;
        }

        // Realize as operações de pré-processamento ou processamento aqui

        cv::imshow("Captura de Câmera", frame);
        std::cout << "FPS: " << camera.getFPS() << std::endl;

        char key = cv::waitKey(1);
        if (key == 27)
            break;
    }

    return 0;
}
