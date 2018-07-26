#include "GamingScene.h"
#include "General.h"
#include "View.h"

extern State current;

GamingState _gamingCurrent;
Direction _direction;
Height _height;

static char gamingSkyBoxPng[][50] = {
	"datas/skybox3/rt.png",
	"datas/skybox3/lf.png",
	"datas/skybox3/up.png",
	"datas/skybox3/dn.png",
	"datas/skybox3/bk.png",
	"datas/skybox3/ft.png",
};

GamingScene::GamingScene():
	gamingSkyBox(),
	goal(),
	s(),
	isParticle(false),
	score(false),
	pe(100),
	peScore(1000)
{
	s.isTrace = false; // No Trace
}

GamingScene::~GamingScene()
{

}

void GamingScene::init()
{
	s.init(vec3(62.0f, -58.5f, 0.0f), 1.0f);
	goal.openFile("datas/goal/goal3.obj", 3, 3);
	gamingSkyBox.init(gamingSkyBoxPng, BIG);

	defencer.init();
	gamingGoalList = glGenLists(1);
	glNewList(gamingGoalList, GL_COMPILE);
	goal.draw();
	glEndList();

	ParticleEffect::ColorInterpolator colors;

	colors.AddValue(0.0f, glm::vec4(1, 0, 0, 1));     // red
	colors.AddValue(0.15f, glm::vec4(1, 0, 1, 1));     // magenta
	colors.AddValue(0.33f, glm::vec4(0, 0, 1, 1));     // blue
	colors.AddValue(0.5f, glm::vec4(0, 1, 1, 1));     // cyan
	colors.AddValue(0.67f, glm::vec4(0, 1, 0, 0.75));  // green
	colors.AddValue(0.84f, glm::vec4(1, 1, 0, 0.5));   // yellow
	colors.AddValue(1.0f, glm::vec4(1, 0, 0, 0));     // red

	pe.SetColorInterplator(colors);
	pe.SetParticleEmitter(&sphereEmitter);
	pe.EmitParticles();

	peScore.SetColorInterplator(colors);
	peScore.SetParticleEmitter(&sphereEmiterSocre);
	peScore.EmitParticles();

	_gamingCurrent = BEFOREKICK;
}

void GamingScene::update()
{
	if(_height == LOW && _direction == LEFT) {
		s.update(false);  // no bump !!!
	} else {
		s.update();
	}
	if(isParticle) pe.Update(0.02f);
	if(score) pe.Update(0.02f);
}

void GamingScene::render()
{
	glUseProgram(0);

	static int time = 0;

	glPushMatrix();
	glTranslated(0.0, -5.0, 0.0);
	defencer.render();
	glPopMatrix();

	glPushMatrix();
	s.render();
	glPopMatrix();

	if(isParticle) {
		glPushMatrix();
		glTranslated(s.worldPos.x, s.worldPos.y, s.worldPos.z);
		glRotated(90.0, 0.0, 1.0, 0.0);
		glScaled(0.1, 0.1, 0.1);
		pe.Render();
		glPopMatrix();
	}

	// next kick begin
	if(!s.sw && _gamingCurrent == AFTERKICK) {
		s.worldPos = { 62.0f, -58.5f, 0.0f };
		GLfloat newEye[3] = { 70.0, -50.0, 0.0 };     // gaming position
		GLfloat newAt[3] = { 20.0, -70.0, 0.0 };
		View::setAt(newAt);
		View::setEye(newEye);
		_gamingCurrent = BEFOREKICK;
	}

	// show socre
	if(score) {
		glPushMatrix();
		time++;
		glTranslated(30.0, -30, time - 50);
		glRotated(90.0, 0.0, 1.0, 0.0);
		peScore.Render();
		glPopMatrix();
		if(time == 100) {
			score = false;
			time = 0;
		}
	}

	glPushMatrix();
	gamingSkyBox.render();
	glPopMatrix();

	glPushMatrix();
	glRotated(90.0, 0.0, 1.0, 0.0);
	glTranslated(0.0, -70.0, 0.0);
	glScaled(0.6, 0.6, 0.5);
	glCallList(gamingGoalList);
	glPopMatrix();
}

void GamingScene::checkEmit(int key)
{
	if(current == GAMING) {
		if(key == 'z') {
			s.setThetaFi(135.0f, 90.0f);
			s.kick(3.5f, 2.5f, -3.0f);
			_direction = LEFT;
			_height = HIGH;
			_direction = LEFT;
			_gamingCurrent = AFTERKICK;
		} else if(key == 'c') {
			s.setThetaFi(135.0f, 90.0f);
			s.kick(3.5f, 2.5f, 3.0f);
			_height = HIGH;
			_direction = RIGHT;
			_gamingCurrent = AFTERKICK;
		} else if(key == 'x') {
			s.setThetaFi(135.0f, 90.0f);
			s.kick(3.5f, 2.5f, 0.0f);
			_height = HIGH;
			_direction = MIDDLE;
			_gamingCurrent = AFTERKICK;
		} else if(key == 'v') {
			s.setThetaFi(170.0f, 90.0f);
			s.kick(3.5f, 1.5f, -3.0f);
			_height = LOW;
			_direction = LEFT;
			_gamingCurrent = AFTERKICK;
			score = true;
		} else if(key == 'b') {
			s.setThetaFi(170.0f, 90.0f);
			s.kick(3.2f, 1.5f, 3.0f);
			_height = LOW;
			_direction = RIGHT;
			_gamingCurrent = AFTERKICK;
		}
	}
}

void GamingScene::checkFunc(int key)
{
	if(current == GAMING) {
		if(key == 'u') {
			isParticle = !isParticle;
		} else if(key == 'i') {
			s.isTrace = !s.isTrace;
		} else if(key == 'o') {
			GLfloat newEye[4] = { 40.0f, -35.0f,  0.0f };
			GLfloat newAt[4] = { 0.0f, -70.0f, 0.0f };
			View::setEye(newEye);
			View::setAt(newAt);
		} else if(key == 'p') {
			GLfloat newEye[4] = { -35.0f, -40.0f,  0.0f };
			GLfloat newAt[4] = { 0.0f, -60.5f, 0.0f };
			View::setEye(newEye);
			View::setAt(newAt);
		}
	}
}