#ifndef CAMERAP3SOLUTION_H
#define CAMERAP3SOLUTION_H

#include <windows.h>
#include <GL/glut.h>
#include "Matrix.h"

class Camera{
private:
	Point eye, look;
	Vector u, v, n, up;
	float H, W, viewAngle, aspect, nearDist, farDist;
	void setModelViewMatrix();

public:
	Camera(){}
	void set(float, float, float, float, float, float, float, float, float);
	void set(Point& eye, Point& look, Vector& up);
	void setShape(float vAng, float asp, float nearD, float farD);
	Vector GetC(int, int, int, int);
	Point GetEye();
};

#endif