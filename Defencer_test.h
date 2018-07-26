#pragma once
#ifndef Defencer_test.h

#include "General.h"
#include "OBJParse.h"


class Defencer
{
public:
	Defencer();
	void init();
	void render();
	OBJParse defencerBody;
private:
	vec3 worldPos;
	bool checkCollision();

};

#endif // !Defencer_test.h
