#include <opencv2\highgui.hpp>
#include <opencv2\core\ocl.hpp>
#include <opencv2\videoio.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace ocl;
using namespace std;

void printInfo() {
    vector<ocl::PlatformInfo> info;
    getPlatfomsInfo(info);

    PlatformInfo sdk = info.at(0);

    if (sdk.deviceNumber() < 1) return;

    cout << "******SDK*******" << endl;
    cout << "Name: " << sdk.name() << endl;
    cout << "Vendor: " << sdk.vendor() << endl;
    cout << "Version: " << sdk.version() << endl;
    cout << "Number of devices: " << sdk.deviceNumber() << endl;

    for (int i = 0; i < sdk.deviceNumber(); i++) {
        Device device;
        sdk.getDevice(device, i);

        cout << "\n*********************\nDevice " << i + 1 << endl;
        cout << "Vendor ID: " << device.vendorID() << endl;
        cout << "Vendor name: " << device.vendorName() << endl;
        cout << "Name: " << device.name() << endl;
        cout << "Driver version: " << device.driverVersion() << endl;

        if (device.isAMD()) cout << "<-- Is an AMD device -->" << endl;
        if (device.isIntel()) cout << "<-- Is a Intel device -->" << endl;
        if (device.isNVidia()) cout << "<-- Is a NVidia device -->" << endl;

        cout << "Global Memory size: " << device.globalMemSize() << endl;
        cout << "Memory cache size: " << device.globalMemCacheSize() << endl;
        cout << "Memory cache type: " << device.globalMemCacheType() << endl;
        cout << "Local Memory size: " << device.localMemSize() << endl;
        cout << "Local Memory type: " << device.localMemType() << endl;
        cout << "Max Clock frequency: " << device.maxClockFrequency() << endl;
    }
}

int main()
{
    printInfo();

    VideoCapture capture;

    if (!capture.open(0)) cout << "Error al abrir la camara." << endl;

    const String titleWindow = "%.2f FPS :: OpenCL %s :: [q] para %s";
    const String nameWindow = "OpenCL & OpenCV (T-API)";

    double elapsed_time = 0;
    double start_time = 0;
    double avgfps = 0;
    unsigned int nframes = 0;

    namedWindow(nameWindow);

    while (true) 
    {
        double t = (double)getTickCount();

        UMat uframe;
        capture >> uframe;

        if (uframe.empty()) continue;

        medianBlur(uframe, uframe, 3);
        blur(uframe, uframe, Size(11, 11));

        char key = (char)waitKey(10);

        if (key == 27) break;
        if (key == 'q') ocl::setUseOpenCL(!ocl::useOpenCL());

        t = (double)getTickCount() - t;
        double fps = getTickFrequency() / t;
        double alpha = ++nframes > 50 ? 0.01 : 1. / nframes;
        avgfps = avgfps * (1 - alpha) + fps * alpha;

        String message = format(titleWindow.c_str(), avgfps,
            ocl::useOpenCL() ? "ON" : "OFF",
            ocl::useOpenCL() ? "desactivar" : "activar");

        Mat frm = uframe.getMat(ACCESS_RW);
        Mat rectangle_src(25, 412, CV_8UC3, Scalar(20, 20, 20));
        Mat rectangle_dst = frm(Rect(5, 5, 412, 25));

        addWeighted(rectangle_src, 0.5, rectangle_dst, 1.0 - 0.5, 0.0, rectangle_dst);

        putText(frm, message, Point(10, 20), FONT_HERSHEY_TRIPLEX, 0.5, Scalar(0, 255, 0), 1, LINE_AA);

        imshow(nameWindow, frm);
    }

    capture.release();
    return 0;
}
//
//int __main(int, char** argv)
//{
//
//    //String dir = "C:/Developer/TEMP/opencv-master/samples/data/";
//
//    // Load and show images...
//    Mat source = imread("image.jpg", IMREAD_COLOR);
//    Mat destination = imread("destino2.jpg", IMREAD_COLOR);
//    Mat mask = imread("mask.jpg", IMREAD_COLOR);
//
//    imshow("source", source);
//    imshow("mask", mask);
//    imshow("destination", destination);
//
//    Mat result;
//
//    Point p; // p will be near top right corner
//    p.x = (float)/*2 **/ destination.size().width / 3;
//    p.y = (float)destination.size().height / 3;
//
//    seamlessClone(source, destination, mask, p, result, NORMAL_CLONE);
//    imshow("result", result);
//
//    cout << "\nDone. Press any key to exit...\n";
//
//    waitKey(); // Wait for key press
//    
//               
//    //double t = (double)getTickCount();
//    // ...
//    //t = (double)getTickCount() - t;
//    //double fps = getTickFrequency() / t;
//    //static double avgfps = 0;
//    //static int nframes = 0;
//    //nframes++;
//    //double alpha = nframes > 50 ? 0.01 : 1. / nframes;
//    //avgfps = avgfps*(1 - alpha) + fps*alpha;
//
//    //putText(uframe, format("OpenCL: %s, fps: %.1f", ocl::useOpenCL() ? "ON" : "OFF", avgfps),
//    //    Point(50, 30), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 255, 0), 2);
//
//
//    return 0;
//}
//
