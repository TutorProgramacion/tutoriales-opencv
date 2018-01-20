#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
    Mat image = imread("image/rgb.png", CV_LOAD_IMAGE_COLOR);

    if (image.empty())
    {
        printf("No se ha podido leer la imagen.\n");
		getchar();
        return -1;
    }

    imshow("Imagen Original", image);

	std::vector<Mat> channels;
	split(image, channels);

	Mat zero = Mat::zeros(image.size(), CV_8UC1);

	std::vector<Mat> B = { channels[0], zero, zero };
	std::vector<Mat> G = { zero, channels[1], zero };
	std::vector<Mat> R = { zero, zero, channels[2] };

	Mat rdst, gdst, bdst;

	merge(R, rdst);
	merge(G, gdst);
	merge(B, bdst);

	imshow("R Channel", rdst);
	imshow("G Channel", gdst);
	imshow("B Channel", bdst);

    waitKey(0);
    return 0;
}