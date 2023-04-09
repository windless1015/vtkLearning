#include "ipritem.h"

vtkStandardNewMacro(MyDrawing);

IPRItem::IPRItem(QWidget* parent) :
    QWidget(parent)
{
    vtkNew<vtkNamedColors> colors;

    openGLWidget = new QVTKOpenGLNativeWidget(this);
    openGLWidget->resize(800, 500);

    mview = vtkSmartPointer<vtkContextView>::New();
    mview->GetRenderer()->SetBackground(0.0, 1.0, 1.0);
    mview->GetRenderWindow()->SetSize(800, 600);

    source = vtkSmartPointer<vtkRandomGraphSource>::New();
    source->SetNumberOfVertices(100);
    source->SetNumberOfEdges(0);
    source->StartWithTreeOn();
    source->Update();
    item = vtkSmartPointer<vtkIPRItem>::New();
    item->SetGraph(source->GetOutput());
    mview->GetScene()->AddItem(item);

    sampleItem = vtkSmartPointer<MyDrawing>::New();


    vtkNew<vtkContextActor> cactor;
    cactor->GetScene()->AddItem(sampleItem);

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(cactor);
    renderer->SetBackground(colors->GetColor3d("WhiteSmoke").GetData());

    openGLWidget->renderWindow()->AddRenderer(renderer);
    openGLWidget->renderWindow()->Render();

    /*mview->SetRenderWindow(openGLWidget->renderWindow());
    mview->GetRenderWindow()->GetInteractor()->Initialize();*/

}

IPRItem::~IPRItem()
{

}

void IPRItem::startinteractor()
{
    //view->GetRenderWindow()->GetInteractor()->Start();
}
