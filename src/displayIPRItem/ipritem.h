#ifndef GRAPHITEM_H
#define GRAPHITEM_H

#include <QWidget>
#include "QVTKOpenGLNativeWidget.h"               //新版本，旧版QVTKWidget
#include "vtkAutoInit.h"
#include "vtkCommand.h"
#include "vtkRandomGraphSource.h"
#include "vtkGraph.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkContextView.h"
#include "vtkContextScene.h"
#include "vtkObjectFactory.h"
#include"vtkContextActor.h"
#include "vtkNamedColors.h"
#include "vtkFollower.h"

#include "vtkipritem.h"

class IPRItem : public QWidget
{
    Q_OBJECT

public:
    explicit IPRItem(QWidget* parent = nullptr);
    ~IPRItem();

    void startinteractor();

private:
    QVTKOpenGLNativeWidget* openGLWidget;
    vtkSmartPointer<vtkIPRItem> item;

    vtkSmartPointer<MyDrawing> sampleItem;

    vtkSmartPointer<vtkRandomGraphSource> source;
    vtkSmartPointer<vtkContextView> mview;
};

#endif // GRAPHITEM_H
