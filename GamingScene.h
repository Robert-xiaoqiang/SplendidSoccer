#define FREEGLUT_STATIC
#ifndef GAMEINGSCENE_H_
#define GAMEINGSCENE_H_

#include "glew.h"
#include "GL\freeglut.h"
#include "SkyBox.h"
#include "OBJParse.h"
#include "Soccer.h"
#include "Defencer.h"
#include "ParticleEffect.h"
#include "SphereEmitter.h"
#include "CubeEmitter.h"


class GamingScene {
public:
	GamingScene();
	virtual ~GamingScene();
	void init();
	void update();
	void render();
	void checkEmit(int key);
	void checkFunc(int key);
	
private:
	bool isParticle;
	
	Defencer defencer;
	GLuint gamingGoalList;
	Soccer   s;
	OBJParse goal;
	SkyBox   gamingSkyBox;
	// later Defencer class

	SphereEmitter sphereEmitter;
	ParticleEffect pe;

	bool score;
	SphereEmitter sphereEmiterSocre;
	ParticleEffect peScore;
};


#endif