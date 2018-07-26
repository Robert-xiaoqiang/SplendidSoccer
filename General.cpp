#include "General.h"
#include "SOIL.h"
#include <cmath>

const vec2 operator+(const vec2& left, const vec2& right)
{
	return vec2(left.x + right.x,
				left.y + right.y);
}

const vec3 operator+(const vec3& left, const vec3& right)
{
	return vec3(left.x + right.x,
				left.y + right.y,
				left.z + right.z);
}

const vec3 operator*(const vec3& v, float scaler)
{
	return vec3(v.x * scaler,
				v.y * scaler,
				v.z * scaler);
}

const vec3 operator*(float scaler, const vec3& v)
{
	return vec3(v.x * scaler,
				v.y * scaler,
				v.z * scaler);
}

const vec3 operator-(const vec3& left, const vec3& right)
{
	return left + vec3(-right.x, -right.y, -right.z);
}

GLuint generateTexId(const char texPicture[])
{
	GLuint textureID;
	int width, height, last;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last);
	glGenTextures(1, &textureID);
	glGetError();
	glBindTexture(GL_TEXTURE_2D, textureID);
	unsigned char *image = SOIL_load_image(texPicture, &width, &height, 0, SOIL_LOAD_RGB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
	glBindTexture(GL_TEXTURE_2D, last);
	SOIL_free_image_data(image);
	return textureID;
}

GLuint generateCubeTexId(char faces[][50])
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for(GLuint i = 0; i < 6; i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

GLuint generateTGATexId(const char texPicture[])
{
	GLuint textureID = SOIL_load_OGL_texture(
		texPicture,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	return textureID;
}

// function: rotate in 2D
// @param 
// (centerX, centerY): center of rotation
// theta: degree of rotation
// v: points in 2D coordinate system
void rotate(double centerX, double centerY, double *v, double theta)
{
	double s = sin(theta), c = cos(theta), newX, newY;
	newX = (v[0] - centerX) * c - s * (v[1] - centerY) + centerX;
	newY = (v[0] - centerX) * s + c * (v[1] - centerY) + centerY;

	v[0] = newX; v[1] = newY;
}

// function: cross product in 3D
// @param
// v3 = v1 X v2
void cross(const vec3& v1, const vec3& v2, vec3& v3)
{
	v3.x = v1.y * v2.z - v1.z * v2.y;
	v3.y = -(v1.x * v2.z - v1.z * v2.x);
	v3.z = v1.x * v2.y - v1.y * v2.x;
}

void normalize(vec3& v)
{
	float length = sqrt((v.x * v.x) +
						(v.y * v.y) +
						(v.z * v.z));
	v.x /= length;
	v.y /= length;
	v.z /= length;

	if(fabs(v.x) <= 1e-5) v.x = 0.0f;
	if(fabs(v.y) <= 1e-5) v.y = 0.0f;
	if(fabs(v.z) <= 1e-5) v.z = 0.0f;

}