#ifndef GENERAL_H_
#define GENERAL_H_

#define FREEGLUT_STATIC
#include "glew.h"
#include "GL\freeglut.h"

enum GamingState { BEFOREKICK, AFTERKICK };
enum Direction { LEFT, MIDDLE, RIGHT };
enum Height { HIGH, LOW };
enum State { MENU, SHOW, GAMING };

struct vec2 {
	float x;
	float y;
	vec2(float x = 0.0f, float y = 0.0f) :
		x(x), y(y)
	{

	}
};

struct vec3 {
	float x;
	float y;
	float z;
	vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) :
		x(x), y(y), z(z)
	{

	}
	vec3(const vec2& xy, float z) :
		x(xy.x), y(xy.y), z(z)
	{

	}
};


// png just
GLuint generateTexId(const char texPicture[]);
GLuint generateTGATexId(const char texPicture[]);
GLuint generateCubeTexId(char faces[][50]);
void rotate(double centerX, double centerY, double *v, double theta);
void cross(const vec3& v1, const vec3& v2, vec3& v3);
void normalize(vec3& v);

const vec2 operator+(const vec2& left, const vec2& right);
const vec3 operator+(const vec3& left, const vec3& right);
const vec3 operator*(const vec3& v, float scaler);
const vec3 operator*(float scaler, const vec3& v);
const vec3 operator-(const vec3& left, const vec3& right);

#endif