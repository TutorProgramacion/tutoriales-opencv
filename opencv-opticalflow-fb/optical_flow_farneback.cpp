#include <iostream>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

// Function to compute the optical flow map
void drawOpticalFlow(const Mat& flowImage, const Mat& flowImageGray)
{
    int stepSize = 16;
    Scalar color = Scalar(0, 255, 0);

    // Draw the uniform grid of points on the input image along with the motion vectors
    for (int y = 0; y < flowImageGray.rows; y += stepSize)
    {
        for (int x = 0; x < flowImageGray.cols; x += stepSize)
        {
            // Circles to indicate the uniform grid of points
            circle(flowImageGray, Point(x, y), 1, color, FILLED);

            // Lines to indicate the motion vectors
            const Point2f& pt = flowImage.at<Point2f>(y, x);
            line(flowImageGray, Point(x, y), Point(cvRound(x + pt.x), cvRound(y + pt.y)), color);
        }
    }
}

void main()
{
    String window = "OpticalFlow :: Farneback";
    VideoCapture capture(0);
    Mat prev_gray;

    // crear la ventana
    namedWindow(window);

    // bucle de captura de video
    while (true) {

        Mat frame, gray, flow;

        // capturar el cuadro actual
        capture >> frame;

        // si no hay datos continuar
        if (frame.empty()) continue;

        // escalar a la mitad, para mejorar rendimiento
        resize(frame, frame, Size(), 0.6, 0.6, INTER_LINEAR);

        // convertir a escala de grises
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        if (!prev_gray.empty())
        {
            calcOpticalFlowFarneback(prev_gray, gray, flow, 0.5, 3, 15, 3, 5, 1.1, 0);
            drawOpticalFlow(flow, frame);
        }

        // esperar por 30 ms ha que se presione una tecla
        if(waitKey(30) == 27) break;

        // mostrar la imagen
        imshow(window, frame);

        // intercambiar las imagenes, la actual es ahora la anterior.
        cv::swap(prev_gray, gray);
    }
}