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
#include <vtkImageData.h>
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
#include <vtkImageActor.h>
#include <vtkPointData.h>
#include <vtkImageMapper3D.h>
#include <vtkSTLReader.h>


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
    //int* clickPos = this->GetInteractor()->GetEventPosition();

    //// Pick from this location.
    //vtkNew<vtkPropPicker> picker;
    //picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

    //double* pos = picker->GetPickPosition();
    //std::cout << "Pick position (world coordinates) is: " << pos[0] << " "
    //          << pos[1] << " " << pos[2] << std::endl;

    //vtkActor2D* Actor2dPicker = picker->GetActor2D();
    //if (Actor2dPicker)
    //{
    //    int a = 1;// ©иртсц
    //}
    //auto pickedActor = picker->GetActor();
    //if (pickedActor == nullptr)
    //{
    //  std::cout << "No actor picked." << std::endl;
    //}
    //else
    //{
    //  std::cout << "Picked actor: " << picker->GetActor() << std::endl;
    //  // Create a sphere
    //  // Create a sphere
    //  vtkNew<vtkSphereSource> sphereSource;
    //  sphereSource->SetCenter(pos[0], pos[1], pos[2]);
    //  sphereSource->SetRadius(0.1);

    //  // Create a mapper and actor
    //  vtkNew<vtkPolyDataMapper> mapper;
    //  mapper->SetInputConnection(sphereSource->GetOutputPort());

    //  vtkNew<vtkActor> actor;
    //  actor->SetMapper(mapper);
    //  actor->GetProperty()->SetColor(colors->GetColor3d("MistyRose").GetData());

    //  this->GetDefaultRenderer()->AddActor(actor);
    //  // Forward events
      //vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    //}
  }

private:
};

vtkStandardNewMacro(MouseInteractorStyle2);

} // namespace


// Execute application.
int main(int, char*[])
{
  vtkNew<vtkNamedColors> colors;



  //vtkSmartPointer<vtkImageData> imageData = STL2Voxel();
  vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();


  //actor->GetMapper()->SetInputData(imageData);

  //// Create a mapper
  //vtkNew<vtkPolyDataMapper> mapper;
  //mapper->SetInputData(polydata);

  //// Create an actor
  //vtkNew<vtkActor> actor;
  //actor->SetMapper(mapper);
  //actor->GetProperty()->SetColor(
  //    colors->GetColor3d("LightGoldenrodYellow").GetData());

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

  renderer->SetBackground(colors->GetColor3d("DodgerBlue").GetData());

  // Render and interact
  renderWindow->Render();
  renderWindowInteractor->Initialize();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}


vtkSmartPointer<vtkImageData> STL2Voxel(vtkSmartPointer<vtkPolyData> stl, int scale, double bounds[6])
{
    scale = 64;
	vtkSmartPointer<vtkImageData> whiteImage = vtkSmartPointer<vtkImageData>::New();
	double spacing[6];
	for (int i = 0; i < 3; i++)
	{
		spacing[i] = (bounds[i * 2 + 1] - bounds[i * 2]) / scale;
	}
	whiteImage->SetSpacing(spacing);

	whiteImage->SetDimensions(scale, scale, scale);
	whiteImage->SetExtent(0, scale - 1, 0, scale - 1, 0, scale - 1);

	double origin[3];
	origin[0] = bounds[0] + spacing[0] / 2;
	origin[1] = bounds[2] + spacing[1] / 2;
	origin[2] = bounds[4] + spacing[1] / 2;
	whiteImage->SetOrigin(origin);
	whiteImage->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

	unsigned char inval = 1;
	unsigned char outval = 0;
	vtkIdType count = whiteImage->GetNumberOfPoints();
	for (vtkIdType i = 0; i < count; ++i)
	{
		whiteImage->GetPointData()->GetScalars()->SetTuple1(i, inval);
	}
   
    return whiteImage;
}