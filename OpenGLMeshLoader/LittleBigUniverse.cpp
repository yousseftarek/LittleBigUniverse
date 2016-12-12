//kernel32.lib, user32.lib, gdi32.lib and advapi32.lib
//#pragma comment(lib, "legacy_stdio_definitions.lib")
#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <string>
#include <iostream>
#include <glut.h>
#include <math.h>
#include "SOIL.h"

void initializeSpace(void);
bool canShowText(int, int);

using namespace std;



#define degree 0.0174533
#define DEG2RAD 3.14159/180.0

int WIDTH = 1280;
int HEIGHT = 720;

int mouseX= WIDTH/2, mouseY = HEIGHT/2;

double rotY = 0;
float angleMercury = 0;
float angleVenus = 0;
float angleEarth = 0;
float angleMars = 0;
float angleJupiter = 0;
float angleUranus = 0;
float angleSaturn = 0;
float angleNeptune = 0;
float anglePluto = 0;

bool showText = true;



//Textures
GLuint tex, sunTexture, mercuryTexture, venusTexture, earthTexture, marsTexture, jupiterTexture, saturnTexture, uranusTexture, neptuneTexture, plutoTexture;


float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle = 0.0;
float lastx, lasty;

double rotSunX, rotSunY, rotSunZ;

struct Moon {
	double radius;
	double distanceFromPlanet;
	char texture[];
};

struct Planet {
	std::string name;
	double radius;
	std::string texture;
}Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune, Pluto; //Yes, Pluto is a f**king planet !


struct Star {
	double radius;
}TheSun;

void TextBig(int x, int y, int z, float r, float g, float b, const char * string) {
	const char *c;
	glColor3f(r, g, b);
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}
}

void TextSmall(int x, int y, int z, float r, float g, float b, const char * string) {
	const char *c;
	glColor3f(r, g, b);
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
	}
}

void initializePlanets() {
	Mercury.radius = 24;
	Mercury.name = "Mercury";
	Mercury.texture = "textures/mercury/mercurymap.bmp";

	Venus.radius = 60;
	Venus.name = "Venus";
	Venus.texture = "textures/venus/venusmap.bmp";

	Earth.radius = 63;
	Earth.name = "Earth";
	Earth.texture = "textures/earth/earth.bmp";

	Mars.radius = 34;
	Mars.name = "Mars";
	Mars.texture = "textures/mars/marsmap1k.bmp";

	Jupiter.radius = 700;
	Jupiter.name = "Jupiter";
	Jupiter.texture = "textures/jupiter/jupiter.bmp";

	Saturn.radius = 582;
	Saturn.name = "Saturn";
	Saturn.texture = "textures/saturn/saturnmap.bmp";

	Uranus.radius = 253;
	Uranus.name = "Uranus";
	Uranus.texture = "textures/uranus/uranusmap.bmp";

	Neptune.radius = 246;
	Neptune.name = "Neptune";
	Neptune.texture = "textures/neptune/neptunemap.bmp";

	Pluto.radius = 15; 
	Pluto.name = "Pluto";
	Pluto.texture = "textures/pluto/plutomap2k.bmp";

	TheSun.radius = 50;
}

void DrawEllipse(float radiusX, float radiusY)
{
	int i;
	glBegin(GL_LINE_LOOP);
	for (i = 0; i<360; i++)
	{
		float rad = i*DEG2RAD;
		glVertex3f(cos(rad)*radiusX, 0,
			sin(rad)*radiusY);
	}
	glEnd();
}

void drawRings(int inner, int outer, int angle, char* texture) {

	glPushMatrix();
	glRotated(angle, 0, 0, 1);
	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricTexture(qobj, 1);
	gluDisk(qobj, inner, outer, 100, 100);
	glPopMatrix();

}

