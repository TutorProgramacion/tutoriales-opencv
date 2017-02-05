#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    try {
        String face_xml = R"(c:\developer\opencv-3.2.0\etc\haarcascades\haarcascade_frontalface_default.xml)";
        String eyel_xml = R"(c:\developer\opencv-3.2.0\etc\haarcascades\haarcascade_eye.xml)";

        CascadeClassifier face_detector, eye_detector;
        
        // cargar los clasificadores en cascada
        if (!eye_detector.load(eyel_xml) || !face_detector.load(face_xml)) {
            cout << "Error: no se ecuentra el cascade .xml" << endl;
        }

        Mat frame, gray_frame;

        // leer la imagen de entrada
        frame = imread("images/face.jpg");

        if (frame.empty()) {
            cout << "Error: no se puede abrir la imagen de entrada." << endl;
        }

        cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
        equalizeHist(gray_frame, gray_frame);

        // detectar rostro, almacenar la region en faces
        std::vector<cv::Rect> faces;
        face_detector.detectMultiScale(gray_frame, faces);

        double EYE_SX = 0.12;
        double EYE_SY = 0.17;
        double EYE_SW = 0.42;
        double EYE_SH = 0.36;

        // procesar los rostros encontrados
        for (auto& region : faces) {

            // crear un cv::Mat que contine el rostro
            Mat face = frame(region);

            int leftX = cvRound(face.cols * EYE_SX);
            int topY = cvRound(face.rows * EYE_SY);
            int widthX = cvRound(face.cols * EYE_SW);
            int heightY = cvRound(face.rows * EYE_SH);
            int rightX = cvRound(face.cols * (1.0 - EYE_SX - EYE_SW));

            // crear cv::Mat que contiene la region aproximada de los ojos 
            Mat topLeftOfFace = face(Rect(leftX, topY, widthX, heightY));
            Mat topRightOfFace = face(Rect(rightX, topY, widthX, heightY));

            std::vector<cv::Rect> left_eye, right_eye;

            // detectar los ojos, izquierdo y derecho, respectivamente
            eye_detector.detectMultiScale(topLeftOfFace, left_eye);
            //eye_detector.detectMultiScale(topRightOfFace, right_eye);

            // clonar el ojo izquierdo
            if (!left_eye.empty())
            {
                // crear el cv::Mat que contiene el ojo izquierdo
                Mat eye = topLeftOfFace(left_eye[0]);

                // crear el cv::Mat para la mascara
                Mat mask(eye.rows, eye.cols, CV_8U, Scalar::all(0));

                Point pos_ellipse(mask.size() / 2);
                Point pos_clone(face.size().width / 2, face.size().height / 5);

                Size size(mask.size().width / 2, (mask.size().height / 2) - 15);

                // crear la mascara y aplicar seamless cloning
                cv::ellipse(mask, pos_ellipse, size , 0, 0, 360, Scalar::all(255), CV_FILLED, LINE_AA);
                cv::seamlessClone(eye, face, mask, pos_clone, face, NORMAL_CLONE);

                imshow("mask", mask);
                imshow("src", eye);
            }

            imshow("dst", face);

            //// clonar el ojo derecho
            //if (!right_eye.empty())
            //{
            //    Mat eye = topRightOfFace(right_eye[0]);
            //    Mat mask(eye.rows, eye.cols, CV_8U, Scalar::all(0));
            //
            //    cv::ellipse(mask, Point(mask.size() / 2), Size(mask.size().width / 2, (mask.size().height / 2) - 2), 0, 0, 360, Scalar::all(255), CV_FILLED, LINE_AA);
            //    cv::seamlessClone(eye, face, mask, Point(face.size().width / 2, face.size().height / 5), face, MIXED_CLONE);
            //}
        }

        imshow("Fotomontajes", frame);

        waitKey(0);
    }
    catch (Exception ex) {
        cout << "Exception: " << ex.what() << endl;
    }

    return 0;
}






