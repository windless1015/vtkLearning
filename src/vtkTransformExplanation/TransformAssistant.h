
//author:zwb
//define some functions to assist transfrom generation
#pragma once

#include "vtkTransform.h"
#include "vtkSmartPointer.h"
#include "vtkActor.h"
#include "vtkMapper.h"
#include <QFile>


#define vSP vtkSmartPointer
class CVector3d;

vtkSmartPointer<vtkTransform>  CreateToLocalTrans(CVector3d Pos, CVector3d XDir, CVector3d YDir);

vtkSmartPointer<vtkTransform> CreatePoseTransfrom(CVector3d fromPos, CVector3d fromXDir, CVector3d fromYDir, CVector3d toPos, CVector3d toXDir, CVector3d toYDir);

vtkSmartPointer<vtkTransform> CreateLocalToWorldTrans(double xDir[3], double yDir[3], double zDir[3], double origin[3]);

void ReadTransFromFile(QFile &File, vtkTransform *DestinationXFM);
void WriteTransToFile(QFile &File, vtkTransform *DestinationXFM);

void TransVectorFromTo(CVector3d &fromPos, CVector3d &fromDir, CVector3d &toPos, CVector3d &toDir, vtkSmartPointer<vtkTransform> TransFromTo);



vtkSmartPointer<vtkTransform> CreateTransform(CVector3d startP, CVector3d startNormal, CVector3d endP, CVector3d endNormal);

vtkSmartPointer<vtkTransform>  CreateWorldToLocalTrans(CVector3d Pos, CVector3d XDir, CVector3d YDir);

////////////////////////////////
vtkSmartPointer<vtkTransform>  CreateWorldToLocalTransform(CVector3d Pos, CVector3d XDir, CVector3d YDir);
vtkSmartPointer<vtkTransform>  CreateLocalToWorldTransform(CVector3d Pos, CVector3d XDir, CVector3d YDir);
vtkSmartPointer<vtkTransform>  CreatePostureTransform(CVector3d fromPos, CVector3d fromXDir, CVector3d fromYDir, CVector3d endPos, CVector3d endXDir, CVector3d endYDir);