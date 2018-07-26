#ifndef MYSHADER_H_
#define MYSHADER_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glew.h"

class MyShader {
public:
	MyShader() { }
	void init(const char *vertName, const char *fragName);
	MyShader(const char *vertName, const char *fragName);
	void work();
public:
	GLuint prog;
};

#endif