#include "edge_detection.h"

using namespace cv;

int pedestrian_detect()
{
    int mode = 0;
    VideoCapture cap(0);

    if (!cap.isOpened()) {
        std::cerr << "Video open failed!" << std::endl;
        return -1;
    }

    HOGDescriptor hogDefaultPeopleDetector;
    hogDefaultPeopleDetector.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

    HOGDescriptor hogDaimPeopleDetector(Size(48, 96), Size(16, 16), Size(8, 8), Size(8, 8), 9);
    hogDaimPeopleDetector.setSVMDetector(HOGDescriptor::getDaimlerPeopleDetector());

    std::vector<Rect> detected;
    Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty())
            break;

        if (mode == 0) {
            hogDefaultPeopleDetector.detectMultiScale(frame, detected);
            for (Rect r : detected) {
                Scalar c = Scalar(std::rand() % 256, std::rand() % 256, std::rand() % 256);
                rectangle(frame, r, c, 3);

            }
            putText(frame, "DefaultPeopleDetector...", cvPoint(30, 30),
                FONT_HERSHEY_COMPLEX_SMALL, 2.0, cvScalar(0, 0, 255), 1, CV_AA);
        }
        else {

            hogDaimPeopleDetector.detectMultiScale(frame, detected, 1.2, Size(8, 8), Size(32, 32), 1.05, 6);
            for (Rect r : detected) {
                Scalar c = Scalar(std::rand() % 256, std::rand() % 256, std::rand() % 256);
                rectangle(frame, r, c, 3);
            }
            putText(frame, "DaimPeopleDetector...", cvPoint(30, 30),
                FONT_HERSHEY_COMPLEX_SMALL, 2.0, cvScalar(0, 0, 255), 1, CV_AA);

        }
        imshow("frame", frame);

        if (waitKey(10) == 27)
            break;
        if (waitKey(10) == 109)
        {
            if (mode == 0)
                mode = 1;
            else
                mode = 0;
        }
        std::cout << "mode : " << mode << std::endl;
    }

    return 0;

}
