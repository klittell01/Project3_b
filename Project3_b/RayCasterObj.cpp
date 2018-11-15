#include "RayCasterObj.h"
#include <string>
#include <algorithm> 

void PrintNum(float& n){
	std::ostringstream oss;
	oss << n;
	std::string s(oss.str());
	std::cout << s;
}


void PrintNum(int& n){
	std::ostringstream oss;
	oss << n;
	std::string s(oss.str());
	std::cout << s;
}



RayCasterObj::RayCasterObj(){
	hide = false; 
}


RayCasterObj::RayCasterObj(int t){
	RayCasterObj();
	type = t;
}


float RayCasterObj::HitTime(Point& s, Vector& c){
	static Point p;
	static Vector v;
	float A, B, C, discriminant, base, quad, hit1, hit2;

	if (hide){
		hit1 = -1;
		hit2 = -1;
	}
	else{
		switch (type){
			case RAYCASTER_SPHERE:

				p.set((invForm * s).x, (invForm * s).y, (invForm * s).z);
				v.set((invForm * c).x, (invForm * c).y, (invForm * c).z);

				A = pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2);
				B = v.dot(p);
				C = pow(p.x, 2) + pow(p.y, 2) + pow(p.z, 2) - 1;

				discriminant = pow(B, 2) - (A*C);
				if (discriminant > 0){
					base = -1*(B/A);
					quad = sqrt(discriminant)/A;

					hit1 = base + quad;
					hit2 = base - quad;
				}
				else{
					hit1 = 0;
					hit2 = 0;
				}

				break;
			
		}
	}

	if (hit1 > hit2){
		return hit2;
	}
	else{
		return hit1;
	}
}

