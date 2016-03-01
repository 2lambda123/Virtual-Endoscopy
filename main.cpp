#include "mainwindow.h"
#include <QApplication>
#include <QVTKApplication.h>

#include "tools/initials.h"
#include "Test/testunit.h"
int main(int argc, char *argv[])
{

//     pre initials
    PreInitials::Initilize();

//   test
//    TestUnit::RunTest();

//    QApplication a(argc, argv);
    QVTKApplication app(argc, argv);

    MainWindow w;
    w.showMaximized();
    w.show();

    return app.exec();
}
