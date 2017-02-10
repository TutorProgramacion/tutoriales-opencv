#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/core/ocl.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
    try {
        // desactivar aceleracion OpenCL
        ocl::setUseOpenCL(false);

        vector<Mat> imgs;

        // cargar las imagenes de prueba
        imgs.push_back(imread("images/images/imagen1.jpg"));
        imgs.push_back(imread("images/images/imagen2.jpg"));

        Mat result;

        // crear el stitcher e iniciar la operacion
        Ptr<Stitcher> stitcher = Stitcher::create(Stitcher::PANORAMA, false);
        Stitcher::Status status = stitcher->stitch(imgs, result);

        // verificar errores 
        if (status != Stitcher::OK)
        {
            cout << "Can't stitch images, error code = " << int(status) << endl;
            return -1;
        }

        //// guardar la imagen resultante
        //if(imwrite("pano.jpg", pano))
        //    cout << "save image: ./pano.jpg" << endl;

        imshow("Imagen 1", imgs[0]);
        imshow("Imagen 2", imgs[1]);
        imshow("Stitching", result);
        
        waitKey(0);
    }
    catch (Exception e) {
        cout << e.what() << endl;   
    }

    return 0;
}