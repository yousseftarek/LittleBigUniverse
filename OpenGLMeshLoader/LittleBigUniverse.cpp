//kernel32.lib, user32.lib, gdi32.lib and advapi32.lib
//#pragma comment(lib, "legacy_stdio_definitions.lib")
#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <string>
#include <glut.h>

#define degree 0.0174533
#define DEG2RAD 3.14159/180.0

int WIDTH = 1280;
int HEIGHT = 720;

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

//Textures
GLuint tex;
GLTexture tex_Mars;
GLTexture tex_Earth;
GLTexture tex_Mercury;
GLTexture tex_Jupiter;
GLTexture tex_Neptune;
GLTexture tex_Saturn;
GLTexture tex_Uranus;
GLTexture tex_Venus;
GLTexture tex_Pluto;

GLTexture tex_SaturnRings;

struct Moon {
	double radius;
	double distanceFromPlanet;
	char texture[];
};

struct Planet {
	std::string name;
	double radius;
	std::string texture;
	//Moon moons[] = { {10,10,""} };
}Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune, Pluto; //Yes, Pluto is a f**king planet !

void initializePlanets() {
	Mercury.radius = 24;
	Mercury.name = "Mercury";
	Mercury.texture = "textures/Mercury/mercurymap.bmp";

	Venus.radius = 60;
	Venus.name = "Venus";
	Venus.texture = "textures/Venus/venusmap.bmp";

	Earth.radius = 63;
	Earth.name = "Earth";
	Earth.texture = "textures/Earth/earth.bmp";

	Mars.radius = 34;
	Mars.name = "Mars";
	Mars.texture = "textures/Mars/marsmap1k.bmp";

	Jupiter.radius = 700;
	Jupiter.name = "Jupiter";
	Jupiter.texture = "textures/Jupiter/jupiter.bmp";

	Saturn.radius = 582;
	Saturn.name = "Saturn";
	Saturn.texture = "textures/Saturn/saturnmap.bmp";

	Uranus.radius = 253;
	Uranus.name = "Uranus";
	Uranus.texture = "textures/Uranus/uranusmap.bmp";

	Neptune.radius = 246;
	Neptune.name = "Neptune";
	Neptune.texture = "textures/Neptune/neptunemap.bmp";

	Pluto.radius = 15; 
	Pluto.name = "Pluto";
	Pluto.texture = "textures/Pluto/plutomap2k.bmp";
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
	//saturn rings
	tex_SaturnRings.Load(texture);
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
		tex_Mars.Load(texturePath);
		radTwo = 227;
		radOne = radTwo / 2;
		angleMars += degree/2;
		angle = angleMars;
	}
	else if (planet.name == "Earth") {
		tex_Earth.Load(texturePath);
		radTwo = 149;
		radOne = radTwo / 2;
		angleEarth += degree;
		angle = angleEarth;
	}
	else if (planet.name == "Mercury") {
		tex_Mercury.Load(texturePath);
		radTwo = 58;
		radOne = radTwo / 2;
		angleMercury += degree*4.14772727;
		angle = angleMercury;
	}
	else if (planet.name == "Jupiter") {
		tex_Jupiter.Load(texturePath);
		radTwo = 778;
		radOne = radTwo/2;
		angleJupiter += degree;
		angle = angleJupiter;
	}
	else if (planet.name == "Neptune") {
		tex_Neptune.Load(texturePath);
		radTwo = 4498;
		radOne = radTwo / 2;
		angleNeptune += degree*0.00608;
		angle = angleNeptune;
	}
	else if (planet.name == "Pluto") {
		tex_Pluto.Load(texturePath);
		radTwo = 5906;
		radOne = radTwo / 2;
		anglePluto += degree*0.0040555;
		angle = anglePluto;
	}
	else if (planet.name == "Saturn") {
		tex_Saturn.Load(texturePath);
		radTwo = 1426;
		radOne = radTwo / 2;
		angleSaturn += degree*0.0331818;
		angle = angleSaturn;
	}
	else if (planet.name == "Uranus") {
		tex_Uranus.Load(texturePath);
		radTwo = 2870;
		radOne = radTwo / 2;
		angleUranus += degree*0.0117741;
		angle = angleUranus;
	}
	else if (planet.name == "Venus") {
		tex_Venus.Load(texturePath);
		radTwo = 108;
		radOne = radTwo / 2;
		angleVenus += degree*1.622222;
		angle = angleVenus;
	}

	glPushMatrix();
	//ellipse path
	float planetx = 0;
	float planety = 0;
	planetx = radOne * cos(angle);
	planety = radTwo * sin(angle);
	DrawEllipse(radOne, radTwo);
	glTranslated(planetx, 0, planety);

	//rotation 7awalen nafsy
	glRotated(rotY * rotFactor, 0, 1, 0);
	glRotated(-90, 1, 0, 0);
	glScaled(0.05, 0.05, 0.05);

	GLUquadricObj *quadric;
	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, planet.radius, 100, 100);

	if (planet.name == "Saturn") {
		//rings
		glPushMatrix();
		drawRings(620,700, 45,"textures/Saturn/Rings/saturnringcolor.bmp");
		drawRings(730,850, 90,"textures/Saturn/Rings/saturnringcolor.bmp");
		drawRings(880,1000, 125,"textures/Saturn/Rings/saturnringcolor.bmp");
		glPopMatrix();
	}

	if (planet.name == "Uranus") {
		//rings
		glPushMatrix();
		drawRings(260, 290, 45, "textures/Uranus/Rings/rings.bmp");
		drawRings(320, 360, 90, "textures/Uranus/Rings/rings.bmp");
		drawRings(380, 420, 125, "textures/Uranus/Rings/rings.bmp");
		glPopMatrix();
	}
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
GLdouble zFar = 4000;

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
Vector Eye(0, 900, 900);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// An Example of how to create and use models (Model Variables)
Model_3DS model_house;
Model_3DS model_tree;

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

	InitLightSource();

	InitMaterial();

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
	//glPushMatrix();
	//glTranslatef(10, 0, 0);
	//glScalef(0.7, 0.7, 0.7);
	//model_tree.Draw();
	//glPopMatrix();

	initializePlanets();
	drawPlanets();
	
	//sky box
	/*glPushMatrix();
	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	glTranslated(50,0,0);
	glRotated(90,1,0,1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj,true);
	gluQuadricNormals(qobj,GL_SMOOTH);
	gluSphere(qobj,500,100,100);
	gluDeleteQuadric(qobj);
	glPopMatrix();*/

	glutSwapBuffers();
}

//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'w':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'r':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
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
void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
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
	//Loading Model files
	//model_house.Load("Models/house/house.3ds");
	//model_tree.Load("Models/tree/Tree1.3ds");

	//Loading texture files
	loadBMP(&tex, "textures/Space/space2.bmp", true);
}

//=======================================================================
// Animate Function
//=======================================================================
void anim()
{
	rotY += 20;
	glutPostRedisplay();	//Re-draw scene 
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
// Timer Function
//=======================================================================
//void time(int val)//timer animation function, allows the user to pass an integer valu to the timer function.
//{
//	glutPostRedisplay();
//	glutTimerFunc(1, time, 0);//recall the time function after 1 ms and pass a zero value as an input to the time func.
//}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);

	glutSpecialFunc(spe);

	glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);

	glutIdleFunc(anim);

	myInit();

	glEnable(GL_TEXTURE_2D);
	
	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}