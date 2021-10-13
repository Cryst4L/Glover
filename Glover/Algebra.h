#pragma once

#include "Types.h"

typedef struct vector3D {
	double d[3];
} Vector3D;

typedef struct matrix3D {
	double d[3][3];
} Matrix3D;

Vector3D V3DAdd(const Vector3D a, const Vector3D b)
{
	Vector3D r;
	for (int i = 0; i < 3; i++)
		r.d[i] = a.d[i] + b.d[i];
	return r;
}

Vector3D V3DSub(const Vector3D a, const Vector3D b)
{
	Vector3D r;
	for (int i = 0; i < 3; i++)
		r.d[i] = a.d[i] - b.d[i];
	return r;
}

Matrix3D M3DMul(const Matrix3D a, const Matrix3D b)
{
	Matrix3D r;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			r.d[i][j] = 0;
			for (int k = 0; k < 3; k++)
				r.d[i][j] += a.d[i][k] * b.d[k][j];
		}
	}

	return r;
}

Vector3D MV3DMul(const Matrix3D m, const Vector3D v)
{
	Vector3D r;
	for (int i = 0; i < 3; i++) {
		r.d[i] = 0;
		for (int j = 0; j < 3; j++) {
			r.d[i] += m.d[i][j] * v.d[j]; 
		}
	}
	return r;
}

double V3DNorm(const Vector3D v)
{
	return sqrt(v.d[0] * v.d[0] + v.d[1] * v.d[1] + v.d[2] * v.d[2]);
}

Vector3D V3DNormalize(const Vector3D v)
{
	Vector3D r;
	double n = V3DNorm(v);
	for (int i = 0; i < 3; i++)
		r.d[i] = v.d[i] / n;
	return r;
}


Matrix3D axisAngleMatrix(Vector3D axis, double angle)
{
	Vector3D u = V3DNormalize(axis);

	double ux = u.d[0];
	double uy = u.d[1];
	double uz = u.d[2];

	Matrix3D m;

	double a = cos(angle);
 	double b = (1 - cos(angle));
	double c = sin(angle);	

	m.d[0][0] = a + ux * ux * b;
	m.d[0][1] = ux * uy * b - uz * c;
	m.d[0][2] = ux * uz * b + uy * c;

	m.d[1][0] = uy * ux * b + uz * c;
	m.d[1][1] = a + uy * uy * b;
	m.d[1][2] = uy * uz * b - ux * c;

	m.d[2][0] = uz * ux * b - uy * c;
	m.d[2][1] = uz * uy * b + ux * c;
	m.d[2][2] = a + uz * uz * b;

	return m;
}

Matrix3D eulerMatrixAlpha(double angle)
{
	double c = cos(angle);
	double s = sin(angle);

	Matrix3D m = {{{   1,   0,   0},
	               {   0,   c,  -s},
	               {   0,   s,   c}}};

	return m;
}

Matrix3D eulerMatrixBeta(double angle)
{
	double c = cos(angle);
	double s = sin(angle);

	Matrix3D m = {{{   c,   0,   s},
	               {   0,   1,   0},
	               {  -s,   0,   c}}};

	return m;
}

Matrix3D eulerMatrixGamma(double angle)
{
	double c = cos(angle);
	double s = sin(angle);

	Matrix3D m = {{{   c,  -s,   0},
	               {   s,   c,   0},
	               {   0,   0,   1}}};

	return m;
}

Vector3D V3DFromVertice(const Vertice v)
{
	Vector3D r = {{v.x, v.y, v.z}};
	return r;
}

Vertice VerticeFromV3D(const Vector3D v)
{
	Vertice r;
	r.x = v.d[0];
	r.y = v.d[1];
	r.z = v.d[2];
	return r;
}




/*
	m.d[0][0] = cos(a) + ux * ux * (1 - cos(a));
	m.d[0][1] = ux * uy * (1 - cos(a)) - uz * sin(a);
	m.d[0][2] = ux * uz * (1 - cos(a)) + uy * sin(a);

	m.d[1][0] = uy * ux * (1 - cos(a)) + uz * sin(a);
	m.d[1][1] = cos(a) + uy * uy * (1 - cos(a));
	m.d[1][2] = uy * uz * (1 - cos(a)) - ux * sin(a);

	m.d[2][0] = uz * ux * (1 - cos(a)) - uy * sin(a);
	m.d[2][1] = uz * uy * (1 - cos(a)) + ux * sin(a);
	m.d[2][2] = cos(a) + uz * uz * (1 - cos(a);
*/
