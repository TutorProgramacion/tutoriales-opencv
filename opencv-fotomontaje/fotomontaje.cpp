#include <opencv2/photo.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int, char** argv)
{
    // Load and show images...
    Mat source = imread("source1.png", IMREAD_COLOR);
    Mat destination = imread("destination1.png", IMREAD_COLOR);
    Mat mask = imread("mask.png", IMREAD_COLOR);

    imshow("source", source);
    imshow("mask", mask);
    imshow("destination", destination);
    
    Mat result;
    
    Point p; // p will be near top right corner
    p.x = (float)2*destination.size().width/3;
    p.y = (float)destination.size().height/4;
    
    seamlessClone(source, destination, mask, p, result, NORMAL_CLONE);
    
    imshow("result", result);
    cout << "\nDone. Press any key to exit...\n";
    
    waitKey(); // Wait for key press
    return 0;
}