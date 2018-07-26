#include "BounceBall.h"
#include <cmath>
#include <iostream>

const float transform = 0.02f;
const float loss = 0.01f;

BounceBall::BounceBall(const vec3& center, float radius, const std::string& texturePng):
	center(center), // synthesis copy ctor
	radius(radius),
	textureID(-1),
	velocity(0.0f),
	ep(0.0f),
	sw(false)
{
	quadric = gluNewQuadric();
	if(texturePng != "")
		textureID = generateTexId(texturePng.c_str());
}

BounceBall::BounceBall():
	textureID(-1),
	velocity(0.0f),
	ep(0.0f),
	sw(false)
{
	quadric = gluNewQuadric();
}

void BounceBall::init(const vec3& center, float radius, const std::string& texturePng)
{
	this->center = center;
	this->radius = radius;
	if(texturePng != "")
		textureID = generateTexId(texturePng.c_str());
}

void BounceBall::render()
{
	if(sw) {
		glPushAttrib(GL_TEXTURE_BIT);

		if(textureID != -1)
			glBindTexture(GL_TEXTURE_2D, textureID);
		gluQuadricDrawStyle(quadric, GLU_FILL);
		gluQuadricTexture(quadric, 1);

		glPushMatrix();
		glTranslatef(center.x, center.y, center.z);
		gluSphere(quadric, radius, 20, 20);
		glPopMatrix();
		
		glPopAttrib();
	}
}

void BounceBall::update()
{
	if(fabs(velocity) >= 0.03f || ep >= 0.03f) {
		center.y += velocity / 2.0f;
		if(velocity >= 0.03f) {
			//velocity -= sqrt(2 * transform) + loss;
			velocity -= transform + loss;
			ep += transform;
		} else if(velocity <= 0.0f && ep >= 0.03) {
			ep -= transform + loss;
			//velocity -= sqrt(2 * transform);
			velocity -= transform;
		} else if( fabs(velocity) < 0.03 && ep >= 0.03) {
			ep -= transform + loss;
			//velocity -= sqrt(2 * transform);
			velocity -= transform;
		}
		if(center.y - radius < 0.1) velocity = -velocity - loss;
		//std::cout << velocity << ' ' << ep << ' ' << (velocity >= 0.025) << std::endl;
	}
}
