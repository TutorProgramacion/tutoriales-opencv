#include <opencv2/photo.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

void loadExposureSeq(String, vector<Mat>&, vector<float>&);

int main(int, char**argv)
{
    vector<Mat> images;
    vector<float> times;

    loadExposureSeq("data/exposures", images, times);

    //--------------- Image HDR -----------------------//

    Mat response;
    Ptr<CalibrateDebevec> calibrate = createCalibrateDebevec();
    calibrate->process(images, response, times);
    
    Mat hdr;
    Ptr<MergeDebevec> merge_debevec = createMergeDebevec();
    merge_debevec->process(images, hdr, times, response);
    
    imwrite("memorial-hdr.hdr", hdr);

    //--------------- Tone Mapping -----------------------//

    Mat ldr;
    Ptr<TonemapDurand> tonemap = createTonemapDurand(2.2f);
    tonemap->process(hdr, ldr);
    
    imwrite("memorial-ldr.png", ldr * 255);

    //--------------- Exposure fusion -----------------------//

    Mat fusion;
    Ptr<MergeMertens> merge_mertens = createMergeMertens();
    merge_mertens->process(images, fusion);

    imshow("fusion", fusion);
    imwrite("fusion-ldr.png", fusion * 255);

    cout << "HDR creado correctamente." << endl;
    cout << "Presione una tecla para continuar..." << endl;

    waitKey();

    return 0;
}

void loadExposureSeq(String path, vector<Mat>& images, vector<float>& times)
{
    path = path + std::string("/");
    ifstream list_file((path + "list.txt").c_str());
    string name;
    float val;
    while (list_file >> name >> val) {
        Mat img = imread(path + name);
        images.push_back(img);
        times.push_back(1 / val);
    }
    list_file.close();
}