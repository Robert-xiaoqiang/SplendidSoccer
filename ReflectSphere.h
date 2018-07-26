#ifndef REFLECTSPHERE_H_
#define REFLECTSPHERE_H_

#include "MyShader.h"

class ReflectSphere {
private:
	MyShader reflectShader;
public:
	ReflectSphere();
	virtual ~ReflectSphere();
	void init();
	void draw(GLuint cubeMapTexture);
};

#endif