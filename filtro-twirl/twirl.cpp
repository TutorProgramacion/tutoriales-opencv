#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void trackbar_callback(int value, void* userdata)
{
    Mat* image = (Mat*)userdata;

    const float width = (float)image->rows;
    const float height = (float)image->cols;

    Mat result(image->rows, image->cols, image->type());

    for (int i = 0; i < image->rows; i++) {
        for (int j = 0; j < image->cols; j++) {

            float x = (j / height) - 0.5f;
            float y = (i / width) - 0.5f;

            float angle = atan2f(y, x);
            float radius = sqrtf((x * x) + (y * y));

            angle += radius * (value / 10.0f);

            float xr = ((radius * sinf(angle)) + 0.5f) * width;
            float yr = ((radius * cosf(angle)) + 0.5f) * height;
      
            int k = (int)std::min(width - 1, std::max(0.0f, xr));
            int m = (int)std::min(height - 1, std::max(0.0f, yr));

            uchar* src = image->ptr<uchar>(k, m);
            uchar* out = result.ptr<uchar>(i, j);

            out[0] = src[0];
            out[1] = src[1];
            out[2] = src[2];
        }
    }

    imshow("Result Image", result);
}

int main(int argc, char** argv)
{
    Mat image = imread("data/lena.jpg", CV_LOAD_IMAGE_COLOR);

    if (image.empty())
    {
        printf("No image data \n");
        return -1;
    }

    try
    {
        const cv::String name_window = "Twirl Image";
        const cv::String name_trackbar = "Twirl";

        namedWindow(name_window);
        createTrackbar(name_trackbar, name_window, NULL, 200, trackbar_callback, &image);
        setTrackbarPos(name_trackbar, name_window, 80);
        imshow(name_window, image);

        waitKey(0);
        destroyAllWindows();
    }
    catch (cv::Exception& e)
    {
        const char* err_msg = e.what();
        std::cout << "exception caught: " << err_msg << std::endl;
    }

    return 0;
}