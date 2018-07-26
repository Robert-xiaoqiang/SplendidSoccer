#ifndef MENU_H_
#define MENU_H_

#include "BounceBall.h"
#include "OBJParse.h"
#include "MyShader.h"
#include "SkyBox.h"

class Menu {
public:
	Menu():
		b(),
		goal(),
		trophy()
	{ 

	}
	virtual ~Menu() 
	{
	
	}
	void init();
	void render();
	void update() 
	{
		b.update(); 
	}

	OBJParse goal;
	OBJParse trophy;
	BounceBall b;
private:
	void renderOptions();
	void renderACube(GLuint i); // textureID
	
	void renderCubesWithShadow();
	void renderCubes();

	GLuint menuGoalList;
	GLuint menuTrophyList;

	GLuint penalty;
	GLuint corner;
	GLuint random;

	GLuint depthMapFBO;
	GLuint depthMapTextureID;
	MyShader shadowShader;
	const GLuint SHADOW_WIDTH = 1024;
	const GLuint SHADOW_HEIGHT = 1024;

	MyShader cubeShader;
	MyShader objectWithShadowShader;
	GLuint cubeVAO, cubeVBO;
	SkyBox beginSky;
};

#endif