void drawPlanet(Planet planet, int x, int y, int z, double rotFactor) {

	char *texturePath = &planet.texture[0];
	float radOne = 10.0;
	float radTwo = 20.0;
	float angle = 0;

	if (planet.name == "Mars") {
		glBindTexture(GL_TEXTURE_2D, marsTexture);
		radTwo = 227;
		radOne = radTwo / 2;
		angleMars += degree/2;
		angle = angleMars;
	}
	else if (planet.name == "Earth") {
		//tex_Earth.Load("", textures[2]);
		glBindTexture(GL_TEXTURE_2D, earthTexture);
		radTwo = 149;
		radOne = radTwo / 2;
		angleEarth += degree;
		angle = angleEarth;
	}
	else if (planet.name == "Mercury") {
		glBindTexture(GL_TEXTURE_2D, mercuryTexture);
		radTwo = 58;
		radOne = radTwo / 2;
		angleMercury += degree*4.14772727;
		angle = angleMercury;
	}
	else if (planet.name == "Jupiter") {
		glBindTexture(GL_TEXTURE_2D, jupiterTexture);
		radTwo = 778;
		radOne = radTwo/2;
		angleJupiter += degree;
		angle = angleJupiter;
	}
	else if (planet.name == "Neptune") {
		glBindTexture(GL_TEXTURE_2D, neptuneTexture);
		radTwo = 4498;
		radOne = radTwo / 2;
		angleNeptune += degree*0.00608;
		angle = angleNeptune;
	}
	else if (planet.name == "Pluto") {
		glBindTexture(GL_TEXTURE_2D, plutoTexture);
		radTwo = 5906;
		radOne = radTwo / 2;
		anglePluto += degree*0.0040555;
		angle = anglePluto;
	}
	else if (planet.name == "Saturn") {
		glBindTexture(GL_TEXTURE_2D, saturnTexture);
		radTwo = 1426;
		radOne = radTwo / 2;
		angleSaturn += degree*0.0331818;
		angle = angleSaturn;
	}
	else if (planet.name == "Uranus") {
		glBindTexture(GL_TEXTURE_2D, uranusTexture);
		radTwo = 2870;
		radOne = radTwo / 2;
		angleUranus += degree*0.0117741;
		angle = angleUranus;
	}
	else if (planet.name == "Venus") {
		glBindTexture(GL_TEXTURE_2D, venusTexture);
		radTwo = 108;
		radOne = radTwo / 2;
		angleVenus += degree*1.622222;
		angle = angleVenus;
	}

	glPushMatrix();

	//ellipse path
	float planetx = 0;
	float planety = 0;
	planetx = radOne * 2 *cos(angle);
	planety = radTwo * 2 * sin(angle);
	DrawEllipse(radOne *2 , radTwo*2);

	//text
	glPushMatrix();
	if (planet.name == "Mercury" && canShowText(planetx, planety)) {
		TextSmall(planetx, 15, planety + 10, 1.0f, 1.0f, 1.0f, "Mercury");
		TextBig(planetx, 10, planety + 10, 1.0f, 1.0f, 1.0f, "170,503 KM/H");
		TextSmall(planetx, 5, planety + 10, 1.0f, 1.0f, 1.0f, "Orbit Velocity");
	}
	else if (planet.name == "Venus" && canShowText(planetx, planety)) {
		TextSmall(planetx, 15, planety + 10, 1.0f, 1.0f, 1.0f, "Venus");
		TextBig(planetx, 10, planety + 10, 1.0f, 1.0f, 1.0f, "126,074 KM/H");
		TextSmall(planetx, 5, planety + 10, 1.0f, 1.0f, 1.0f, "Orbit Velocity");
	}
	else if (planet.name == "Earth" && canShowText(planetx, planety)) {
		TextSmall(planetx, 15, planety + 10, 1.0f, 1.0f, 1.0f, "Earth");
		TextBig(planetx, 10, planety + 10, 1.0f, 1.0f, 1.0f, "107,218 KM/H");
		TextSmall(planetx, 5, planety + 10, 1.0f, 1.0f, 1.0f, "Orbit Velocity");
	}
	else if (planet.name == "Mars" && canShowText(planetx, planety)) {
		TextSmall(planetx, 15, planety + 10, 1.0f, 1.0f, 1.0f, "Mars");
		TextBig(planetx, 10, planety + 10, 1.0f, 1.0f, 1.0f, "86,677 KM/H");
		TextSmall(planetx, 5, planety + 10, 1.0f, 1.0f, 1.0f, "Orbit Velocity");
	}
	else if (planet.name == "Jupiter" && canShowText(planetx, planety)) {
		TextSmall(planetx, 15, planety + 10, 1.0f, 1.0f, 1.0f, "Jupiter");
		TextBig(planetx, 10, planety + 10, 1.0f, 1.0f, 1.0f, "47,002 KM/H");
		TextSmall(planetx, 5, planety + 10, 1.0f, 1.0f, 1.0f, "Orbit Velocity");
	}
	else if (planet.name == "Saturn" && canShowText(planetx, planety)) {
		TextSmall(planetx, 15, planety + 10, 1.0f, 1.0f, 1.0f, "Saturn");
		TextBig(planetx, 10, planety + 10, 1.0f, 1.0f, 1.0f, "34,701 KM/H");
		TextSmall(planetx, 5, planety + 10, 1.0f, 1.0f, 1.0f, "Orbit Velocity");
	}
	else if (planet.name == "Uranus" && canShowText(planetx, planety)) {
		TextSmall(planetx, 15, planety + 10, 1.0f, 1.0f, 1.0f, "Uranus");
		TextBig(planetx, 10, planety + 10, 1.0f, 1.0f, 1.0f, "24,477 KM/H");
		TextSmall(planetx, 5, planety + 10, 1.0f, 1.0f, 1.0f, "Orbit Velocity");
	}
	else if (planet.name == "Neptune" && canShowText(planetx, planety)) {
		TextSmall(planetx, 15, planety + 10, 1.0f, 1.0f, 1.0f, "Neptune");
		TextBig(planetx, 10, planety + 10, 1.0f, 1.0f, 1.0f, "19,566 KM/H");
		TextSmall(planetx, 5, planety + 10, 1.0f, 1.0f, 1.0f, "Orbit Velocity");
	}
	else if (planet.name == "Pluto" && canShowText(planetx, planety)) {
		TextSmall(planetx, 15, planety + 10, 1.0f, 1.0f, 1.0f, "Pluto");
		TextBig(planetx, 10, planety + 10, 1.0f, 1.0f, 1.0f, "16,809 KM/H");
		TextSmall(planetx, 5, planety + 10, 1.0f, 1.0f, 1.0f, "Orbit Velocity");
	}
	glPopMatrix();

	glTranslated(planetx, 0, planety);

	//rotation 7awalen nafsy
	glRotated(rotY * rotFactor, 0, 1, 0);
	glRotated(-90, 1, 0, 0);
	glScaled(0.1, 0.1, 0.1);

	GLUquadricObj *quadric;
	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, planet.radius, 100, 100);

	//rings
	if (planet.name == "Saturn") {
		glPushMatrix();
		drawRings(620,770, 45,"textures/Saturn/Rings/saturnringcolor.bmp");
		drawRings(790,820, 90,"textures/Saturn/Rings/saturnringcolor.bmp");
		glPopMatrix();
	}
	if (planet.name == "Uranus") {
		glPushMatrix();
		drawRings(260, 290, 45, "textures/Uranus/Rings/rings.bmp");
		drawRings(320, 360, 90, "textures/Uranus/Rings/rings.bmp");
		drawRings(380, 420, 125, "textures/Uranus/Rings/rings.bmp");
		glPopMatrix();
	}
	glPopMatrix();

}

