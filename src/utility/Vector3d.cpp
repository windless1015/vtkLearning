#include "Vector3d.h"


CVector3d::CVector3d()
{
}


CVector3d::~CVector3d()
{
}


double CVector3d::GetX()
{
	return (*this)[0];
}
double CVector3d::GetY()
{
	return (*this)[1];
}
double CVector3d::GetZ()
{
	return (*this)[2];
}

void CVector3d::UnitVector()
{


	Unit();
}
CVector3d &CVector3d::Unit()
{
	if (Length() < 0.00001)
		return *this;
	return (operator/=(Length())); 
}

void CVector3d::SetValue(double *v3d)
{
	SetPoint(v3d);
}

void CVector3d::SetValue(double x, double y, double z)
{
	SetPoint(x,  y,  z);
}


void CVector3d::GetValue(double *v)
{
	//GetValue(v);
	GetPoint(v[0], v[1], v[2]);
}





double CVector3d::DotVector(CVector3d &vector)
{
	return Dot(vector);
}

CVector3d CVector3d::CrossVector(CVector3d &vector)
{
	CVector3d tmp;
	tmp.SetValue(((*this) ^ vector).point);
	tmp.Unit();
	return tmp;
}


