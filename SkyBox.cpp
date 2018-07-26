#include "SkyBox.h"
#include "MyShader.h"
#include "View.h"

GLfloat skyboxVertices[] = {
	// Positions          
	-5.0f,  5.0f, -5.0f,
	-5.0f, -5.0f, -5.0f,
	5.0f, -5.0f, -5.0f,
	5.0f, -5.0f, -5.0f,
	5.0f,  5.0f, -5.0f,
	-5.0f,  5.0f, -5.0f,

	-5.0f, -5.0f,  5.0f,
	-5.0f, -5.0f, -5.0f,
	-5.0f,  5.0f, -5.0f,
	-5.0f,  5.0f, -5.0f,
	-5.0f,  5.0f,  5.0f,
	-5.0f, -5.0f,  5.0f,

	5.0f, -5.0f, -5.0f,
	5.0f, -5.0f,  5.0f,
	5.0f,  5.0f,  5.0f,
	5.0f,  5.0f,  5.0f,
	5.0f,  5.0f, -5.0f,
	5.0f, -5.0f, -5.0f,

	-5.0f, -5.0f,  5.0f,
	-5.0f,  5.0f,  5.0f,
	5.0f,  5.0f,  5.0f,
	5.0f,  5.0f,  5.0f,
	5.0f, -5.0f,  5.0f,
	-5.0f, -5.0f,  5.0f,

	-5.0f,  5.0f, -5.0f,
	5.0f,  5.0f, -5.0f,
	5.0f,  5.0f,  5.0f,
	5.0f,  5.0f,  5.0f,
	-5.0f,  5.0f,  5.0f,
	-5.0f,  5.0f, -5.0f,

	-5.0f, -5.0f, -5.0f,
	-5.0f, -5.0f,  5.0f,
	5.0f, -5.0f, -5.0f,
	5.0f, -5.0f, -5.0f,
	-5.0f, -5.0f,  5.0f,
	5.0f, -5.0f,  5.0f
};

GLfloat skyboxVertices2[] = {
	-60.0f,  60.0f, -60.0f,
	-60.0f, -60.0f, -60.0f,
	60.0f, -60.0f, -60.0f,
	60.0f, -60.0f, -60.0f,
	60.0f,  60.0f, -60.0f,
	-60.0f,  60.0f, -60.0f,

	-60.0f, -60.0f,  60.0f,
	-60.0f, -60.0f, -60.0f,
	-60.0f,  60.0f, -60.0f,
	-60.0f,  60.0f, -60.0f,
	-60.0f,  60.0f,  60.0f,
	-60.0f, -60.0f,  60.0f,

	60.0f, -60.0f, -60.0f,
	60.0f, -60.0f,  60.0f,
	60.0f,  60.0f,  60.0f,
	60.0f,  60.0f,  60.0f,
	60.0f,  60.0f, -60.0f,
	60.0f, -60.0f, -60.0f,

	-60.0f, -60.0f,  60.0f,
	-60.0f,  60.0f,  60.0f,
	60.0f,  60.0f,  60.0f,
	60.0f,  60.0f,  60.0f,
	60.0f, -60.0f,  60.0f,
	-60.0f, -60.0f,  60.0f,

	-60.0f,  60.0f, -60.0f,
	60.0f,  60.0f, -60.0f,
	60.0f,  60.0f,  60.0f,
	60.0f,  60.0f,  60.0f,
	-60.0f,  60.0f,  60.0f,
	-60.0f,  60.0f, -60.0f,

	-60.0f, -60.0f, -60.0f,
	-60.0f, -60.0f,  60.0f,
	60.0f, -60.0f, -60.0f,
	60.0f, -60.0f, -60.0f,
	-60.0f, -60.0f,  60.0f,
	60.0f, -60.0f,  60.0f
};

MyShader skyboxShader;

GLuint skyboxVAO, skyboxVBO;

void SkyBox::init(char texturePng[][50], Edge e)
{
	// Setup skybox VAO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	if(e == BIG) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices2), &skyboxVertices2, GL_STATIC_DRAW);
	} else {
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	}
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	cubeMapTexture = generateCubeTexId(texturePng);
	skyboxShader.init("shaders/skybox.vert", "shaders/skybox.frag");
}

void SkyBox::render()
{
	GLfloat view[16], projection[16];
	glUseProgram(0);
	glDepthMask(GL_FALSE);       // Remember to turn depth writing off
	glPushAttrib(GL_TEXTURE_BIT);
	
	View::getViewMatrix(view);
	View::getProjectionMatrix(projection);
	view[12] = view[13] = view[14] = view[15] = 1.0f;

	skyboxShader.work();
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.prog, "view"), 1, GL_FALSE, view);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.prog, "projection"), 1, GL_FALSE, projection);

	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(skyboxShader.prog, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	glUseProgram(0);
	glDepthMask(GL_TRUE);
	glPopAttrib();
}