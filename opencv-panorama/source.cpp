#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
    Mat imageA = imread("data/images/puente-americas-1.jpg"/*, IMREAD_GRAYSCALE*/);
    Mat imageB = imread("data/images/puente-americas-2.jpg"/*, IMREAD_GRAYSCALE*/);

    if (imageA.empty() || imageB.empty())
    {
        printf("No image data.");
        getchar();

        return -1;
    }

    Ptr<Feature2D> detect = BRISK::create();

    vector<KeyPoint> kpA, kpB;
    Mat descA, descB;

    detect->detectAndCompute(imageA, noArray(), kpA, descA);
    detect->detectAndCompute(imageB, noArray(), kpB, descB);

    vector<DMatch> matches;

    Ptr<DescriptorMatcher> matcher = BFMatcher::create(NORM_HAMMING, true);
    matcher->match(descA, descB, matches);

    sort(matches.begin(), matches.end());
    matches.erase(matches.begin() + 30, matches.end());


    // Visualizar pareo de puntos
    Scalar color = Scalar::all(-1);
    int flags = DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS;

    Mat result;
    drawMatches(imageA, kpA, imageB, kpB, matches, result, color, color, vector<char>(), flags);
    // Fin de visualizacion

    vector<Point2f> pts1, pts2;
    for (auto& m : matches) {
        pts1.push_back(kpA[m.queryIdx].pt);
        pts2.push_back(kpB[m.trainIdx].pt);
    }

    Mat h = findHomography(pts2, pts1, noArray(), CV_RANSAC);

    cout << "Homography Mat: " << endl << h << endl;

    vector<Point2f> box;
    box.push_back(Point2f(0, 0));
    box.push_back(Point2f(imageB.cols, 0));
    box.push_back(Point2f(imageB.cols, imageB.rows));
    box.push_back(Point2f(0, imageB.rows));

    vector<Point2f> box_dst;
    perspectiveTransform(box, box_dst, h);

    Rect rc = boundingRect(box_dst);

    Mat dst;

    warpPerspective(imageB, dst, h, Size(rc.width + rc.x, rc.height + rc.y));
    imageA.copyTo(dst(Rect(Point(0, 0), imageA.size())));

    //rectangle(dst, rc, Scalar(0, 255, 0));
    //rectangle(dst, Rect(0, 0, imageA.cols, imageA.rows), Scalar(0, 255, 0));

    //for (auto& dm : matches)
    //    circle(dst, kpA[dm.queryIdx].pt, 5, Scalar(0, 0, 255), 1, LINE_AA);

    imshow("Puenta de las Americas", dst);

    imshow("OpenCV :: Match Keypoints", result);
    waitKey(0);

    return 0;
}