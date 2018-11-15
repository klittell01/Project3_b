
#include <windows.h>

#include <string>
#include "CameraP3solution.h"
#include "RayCasterObj.h"
#include <GL/glut.h>


#define GL 0
#define CASTER 1

int const screenWidth = 640;
int const screenHeight = 480;
int const pixelCount = screenWidth * screenHeight;
double const winHt = 1.0;

int Drawing = CASTER;
int clickActionX, clickActionY;
float tmp = 0.0;
bool initComplete = false;

RayCasterLight lights[5];
RayCasterObjBuffer objBuffer;
RayCasterObj casterObj;
Camera cam;
int resolution = 1;

void MyDisplay();

void PrintNm(float& n){
	std::ostringstream oss;
	oss << n;
	std::string s(oss.str());
	std::cout << s;
}


void PrintNm(int& n){
	std::ostringstream oss;
	oss << n;
	std::string s(oss.str());
	std::cout << s;
}


void ResetCamera(){

	cam.set(0, 0, 1, 0, 0, 0, 0.0, 1.0, 0.0);
	MyDisplay();
}







void DrawRaster(){
	Vector c;
	Color color, myColor;
	float pixelCount;
	int i, j, curPixel, copyPixel;

	pixelCount = screenWidth*screenHeight;
	
	float* pixels = new float[pixelCount*3];

	for (j=0; j<screenHeight; ++j){ //loop through all pixels
		for (i=0; i<screenWidth; ++i){
			curPixel = ((j*screenWidth) + i)*3;

			if (j%resolution == 0){
				if (i%resolution == 0){
					c.set((cam.GetC(screenWidth, screenHeight, i, j)).x, (cam.GetC(screenWidth, screenHeight, i, j)).y,
						(cam.GetC(screenWidth, screenHeight, i, j)).z);
					Point myP;
					myP.set((cam.GetEye()).x, (cam.GetEye()).y, (cam.GetEye()).z);
					color = objBuffer.CastRay(myP, c, lights, objBuffer); //finding hit point and shading for the pixel
				}
				else{
					copyPixel = curPixel - 3;
					color.r = pixels[copyPixel];
					color.g = pixels[copyPixel+1];
					color.b = pixels[copyPixel+2];
				}
			}
			else{
				copyPixel = curPixel - (screenWidth * 3);
				color.r = pixels[copyPixel];
				color.g = pixels[copyPixel+1];
				color.b = pixels[copyPixel+2];
			}

			pixels[curPixel] = color.r;
			pixels[curPixel+1] = color.g;
			pixels[curPixel+2] = color.b;
		}
	}
	glDrawPixels(screenWidth,screenHeight,GL_RGB,GL_FLOAT,pixels);
}




void MyDisplay(){

	if (initComplete){

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
		DrawRaster(); //DRAW USING RAY TRACING METHODS
		glutSwapBuffers();
	}
}




