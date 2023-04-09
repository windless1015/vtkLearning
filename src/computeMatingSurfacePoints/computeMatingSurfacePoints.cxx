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
#include <vtkBox.h>
#include <vtkBoundingBox.h>
#include <vtkPolyDataNormals.h>
#include <vtkOutlineFilter.h>
#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkLineSource.h>
#include <vtkTransformFilter.h>

#include "../utility/Vector3d.h"
#include "../utility/TransformAssistant.h"
#include "../utility/DisplayComponents.h"

#define vSPNew(instance, type) vtkSmartPointer<type> instance = vtkSmartPointer<type>::New();

void DisplayArrow(vtkSmartPointer<vtkRenderer>& renderer);
void GetBoundingBox(vtkPolyData* polyData, vtkSmartPointer<vtkRenderer>& renderer);
void DisplayBoundingBox(vtkPolyData* polyData, vtkSmartPointer<vtkRenderer>& renderer);
void DisplayFourPointsWhenScaling();

int main(int, char*[])
{
	DisplayFourPointsWhenScaling();
	return 0;
	vtkNew<vtkNamedColors> colors;

	std::string filePath = "D:\\DataRepo\\mesh\\attachments\\h2.stl";
	vtkNew<vtkSTLReader> stlReader;
	stlReader->SetFileName(filePath.c_str());
	stlReader->Update();
  

	// Visualize
	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(stlReader->GetOutputPort());

	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);
	actor->GetProperty()->SetDiffuse(0.8);
	actor->GetProperty()->SetDiffuseColor(
		colors->GetColor3d("LightSteelBlue").GetData());
	actor->GetProperty()->SetSpecular(0.3);
	actor->GetProperty()->SetSpecularPower(60.0);
	actor->GetProperty()->SetOpacity(0.5);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkNew<vtkRenderWindow> renderWindow;
	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("ReadSTL");

	vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
	renderWindowInteractor->SetRenderWindow(renderWindow);

	GetBoundingBox(stlReader->GetOutput(), renderer);
	DisplayBoundingBox(stlReader->GetOutput(), renderer);
	DisplayArrow(renderer);
	//DisplayAGivenVecoter(renderer, CVector3d(1, 0, 0).point, CVector3d(1, 0, 0).point,0.5, 1);
	renderer->AddActor(actor);
	renderer->SetBackground(colors->GetColor3d("DarkOliveGreen").GetData());

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}


void DisplayArrow(vtkSmartPointer<vtkRenderer>& renderer)
{

	vtkSmartPointer<vtkAxesActor> axesActor =
		vtkSmartPointer<vtkAxesActor>::New();
	axesActor->SetPosition(0, 0, 0);
	axesActor->SetTotalLength(6, 6, 6); /////
	axesActor->SetShaftType(0);
	axesActor->SetAxisLabels(0);
	axesActor->SetCylinderRadius(0.02);
	axesActor->AxisLabelsOn();
	renderer->AddActor(axesActor);
}


void GetBoundingBox(vtkPolyData* polyData, vtkSmartPointer<vtkRenderer>& renderer)
{
	// Compute bounding box
	vtkSmartPointer<vtkBox> boundingBox = vtkSmartPointer<vtkBox>::New();
	boundingBox->SetBounds(polyData->GetBounds());

	// Print bounding box
	double bounds[6];
	boundingBox->GetBounds(bounds);
	std::cout << "Bounding box: " << std::endl;
	std::cout << "  xmin: " << bounds[0] << std::endl;
	std::cout << "  xmax: " << bounds[1] << std::endl;
	std::cout << "  ymin: " << bounds[2] << std::endl;
	std::cout << "  ymax: " << bounds[3] << std::endl;
	std::cout << "  zmin: " << bounds[4] << std::endl;
	std::cout << "  zmax: " << bounds[5] << std::endl;

	double boundsLength[3];
	for (int i = 0; i < 3; i++)
	{
		boundsLength[i] = bounds[i * 2 + 1] - bounds[i * 2];
	}

	//display the mating surface points
	//get center
	CVector3d centerOfMesh;
	polyData->GetCenter(centerOfMesh.point);

	CVector3d XDir(1, 0, 0); 
	CVector3d YDir(0, 1, 0);
	CVector3d ZDir(0, 0, 1);
	CVector3d fourPointsOnZSurface[4];

	fourPointsOnZSurface[0] = centerOfMesh;


	fourPointsOnZSurface[0] = fourPointsOnZSurface[0] - boundsLength[2] / 2.0f * ZDir;
	fourPointsOnZSurface[0] = fourPointsOnZSurface[0] + boundsLength[0] / 2.0f * XDir + boundsLength[1] / 2.0f * YDir;

	fourPointsOnZSurface[1] = fourPointsOnZSurface[0];
	fourPointsOnZSurface[1] = fourPointsOnZSurface[1] - boundsLength[1]* YDir;

	fourPointsOnZSurface[2] = fourPointsOnZSurface[1];
	fourPointsOnZSurface[2] = fourPointsOnZSurface[2] - boundsLength[0] * XDir;

	fourPointsOnZSurface[3] = fourPointsOnZSurface[2];
	fourPointsOnZSurface[3] = fourPointsOnZSurface[3] + boundsLength[1] * YDir;

	DisplayAGivenSpherePoint(renderer, fourPointsOnZSurface[0].point, 0.1, 1);
	DisplayAGivenSpherePoint(renderer, fourPointsOnZSurface[1].point, 0.1, 2);
	DisplayAGivenSpherePoint(renderer, fourPointsOnZSurface[2].point, 0.1, 2);
	DisplayAGivenSpherePoint(renderer, fourPointsOnZSurface[3].point, 0.1, 1);
}

