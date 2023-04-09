#include <QApplication>
#include <qwidget.h>
#include "graphitem.h"

VTK_MODULE_INIT(vtkRenderingContextOpenGL2);

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    GraphItem* mGraphItem = new GraphItem(&w);
    w.show();
    return a.exec();
}