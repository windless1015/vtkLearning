#include "TransformAssistant.h"
#include "Vector3d.h"
#include <vtkMath.h>

vtkSmartPointer<vtkTransform>  CreateToLocalTrans(CVector3d Pos, CVector3d XDir, CVector3d YDir)
{
	XDir.Unit();
	YDir.Unit();
	vtkSmartPointer<vtkTransform> ResultTrans = vtkSmartPointer<vtkTransform>::New();


	double elements[16];

	//rotation
	elements[0] = XDir[0];
	elements[1] = XDir[1];
	elements[2] = XDir[2];
	elements[3] = 0;

	elements[4] = YDir[0];
	elements[5] = YDir[1];
	elements[6] = YDir[2];
	elements[7] = 0;

	CVector3d ZDir = XDir^YDir;
	ZDir.Unit();

	elements[8] = ZDir[0];
	elements[9] = ZDir[1];
	elements[10] = ZDir[2];
	elements[11] = 0;

	elements[12] = 0;
	elements[13] = 0;
	elements[14] = 0;
	elements[15] = 1;

	ResultTrans->Concatenate(elements);


	//translation
	elements[0] = 1;
	elements[1] = 0;
	elements[2] = 0;
	elements[3] = -Pos[0];

	elements[4] = 0;
	elements[5] = 1;
	elements[6] = 0;
	elements[7] = -Pos[1];

	elements[8] = 0;
	elements[9] = 0;
	elements[10] = 1;
	elements[11] = -Pos[2];

	elements[12] = 0;
	elements[13] = 0;
	elements[14] = 0;
	elements[15] = 1;

	ResultTrans->Concatenate(elements);
	ResultTrans->Update();


	return ResultTrans;
}


vtkSmartPointer<vtkTransform> CreatePoseTransfrom(CVector3d fromPos, CVector3d fromXDir, CVector3d fromYDir, CVector3d toPos, CVector3d toXDir, CVector3d toYDir)
{
	fromXDir.Unit();
	fromYDir.Unit();
	toXDir.Unit();
	toYDir.Unit();

	vSP<vtkTransform> fromToLocal = CreateToLocalTrans(fromPos, fromXDir, fromYDir);
	//fromToLocal->Print(std::cout);

	vSP<vtkTransform> fromToWord = CreateToLocalTrans(fromPos, fromXDir, fromYDir);
	//fromToWord->Print(std::cout);

	fromToWord->Inverse();
	fromToWord->Update();


	CVector3d LocalToPosInFromCoor, LocalToXDirInFromCoor, LocalToYDirInFromCoor;
	fromToLocal->InternalTransformPoint(toPos.point, LocalToPosInFromCoor.point);
	fromToLocal->InternalTransformVector(toXDir.point, LocalToXDirInFromCoor.point);
	fromToLocal->InternalTransformVector(toYDir.point, LocalToYDirInFromCoor.point);
	vSP<vtkTransform> toToPoseXFMInFromCoor = CreateToLocalTrans(LocalToPosInFromCoor, LocalToXDirInFromCoor, LocalToYDirInFromCoor);

	toToPoseXFMInFromCoor->InternalTransformPoint(fromPos.point, LocalToPosInFromCoor.point);

	toToPoseXFMInFromCoor->Inverse();
	toToPoseXFMInFromCoor->Update();
	toToPoseXFMInFromCoor->InternalTransformPoint(fromPos.point, LocalToPosInFromCoor.point);
	//construct pose transform
	vSP<vtkTransform> ResultTrans = vSP<vtkTransform>::New();
	ResultTrans->PostMultiply();
	ResultTrans->Concatenate(fromToWord->GetMatrix());
	ResultTrans->Concatenate(toToPoseXFMInFromCoor->GetMatrix());
	ResultTrans->Concatenate(fromToLocal->GetMatrix());
	ResultTrans->Update();

	return ResultTrans;
}

vtkSmartPointer<vtkTransform> CreateLocalToWorldTrans(double xDir[3], double yDir[3], double zDir[3], double origin[3])
{
	vtkMath::Normalize(xDir);
	vtkMath::Normalize(yDir);
	vtkMath::Normalize(zDir);


	double A[16] = { 
		xDir[0], yDir[0], zDir[0], origin[0],
		xDir[1], yDir[1], zDir[1], origin[1],
		xDir[2], yDir[2], zDir[2], origin[2],
		0, 0, 0, 1 };


	//vtkMatrix4x4 *localToWorldMatrix = vtkMatrix4x4::New();
	//localToWorldMatrix->DeepCopy(A);

	vSP<vtkTransform> localToWorldTrans = vSP<vtkTransform>::New();
	localToWorldTrans->SetMatrix(A);
	localToWorldTrans->Update();
	localToWorldTrans->Print(std::cout);
	return localToWorldTrans;

}
void ReadTransFromFile(QFile &File, vtkTransform *DestinationXFM)
{
	double dTrans[16];


	DestinationXFM->Identity();
	DestinationXFM->PostMultiply();
	

	for (int j = 0; j<16; j++)
	{
		File.read((char *)&dTrans[j], 8);
	}

	DestinationXFM->SetMatrix(dTrans);
	DestinationXFM->Update();
}