void DisplayBoundingBox(vtkPolyData* polyData, vtkSmartPointer<vtkRenderer>& renderer)
{
	vtkSmartPointer<vtkPolyDataNormals> normals =
		vtkSmartPointer<vtkPolyDataNormals>::New();
	normals->SetInputData(polyData);

	vtkSmartPointer<vtkOutlineFilter> outlineFilter =
		vtkSmartPointer<vtkOutlineFilter>::New();
	outlineFilter->SetInputConnection(normals->GetOutputPort());


	vtkSmartPointer<vtkPolyDataMapper> outLinemapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	outLinemapper->SetInputConnection(outlineFilter->GetOutputPort());

	vtkSmartPointer<vtkActor> outlineActor = vtkSmartPointer<vtkActor>::New();
	outlineActor->SetMapper(outLinemapper);
	outlineActor->GetProperty()->SetColor(1, 1, 1);

	renderer->AddActor(outlineActor);
}


void DisplayFourPointsWhenScaling()
{
	vSPNew(renderer, vtkRenderer);


	vSPNew(source, vtkCubeSource);
	source->Update();
	auto* pd = source->GetOutput();
	CVector3d center(2, 2, 1);

	/*GetBoundingBox(source->GetOutput(), renderer);*/


	CVector3d vec(1, -1, 0);
	vec.Unit();
	cout << "vec: " << vec[0] << ", " << vec[1] << ", " << vec[2] << endl;


	vSPNew(trans0, vtkTransform);
	trans0->Translate(center.point);
	trans0->RotateZ(-45);
	trans0->RotateY(45);
	trans0->Translate(center.point);
	trans0->Update();

	vSPNew(trans1, vtkTransform);
	trans1->RotateZ(-45);
	trans1->Update();

	vSPNew(trans0Inv, vtkTransform);
	trans0Inv->DeepCopy(trans0);
	trans0Inv->Inverse();
	trans0Inv->Update();
	trans0Inv->TransformVector(vec.point, vec.point);


	CVector3d crossVec = CVector3d(1, 0, 0) ^ vec;
	auto dotValue = CVector3d(1, 0, 0).Dot(vec);
	auto radius = std::acos(dotValue);
	auto degree = vtkMath::DegreesFromRadians(radius);
	cout << "degree: " << degree << endl;
	cout << "crossVec: " << crossVec[0] << ", " << crossVec[1] << ", " << crossVec[2] << endl;


	vSPNew(rotateTrans, vtkTransform);
	rotateTrans->RotateWXYZ(-degree, crossVec.point);
	rotateTrans->Update();


	vSPNew(rotateTransInv, vtkTransform);
	rotateTransInv->DeepCopy(rotateTrans);
	rotateTransInv->Inverse();
	rotateTransInv->Update();


	vSPNew(scaleTrans, vtkTransform);
	scaleTrans->Scale(2, 1, 1);
	scaleTrans->Update();


	vSPNew(finalScaleTrans, vtkTransform);
	finalScaleTrans->Concatenate(rotateTransInv);
	finalScaleTrans->Concatenate(scaleTrans);
	finalScaleTrans->Concatenate(rotateTrans);
	finalScaleTrans->Update();


	vSPNew(finalTrans, vtkTransform);
	finalTrans->Concatenate(trans0);
	finalTrans->Concatenate(finalScaleTrans);
	finalTrans->Update();


	/*vSPNew(transFilter, vtkTransformFilter);
	transFilter->SetInputData(pd);
	transFilter->SetTransform(finalTrans);
	transFilter->Update(); */

	vSPNew(transFilter, vtkTransformFilter);
	transFilter->SetInputData(pd);
	transFilter->SetTransform(finalScaleTrans);
	transFilter->Update();


	GetBoundingBox(transFilter->GetPolyDataOutput(), renderer);

	vSPNew(mapper, vtkPolyDataMapper);
	mapper->SetInputData(transFilter->GetPolyDataOutput());


	vSPNew(actor, vtkActor);
	actor->SetMapper(mapper);
	actor->SetUserTransform(trans0);


	vSPNew(axesActor, vtkAxesActor);
	axesActor->SetTotalLength(5, 5, 5);
	axesActor->SetUserTransform(trans1);


	vSPNew(lineSource, vtkLineSource);
	CVector3d pt0(-3, 3, 0), pt1(3, -3, 0), tmpPt(0, 0, 0);
	trans0->TransformPoint(tmpPt.point, tmpPt.point);
	lineSource->SetPoint1((pt0 + tmpPt).point);
	lineSource->SetPoint2((pt1 + tmpPt).point);
	lineSource->Update();


	vSPNew(lineMapper, vtkPolyDataMapper);
	lineMapper->SetInputData(lineSource->GetOutput());


	vSPNew(lineActor, vtkActor);
	lineActor->SetMapper(lineMapper);
	lineActor->GetProperty()->SetColor(1, 0, 0);



	renderer->AddActor(actor);
	renderer->AddActor(lineActor);
	renderer->AddActor(axesActor);
	renderer->SetBackground(0, 0, 0);


	vSPNew(renderWindow, vtkRenderWindow);
	renderWindow->AddRenderer(renderer);


	vSPNew(renderWindowInteractor, vtkRenderWindowInteractor);
	renderWindowInteractor->SetRenderWindow(renderWindow);


	renderer->ResetCamera();
	renderWindow->Render();
	renderWindowInteractor->Start();

}