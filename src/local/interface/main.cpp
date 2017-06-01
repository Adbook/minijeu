
#include <QApplication>

#include "myWindow.hpp"
#include "../../lib/interface/application_qt.hpp"

int main(int argc, char *argv[])
{
    srand(time(0));
    //Qt main application (handles exceptions)
    application_qt app(argc, argv);

    //The main window
    myWindow win;
    win.show();

    //Start Qt Application
    return app.exec();
}
