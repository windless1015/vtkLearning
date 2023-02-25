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
#include "Vector3d.h"
#include "TransformAssistant.h"

void DisplayArrow(vtkSmartPointer<vtkRenderer>& renderer);
void  printActorTransform(vtkSmartPointer<vtkActor>& actor);
void TransformationTest();
void CoordinateTransform();
int main(int, char*[])
{
	CoordinateTransform();
	return 1;
	TransformationTest();

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

	///////////////////////first translate and then rotate (premulitply)////////////////////////////////
	objTransform->Identity();
	objTransform->PreMultiply();
	objTransform->Translate(1, 0, 0);
	objTransform->RotateZ(30);
	objTransform->Update();
	printActorTransform(objectActor);

	////////////////////////////////////////////
	objTransform->Identity();
	objTransform->PostMultiply();
	objTransform->Translate(1, 0, 0);
	objTransform->Translate(0,1,0);
	objTransform->RotateZ(30);
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


void TransformationTest()
{
	CVector3d pos(1,1,0);
	CVector3d localXAxis(1,1,0);
	CVector3d localYAxis(-1, 1, 0);
	vSP<vtkTransform> worldToLocal = CreateWorldToLocalTrans(pos, localXAxis, localYAxis);

	CVector3d worldPoint1(0,0,1);
	CVector3d localPoint1 = worldToLocal->TransformPoint(worldPoint1.point);
	int a = 1;
}

void CoordinateTransform()
{
	CVector3d x(1, 1, 0);
	CVector3d y(-1, 1, 0);
	CVector3d z(0, 0, 1);
	CVector3d ori(1, 1, 0);
	vSP<vtkTransform> tempaaa = CreateLocalToWorldTrans(x.point, y.point, z.point, ori.point);
	vSP<vtkTransform> tempaaa1 = CreateLocalToWorldTransform(ori.point, x.point, y.point);


	CVector3d xEnd(-1, 1, 0);
	CVector3d yEnd(1, 1, 1);
	CVector3d oriEnd(2, 1, 1);
	vSP<vtkTransform> pose1 = CreatePoseTransfrom(ori, x, y, oriEnd, xEnd, yEnd);
	vSP<vtkTransform> pose2 = CreatePostureTransform(ori, x, y, oriEnd, xEnd, yEnd);
	pose1->Print(std::cout);
	pose2->Print(std::cout);


	vtkSmartPointer<vtkAxesActor> worldAxesActor =
		vtkSmartPointer<vtkAxesActor>::New();
	worldAxesActor->SetPosition(0, 0, 0);
	worldAxesActor->SetTotalLength(3, 3, 3); ///////
	worldAxesActor->SetShaftType(0);
	worldAxesActor->SetAxisLabels(0);
	worldAxesActor->SetCylinderRadius(0.02);

	vtkSmartPointer<vtkAxesActor> localAxesActor =
		vtkSmartPointer<vtkAxesActor>::New();
	//localAxesActor->SetPosition(0, 0, 0);
	vtkNew<vtkTransform> localAxisTransform;
	localAxisTransform->Translate(1, 1, 0);
	localAxisTransform->Update();
	localAxisTransform->Print(std::cout);

	localAxisTransform->RotateZ(45);
	localAxisTransform->Update();
	localAxisTransform->Print(std::cout);
	localAxesActor->SetUserTransform(localAxisTransform);

	vtkNew<vtkTransform> tmp;
	tmp->DeepCopy(localAxisTransform);
	tmp->Inverse();
	tmp->Update();
	tmp->Print(std::cout);



	localAxesActor->SetTotalLength(1, 1, 1); ///////
	localAxesActor->SetShaftType(0);
	localAxesActor->SetAxisLabels(0);
	localAxesActor->SetCylinderRadius(0.02);


	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkNew<vtkRenderWindow> renderWindow;
	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("ReadSTL");

	vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
	renderWindowInteractor->SetRenderWindow(renderWindow);

	//DisplayArrow(renderer);
	renderer->AddActor(worldAxesActor);
	renderer->AddActor(localAxesActor);
	vtkNew<vtkNamedColors> colors;
	renderer->SetBackground(colors->GetColor3d("DarkOliveGreen").GetData());

	renderWindow->Render();
	renderWindowInteractor->Start();

}