void drawSun(Star sun) {
	glPushMatrix();
	
	glEnable(GL_LIGHT1);

	GLfloat ambient[] = { 0.8f, 0.8f, 0.8, 1.0f };
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);

	GLfloat sun_material[] = { 0.8,0.8,0.8, 1};
	glMaterialfv(GL_FRONT, GL_EMISSION, sun_material);

	//tex_sun.Load("Textures/Sun/sun.bmp");
	//tex_sun.Load("",textures[9]);
	glBindTexture(GL_TEXTURE_2D, sunTexture);
	glRotated(rotSunY, 0,1,0);
	glRotated(90, 1, 0, 0);
	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricTexture(qobj, 1);
	gluSphere(qobj, sun.radius, 100, 100);
	glPopMatrix();
}

void drawPlanets() {

	//distance from sun in days
	int maxDistance = 5096;
	int scalingFactor = 1000;

	glPushMatrix();

	//mercury
	glPushMatrix();
	drawPlanet(Mercury, scalingFactor * 58 / maxDistance, 0, 0, 14.07);
	glPopMatrix();

	//venus
	glPushMatrix();
	drawPlanet(Venus, scalingFactor * 108 / maxDistance, 0, 0, -58.32);
	glPopMatrix();

	//earth
	glPushMatrix();
	drawPlanet(Earth, scalingFactor * 149 / maxDistance, 0, 0, 0.24);
	glPopMatrix();
	
	//mars
	glPushMatrix();
	drawPlanet(Mars, scalingFactor * 227 / maxDistance, 0, 0, 0.24);
	glPopMatrix();

	//jupiter
	glPushMatrix();
	drawPlanet(Jupiter, scalingFactor * 778 / maxDistance, 0, 0, 0.10);
	glPopMatrix();

	//saturn
	glPushMatrix();
	drawPlanet(Saturn, scalingFactor * 1426 / maxDistance, 0, 0, -0.10);
	glPopMatrix();

	//uranus
	glPushMatrix();
	drawPlanet(Uranus, scalingFactor * 2870 / maxDistance, 0, 0, -0.17);
	glPopMatrix();

	//neptune
	glPushMatrix();
	drawPlanet(Neptune, scalingFactor * 4498 / maxDistance, 0, 0, -0.16);
	glPopMatrix();

	//pluto
	glPushMatrix();
	drawPlanet(Pluto, scalingFactor * 5906 / maxDistance, 0, 0, -1.53);
	glPopMatrix();

	glPopMatrix();
}


//GLuint tex;
char title[] = "Little Big Universe";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 50000;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

//Initialization of vectors controlling the camera.
Vector Eye(0,0,500);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;


