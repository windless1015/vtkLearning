#pragma once
#include "vtkSmartPointer.h"


#define vSP vtkSmartPointer
class CVector3d;
class vtkRenderer;
//display a point
void DisplayAGivenVecoter(vtkSmartPointer<vtkRenderer>& renderer, double* fromPoint, double* theVector, double scaleFactor, int color);
void DisplayAGivenPoint(vtkSmartPointer<vtkRenderer>& renderer, double* displayPoints, int radius, int color);
void DisplayAGivenPlane(vtkSmartPointer<vtkRenderer>& renderer, double* center, double* norm);