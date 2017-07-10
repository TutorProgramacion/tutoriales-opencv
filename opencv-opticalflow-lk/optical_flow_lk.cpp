#include <iostream>
#include <vector>
#include <deque>

#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

void main() 
{
    String window = "OpticalFlow :: Lucas-Kanade";
    VideoCapture capture(0);

    deque<vector<Point2f>> records;
    vector<Point2f> points[2];

    Mat prev_gray;

    Scalar COLOR(255, 255, 0);

    // crear la ventana
    namedWindow(window);

    // bucle de captura de video
    while (true) {

        // capturar el cuadro actual
        Mat frame, gray;
        capture >> frame;

        // si no hay datos continuar
        if (frame.empty()) continue;

        // convertir a escala de grises
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        // verificar si hay puntos anteriores
        if (!points[0].empty()) 
        {
            vector<uchar> status;

            // aseguarse de que la imagen anterior no esta vacia
            if (prev_gray.empty()) {
                gray.copyTo(prev_gray);
            }

            // calcular flujo optico con el metodo de Lucas-Kanade
            calcOpticalFlowPyrLK(prev_gray, gray, points[0], points[1], status, cv::noArray());

            // guardar los puntos obtenidos
            records.push_front(points[1]);

            // guardar solo un maximo de 10 conjuntos de puntos anteriores
            if (records.size() >= 10) {
                records.pop_back();
            }

            // dibujar los datos
            for (size_t i = 0; i < points[1].size(); i++)
            {
                if (!status[i]) continue;

                // dibujar el conjunto de lineas guardo previamente, 
                // estos representan el movimiento de los puntos.
                for (size_t j = 0; j < records.size() - 1; j++)
                    line(frame, records[j][i], records[j + 1][i], COLOR, 1, LINE_AA);

                // dibujar los puntos de interes
                circle(frame, points[1][i], 3, COLOR, FILLED, LINE_AA);
            }
        }

        // esperar por 30 ms ha que se presione una tecla
        char key = (char)waitKey(30);

        // si la tecla es ESC salir, cualquier otra captura los puntos caracteristicos.
        if (key == 27) break;
        else if (key != -1)
        {
            // obtener los puntos de interes
            goodFeaturesToTrack(gray, points[1], 100, 0.01, 10);

            // limpiar los puntos previos
            records.clear();
        }

        // mostrar la imagen
        imshow(window, frame);

        // intercambiar los puntos, los actuales pasan a ser los anteriores.
        std::swap(points[1], points[0]);

        // intercambiar las imagenes, la actual es ahora la anterior.
        cv::swap(prev_gray, gray);
    }
}