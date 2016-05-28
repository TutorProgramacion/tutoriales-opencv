#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

void drawLine(Mat histImage, int bin_w, int i, int height, Mat b_hist, Scalar color) {
    line(histImage,
        Point(bin_w*(i - 1), height - cvRound(b_hist.at<float>(i - 1))),
        Point(bin_w*(i), height - cvRound(b_hist.at<float>(i))),
        color);
}

void drawHistogram(cv::InputArray img)
{
    // Dividir los canales BRG
    vector<Mat> bgr;
    split(img, bgr);

    // Crear un histograma con 256 bin (numero de subdivisiones) uno por pixel [0..256]
    int numbins = 256;

    // Establecer rango para los canales (B, G, R)
    float range[] = { 0, 256 };
    const float* histRange = { range };

    Mat b_hist, g_hist, r_hist;

    // Calcular el histograma para cada canal
    calcHist(&bgr[0], 1, 0, Mat(), b_hist, 1, &numbins, &histRange);
    calcHist(&bgr[1], 1, 0, Mat(), g_hist, 1, &numbins, &histRange);
    calcHist(&bgr[2], 1, 0, Mat(), r_hist, 1, &numbins, &histRange);

    // Tamano del histograma
    int h_width = 512;
    int h_height = 400;

    // Crear una imagen para dibujar en ella
    Mat histImage(h_height, h_width, CV_8UC3, Scalar(20, 20, 20));

    // Normalizar los histogramas 
    normalize(b_hist, b_hist, 0, h_height, NORM_MINMAX);
    normalize(g_hist, g_hist, 0, h_height, NORM_MINMAX);
    normalize(r_hist, r_hist, 0, h_height, NORM_MINMAX);

    int bin_width = cvRound((float)h_width / (float)numbins);

    // Dibujar cada una de las lineas
    for (int i = 1; i < numbins; i++)
    {
        drawLine(histImage, bin_width, i, h_height, b_hist, Scalar(255, 0, 0));
        drawLine(histImage, bin_width, i, h_height, g_hist, Scalar(0, 255, 0));
        drawLine(histImage, bin_width, i, h_height, r_hist, Scalar(0, 0, 255));
    }

    // Mostrar el histograma
    imshow("Histograma BGR", histImage);
}

void equalizeColorImage(cv::InputArray src, cv::OutputArray dst)
{
    // Convertir BGR image a YCrCb
    Mat ycrcb;
    cvtColor(src, ycrcb, COLOR_BGR2YCrCb);

    // Extarer cada uno de los canales
    vector<Mat> channels;
    split(ycrcb, channels);

    // Ecualizar histograma del canal Y (luminosidad) 
    equalizeHist(channels[0], channels[0]);

    // Unir los canales nuevamente
    merge(channels, ycrcb);

    // Convertir color YCrCb a BGR
    cvtColor(ycrcb, dst, COLOR_YCrCb2BGR);
}

int main(int, char** argv)
{
    cv::Mat src, dst, hist;

    std::string original = "Imagen Original";
    std::string equalized = "Imagen Equalizada";

    // leer la imagen
    src = cv::imread("image/lena.jpg", 1);
    if (src.empty()) return -1;

    equalizeColorImage(src, dst);
    drawHistogram(src);

    //// convertir en escala de grises y ecualizar histograma
    //cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
    //cv::equalizeHist(src, dst);

    // mostrar la imagen original y ecualizada
    cv::imshow(original, src);
    cv::imshow(equalized, dst);

    cv::waitKey(0);
    return 0;
}