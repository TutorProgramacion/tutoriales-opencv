#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
    Mat image = imread("image/lena.jpg", CV_LOAD_IMAGE_COLOR);

    if (image.empty())
    {
        printf("No image data \n");
        return -1;
    }

    imshow("Original Image", image);

    //------ pyrDown --------//
    Mat image1 = image.clone();

    for (size_t i = 0; i < 3; i++)
    {
        pyrDown(image1, image1);
        imshow(to_string(image1.size().width), image1);
    }

    ////------ pyrUp --------//
    //Mat image2 = image.clone();

    //for (size_t i = 0; i < 3; i++)
    //{
    //    pyrUp(image2, image2);
    //    imshow(to_string(image2.size().width), image2);
    //}

    ////------ Laplacian --------//
    //Mat temp1, temp2, temp3;
    //image.copyTo(temp1);

    //for (size_t i = 0; i < 4; i++)
    //{
    //    pyrDown(temp1, temp2);
    //    pyrUp(temp2, temp3, temp1.size());

    //    Mat lap = temp1 - temp3;

    //    imshow(to_string(lap.size().width), lap);
    //    temp1 = temp2;
    //}

    waitKey(0);
    return 0;
}