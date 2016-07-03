#include <iostream>
#include <cstdarg>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void getSquareImage(cv::InputArray img, cv::OutputArray dst, int size)
{
    if (size < 2) size = 2;
    int width = img.cols(), height = img.rows();

    cv::Mat square = dst.getMat();

    // si la imagen es cuadrada solo redimensionar
    if (width == height) {
        cv::resize(img, square, Size(size, size));
        return;
    }

    // establecer color de fondo del cuadrante
    square.setTo(Scalar::all(0));

    int max_dim = (width >= height) ? width : height;
    float scale = ((float)size) / max_dim;

    // calcular la region centrada 
    cv::Rect roi;

    if (width >= height)
    {
        roi.width = size;
        roi.x = 0;
        roi.height = (int)(height * scale);
        roi.y = (size - roi.height) / 2;
    }
    else
    {
        roi.y = 0;
        roi.height = size;
        roi.width = (int)(width * scale);
        roi.x = (size - roi.width) / 2;
    }

    // redimensionar imagen en la region calculada
    cv::resize(img, square(roi), roi.size());
}

void showImages(const String& window_name, int rows, int cols, int size, std::initializer_list<const Mat*> images, int pad = 1)
{
    if (pad <= 0) pad = 0;

    int width = size * cols + ((cols + 1) * pad);
    int height = size * rows + ((rows + 1) * pad);

    // crear la imagen de salida con un color de fondo blanco
    Mat dst = Mat(height, width, CV_8UC3, Scalar::all(255));

    int x = 0, y = 0, cols_counter = 0, img_counter = 0;

    // recorrer la lista de imagenes
    for (auto& img : images) {
        Mat roi = dst(Rect(x + pad, y + pad, size, size));
        
        // dibujar la imagen en el cuadrante indicado
        getSquareImage(*img, roi, size);

        // avanzar al siguiente cuadrante
        x += roi.cols + pad;

        // avanza a la siguiente fila
        if (++cols_counter == cols) {
            cols_counter = x = 0;
            y += roi.rows + pad;
        }

        // detener si no hay mas cuadrantes disponibles
        if (++img_counter >= rows * cols) break;
    }

    imshow(window_name, dst);
}

int main(int argc, char** argv)
{
    Mat image0 = imread("image/lena.jpg", CV_LOAD_IMAGE_COLOR);
    Mat image1 = imread("image/baboon.jpg", CV_LOAD_IMAGE_COLOR);
    Mat image2 = imread("image/butterfly.jpg", CV_LOAD_IMAGE_COLOR);

    Mat image4;
    cvtColor(image0, image4, CV_BGR2GRAY);
    cvtColor(image4, image4, CV_GRAY2BGR);

    Mat image5;
    filter2D(image1, image5, image5.depth(), Mat::eye(2, 2, CV_8UC1));

    Mat image6;
    Sobel(image2, image6, -1, 1, 1);

    if (!image0.empty() || !image1.empty() || !image2.empty())
    {
        namedWindow("Display Multiple Image", WINDOW_AUTOSIZE);
        showImages("Display Multiple Image", 2, 3, 240, { &image0, &image1, &image2, &image4, &image5, &image6 }, 5);
    }
    else cout << "No image data." << endl;

    waitKey(0);
    return 0;
}