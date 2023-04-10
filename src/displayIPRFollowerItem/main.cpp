#include <QApplication>
#include <qwidget.h>
#include "followerIPRWidget.h"

//VTK_MODULE_INIT(vtkRenderingContextOpenGL2);

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    FollowerIPRWidget follwerWidget;
    QString file = "D:\\DataRepo\\mesh\\s0.stl";
    follwerWidget.readSTLFile(file);
    follwerWidget.show();
    return a.exec();
}