#ifndef BOUNCE_H_
#define BOUNCE_H_

#define FREEGLUT_STATIC
#include "General.h"
#include "glew.h"
#include "GL\freeglut.h"
#include <string>

class BounceBall {
public:
	BounceBall(const vec3& center, float radius, const std::string& texturePng = "");
	BounceBall();

	virtual ~BounceBall() 
	{ 
		gluDeleteQuadric(quadric);
	}
	
	void init(const vec3& center, float radius, const std::string& texturePng = "");
	vec3 getCenter() const { return center; }
	void setCenter(const vec3& center) { this->center = center; } // synthesis copy assignment
	void enable(float velocity, float ep) 
	{ 
		sw = true;
		this->velocity = velocity;
		this->ep = ep;
	}
	void disable() { sw = false;  }
	void render();
	void update();
	void kick();

private:
	GLUquadric *quadric;
	vec3 center;
	float radius;
	GLuint textureID;
	float velocity;		// Kinetic Energy just y change !!! up + / down -
	float ep;			// potential Energy
	bool sw;
};

#endif