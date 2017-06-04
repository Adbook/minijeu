
#include <QApplication>
#include <iostream>

#include "myWindow.hpp"
#include "../../lib/interface/application_qt.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

int main(int argc, char *argv[])
{
    cv::VideoCapture capture = cv::VideoCapture(0);
    if(!capture.isOpened())
    {
        std::cerr<<"Failed to open Camera"<<std::endl;
        exit(1);
    }

    cv::Mat frame;
    capture.read(frame);
    cv::imshow("frame", frame);

    srand(time(0));
    //Qt main application (handles exceptions)
    application_qt app(argc, argv);

    //The main window
    myWindow win;
    win.show();

    //Start Qt Application
    int ret = app.exec();
    cv::destroyAllWindows();

    return ret;
}
