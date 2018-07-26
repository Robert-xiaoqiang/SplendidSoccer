#include "Soccer.h"
#include "SOIL.h"
#include "glew.h"
#include "GL\freeglut.h"
#include "View.h"
#include <cmath>
#include <iostream>

extern State current; // from main.cpp

const float PI = 3.142f;
const float G = 0.988f;

// theta fi is sphere coord for x-y-z Local
// ??? !!! ???
vec3 xLocal = { 0.0f, 0.0f, -1.0f };
vec3 yLocal = { -1.0f, 0.0f, 0.0f };
vec3 zLocal = { 0.0f, 1.0f, 0.0f };


// theta 0 -> 360d
// fi	 0 -> 180d
// kickDirection is a local direction related initPosition
void Soccer::setThetaFi(float theta, float fi)
{
	theta = theta / 180.0f * PI;
	fi = fi / 180.0f * PI;
	float s = sin(fi);
	kickDirection.x = 1.0f * s * cos(theta);
	kickDirection.y = 1.0f * s * sin(theta);
	kickDirection.z = 1.0f * cos(fi);
}

void Soccer::buildLocal(vec3& u, vec3& v, vec3& t) const
{
	v = { 0.0f, 1.0f, 0.0f };   // v vertical == y
	cross(kickDirection, v, t); // t horizonal
	cross(v, t, u);				// u forwwad horizonal
	normalize(t);
	normalize(u);
}

void Soccer::update(bool isBump)
{
	if(current == MENU) {
		if(!checkCollision() && sw) {
			float duration = 0.02f;
			float vTemp = vVelocity - duration * G;
			vec3 uPos = uFly * (uVelocity * duration);
			vec3 vPos = vFly * ((vVelocity + vTemp) / 2 * duration);
			vec3 tPos = tFly * (tVelocity * duration);
			worldPos = worldPos + (uPos + vPos + tPos);
			vVelocity = vTemp;
		}
	} else if(current == GAMING && sw) {
		bool tag = fabs(worldPos.y - size + 60.0) < 1e-1f;
		if(tag) {
			std::cout << worldPos.x << std::endl;
			sw = false;
			worldPos.y = -58.5f;
			trace.clear();
		} else {
			float duration = 0.03f;
			float vTemp = vVelocity - duration * G;
			vec3 uPos = uFly * (uVelocity * duration);
			vec3 vPos = vFly * ((vVelocity + vTemp) / 2 * duration);
			vec3 tPos = tFly * (tVelocity * duration);
			if(trace.size() < 20) {
				trace.push_back(worldPos);
			} else {
				trace.erase(trace.begin());
				trace.push_back(worldPos);
			}
			worldPos = worldPos + (uPos + vPos + tPos);
			vVelocity = vTemp;
		}

		// virtual plane in goal
		bool tag2 = fabs(worldPos.x - size - 20.0f) < 1e-1f;
		if(tag2 && isBump) {
			uVelocity = -uVelocity - 1.5f;
		}
	} 

}

//--------------------------------------------------------------------------------//
// only apply to MENU state!
bool Soccer::checkCollision()
{
	bool ret = fabs(worldPos.y - size) < 1e-1f;
	if(ret) {
		sw = false;
		worldPos.y = size + 0.2f;
		trace.clear();
		if(current == MENU) {
			current = SHOW;
			GLfloat newEye[3] = { 48.319942f, 28.846288f, -21.849183f };
			View::setEye(newEye);
		}
	}

	return ret;
}
//--------------------------------------------------------------------------------//

void Soccer::render()
{
	//std::cout << "Soccer " << worldPos.x << ' ' << worldPos.y << ' ' << worldPos.z << std::endl;
	glPushAttrib(GL_TEXTURE_BIT);
	GLfloat dummy[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dummy);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, dummy);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, dummy);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0f);

	glBindTexture(GL_TEXTURE_2D, textureID);
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricTexture(quadric, 1);
	glPushMatrix();
	glTranslatef(worldPos.x, worldPos.y, worldPos.z);
	gluSphere(quadric, size, 20, 20);
	glPopMatrix();
	if(isTrace) {
		for(int i = 0; i < trace.size(); i++) {
			//dummy[0] -= (GLfloat)i / trace.size();
			//dummy[1] -= (GLfloat)i / trace.size();
			//dummy[2] -= (GLfloat)i / trace.size();
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, dummy);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dummy);
			glPushMatrix();
			glTranslatef(trace[i].x, trace[i].y, trace[i].z);
			gluSphere(quadric, size, 20, 20);
			glPopMatrix();
		}
	}
	glPopAttrib();
}

void Soccer::kick(float uv, float vv, float tv)
{
	uVelocity = uv * 3.0f; // positive
	vVelocity = vv * 2.0f; // positive
	tVelocity = tv;
	// tVelocity is related rotated -> left/right rotated!
	buildLocal(uFly, vFly, tFly);
	sw = true;
}