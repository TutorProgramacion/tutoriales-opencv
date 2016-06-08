#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void pixelate(Mat& src, Mat& dst, int pixel_size = 1) {
    try {
        dst.create(src.rows, src.cols, src.type());

        Rect rect;

        for (int r = 0; r < src.rows; r += pixel_size)
        {
            for (int c = 0; c < src.cols; c += pixel_size)
            {
                rect.x = c;
                rect.y = r;
                rect.width = c + pixel_size < src.cols ? pixel_size : src.cols - c;
                rect.height = r + pixel_size < src.rows ? pixel_size : src.rows - r;

                Scalar color = mean(Mat(src, rect));
                rectangle(dst, rect, color, CV_FILLED);
            }
        }
    }
    catch (cv::Exception &ex) {
        cout << ex.what() << endl;
    }
}

void ontrack(int value, void* data) {
    if (value <= 0 || data == nullptr) return;

    Mat dst;
    pixelate(*((Mat*)data), dst, value);
    imshow("Pixelate Effect", dst);
}

int main(int argc, char** argv)
{
    Mat image = imread("image/lena.jpg", CV_LOAD_IMAGE_COLOR);

    if (image.empty()) {
        printf("No image data \n");
        return -1;
    }

    int pixel_size = 8;

    namedWindow("Pixelate Effect", WINDOW_AUTOSIZE);
    createTrackbar("Pixel Size", "Pixelate Effect", &pixel_size, 50, ontrack, &image);

    ontrack(pixel_size, &image);

    waitKey(0);
    destroyAllWindows();

    return 0;
}