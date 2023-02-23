#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkDoubleArray.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSTLReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkMatrix4x4.h>
#include <vtkAxesActor.h>
#include <vtkConeSource.h>
#include <vtkTextActor.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>

void DisplayArrow(vtkSmartPointer<vtkRenderer>& renderer);
void  printActorTransform(vtkSmartPointer<vtkActor>& actor);
int main(int, char*[])
{
	vtkNew<vtkNamedColors> colors;

	//std::string filePath = "D:\\DataRepo\\mesh\\s0.stl";
	//vtkNew<vtkSTLReader> stlReader;
	//stlReader->SetFileName(filePath.c_str());
	//stlReader->Update();
 // 
	//// Visualize
	//vtkNew<vtkPolyDataMapper> mapper;
	//mapper->SetInputConnection(stlReader->GetOutputPort());

	//vtkNew<vtkActor> actor;
	//actor->SetMapper(mapper);
	//actor->GetProperty()->SetDiffuse(0.8);
	//actor->GetProperty()->SetDiffuseColor(
	//	colors->GetColor3d("LightSteelBlue").GetData());
	//actor->GetProperty()->SetSpecular(0.3);
	//actor->GetProperty()->SetSpecularPower(60.0);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkNew<vtkRenderWindow> renderWindow;
	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("ReadSTL");

	vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
	renderWindowInteractor->SetRenderWindow(renderWindow);

	DisplayArrow(renderer);
	//renderer->AddActor(actor);
	renderer->SetBackground(colors->GetColor3d("DarkOliveGreen").GetData());

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}


