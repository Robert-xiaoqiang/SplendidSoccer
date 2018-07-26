#define FREEGLUT_STATIC
#include "glew.h"
#include "GL/freeglut.h"
#include "OBJParse.h"
#include "Menu.h"
#include "SkyBox.h"
#include "Soccer.h"
#include "View.h"
#include "Player.h"
#include "ReflectSphere.h"
#include "GamingScene.h"
#include "ParticleEffect.h"
#include "CubeEmitter.h"

#define WIDTH                   640 * 2
#define HEIGHT                  480 * 2

void init();
void setLights();
void display(void);
void keyboard(unsigned char key, int x, int y);
//void printText(int x, int y, const std::string& str);
void idle();

GLfloat lightPosition[4] = { 10.0f, 10.0f, 0.0f, 0.0f };
static GLfloat lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightAmbient[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	
static GLfloat lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

static GLfloat lightPosition1[4] = { 50.0f, -100.0f, 20.0f, 0.0f };
static GLfloat lightSpecular1[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightAmbient1[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightDiffuse1[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

static char stadiumSkyBoxPng[][50] = {
	"datas/skybox0/rt.png",
	"datas/skybox0/lf.png",
	"datas/skybox0/up.png",
	"datas/skybox0/dn.png",
	"datas/skybox0/bk.png",
	"datas/skybox0/ft.png",
};

Menu me;
Soccer s;
Player p;

GLuint stadiumList;
OBJParse stadium;
SkyBox stadiumSkyBox;
ReflectSphere refSphere;
GamingScene gs;
SphereEmitter seStadium;
ParticleEffect peStadium(2000);

State current = MENU;

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("SplendidSoccer");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(passiveMouseMotion);
	glutIdleFunc(idle);
	//glutMouseWheelFunc();
	glutMainLoop();

	return 0;
}

void init()
{
	glewExperimental = GL_TRUE;
	glewInit();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	setLights();
	
	s.init(vec3(32.0f, 0.7f, 0.0f), 0.5f);
	me.init();
	p.init();
	gs.init();

	stadium.openFile("stadium.obj", 3, 2); // 3 faceN, 2textureN
	stadiumSkyBox.init(stadiumSkyBoxPng, BIG);
	stadiumList = glGenLists(1);           // for stadium in main.cpp
	glNewList(stadiumList, GL_COMPILE);
	stadium.draw();
	glEndList();

	ParticleEffect::ColorInterpolator colors;
	colors.AddValue(0.0f, glm::vec4(1, 0, 0, 1));     // red
	colors.AddValue(0.15f, glm::vec4(1, 0, 1, 1));     // magenta
	colors.AddValue(0.33f, glm::vec4(0, 0, 1, 1));     // blue
	colors.AddValue(0.5f, glm::vec4(0, 1, 1, 1));     // cyan
	colors.AddValue(0.67f, glm::vec4(0, 1, 0, 0.75));  // green
	colors.AddValue(0.84f, glm::vec4(1, 1, 0, 0.5));   // yellow
	colors.AddValue(1.0f, glm::vec4(1, 0, 0, 0));     // red

	peStadium.SetColorInterplator(colors);
	peStadium.SetParticleEmitter(&seStadium);
	peStadium.EmitParticles();
	
	refSphere.init();
}

// for animation or a small "film"
void update()
{
	if(current == MENU) {
		me.update(); // bounce ball
		s.update();  // MENU soccer movement

	} else if(current == GAMING) {
		gs.update(); // particle & GAMING soccer
	} else if(current == SHOW) {
		peStadium.Update(0.02f);
	}
	glutPostRedisplay();
}

// render all models of solar system in world system
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(current == MENU) {
		glPushMatrix();
		me.render();
		glPopMatrix();
		glPushMatrix();
		s.render();
		glPopMatrix();
		glPushMatrix();
		glTranslated(0.0, 3.5, 0.0);
		p.render();
		glPopMatrix();
	} else if(current == SHOW) {
		glPushMatrix();
		glRotated(-90.0, 1.0, 0.0, 0.0);
		glCallList(stadiumList);
		glPopMatrix();
		glPushMatrix();
		stadiumSkyBox.render();
		glPopMatrix();
		glPushMatrix();
		refSphere.draw(stadiumSkyBox.cubeMapTexture);
		glPopMatrix();
		going();
		glPushMatrix();
		glRotated(70.0, 0.0, 1.0, 0.0);
		glScaled(0.5, 0.5, 0.5);
		peStadium.Render();
		glPopMatrix();
	} else if(current == GAMING){
		gs.render();
	}	
	glutSwapBuffers();
	update();
}

void setLights()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	
	//glEnable(GL_LIGHT1);

	//glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular1);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse1);
	//glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient1);
	
}

void keyboard(unsigned char key, int x, int y)
{
	processKeyboard(key, x, y);
	gs.checkEmit(key);
	gs.checkFunc(key);
}

void idle()
{
	glutPostRedisplay();
}