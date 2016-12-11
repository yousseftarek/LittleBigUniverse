//kernel32.lib, user32.lib, gdi32.lib and advapi32.lib
//#pragma comment(lib, "legacy_stdio_definitions.lib")
#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <iostream>
#include <glut.h>
#include <math.h>

void initializeSpace(void);

using namespace std;

int WIDTH = 1280;
int HEIGHT = 720;

float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle = 0.0;
float lastx, lasty;

double rotSunX, rotSunY, rotSunZ;

GLTexture tex_sun;

struct Moon {
	double radius;
	double distanceFromPlanet;
	char texture[];
};

struct Planet {
	double radius;
	char texture[];
	//Moon moons[] = { {10,10,""} };
}Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune, Pluto; //Yes, Pluto is a f**king planet !

struct Star {
	double radius;
}TheSun;



void drawPlanet(Planet planet, int x, int y, int z) {
	glPushMatrix();
	glTranslated(x, y, z);
	glutSolidSphere(planet.radius, 50, 50);
	glPopMatrix();
}

void drawSun(Star sun) {
	glPushMatrix();
	
	glEnable(GL_LIGHT1);

	GLfloat ambient[] = { 0.8f, 0.8f, 0.8, 1.0f };
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);

	GLfloat sun_material[] = { 0.8,0.8,0.8, 1};
	glMaterialfv(GL_FRONT, GL_EMISSION, sun_material);

	tex_sun.Load("Textures/Sun/sun.bmp");
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
	drawPlanet(Mercury, 100, 0, 0);
	drawPlanet(Mercury, 100, 0, 0);
	drawPlanet(Mercury, 100, 0, 0);
	drawPlanet(Mercury, 100, 0, 0);
	drawPlanet(Mercury, 100, 0, 0);
	drawPlanet(Mercury, 100, 0, 0);
	drawPlanet(Mercury, 100, 0, 0);
}


GLuint tex;
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
Vector Eye(0,0,2500);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// An Example of how to create and use models (Model Variables)
Model_3DS model_house;
Model_3DS model_tree;

// Textures
GLTexture tex_ground;

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



	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = {0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	glEnable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	// Draw Tree Model
	//glPushMatrix();
	//glTranslatef(10, 0, 0);
	//glScalef(0.7, 0.7, 0.7);
	//model_tree.Draw();
	//glPopMatrix();

	glPushMatrix();

	glRotatef(xrot, 1.0, 0.0, 0.0);

	glRotatef(yrot, 0.0, 1.0, 0.0);  //rotate our camera on the 
									 //y - axis(up and down)
	glTranslated(-xpos, 0.0f, -zpos); //translate the screen
									  //to the position of our camera

	drawSun(TheSun);


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
		xpos += 100* float(sin(yrotrad));
		zpos -= 100 * float(cos(yrotrad));
		ypos -= 100 * float(sin(xrotrad));
	}

	if (key == 's')
	{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		xpos -= 100 * float(sin(yrotrad));
		zpos += 100 * float(cos(yrotrad));
		ypos += 100 * float(sin(xrotrad));
	}

	if (key == 'd')
	{
		float yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xpos += 100 * float(cos(yrotrad)) * 0.2;
		zpos += 100 * float(sin(yrotrad)) * 0.2;
	}

	if (key == 'a')
	{
		float yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xpos -= 100 * float(cos(yrotrad)) * 0.2;
		zpos -= 100 * float(sin(yrotrad)) * 0.2;
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
void LoadAssets()
{
	//// Loading Model files
	//model_house.Load("Models/house/house.3ds");
	//model_tree.Load("Models/tree/Tree1.3ds");

	//// Loading texture files
	//tex_ground.Load("Textures/ground.bmp");
	//loadBMP(&tex, "Textures/sky4-jpg.bmp", true);
	loadBMP(&tex, "textures/Space/space2.bmp", true);
	
}

void anim() {
	if (rotSunY < 360) {
		rotSunY += 1;
	}
	else {
		rotSunY = 0;
	}

	glutPostRedisplay();
}

void initializePlanets() {
	Mercury.radius = 24;
	Venus.radius = 60;
	Earth.radius = 63;
	Mars.radius = 34;
	Jupiter.radius = 700;
	Saturn.radius = 582;
	Uranus.radius = 253;
	Neptune.radius = 246;
	Pluto.radius = 15;

	TheSun.radius = 400;
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

	glutMotionFunc(myMotion);

	glutPassiveMotionFunc(myMouse);

	glutReshapeFunc(myReshape);

	glutIdleFunc(anim);

	myInit();

	LoadAssets();
	initializePlanets();
	
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	

	glColor3f(0, 0, 0);

	glutMainLoop();
}