Color RayCasterObj::PhongShading(Point& p, Vector& dir, int depth, RayCasterObj obj, RayCasterLight lights[], float t) {
	Color color, reflection;
	//RayCasterLightNode* cur;
	float lambert, phong;
	float redTotal, greenTotal, blueTotal;

	////////////////////////
	/// ambient Lighting ///
	////////////////////////

	// ambient red
	color.r = obj.objColor.r * lights[0].r;
	float redAmb = 0;
	for (int i = 0; i < 4; i++) {
		if (lights[i].lightType == "ambient") {
			float tmp;
			tmp = obj.ambient.r * lights[i].r;
			redAmb = redAmb + tmp;
		}
	}

	// ambient green
	color.g = obj.objColor.g * lights[0].g;
	float greenAmb = 0;
	for (int i = 0; i < 4; i++) {
		if (lights[i].lightType == "ambient") {
			float tmp;
			tmp = obj.ambient.g * lights[i].g;
			greenAmb = greenAmb + tmp;
		}
	}
	color.g = obj.objColor.g + greenAmb;

	// ambient blue
	color.b = obj.objColor.b * lights[0].b;
	float blueAmb = 0;
	for (int i = 0; i < 4; i++) {
		if (lights[i].lightType == "ambient") {
			float tmp;
			tmp = obj.ambient.b * lights[i].b;
			blueAmb = blueAmb + tmp;
		}
	}
	color.b = obj.objColor.b + blueAmb;

	

	////////////////////////
	/// diffuse lighting ///
	////////////////////////
	// the issue lies within the diffuse lighting algorithm
	// diffuse red
	float redDiffuse = 0;
	for (int i = 0; i < 4; i++) {
		if (lights[i].lightType == "diffuse") {
			float tmp, myMax, lDotN;
			tmp = obj.diffuse.r * lights[i].r;
			Point hitPoint, centerPoint;
			Vector hitVector, normalVector, dirVector;
			hitVector = dir * t;
			hitPoint = p + hitVector;
			centerPoint.set(obj.xForm.matrix.m[12], obj.xForm.matrix.m[13], obj.xForm.matrix.m[14]);
			normalVector = hitPoint - centerPoint;
			dirVector.set(lights[i].direction.x, lights[i].direction.y, lights[i].direction.z);
			dirVector.normalize();
			lDotN = lights[i].direction.dot(normalVector);

			myMax = std::max(lDotN, 0.0f);
			redDiffuse = tmp * myMax;
		}
	}

	// diffuse green
	float greenDiffuse = 0;
	for (int i = 0; i < 4; i++) {
		if (lights[i].lightType == "diffuse") {
			float tmp, myMax, lDotN;
			tmp = obj.diffuse.g * lights[i].g;
			Point hitPoint, centerPoint;
			Vector hitVector, normalVector, dirVector;
			hitVector = dir * t;
			hitPoint = p + hitVector;
			centerPoint.set(obj.xForm.matrix.m[12], obj.xForm.matrix.m[13], obj.xForm.matrix.m[14]);
			normalVector = hitPoint - centerPoint;
			dirVector.set(lights[i].direction.x, lights[i].direction.y, lights[i].direction.z);
			dirVector.normalize();

			lDotN = lights[i].direction.dot(normalVector);

			myMax = std::max(lDotN, 0.0f);
			greenDiffuse = tmp * myMax;
		}
	}

	// diffuse blue
	float blueDiffuse = 0;
	for (int i = 0; i < 4; i++) {
		if (lights[i].lightType == "diffuse") {
			float tmp, myMax, lDotN;
			tmp = obj.diffuse.b * lights[i].b;
			Point hitPoint, centerPoint;
			Vector hitVector, normalVector, dirVector;
			hitVector = dir * t;
			hitPoint = p + hitVector;
			centerPoint.set(obj.xForm.matrix.m[12], obj.xForm.matrix.m[13], obj.xForm.matrix.m[14]);
			normalVector = hitPoint - centerPoint;
			dirVector.set(lights[i].direction.x, lights[i].direction.y, lights[i].direction.z);
			dirVector.normalize();

			lDotN = lights[i].direction.dot(normalVector);

			myMax = std::max(lDotN, 0.0f);
			blueDiffuse = tmp * myMax;
		}
	}

	/////////////////////////
	/// specular lighting ///
	/////////////////////////
	// also an issue here, the shinyness i had is gone :(
	// specular red
	float redSpecular = 0;
	for (int i = 0; i < 4; i++) {
		if (lights[i].lightType == "specular") {
			float tmp, myMax, hDotN, exp;
			tmp = obj.specular.r * lights[i].r;
			Point hitPoint, centerPoint, eyePoint;
			eyePoint.set(0, 0, 1);
			Vector hitVector, normalVector, viewVector, halfVector;
			hitVector = dir * t;
			hitPoint = p + hitVector;
			centerPoint.set(obj.xForm.matrix.m[12], obj.xForm.matrix.m[13], obj.xForm.matrix.m[14]);
			normalVector = hitPoint - centerPoint;
		//	normalVector.normalize();
			viewVector = eyePoint - hitPoint;
		//	viewVector.normalize();
			halfVector = (viewVector + lights[i].direction);
		//	halfVector = halfVector * .5f;
		//	halfVector.normalize();
			hDotN = halfVector.dot(normalVector);
			exp = obj.shinyness;
			myMax = std::pow(std::max(hDotN, 0.0f), exp);
			redSpecular = tmp * myMax;
		}
	}

	// specular green
	float greenSpecular = 0;
	for (int i = 0; i < 4; i++) {
		if (lights[i].lightType == "specular") {
			float tmp, myMax, hDotN, exp;
			tmp = obj.specular.g * lights[i].g;
			Point hitPoint, centerPoint, eyePoint;
			eyePoint.set(0, 0, 1);
			Vector hitVector, normalVector, viewVector, halfVector;
			hitVector = dir * t;
			hitPoint = p + hitVector;
			centerPoint.set(obj.xForm.matrix.m[12], obj.xForm.matrix.m[13], obj.xForm.matrix.m[14]);
			normalVector = hitPoint - centerPoint;
		//	normalVector.normalize();
			viewVector = eyePoint - hitPoint;
		//	viewVector.normalize();
			halfVector = (viewVector + lights[i].direction);
		//	halfVector = halfVector * .5f;
		//	halfVector.normalize();
			hDotN = halfVector.dot(normalVector);
			exp = obj.shinyness;
			myMax = std::pow(std::max(hDotN, 0.0f), exp);
			greenSpecular = tmp * myMax;
		}
	}

	// specular blue
	float blueSpecular = 0;
	for (int i = 0; i < 4; i++) {
		if (lights[i].lightType == "specular") {
			float tmp, myMax, hDotN, exp;
			tmp = obj.specular.b * lights[i].b;
			Point hitPoint, centerPoint, eyePoint;
			eyePoint.set(0, 0, 1);
			Vector hitVector, normalVector, viewVector, halfVector;
			hitVector = dir * t;
			hitPoint = p + hitVector;
			centerPoint.set(obj.xForm.matrix.m[12], obj.xForm.matrix.m[13], obj.xForm.matrix.m[14]);
			normalVector = hitPoint - centerPoint;
		//	normalVector.normalize();
			viewVector = eyePoint - hitPoint;
		//	viewVector.normalize();
			halfVector = (viewVector + lights[i].direction);
		//  halfVector = halfVector * .5f;
		//	halfVector.normalize();
			hDotN = halfVector.dot(normalVector);
			exp = obj.shinyness;
			myMax = std::pow(std::max(hDotN, 0.0f), exp);
			blueSpecular = tmp * myMax;
		}
	}

	redTotal = redAmb + redSpecular;// +redDiffuse;
	greenTotal = greenAmb + greenSpecular;// +greenDiffuse;
	blueTotal = blueAmb + blueSpecular;// +blueDiffuse;

	color.r = redTotal;
	color.g = greenTotal;
	color.b = blueTotal;
	return color;
}





