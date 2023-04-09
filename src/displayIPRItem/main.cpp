#include <QApplication>
#include <qwidget.h>
#include "ipritem.h"

//VTK_MODULE_INIT(vtkRenderingContextOpenGL2);

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    IPRItem iprItem;
    iprItem.show();
    return a.exec();
}