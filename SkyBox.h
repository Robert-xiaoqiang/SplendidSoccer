#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#define FREEGLUT_STATIC
#include "glew.h"
#include "GL\freeglut.h"
#include "General.h"

#define XWDITH  15.0
#define YWIDTH  15.0
#define ZWIDTH  15.0

enum Edge {SMALL, BIG};

struct SkyBox {
	GLuint cubeMapTexture;
	SkyBox() { }
	void init(char texturePng[][50], Edge e);
	void render();

};

#endif