void WriteTransToFile(QFile &File, vtkTransform *DestinationXFM)
{
	double dTrans[16];

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			dTrans[i * 4 + j] = DestinationXFM->GetMatrix()->GetElement(i, j);
		}
	}

	for (int j = 0; j<16; j++)
	{
		File.write((char *)&dTrans[j], 8);
	}

}



void TransVectorFromTo(CVector3d &fromPos, CVector3d &fromDir, CVector3d &toPos, CVector3d &toDir, vtkSmartPointer<vtkTransform> TransFromTo)
{

	fromDir.UnitVector();//单位化
	toDir.UnitVector();

	CVector3d Vector3dAxis = fromDir.CrossVector(toDir);//获取转的轴
	//double 3dAxis[3]={Vector3dAxis->GetX(),Vector3dAxis->GetY(),Vector3dAxis->GetZ()};
	double length = Vector3dAxis.Length();
	double angle = acosf(fromDir.DotVector(toDir)) * 180 / 3.14159;
	if (fabs(length)<0.000000000000001)
	{
		CVector3d Vector3dTranslate = toPos - fromPos;//两点之间移动的距离
		

		TransFromTo->Identity();
		TransFromTo->PostMultiply();//设置后乘

		if (angle < 0.1) //表示原始方向向量和目标方向向量是通向,所以只需要平移即可
		{
			//同向
			TransFromTo->Translate(-fromPos[0], -fromPos[1], -fromPos[2]);
			//		TransFromTo->RotateWXYZ(radian,Vector3dAxis[0],Vector3dAxis[1],Vector3dAxis[2]);
			TransFromTo->Translate(Vector3dTranslate[0], Vector3dTranslate[1], Vector3dTranslate[2]);
			TransFromTo->Translate(fromPos[0], fromPos[1], fromPos[2]);
		}
		else if (angle>179.9) //大于180°的时候需要反向
		{
			//反向
			if (fabs(toDir[0]) > 0.001)
			{
				Vector3dAxis[0] = -(toDir[1] + toDir[2]) / toDir[0];
				Vector3dAxis[1] = 1;
				Vector3dAxis[2] = 1;
			}
			else if (fabs(toDir[1]) > 0.001)
			{
				Vector3dAxis[0] = 1;
				Vector3dAxis[1] = -(toDir[0] + toDir[2]) / toDir[1];
				Vector3dAxis[2] = 1;
			}
			else
			{
				Vector3dAxis[0] = 1;
				Vector3dAxis[1] = 1;
				Vector3dAxis[2] = -(toDir[0] + toDir[1]) / toDir[2];
			}
			Vector3dAxis.Unit();
			TransFromTo->Translate(-fromPos[0], -fromPos[1], -fromPos[2]);
			TransFromTo->RotateWXYZ(angle, Vector3dAxis[0], Vector3dAxis[1], Vector3dAxis[2]);
			TransFromTo->Translate(fromPos[0], fromPos[1], fromPos[2]);
			TransFromTo->Translate(Vector3dTranslate[0], Vector3dTranslate[1], Vector3dTranslate[2]);
		}

	}
	else
	{
		Vector3dAxis.UnitVector();
//		CVector3d Vector3dTranslate = toPos - fromPos;//两点之间移动的距离
		CVector3d Vector3dTranslate ;//两点之间移动的距离
		Vector3dTranslate.SetValue(toPos[0] - fromPos[0], toPos[1] - fromPos[1], toPos[2] - fromPos[2]);
		TransFromTo->Identity();
		TransFromTo->PostMultiply();//设置后乘

		TransFromTo->Translate(-fromPos[0], -fromPos[1], -fromPos[2]);
		TransFromTo->RotateWXYZ(angle, Vector3dAxis[0], Vector3dAxis[1], Vector3dAxis[2]);
		TransFromTo->Translate(fromPos[0], fromPos[1], fromPos[2]);
		TransFromTo->Translate(Vector3dTranslate[0], Vector3dTranslate[1], Vector3dTranslate[2]);	
	}

	TransFromTo->Update();
}
vtkSmartPointer<vtkTransform> CreateTransform(CVector3d startP, CVector3d startNormal, CVector3d endP, CVector3d endNormal)
{
	CVector3d raxis;
	float rangle;

	raxis = startNormal.CrossVector(endNormal);
	raxis.Unit();
	float temp = startNormal.Dot(endNormal);
	if (temp - 1<0.001 && temp - 1>-0.001)
		rangle = 0;
	else
		rangle = acos(temp);

	vSP<vtkTransform> TM = vtkSmartPointer<vtkTransform>::New();
	TM->PostMultiply();

	TM->Translate(-startP[0], -startP[1], -startP[2]);
	TM->RotateWXYZ(rangle * 180 / 3.1415926, raxis[0], raxis[1], raxis[2]);
	TM->Translate(endP[0], endP[1], endP[2]);

	TM->Update();

	return TM;
}

