#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat image;
int h = 110, r = 40;

void colorAccent(Mat& src, Mat& dst, int h, int r)
{
    Mat hsv;
    cvtColor(src, hsv, CV_BGR2HSV);

    int channels = src.channels();
    int nRows = src.rows;
    int nCols = src.cols * channels;

    dst.create(src.rows, src.cols, src.type());

    if (src.isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }

    uchar h1 = (h - (r / 2) + 360) % 360;
    uchar h2 = (h + (r / 2) + 360) % 360;

    uchar *ptr_dst, *ptr_src, *ptr_hsv;
    bool in_range_color = false;

    for (int i = 0; i < nRows; i++) {
        ptr_dst = dst.ptr<uchar>(i); 
        ptr_src = src.ptr<uchar>(i);
        ptr_hsv = hsv.ptr<uchar>(i);

        for (int j = 0; j < nCols; j += 3) {
            in_range_color = false;

            // obtener el valor hue (H)
            uchar H = ptr_hsv[j];

            // verificar si se encuantra en el rango indicado
            if (h1 <= h2) {
                if (H >= h1 && H <= h2)
                    in_range_color = true;
            }
            else if (H >= h1 || H <= h2)
                in_range_color = true;

            // si esta en el rango conservar el color
            // en caso contrario convertir a grises
            if (in_range_color == true) {
                ptr_dst[j + 0] = ptr_src[j + 0];
                ptr_dst[j + 1] = ptr_src[j + 1];
                ptr_dst[j + 2] = ptr_src[j + 2];
            }
            else {
                // conversion a grises usando usando en metodo de promedio
                uchar gray = (ptr_src[j] + ptr_src[j + 1] + ptr_src[j + 2]) / 3;
                ptr_dst[j + 2] = ptr_dst[j + 1] = ptr_dst[j] = gray;
            }
        }
    }

}

void showAccentColor() {
    Mat dst;
    colorAccent(image, dst, h, r);
    imshow("Accent Color", dst);
}

void tb_hue_callback(int v, void *) { showAccentColor(); }

void tb_range_callback(int v, void *) { showAccentColor(); }

int main(int argc, char** argv)
{
    const String window_name = "Original Image";

    image = imread("image/messi.jpg", CV_LOAD_IMAGE_COLOR);

    if (image.empty()) {
        printf("No image data \n");
        return -1;
    }

    namedWindow("Original Image", WINDOW_AUTOSIZE);

    createTrackbar("Hue  ", window_name, &h, 255, tb_hue_callback);
    createTrackbar("Range", window_name, &r, 100, tb_range_callback);

    imshow(window_name, image);
    showAccentColor();

    waitKey(0);
    return 0;
}