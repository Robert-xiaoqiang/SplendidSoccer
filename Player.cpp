#include "Player.h"
#include "Soccer.h"
#include "View.h"
#include "General.h"
#include <iostream>

extern Soccer s;      // from main.cpp
extern State current; // from main.cpp
const double PI = 3.142;

Player::Player():
	body()
{
	GLfloat at[3], eye[3];
	View::getAt(at);
	View::getEye(eye);
	worldPos = { (float)eye[0], (float)eye[1], (float)eye[2] };
}

Player::~Player()
{

}

bool Player::checkCollision()
{
	bool ret = false;
	if(fabs(worldPos.x - s.worldPos.x) <= 1.0 && !s.sw) {
		s.setThetaFi(135.0f, 90.0f);
		s.kick(3.0f, 3.0f, 0.0f);
		ret = true;
	}
	return ret;
}

void Player::init()
{
	body.openFile("datas/player/player.obj", 3, 2);
	playerList = glGenLists(1);     // for player.cpp

	glNewList(playerList, GL_COMPILE);
	body.draw();
	glEndList();
}

void Player::render()
{	
	checkCollision();
	GLfloat at[3], eye[3];
	View::getAt(at);
	View::getEye(eye);
	vec3 viewVector = { (float)(at[0] - eye[0]), (float)(at[1] - eye[1]), (float)(at[2] - eye[2]) };
	normalize(viewVector);
	//std::cout << "player" << worldPos.x << ' ' << worldPos.y << ' ' << worldPos.z << std::endl;
	worldPos = {
		(float)eye[0] + viewVector.x * 5.0f,
		0.0f,
		(float)eye[2] + viewVector.z * 5.0f
	};

	glPushMatrix();
	glTranslated(worldPos.x, worldPos.y, worldPos.z);
	double theta = atan2(viewVector.z, abs(viewVector.x));
	glRotated(theta * 180.0 / PI, 0.0, 1.0, 0.0);
	//here player -> view -x
	glRotated(-90.0, 0.0, 1.0, 0.0);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	glScaled(2.0, 2.0, 2.0);
	glCallList(playerList);
	glPopMatrix();
}