vtkSmartPointer<vtkTransform>  CreateWorldToLocalTrans(CVector3d Pos, CVector3d XDir, CVector3d YDir)
{
	XDir.UnitVector();
	YDir.UnitVector();
	vSP<vtkTransform> ResultTrans = vtkSmartPointer<vtkTransform>::New();
	ResultTrans->PostMultiply();
	ResultTrans->Translate(-Pos.point[0], -Pos.point[1], -Pos.point[2]);
	ResultTrans->Update();
	//ResultTrans->Print(std::cout);
	CVector3d ZDir = XDir ^ YDir;
	ZDir.Unit();
	double elements[16] = {
		XDir[0], XDir[1], XDir[2], 0,
		YDir[0], YDir[1], YDir[2], 0,
		ZDir[0], ZDir[1], ZDir[2], 0,
			0 ,        0,         0,      1
	};


	ResultTrans->Concatenate(elements);
	ResultTrans->Update();
	//ResultTrans->Print(std::cout);

	return ResultTrans;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vtkSmartPointer<vtkTransform>  CreateWorldToLocalTransform(CVector3d Pos, CVector3d XDir, CVector3d YDir)
{
	XDir.UnitVector();
	YDir.UnitVector();
	CVector3d ZDir = XDir ^ YDir;
	ZDir.Unit();
	vSP<vtkTransform> ResultTrans = vSP<vtkTransform>::New();
	double elements[16] = {
		XDir[0], XDir[1], XDir[2], -Pos.point[0],
		YDir[0], YDir[1], YDir[2], -Pos.point[1],
		ZDir[0], ZDir[1], ZDir[2],-Pos.point[2],
			0 ,        0,         0,      1
	};
	ResultTrans->SetMatrix(elements);
	ResultTrans->Update();
	return ResultTrans;
}

vtkSmartPointer<vtkTransform>  CreateLocalToWorldTransform(CVector3d Pos, CVector3d XDir, CVector3d YDir)
{
	XDir.UnitVector();
	YDir.UnitVector();
	CVector3d ZDir = XDir ^ YDir;
	ZDir.Unit();
	vSP<vtkTransform> ResultTrans = vSP<vtkTransform>::New();
	double elements[16] = {
		XDir[0], YDir[0], ZDir[0], Pos.point[0],
		XDir[1], YDir[1], ZDir[1],  Pos.point[1],
		XDir[2],YDir[2], ZDir[2], Pos.point[2],
			0 ,        0,         0,      1
	};
	ResultTrans->SetMatrix(elements);
	ResultTrans->Update();
	return ResultTrans;
}

vtkSmartPointer<vtkTransform>  CreatePostureTransform(CVector3d fromPos, CVector3d fromXDir, CVector3d fromYDir, 
	CVector3d endPos, CVector3d endXDir, CVector3d endYDir)
{
	fromXDir.UnitVector();
	fromYDir.UnitVector();
	CVector3d fromZDir = fromXDir ^ fromYDir;
	fromZDir.Unit();

	endXDir.UnitVector();
	endYDir.UnitVector();
	CVector3d endZDir = endXDir ^ endYDir;
	endZDir.Unit();


	vSP<vtkTransform> ResultTrans = vSP<vtkTransform>::New();
	ResultTrans->PostMultiply();
	//there are two coordinates, from coordinates,  end coordinates
	//from coordinates to wolrd
	double fromCoorToWorld[16] = {
		fromXDir[0], fromYDir[0], fromZDir[0], fromPos.point[0],
		fromXDir[1], fromYDir[1], fromZDir[1],  fromPos.point[1],
		fromXDir[2],fromYDir[2], fromZDir[2], fromPos.point[2],
			0 ,        0,         0,      1
	};
	ResultTrans->SetMatrix(fromCoorToWorld);

	//world to end coordinates
	double worldToEndCoordinate[16] = {
		endXDir[0], endXDir[0], endXDir[0], -endPos.point[0],
		endYDir[1], endYDir[1], endYDir[1],  -endPos.point[1],
		endZDir[2],endZDir[2], endZDir[2], -endPos.point[2],
			0 ,        0,         0,      1
	};
	ResultTrans->Concatenate(worldToEndCoordinate);
	ResultTrans->Update();
	/*
	world = fromCoorToWorld * fromLocal
	endLocal = worldToEndCoordinate * world
	so the result is: worldToEndCoordinate * fromCoorToWorld
	*/
	return ResultTrans;
}