#define FREEGLUT_STATIC
#include "ReflectSphere.h"
#include "glew.h"
#include "GL\freeglut.h"
#include "View.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <cmath>

//---------------------------------------------------------------------------
const float M_PI = 3.142f;
const int na = 36;          // vertex grid size
const int nb = 18;
const int na3 = na * 3;     // line in grid size
const int nn = nb*na3;      // whole grid size
GLfloat sphere_pos[nn];     // vertex
GLfloat sphere_nor[nn];     // normal
						    //GLfloat sphere_col[nn];   // color
GLuint sphere_ix[na*(nb - 1) * 6];                      // indices
GLuint sphere_vbo[4] = { 0,0,0,0 };

GLuint cubeVAO;
GLuint sphereVAO;

ReflectSphere::ReflectSphere()
{

}


ReflectSphere::~ReflectSphere()
{

}

void ReflectSphere::init()
{
	// generate the sphere data
	GLfloat x, y, z, a, b, da, db, r = 3.0f;
	int ia, ib, ix, iy;
	da = 2.0f*M_PI / GLfloat(na);
	db = M_PI / GLfloat(nb - 1);
	// [Generate sphere point data]
	// spherical angles a,b covering whole sphere surface
	for(ix = 0, b = -0.5*M_PI, ib = 0; ib<nb; ib++, b += db)
		for(a = 0.0, ia = 0; ia<na; ia++, a += da, ix += 3)
		{
			// unit sphere
			x = cos(b)*cos(a);
			y = cos(b)*sin(a);
			z = sin(b);
			sphere_pos[ix + 0] = x*r;
			sphere_pos[ix + 1] = y*r;
			sphere_pos[ix + 2] = z*r;
			sphere_nor[ix + 0] = x;
			sphere_nor[ix + 1] = y;
			sphere_nor[ix + 2] = z;
		}
	// [Generate GL_TRIANGLE indices]
	for(ix = 0, iy = 0, ib = 1; ib<nb; ib++)
	{
		for(ia = 1; ia<na; ia++, iy++)
		{
			// first half of QUAD
			sphere_ix[ix] = iy;      ix++;
			sphere_ix[ix] = iy + 1;    ix++;
			sphere_ix[ix] = iy + na;   ix++;
			// second half of QUAD
			sphere_ix[ix] = iy + na;   ix++;
			sphere_ix[ix] = iy + 1;    ix++;
			sphere_ix[ix] = iy + na + 1; ix++;
		}
		// first half of QUAD
		sphere_ix[ix] = iy;       ix++;
		sphere_ix[ix] = iy + 1 - na;  ix++;
		sphere_ix[ix] = iy + na;    ix++;
		// second half of QUAD
		sphere_ix[ix] = iy + na;    ix++;
		sphere_ix[ix] = iy - na + 1;  ix++;
		sphere_ix[ix] = iy + 1;     ix++;
		iy++;
	}
	// [VAO/VBO stuff]
	GLuint i;
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(4, sphere_vbo);
	glBindVertexArray(sphereVAO);
	i = 0; // vertex
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_pos), sphere_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 1; // normal
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_nor), sphere_nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	reflectShader.init("shaders/environmentTexture.vert", "shaders/environmentTexture.frag");
}


void ReflectSphere::draw(GLuint cubeMaptexture)
{
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f));
	GLfloat	view[16], projection[16], eye[3];
	View::getViewMatrix(view);
	View::getProjectionMatrix(projection);
	View::getEye(eye);

	reflectShader.work();
	glUniformMatrix4fv(glGetUniformLocation(reflectShader.prog, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(reflectShader.prog, "view"), 1, GL_FALSE, view);
	glUniformMatrix4fv(glGetUniformLocation(reflectShader.prog, "projection"), 1, GL_FALSE, projection);
	glUniform3f(glGetUniformLocation(reflectShader.prog, "cameraPos"), eye[0], eye[1], eye[2]);

	glBindVertexArray(sphereVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMaptexture);
	glDrawElements(GL_TRIANGLES, na*(nb - 1) * 6, GL_UNSIGNED_INT, sphere_ix);
	glBindVertexArray(0);

	glUseProgram(0);
}