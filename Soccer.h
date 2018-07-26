#ifndef _SOCCER_H_
#define _SOCCER_H_

#include <string>
#include "General.h"
#include <vector>

class Soccer {
	GLUquadricObj *quadric;
	vec3 kickDirection;      // identity && local

	GLuint textureID;
	std::vector<vec3> trace; // 100 frame

	void buildLocal(vec3& u, vec3& v, vec3& t) const; // v vertical == t must!
	bool checkCollision();
	
public:
	bool sw;            // run 1 -> static 0
	bool isTrace;       // trace 25 frame

	vec3 worldPos;      // initilize to position -> kick -> change!

	vec3 uFly;          // fly coordinate
	vec3 vFly;
	vec3 tFly;

	float uVelocity;
	float vVelocity;    // negative downdown ; positive upup
	float tVelocity;

	float size;

	Soccer():
		size(0.0f),
		worldPos(vec3(0.0f, 0.0f, 0.0f)),
		sw(false)
	{
		quadric = gluNewQuadric();
	}

	virtual ~Soccer()
	{
		gluDeleteQuadric(quadric);
	}

	void init(const vec3& initPos, float size)
	{
		this->size = size;
		this->worldPos = initPos;
		textureID = generateTexId("datas/soccer.png");
	}

	void setThetaFi(float theta, float fi);
	void update(bool isBump = true);
	void render(); // update / render
	void kick(float uv, float vv, float tv);
};

#endif