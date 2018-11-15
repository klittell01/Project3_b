#ifndef RAYCASTEROBJ_H
#define RAYCASTEROBJ_H

#include "Matrix.h"
#include <string>

#define RAYCASTER_SPHERE 0
#define RAYCASTER_PLANE 1


class RayCasterObjBuffer;

class Color{
public:
	float r, g, b, a;

	Color(){r = 0.0; g = 0.0; b = 0.0; a = 1;}
};

class Material {
public:
	float r, g, b, a;

	Material() { r = 0.0; g = 0.0; b = 0.0; a = 1; }
};

class RayCasterLight{
public:
	Point location;
	Vector direction;
	float r,g,b,a;

	std::string lightType;
	float tmp;
	float exp; 
	float cutoff;
	float angle;
	int type;
	bool hide;

};


//objects in the ray caster world, spheres only incorporated currently
class RayCasterObj{
public:
	bool hide;
	int type;
	Transform xForm;
	Matrix invForm;
	
	Color objColor;
	Material ambient;
	Material diffuse;
	Material specular;

	float shinyness;


	RayCasterObj();
	RayCasterObj(int t);

	float DegToRad(float theta){ float PI=  3.14159; return (theta* PI /180);}
	float HitTime(Point&, Vector&);
	Color PhongShading(Point&, Vector&, int, RayCasterObj, RayCasterLight[], float);
	//void SetMaterial(int, float, float, float, float);
};

//Linked list of objects in the ray caster world
class RayCasterObjNode{
public:
	RayCasterObjNode(){next = NULL;}

	RayCasterObjNode* next;
	RayCasterObj obj;
};

//handle the object list/buffer
class RayCasterObjBuffer{
private:
	int count;

public:

	Color background;

	RayCasterObjBuffer(){background.r = .8f; background.g = .8f; background.b = .8f; background.a = 1; count = 0; front = NULL; end = NULL;}

	RayCasterObjNode* front;
	RayCasterObjNode* end;

	int Size();

	int AddObj(int);
	RayCasterObjNode* GetObj(int);
	void HideObj(int, bool);

	Color CastRay(Point&, Vector&, RayCasterLight lights[], RayCasterObjBuffer);
};

#endif