bool canShowText(int x, int y) {
	if (abs(At.z - y) < 150 && abs(At.x - x) < 150)
		showText = true;
	else
		showText = false;
	return showText;
}

// An Example of how to create and use models (Model Variables)
//Model_3DS model_house;
//Model_3DS model_tree;

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	//InitLightSource();

	glEnable(GL_LIGHTING);

	//InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Display Function
//=======================================================================

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = {0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
	glEnable(GL_TEXTURE_2D);*/
	//Draw Tree Model
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	
	glPushMatrix();
	glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(yrot, 0.0, 1.0, 0.0);  //rotate our camera on the y - axis(up and down)
	glTranslated(-xpos, 0.0f, -zpos); //translate the screen to the position of our camera

	drawSun(TheSun);
	drawPlanets();

	//sky box
	initializeSpace();
	glPopMatrix();
	
	glutSwapBuffers();
	
}

//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char key, int x, int y)
{

	if (key == 'q')
	{
		xrot += 1;
		if (xrot >360) xrot -= 360;
	}

	if (key == 'z')
	{
		xrot -= 1;
		if (xrot < -360) xrot += 360;
	}

	if (key == 'w')
	{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		xpos += 10* float(sin(yrotrad));
		zpos -= 10 * float(cos(yrotrad));
		ypos -= 10 * float(sin(xrotrad));
	}

	if (key == 's')
	{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		xpos -= 10 * float(sin(yrotrad));
		zpos += 10 * float(cos(yrotrad));
		ypos += 10 * float(sin(xrotrad));
	}

	if (key == 'd')
	{
		float yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xpos += 10 * float(cos(yrotrad)) * 0.2;
		zpos += 10 * float(sin(yrotrad)) * 0.2;
	}

	if (key == 'a')
	{
		float yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xpos -= 10 * float(cos(yrotrad)) * 0.2;
		zpos -= 10 * float(sin(yrotrad)) * 0.2;
	}

	if (key == 27)
	{
		exit(0);
	}
	glutPostRedisplay();
}

//=======================================================================
// Motion Function 
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int x, int y)
{
	
	int diffx = x - lastx; //check the difference between the 
						   //current x and the last x position
	int diffy = y - lasty; //check the difference between the 
						   //current y and the last y position
	lastx = x; //set lastx to the current x position
	lasty = y; //set lasty to the current y position
	xrot += (float)diffy; //set the xrot to xrot with the addition
						  //of the difference in the y position
	yrot += (float)diffx;    //set the xrot to yrot with the addition
							 //of the difference in the x position
	
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================


void loadImages() {

	tex = SOIL_load_OGL_texture(
		"textures/Space/space2.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	sunTexture = SOIL_load_OGL_texture(
		"textures/sun/sun.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	mercuryTexture = SOIL_load_OGL_texture(
		"textures/mercury/mercurymap.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	venusTexture = SOIL_load_OGL_texture(
		"textures/venus/venusmap.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	earthTexture = SOIL_load_OGL_texture(
		"textures/earth/earth.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	marsTexture = SOIL_load_OGL_texture(
		"textures/mars/marsmap1k.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	jupiterTexture = SOIL_load_OGL_texture(
		"textures/jupiter/jupiter.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	saturnTexture = SOIL_load_OGL_texture(
		"textures/saturn/saturnmap.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	uranusTexture = SOIL_load_OGL_texture(
		"textures/uranus/uranusmap.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	neptuneTexture = SOIL_load_OGL_texture(
		"textures/neptune/neptunemap.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	plutoTexture = SOIL_load_OGL_texture(
		"textures/pluto/plutomap2k.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
}

void anim() {
	if (rotSunY < 360) {
		rotSunY += 1;
	}
	else {
		rotSunY = 0;
	}
	rotY += 20;

	//glutWarpPointer(WIDTH / 2, HEIGHT / 2);

	
	glutPostRedisplay();
}

void initializeSpace() {
	glPushMatrix();

	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	glRotated(90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 20000, 100, 100);
	gluDeleteQuadric(qobj);

	glPopMatrix();
}


//=======================================================================
// Special Keys Function
//=======================================================================
void spe(int k, int x, int y)
{
	if (k == GLUT_KEY_RIGHT)
	//	tranX += 10;
	if (k == GLUT_KEY_LEFT)
	//	tranX -= 10;

	glutPostRedisplay();
}



//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(0, 0);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);

	glutSpecialFunc(spe);

	glutMotionFunc(myMotion);

	glutPassiveMotionFunc(myMouse);

	glutReshapeFunc(myReshape);

	glutIdleFunc(anim);

	myInit();

	glEnable(GL_TEXTURE_2D);
	
	loadImages();
	initializePlanets();
	glutWarpPointer(WIDTH / 2, HEIGHT / 2);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);	

	glColor3f(0, 0, 0);

	glutMainLoop();
}