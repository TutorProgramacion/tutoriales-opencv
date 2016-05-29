#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int, char** argv)
{
    const char* image_window = "Source Image";
    const char* result_window = "Result window";

    Mat img_src = imread("image/messi.jpg", 1);
    Mat templ = imread("image/templ.jpg", 1);

    // obtener las dimensiones para el cv::Mat resultado
    int result_cols = img_src.cols - templ.cols + 1;
    int result_rows = img_src.rows - templ.rows + 1;

    Mat result(result_rows, result_cols, CV_32FC1);

    // establecer el match template mode
    cv::TemplateMatchModes match_mode = cv::TemplateMatchModes::TM_CCOEFF_NORMED;

    // aplicar match template con el modo indicado y normalizar el resultado
    matchTemplate(img_src, templ, result, match_mode);
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    double minVal, maxVal;
    Point minLoc, maxLoc, matchLoc;

    // obtener max y min junto con sus ubicaciones
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

    if (match_mode == TM_SQDIFF || match_mode == TM_SQDIFF_NORMED)
        matchLoc = minLoc;
    else
        matchLoc = maxLoc;

    // dibujar el rectangulo en la posicion encontrada
    rectangle(img_src, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(0, 255, 0), 2, CV_AA, 0);

    // mostrar las imagenes
    imshow(result_window, result);
    imshow(image_window, img_src);

    waitKey(0);
    return 0;
}



