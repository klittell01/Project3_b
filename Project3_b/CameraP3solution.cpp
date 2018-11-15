#include "CameraP3solution.h"

void PrintN(float& n){
	std::ostringstream oss;
	oss << n;
	std::string s(oss.str());
	std::cout << s;
}


// setModelViewMatrix used in the book "Computer Graphics using OpenGL" Hill/Kelley page 336
void Camera::setModelViewMatrix(){
	float m[16];
	Vector eVec(eye.x, eye.y, eye.z);

	m[0] = u.x;
	m[1] = v.x;
	m[2] = n.x;
	m[3] = 0;
	m[4] = u.y;
	m[5] = v.y;
	m[6] = n.y;
	m[7] = 0;
	m[8] = u.z;
	m[9] = v.z;
	m[10] = n.z;
	m[11] = 0;
	m[12] = -eVec.dot(u);
	m[13] = -eVec.dot(v);
	m[14] = -eVec.dot(n);
	m[15] = 1.0;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m);
}

void Camera::set(float ex, float ey, float ez, float lx, float ly, float lz, float ux, float uy, float uz){
	Point e(ex, ey, ez);
	Point l(lx, ly, lz);
	Vector y(ux, uy, uz);
	set(e, l, y);
}

// set used in the book "Computer Graphics using OpenGL" Hill/Kelley page 336
void Camera::set(Point& e, Point& l, Vector& y){
	eye.set(e);
	look.set(l);
	up.set(y);
	n.set((e.x - l.x), (e.y - l.y), (e.z - l.z));
	u.set((y.cross(n).x), (y.cross(n).y), (y.cross(n).z));
	n.normalize();
	u.normalize();
	v.set((n.cross(u).x), (n.cross(u).y), (n.cross(u).z));
	v.normalize();
	setModelViewMatrix();
}



//Change camera to perspective or parallel
void Camera::setShape(float vAng, float asp, float nearD, float farD){
	float degrees;
	viewAngle = vAng;
	aspect = asp;
	nearDist = nearD;
	farDist = farD;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(viewAngle, aspect, nearDist, farDist);
	
	degrees = ((viewAngle/2)*pi)/180;
	H = nearDist*tan(degrees);
	W = H*aspect;

	printf("H:");
	PrintN(H);
	printf("\nW:");
	PrintN(W);
	printf("\n");
}


Vector Camera::GetC(int width, int height, int i, int j){
	Vector c;
	Vector nn;
	Vector uu;
	Vector vv;
	Vector adder;
	nn.set((n*(nearDist*-1)).x, (n*(nearDist*-1)).y, (n*(nearDist*-1)).z);
	uu.set((u*(W*((float(2 * i) / float(width)) - 1.0f))).x, (u*(W*((float(2 * i) / float(width)) - 1.0f))).y,
		(u*(W*((float(2 * i) / float(width)) - 1.0f))).z);
	vv.set((v*(H*((float(2 * j) / float(height)) - 1.0f))).x, (v*(H*((float(2 * j) / float(height)) - 1.0f))).y,
		(v*(H*((float(2 * j) / float(height)) - 1.0f))).z);
	adder = nn + uu + vv;
	c.set(adder.x, adder.y, adder.z);
	c.normalize();
	return c;
}

Point Camera::GetEye(){
	return eye;
}