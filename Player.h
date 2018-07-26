#ifndef PLAYER_H_
#define PLAYER_H_

#include "General.h"
#include "OBJParse.h"

class Player {
public:
	Player();
	virtual ~Player();
	void init();
	void render();
	OBJParse body;
private:
	GLuint playerList;
	vec3 worldPos;
	bool checkCollision();
};

#endif