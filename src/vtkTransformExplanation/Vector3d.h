#include "math.h"
#include <ostream>


class CVector3d
{
public:
	double point[3];
	CVector3d();
	CVector3d(double x, double y, double z) { point[0] = x; point[1] = y; point[2] = z; };
	CVector3d(double* v) { point[0] = v[0]; point[1] = v[1]; point[2] = v[2]; };
	~CVector3d();

	void GetPoint(double& x, double& y, double& z) { x = point[0]; y = point[1]; z = point[2]; }
	CVector3d& SetPoint(double x, double y, double z) { point[0] = x; point[1] = y;	point[2] = z; return *this; }
	CVector3d& SetPoint(double p[]) { point[0] = p[0];	point[1] = p[1];	point[2] = p[2];	return *this; }
	CVector3d& SetPoint(float p[]) { point[0] = p[0];	point[1] = p[1];	point[2] = p[2];	return *this; }

	void UnitVector();
	double GetX();
	double GetY();
	double GetZ();
	void SetValue(double* v3d);
	void SetValue(double x, double y, double z);
	void GetValue(double* v);
	double DotVector(CVector3d& vector);
	CVector3d CrossVector(CVector3d& vector);
	CVector3d operator *= (double d) { CVector3d nV3d; nV3d.SetPoint(point[0] * d, point[1] * d, point[2] * d); return nV3d; };


	double& operator[](int i) { return point[i]; }
	double operator[](int i) const { return point[i]; }

	CVector3d& operator=(CVector3d p) { SetPoint(p.point); return *this; }
	CVector3d& operator+=(CVector3d& p) { point[0] += p[0];	point[1] += p[1]; point[2] += p[2];	return *this; }
	CVector3d& operator-=(CVector3d& p) { point[0] -= p[0];	point[1] -= p[1]; point[2] -= p[2];	return *this; }
	CVector3d& operator/=(double v) { point[0] /= v; point[1] /= v; point[2] /= v;	return *this; }
	CVector3d& operator^=(CVector3d& p) { SetPoint(point[1] * p.point[2] - point[2] * p.point[1], point[2] * p.point[0] - point[0] * p.point[2], point[0] * p.point[1] - point[1] * p.point[0]); return *this; }

	CVector3d operator+(double v) { return CVector3d(point[0] + v, point[1] + v, point[2] + v); }
	CVector3d operator-(double v) { return CVector3d(point[0] - v, point[1] - v, point[2] - v); }
	CVector3d operator*(double v) { return CVector3d(point[0] * v, point[1] * v, point[2] * v); }
	//CVector3d operator/(double v) { return CVector3d(point[0] / v, point[1] / v, point[2] / v); }
	CVector3d operator-() { return CVector3d(-point[0], -point[1], -point[2]); }
	double Dot(CVector3d& p) { return point[0] * p.point[0] + point[1] * p.point[1] + point[2] * p.point[2]; }

	double Length() { return sqrt(point[0] * point[0] + point[1] * point[1] + point[2] * point[2]); }
	CVector3d& Unit();

	friend CVector3d operator*(double v, CVector3d& p) { return p * v; };
	friend CVector3d operator/(CVector3d& p, double v) { return p * (1. / v); }
	friend CVector3d operator+(CVector3d& p1, CVector3d& p2) { return CVector3d(p1[0] + p2[0], p1[1] + p2[1], p1[2] + p2[2]); }
	friend CVector3d operator-(CVector3d& p1, CVector3d& p2) { return CVector3d(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]); }
	friend double operator*(CVector3d& p1, CVector3d& p2) { return p1[0] * p2[0] + p1[1] * p2[1] + p1[2] * p2[2]; }
	friend CVector3d operator^(CVector3d& p1, CVector3d& p2) { return CVector3d(p1[1] * p2[2] - p1[2] * p2[1], p1[2] * p2[0] - p1[0] * p2[2], p1[0] * p2[1] - p1[1] * p2[0]); } //œÚ¡ø≤Ê≥À
	friend bool operator==(CVector3d& p1, CVector3d& p2) { return ((p1[0] == p2[0]) && (p1[1] == p2[1]) && (p1[2] == p2[2])); }
	friend bool operator!=(CVector3d& p1, CVector3d& p2) { return ((p1[0] != p2[0]) || (p1[1] != p2[1]) || (p1[2] != p2[2])); }
	friend bool operator<(CVector3d& p1, CVector3d& p2) { return (p1[0] <= p2[0] && p1[1] <= p2[1] && p1[2] <= p2[2]); }
	//friend ostream& operator<<(ostream & os, CVector3d & p) { os << "CVector3d [" << p[0] << ", " << p[1] << ", " << p[2] << "]\n"; return os; }


};