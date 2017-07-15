#include <opencv2\opencv.hpp>

#include "cvui.h"

#define WINDOW_NAME "CVUI Test"

int main(int argc, const char *argv[])
{
    cv::Mat src = cv::imread("imagen/lena.jpg");
    cv::Mat frame;

    cv::namedWindow(WINDOW_NAME);
    cvui::init(WINDOW_NAME);

    bool isGrayImage = false;
    int ksize = 1;

    while (true) {
        frame = src.clone();

        if (isGrayImage) {
            cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        }

        cv::blur(frame, frame, cv::Size(ksize, ksize));

        cvui::checkbox(frame, 10, 28, "Escala de grises", &isGrayImage);
        cvui::trackbar(frame, 160, 10, 340, &ksize, 1, 50);
        cvui::printf(frame, 10, 490, "Tutor de programacion :: Blur size: %d", ksize);

        cvui::update();

        cv::imshow(WINDOW_NAME, frame);

        if(cv::waitKey(30) == 27) break;
    }

    return 0;
}