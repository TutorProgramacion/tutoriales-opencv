#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat image, original;

int connectivity = 4;
int newMaskVal = 255;

static void onMouse(int event, int x, int y, int, void*)
{
    if (event != EVENT_LBUTTONDOWN) return;

    // obtener el punto donde se hizo clic
    Point seed = Point(x, y);

    int ffillMode = 1;
    int flags = connectivity + (newMaskVal << 8) + (ffillMode == 1 ? FLOODFILL_FIXED_RANGE : 0);
    int b = (unsigned)theRNG() & 255;
    int g = (unsigned)theRNG() & 255;
    int r = (unsigned)theRNG() & 255;
    int lo = 20, up = 20;

    Rect ccomp;

    // generar un color de manera aleatoria
    Scalar newVal = Scalar(r * 0.299 + g * 0.587 + b * 0.114);

    Mat dst, mask, inpaint_mask;

    // inicializar el Mat mask
    mask.create(image.rows + 2, image.cols + 2, CV_8UC1);
    mask = Scalar::all(0);

    // obtener el mask para reparar la imagen 
    floodFill(image, mask, seed, newVal, &ccomp,
        Scalar(lo, lo, lo),
        Scalar(up, up, up), flags);

    // obtener inpaintMask y restaurar la imagen
    inpaint_mask = Mat(mask, Rect(1, 1, mask.cols - 2, mask.rows - 2));
    inpaint(image, inpaint_mask, dst, 3, INPAINT_NS);

    imshow("image", image);
    imshow("mask", mask);
    imshow("inpaint", dst);

    original.copyTo(image);
}


int main(int argc, char** argv)
{
    char* filename = argc >= 2 ? argv[1] : (char*)"data/baboon.jpg";
    image = imread(filename, 1);

    if (image.empty())
    {
        cout << "No se puede abrir: data/baboon.jpg" << endl;
        system("pause");
        return 0;
    }

    // guardar una copia de la imagen original
    image.copyTo(original);

    namedWindow("image", 0);
    namedWindow("mask", 0);
    namedWindow("inpaint", 0);

    // responder a eventos del raton
    setMouseCallback("image", onMouse, 0);

    for (;;)
    {
        imshow("image", image);

        int c = waitKey(0);
        if ((c & 255) == 27)
        {
            cout << "Exiting ...\n";
            break;
        }
    }

    return 0;
}