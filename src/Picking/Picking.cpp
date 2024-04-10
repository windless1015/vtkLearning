#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkPlaneSource.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropPicker.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkSphereSource.h>

#include "vtkAutoInit.h"
#include "vtkContextItem.h"
#include <vtkCommand.h>
#include "vtkMinimalStandardRandomSequence.h"
#include "vtkVariant.h"
#include "vtkContext2D.h"
#include "vtkContextScene.h"
#include <vtkTextProperty.h>
#include <vtkPen.h>
#include <vtkBrush.h>
#include <vtkTextActor.h>
#include <vtkProperty2D.h>
#include <vtkActor2D.h>
#include <vtkCaptionActor2D.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingContextOpenGL2);

namespace {

// Handle mouse events
class MouseInteractorStyle2 : public vtkInteractorStyleTrackballCamera
{
public:
  static MouseInteractorStyle2* New();
  vtkTypeMacro(MouseInteractorStyle2, vtkInteractorStyleTrackballCamera);
  vtkNew<vtkNamedColors> colors;

  virtual void OnLeftButtonDown() override
  {
    int* clickPos = this->GetInteractor()->GetEventPosition();

    // Pick from this location.
    vtkNew<vtkPropPicker> picker;
    picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

    double* pos = picker->GetPickPosition();
    std::cout << "Pick position (world coordinates) is: " << pos[0] << " "
              << pos[1] << " " << pos[2] << std::endl;

    vtkActor2D* Actor2dPicker = picker->GetActor2D();
    if (Actor2dPicker)
    {
        int a = 1;// 可以用
    }
    auto pickedActor = picker->GetActor();
    if (pickedActor == nullptr)
    {
      std::cout << "No actor picked." << std::endl;
    }
    else
    {
      std::cout << "Picked actor: " << picker->GetActor() << std::endl;
      // Create a sphere
      // Create a sphere
      vtkNew<vtkSphereSource> sphereSource;
      sphereSource->SetCenter(pos[0], pos[1], pos[2]);
      sphereSource->SetRadius(0.1);

      // Create a mapper and actor
      vtkNew<vtkPolyDataMapper> mapper;
      mapper->SetInputConnection(sphereSource->GetOutputPort());

      vtkNew<vtkActor> actor;
      actor->SetMapper(mapper);
      actor->GetProperty()->SetColor(colors->GetColor3d("MistyRose").GetData());

      this->GetDefaultRenderer()->AddActor(actor);
      // Forward events
      vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }
  }

private:
};

vtkStandardNewMacro(MouseInteractorStyle2);

} // namespace

void addCaption2d(vtkSmartPointer<vtkRenderer>& renderer);
// Execute application.
int main(int, char*[])
{
  vtkNew<vtkNamedColors> colors;

  vtkNew<vtkPlaneSource> planeSource;
  planeSource->Update();

  // Create a polydata object
  vtkPolyData* polydata = planeSource->GetOutput();

  // Create a mapper
  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputData(polydata);

  // Create an actor
  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(
      colors->GetColor3d("LightGoldenrodYellow").GetData());

  std::cout << "Actor address: " << actor << std::endl;

  // A renderer and render window
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  renderWindow->SetWindowName("Picking");

  // An interactor
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Set the custom stype to use for interaction.
  vtkNew<MouseInteractorStyle2> style;
  style->SetDefaultRenderer(renderer);

  renderWindowInteractor->SetInteractorStyle(style);

  // Add the actors to the scene
  renderer->AddActor(actor);
  addCaption2d(renderer);
  renderer->SetBackground(colors->GetColor3d("DodgerBlue").GetData());

  // Render and interact
  renderWindow->Render();
  renderWindowInteractor->Initialize();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}


void addCaption2d(vtkSmartPointer<vtkRenderer>& renderer)
{
    vtkNew<vtkCaptionActor2D > caption2D;
    caption2D->SetCaption("0.2");
    caption2D->BorderOff(); //最外层边框消失
    caption2D->GetTextActor()->SetTextScaleModeToViewport(); //调节文本的大小
    caption2D->GetCaptionTextProperty()->SetFontSize(10);
    caption2D->SetPosition(100, 0);
    //caption2D->SetPosition2(200, 20);

    caption2D->GetProperty()->SetColor(0, 0, 1); //vtkCaptionActor2D 源代码中,3d指示线的颜色是根据其自身设置的
    caption2D->GetProperty()->SetLineWidth(1);

    caption2D->GetCaptionTextProperty()->FrameOn(); // 打开 text frame
    caption2D->GetCaptionTextProperty()->SetFrameWidth(1);// 边框的宽度为1个像素

    if (false) //如果这个数字被选中
    {
        caption2D->GetCaptionTextProperty()->SetColor(1.0f, 1.0f, 1.0f); //ipr字体白色
        caption2D->GetCaptionTextProperty()->SetBackgroundColor(0., 0., 1.0f); //背景为蓝色
        caption2D->GetCaptionTextProperty()->SetBackgroundOpacity(0.9);
    }
    else
    {
        caption2D->GetCaptionTextProperty()->SetColor(0, 0, 0); //ipr字体黑色
    }

    caption2D->GetCaptionTextProperty()->SetFrameColor(0., 0., 1.0f);

    caption2D->GetCaptionTextProperty()->SetFontFamily(2);//2号字体比较合适
    caption2D->GetCaptionTextProperty()->ShadowOff(); //关闭数字阴影,否则会出现重影
    caption2D->SetPadding(0);
    caption2D->SetThreeDimensionalLeader(true); //指示线为三维,否则有时候会被遮挡
    caption2D->LeaderOn(); //指示线开
    caption2D->SetPickable(true);
    caption2D->SetAttachmentPoint(0.0, 0.0, 2.0); // caption2d 所在的

    //renderer->AddActor(caption2D);
    renderer->AddActor2D(caption2D);//这两个都可以拾取到
}