void setRayTracingMaterials(){
	
	RayCasterLight globalAmb;
	globalAmb.type = 0; // global ambient light
	globalAmb.r = .25;
	globalAmb.g = .05;
	globalAmb.b = .05;
	globalAmb.a = 1.0;
	globalAmb.lightType = "ambient";
	lights[0] = globalAmb;

	RayCasterLight pointLight;
	pointLight.type = 1; // point light
	Point pointPos1;
	pointPos1.set(200, 100, 50);
	pointLight.location = pointPos1;
	pointLight.r = .65;
	pointLight.g = .65;
	pointLight.b = .65;
	pointLight.a = 1.0;
	pointLight.lightType = "ambient";
	lights[1] = pointLight;

	RayCasterLight spotLight;
	pointLight.type = 2; // spot light
	Point pointPos2;
	Vector spotDir;
	spotDir.set(0, 0, -1);
	spotLight.direction = spotDir;
	pointPos2.set(1, 0, 1);
	spotLight.location = pointPos2;
	spotLight.r = .4;
	spotLight.g = .4;
	spotLight.b = .7;
	spotLight.a = 1.0;
	spotLight.lightType = "specular";
	spotLight.angle = .3;
	spotLight.exp = .5;
	lights[2] = spotLight;

	RayCasterLight dirLight;
	dirLight.type = 3; // dir light
	Vector dir;
	dir.set(-400, 692, 0);
	dirLight.direction = dir;
	dirLight.r = .8;
	dirLight.g = .8;
	dirLight.b = .6;
	dirLight.a = 1.0;
	dirLight.lightType = "diffuse";
	lights[3] = dirLight;

	int sphere1 = objBuffer.AddObj(RAYCASTER_SPHERE);
	objBuffer.GetObj(sphere1)->obj.xForm.Translate(0.125, -0.25, -1);
	objBuffer.GetObj(sphere1)->obj.xForm.Scale(0.125,0.125,0.125);
	objBuffer.GetObj(sphere1)->obj.invForm = objBuffer.GetObj(sphere1)->obj.xForm.matrix.Inverse();
	objBuffer.GetObj(sphere1)->obj.objColor.r=1.0;
	objBuffer.GetObj(sphere1)->obj.objColor.g=0.0;
	objBuffer.GetObj(sphere1)->obj.objColor.b=0.0;
	// ambient material properties
	objBuffer.GetObj(sphere1)->obj.ambient.r = 0.5;
	objBuffer.GetObj(sphere1)->obj.ambient.g = 0.5;
	objBuffer.GetObj(sphere1)->obj.ambient.b = 0.5;
	objBuffer.GetObj(sphere1)->obj.ambient.a = 1.0;
	// diffuse material properties
	objBuffer.GetObj(sphere1)->obj.diffuse.r = 0.7;
	objBuffer.GetObj(sphere1)->obj.diffuse.g = 0.7;
	objBuffer.GetObj(sphere1)->obj.diffuse.b = 0.7;
	objBuffer.GetObj(sphere1)->obj.diffuse.a = 1.0;
	// specular material properties
	objBuffer.GetObj(sphere1)->obj.specular.r = 0.9;
	objBuffer.GetObj(sphere1)->obj.specular.g = 0.9;
	objBuffer.GetObj(sphere1)->obj.specular.b = 0.9;
	objBuffer.GetObj(sphere1)->obj.specular.a = 1.0;
	// shinyness material properity
	objBuffer.GetObj(sphere1)->obj.shinyness = 3.0;

	int sphere2 = objBuffer.AddObj(RAYCASTER_SPHERE);
	objBuffer.GetObj(sphere2)->obj.xForm.Translate(0.5, 0.5, -1.75);
	objBuffer.GetObj(sphere2)->obj.xForm.Scale(0.375,0.375,0.375);
	objBuffer.GetObj(sphere2)->obj.invForm = objBuffer.GetObj(sphere2)->obj.xForm.matrix.Inverse();
	objBuffer.GetObj(sphere2)->obj.objColor.r=0.0;
	objBuffer.GetObj(sphere2)->obj.objColor.g=1.0;
	objBuffer.GetObj(sphere2)->obj.objColor.b=0.0;
	// ambient material properties
	objBuffer.GetObj(sphere2)->obj.ambient.r = 0.5;
	objBuffer.GetObj(sphere2)->obj.ambient.g = 0.5;
	objBuffer.GetObj(sphere2)->obj.ambient.b = 0.5;
	objBuffer.GetObj(sphere2)->obj.ambient.a = 1.0;
	// diffuse material properties
	objBuffer.GetObj(sphere2)->obj.diffuse.r = 0.4;
	objBuffer.GetObj(sphere2)->obj.diffuse.g = 0.4;
	objBuffer.GetObj(sphere2)->obj.diffuse.b = 0.4;
	objBuffer.GetObj(sphere2)->obj.diffuse.a = 1.0;
	// specular material properties
	objBuffer.GetObj(sphere2)->obj.specular.r = 0.001;
	objBuffer.GetObj(sphere2)->obj.specular.g = 0.001;
	objBuffer.GetObj(sphere2)->obj.specular.b = 0.001;
	objBuffer.GetObj(sphere2)->obj.specular.a = 1.0;
	// shinyness material properity
	objBuffer.GetObj(sphere2)->obj.shinyness = 1.0;
	
	int sphere3 = objBuffer.AddObj(RAYCASTER_SPHERE);
	objBuffer.GetObj(sphere3)->obj.xForm.Translate(-0.5, 0, -2.5);
	objBuffer.GetObj(sphere3)->obj.xForm.Scale(0.75,0.75,0.75);
	objBuffer.GetObj(sphere3)->obj.invForm = objBuffer.GetObj(sphere3)->obj.xForm.matrix.Inverse();
	objBuffer.GetObj(sphere3)->obj.objColor.r=0.0;
	objBuffer.GetObj(sphere3)->obj.objColor.g=0.0;
	objBuffer.GetObj(sphere3)->obj.objColor.b=1.0;
	// ambient material properties
	objBuffer.GetObj(sphere3)->obj.ambient.r = 0.23125;
	objBuffer.GetObj(sphere3)->obj.ambient.g = 0.23125;
	objBuffer.GetObj(sphere3)->obj.ambient.b = 0.23125;
	objBuffer.GetObj(sphere3)->obj.ambient.a = 1.0;
	// diffuse material properties
	objBuffer.GetObj(sphere3)->obj.diffuse.r = 0.2775;
	objBuffer.GetObj(sphere3)->obj.diffuse.g = 0.2775;
	objBuffer.GetObj(sphere3)->obj.diffuse.b = 0.2775;
	objBuffer.GetObj(sphere3)->obj.diffuse.a = 1.0;
	// specular material properties
	objBuffer.GetObj(sphere3)->obj.specular.r = 0.6;
	objBuffer.GetObj(sphere3)->obj.specular.g = 0.6;
	objBuffer.GetObj(sphere3)->obj.specular.b = 0.6;
	objBuffer.GetObj(sphere3)->obj.specular.a = 1.0;
	// shinyness material properity
	objBuffer.GetObj(sphere3)->obj.shinyness = 5.8;


}

void setObjectMaterials(){
	setRayTracingMaterials();
	}




void main(int argc, CHAR *argv){
	glutInit(&argc, &argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(250, 50);
	glutCreateWindow("Ray Casting!");

	glClearColor(0.5 * 0.25, 0.5 * 0.05, 0.5 * 0.05, 1.0); //for opengl use only
	glColor3f(0.0, 0.0, 0.0);

	Color backGround;
	backGround.r = 0.5;
	backGround.g = 0.5;
	backGround.b = 0.5;
	objBuffer.background = backGround;

	glPointSize(1.0);
	glLineWidth(1.0);
	

    setObjectMaterials();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(-winHt*screenWidth/screenHeight, winHt*screenWidth/screenHeight, -winHt, winHt, 0.1, 200.0);

	cam.setShape(30.0f, float(screenWidth)/float(screenHeight), 1.0f, 200.0f);
	ResetCamera();

	glutDisplayFunc(MyDisplay);

	initComplete = true;
	glutMainLoop();
}