int RayCasterObjBuffer::Size() {
	return count;
}


int RayCasterObjBuffer::AddObj(int t) {

	if (front == NULL) {
		front = new RayCasterObjNode;
		front->obj.type = t;
		end = front;
	}
	else {
		end->next = new RayCasterObjNode;
		end = end->next;
		end->obj.type = t;
	}
	return ++count;
}


RayCasterObjNode* RayCasterObjBuffer::GetObj(int c) {
	int i;
	RayCasterObjNode* cur;

	if ((c > count) || (c < 1)) {
		return NULL;
	}
	else {
		cur = front;
		for (i = 1; i < c; ++i) {
			cur = cur->next;
		}
		return cur;
	}
}



//find closest hit point
Color RayCasterObjBuffer::CastRay(Point& s, Vector& c, RayCasterLight lights[], RayCasterObjBuffer objBuf) {
	RayCasterObjNode* cur;
	RayCasterObjNode* closest;
	RayCasterObj castObj;
	float closestHit = -1.0;
	float hit;
	int i;

	for (i = 1; i <= count; ++i) {
		cur = GetObj(i);
		if (!cur->obj.hide) {
			hit = cur->obj.HitTime(s, c);
			if (((hit > 0.0f) && ((hit < closestHit) || (closestHit <= 0)))) {
				closestHit = hit;
				closest = cur;
			}
		}
	}

	Color tmp;
	tmp.r = 1.0;
	tmp.b = 0;
	tmp.g = 0;
	tmp.a = 1;

	if (closestHit <= 0) {
		Color newBG;
		newBG.r = background.r * lights[0].r;
		newBG.g = background.g * lights[0].g;
		newBG.b = background.b * lights[0].b;

		return newBG;
	}
	else {
		Color myColor, itemColor;
		itemColor = closest->obj.objColor;
		myColor = castObj.PhongShading(s, c, 1, closest->obj, lights, hit);
		itemColor = myColor;
		//printf("obj color before shade is: %f, shade color is: %f\n", closest->obj.objColor.g, myColor.g);
		return itemColor;
		
	}
}

