
#include "vtkTransform.h"
#include "vtkSmartPointer.h"
#include "vtkActor.h"
#include "vtkMapper.h"
#include <vtkMath.h>
#include <vtkArrowSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkPlaneSource.h>
#include <vtkSphereSource.h>


void DisplayAGivenVecter(vtkSmartPointer<vtkRenderer>& renderer, double* fromPoint, double* theVector, double scaleFactor, int color)
{
	//vector normalization
	double length = sqrt(theVector[0] * theVector[0] + theVector[1] * theVector[1] + theVector[2] * theVector[2]);
	theVector[0] = theVector[0] / length;
	theVector[1] = theVector[1] / length;
	theVector[2] = theVector[2] / length;

	//get rotate axis
	double rotateAxis[3] = { 0 };
	double originVecctor[3] = { 1, 0, 0 }; //arrow default direction is the x axis
	vtkMath::Cross(originVecctor, theVector, rotateAxis);

	//get the angle between two vector
	double angle = acosf(vtkMath::Dot(originVecctor, theVector)) * 180 / 3.14159; //得到的是角度

	// Create an arrow.
	vtkSmartPointer<vtkArrowSource> arrowSource =
		vtkSmartPointer<vtkArrowSource>::New();
	arrowSource->SetShaftRadius(0.03); // 箭头柄 部的半径
	arrowSource->SetTipLength(0.5);  //箭头部分的长度
	arrowSource->Update();
	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(arrowSource->GetOutputPort());
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	vtkSmartPointer<vtkTransform> curTrans = vtkSmartPointer<vtkTransform>::New();
	curTrans->Identity();
	curTrans->PostMultiply();
	//默认是原点
	curTrans->Scale(scaleFactor, scaleFactor, scaleFactor);
	curTrans->RotateWXYZ(angle, rotateAxis[0], rotateAxis[1], rotateAxis[2]);
	curTrans->Translate(fromPoint);
	curTrans->Update();
	actor->SetUserTransform(curTrans);
	if (color == 1)
		actor->GetProperty()->SetColor(1, 0, 0);
	else
		actor->GetProperty()->SetColor(0, 1, 0);
	renderer->AddActor(actor);
	//renderer->Render();
}

void DisplayAGivenSpherePoint(vtkSmartPointer<vtkRenderer>& renderer, double* displayPoints, double radius, int color)
{
	vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetCenter(displayPoints);
	sphereSource->SetRadius(radius);
	sphereSource->SetThetaResolution(10);
	sphereSource->SetPhiResolution(10);
	sphereSource->Update();

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(sphereSource->GetOutput());
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetPointSize(radius);
	if (color == 1)
		actor->GetProperty()->SetColor(1, 0, 0);
	else
		actor->GetProperty()->SetColor(0, 0, 1);
	renderer->AddActor(actor);
}

void DisplayAGivenPoint(vtkSmartPointer<vtkRenderer>& renderer, double* displayPoints, int radius, int color)
{
	// Create the geometry of a point (the coordinate)
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();

	// Create the topology of the point (a vertex)
	vtkSmartPointer<vtkCellArray> vertices =
		vtkSmartPointer<vtkCellArray>::New();
	vtkIdType pid[1];
	pid[0] = points->InsertNextPoint(displayPoints);
	vertices->InsertNextCell(1, pid);
	// Create a polydata object
	vtkSmartPointer<vtkPolyData> point =
		vtkSmartPointer<vtkPolyData>::New();
	// Set the points and vertices we created as the geometry and topology of the polydata
	point->SetPoints(points);
	point->SetVerts(vertices);
	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(point);
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetPointSize(radius);
	if (color == 1)
		actor->GetProperty()->SetColor(1, 0, 0);
	else
		actor->GetProperty()->SetColor(0, 0, 1);
	renderer->AddActor(actor);
}

void DisplayAGivenPlane(vtkSmartPointer<vtkRenderer>& renderer, double* center, double* norm)
{
	// Create a plane
	vtkSmartPointer<vtkPlaneSource> planeSource =
		vtkSmartPointer<vtkPlaneSource>::New();

	double xtemp = ((center[1] * norm[1] + center[2] * norm[2]) / norm[0]) + center[0];
	double ytemp = ((center[0] * norm[0] + center[2] * norm[2]) / norm[1]) + center[1];
	planeSource->SetPoint1(xtemp, 0, 0);
	planeSource->SetPoint2(0, xtemp, 0);

	planeSource->SetCenter(center[0], center[1], center[2]);
	planeSource->SetNormal(norm[0], norm[1], norm[2]);
	double n[3] = { norm[0], norm[1], norm[2] };
	planeSource->Update();

	vtkPolyData* plane = planeSource->GetOutput();
	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(plane);
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(0, 1, 0);
	renderer->AddActor(actor);
}