void DisplayArrow(vtkSmartPointer<vtkRenderer>& renderer)
{
	vtkSmartPointer<vtkSphereSource> sphereSource =
		vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetRadius(0.1);//设置半径
	sphereSource->Update();

	vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

	vtkSmartPointer<vtkActor> sphereActor =
		vtkSmartPointer<vtkActor>::New();
	sphereActor->SetPosition(0, 0, 0);//设置演员的位置
	sphereActor->SetMapper(sphereMapper);//设置演员的映射器
	sphereActor->GetProperty()->SetColor(1, 0, 0);//设置演员的颜色

	vtkSmartPointer<vtkConeSource> coneSource =
		vtkSmartPointer<vtkConeSource>::New();
	coneSource->SetRadius(.2);
	coneSource->SetHeight(.5);
	coneSource->SetCenter(0, 0, 0);
	vtkSmartPointer<vtkPolyDataMapper> coneMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	coneMapper->SetInputConnection(coneSource->GetOutputPort());

	vtkSmartPointer<vtkActor> objectActor =
		vtkSmartPointer<vtkActor>::New();
	objectActor->SetMapper(coneMapper);

	vtkSmartPointer<vtkActor> oriConeActor =
		vtkSmartPointer<vtkActor>::New();
	oriConeActor->SetMapper(coneMapper);

	vtkSmartPointer<vtkAxesActor> oriAxesActor =
		vtkSmartPointer<vtkAxesActor>::New();
	oriAxesActor->SetPosition(0, 0, 0);
	oriAxesActor->SetTotalLength(1, 1, 1); ///////
	oriAxesActor->SetShaftType(0);
	oriAxesActor->SetAxisLabels(0);
	oriAxesActor->SetCylinderRadius(0.02);

	vtkSmartPointer<vtkAxesActor> axesActor =
		vtkSmartPointer<vtkAxesActor>::New();
	axesActor->SetPosition(0, 0, 0);
	axesActor->SetTotalLength(1, 1, 1); /////
	axesActor->SetShaftType(0);
	axesActor->SetAxisLabels(0);
	axesActor->SetCylinderRadius(0.02);

	//set identity transform to object 
	vtkNew<vtkTransform> objTransform;
	//objTransform->PreMultiply(); //default mode

	objTransform->Identity();
	objTransform->Update();
	objectActor->SetUserTransform(objTransform);
	//first print is identity matrix, it  locates at original point
	printActorTransform(objectActor);

	//rotate by z axis, according to the euler angle, we can get the R(z) matrix as below
	/* rotation by z with 30 degree
	cos30    -sin30  0 
	sin30     cos30  0
	   0          0       1
	*/

	objTransform->RotateZ(30);// https://zh.wikipedia.org/zh/%E6%AC%A7%E6%8B%89%E8%A7%92
	// translate by x axis with 1 unit 
	/*
	1 0 0 1
	0 1 0 0 
	0 0 1 0
	0 0 0 1
	*/

	//////////////////////////////Pre Multiply///////////////////////////////////////////////////
	// vtk default matrix mulply mode is  preMultiply, that is      Matrix(after) = Matrix(original) * T(1) * T(2), T1 is the first transform, T2 is the second transform
	/*
	| 1 0 0 0 |	    |cos30  -sin30  0    0 |       | 1   0   0   1 |
	| 0	 1 0 0|	    |sin30    cos30  0    0 | *    | 0   1   0   0 |
	| 0 0 1 0 | *    | 0          0        1    0 |       |  0  0   1   0 |
	| 0	 0 0 1|	    |  0         0        0    1 |       |  0  0    0  1 |
	result is:
	|cos30    -sin30  0    cos30 |
	|sin30     cos30  0    sin30  |
	|  0          0        1       0     |
	|  0          0        0       1     |
	*/
	objTransform->Translate(1, 0, 0);
	objTransform->Update();
	printActorTransform(objectActor);




	/////////////////////////////////Post Multiply//////////////////////////////////
	/*
	| 1 0 0 1 |	    |cos30    -sin30  0    0 |       | 1   0   0   0 |
	| 0	 1 0 0|	    |sin30     cos30  0    0 |   *   |  0  1   0   0 |
	| 0 0 1 0	|	*   |  0          0        1    0 |       |  0  0   1   0 |
	| 0	 0 0 1|	    |  0          0        0    1 |        |  0  0    0  1 |

	result is
	|cos30    -sin30  0    1 |
	|sin30     cos30  0    0 |
	|  0          0        1    0 |
	|  0          0        0    1 |
	*/
	objTransform->Identity();
	objTransform->PostMultiply();
	objTransform->RotateZ(30);
	objTransform->Translate(1, 0, 0);
	objTransform->Update();
	printActorTransform(objectActor);

	////////////////////pre multiply concatenate///////////////////////
	objTransform->Identity();
	objTransform->PreMultiply();
	vtkNew<vtkTransform> rotationByZWith30Degree;
	double rotateMatrix[16] = { 0.866, -0.5, 0, 0,
											0.5, 0.866, 0, 0,
											0, 0, 1, 0,
											0, 0, 0, 1 };
	rotationByZWith30Degree->SetMatrix(rotateMatrix);
	rotationByZWith30Degree->Update();
	objTransform->Concatenate(rotationByZWith30Degree); // E * R
	objTransform->Update();
	vtkNew<vtkTransform> translationByXWith1Unit;
	double translateMatrix[16] = { 1, 0, 0, 1,
												0, 1, 0, 0,
												0, 0, 1, 0,
												0, 0, 0, 1};
	translationByXWith1Unit->SetMatrix(translateMatrix);
	translationByXWith1Unit->Update();
	objTransform->Concatenate(translationByXWith1Unit); // (E * R) * T
	objTransform->Update();
	printActorTransform(objectActor);


	renderer->AddActor(sphereActor);
	renderer->AddActor(oriConeActor);
	renderer->AddActor(objectActor);
	renderer->AddActor(oriAxesActor);
	renderer->AddActor(axesActor);
}

void  printActorTransform(vtkSmartPointer<vtkActor>& actor)
{
	vtkMatrix4x4* curMatrix = actor->GetUserMatrix();
	curMatrix->Print(std::cout); //print current matrix
}
