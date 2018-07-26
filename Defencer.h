#pragma once
#include "General.h"
#include "OBJParse.h"
#include "Defencer.h"

class Defencer
{
public:
	Defencer();
	void init();
	void render();
	OBJParse defencerBody;
private:
	//bool checkCollision();
	GLuint defencerList;
};
