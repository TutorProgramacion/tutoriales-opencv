#include <opencv2\highgui.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\calib3d.hpp>

#include <vector>

using namespace cv;
using namespace std;

void main() {

    Ptr<Feature2D> detector = BRISK::create();
    Ptr<DescriptorMatcher> matcher = BFMatcher::create(NORM_HAMMING, true);

    // cargar la imagen de referencia, aquel objeto plano que deseamos localizar.
    Mat img_object = imread("image/box.png", IMREAD_GRAYSCALE);
    Mat desc_object;

    // cargar la imagen de la escena en donde buscaremos el objeto plano.
    Mat img_scene = imread("image/box_in_scene.png", IMREAD_GRAYSCALE);
    Mat desc_scene;

    // detectar y extraer los puntos de interes y sus respectivos descriptores.
    vector<KeyPoint> kp_object;
    detector->detectAndCompute(img_object, noArray(), kp_object, desc_object);

    // detectar y extraer los puntos de interes y sus respectivos descriptores.
    vector<KeyPoint> kp_scene;
    detector->detectAndCompute(img_scene, noArray(), kp_scene, desc_scene);

    // hacer el pareo de los key points.
    vector<DMatch> matches;
    matcher->match(desc_object, desc_scene, matches);

    // filtrar los mejores key points.
    sort(matches.begin(), matches.end());
    matches.erase(matches.begin() + 30, matches.end());

    vector<Point2f> pts_object, pts_scene;
    vector<DMatch> good_match;

    // guardar los puntos correspondientes a cada uno de los key points.
    for (auto& m : matches)
    {
        pts_object.push_back(kp_object[m.queryIdx].pt);
        pts_scene.push_back(kp_scene[m.trainIdx].pt);

        good_match.push_back(m);
    }

    Mat dst;

    // dibujar el pareo
    drawMatches(
        img_object, kp_object,
        img_scene, kp_scene,
        good_match, dst, Scalar::all(-1), Scalar::all(-1),
        vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    // buscar la matriz de homografia que representa la tranformación entre los puntos del objeto y la escena.
    Mat H = findHomography(pts_object, pts_scene, CV_RANSAC);
    if (!H.empty()) {

        // obtener las coordenadas de la imagen de referencia, corresponde a las 4 esquinas de la imagen.
        std::vector<Point2f> obj_corners(4);
        obj_corners[0] = Point2f(0, 0);
        obj_corners[1] = Point2f((float)img_object.cols, 0);
        obj_corners[2] = Point2f((float)img_object.cols, (float)img_object.rows);
        obj_corners[3] = Point2f(0, (float)img_object.rows);

        // aplicar la tranformación a las corredenadas previas para obtener la posición de cada punto en la escena.
        std::vector<Point2f> scene_corners(4);
        perspectiveTransform(obj_corners, scene_corners, H);

        Mat img_scene_dst;
        cvtColor(img_scene, img_scene_dst, CV_GRAY2BGR);

        // Dibujamos las coordenadas del objeto plano en la escena.
        line(img_scene_dst, scene_corners[0], scene_corners[1], Scalar(0, 255, 0), 3, LINE_AA);
        line(img_scene_dst, scene_corners[1], scene_corners[2], Scalar(0, 255, 0), 3, LINE_AA);
        line(img_scene_dst, scene_corners[2], scene_corners[3], Scalar(0, 255, 0), 3, LINE_AA);
        line(img_scene_dst, scene_corners[3], scene_corners[0], Scalar(0, 255, 0), 3, LINE_AA);

        imshow("Detected Object", img_scene_dst);

/**/        
        line(dst, scene_corners[0] + Point2f((float)img_object.cols, 0), scene_corners[1] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 1);
        line(dst, scene_corners[1] + Point2f((float)img_object.cols, 0), scene_corners[2] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 1);
        line(dst, scene_corners[2] + Point2f((float)img_object.cols, 0), scene_corners[3] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 1);
        line(dst, scene_corners[3] + Point2f((float)img_object.cols, 0), scene_corners[0] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 1);
/**/
    }

    imshow("Planar Object Detect", dst);
    waitKey();
}