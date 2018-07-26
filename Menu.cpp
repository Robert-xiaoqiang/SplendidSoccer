#include "Menu.h"
#include "View.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

extern GLfloat lightPosition[4]; // from main.cpp

char beginSkyTex[][50] = {
	"datas/skybox3/rt.png",
	"datas/skybox3/lf.png",
	"datas/skybox3/up.png",
	"datas/skybox3/dn.png",
	"datas/skybox3/bk.png",
	"datas/skybox3/ft.png",
};

GLfloat cubeVertices[] = {
	// Positions          // Texture Coords
	3.5f, -3.5f, -3.5f, 0.0f, 1.0f,
	3.5f, 3.5f, -3.5f, 0.0f, 0.0f,
	-3.5f, 3.5f, -3.5f, 1.0f, 0.0f,
	-3.5f, -3.5f, -3.5f, 1.0f, 1.0f,

	-3.5f, -3.5f, 3.5f, 0.0f, 1.0f,
	3.5f, -3.5f, 3.5f, 0.0f, 0.0f,
	3.5f, 3.5f, 3.5f, 1.0f, 0.0f,
	-3.5f, 3.5f, 3.5f, 1.0f, 1.0f,

	-3.5f, 3.5f, 3.5f, 1.0f, 0.0f,
	-3.5f, 3.5f, -3.5f, 0.0f, 0.0f,
	-3.5f, -3.5f, -3.5f, 0.0f, 1.0f,
	-3.5f, -3.5f, 3.5f, 1.0f, 1.0f,

	3.5f, 3.5f, 3.5f, 1.0f, 0.0f,
	3.5f, 3.5f, -3.5f, 0.0f, 0.0f,
	3.5f, -3.5f, -3.5f, 0.0f, 1.0f,
	3.5f, -3.5f, 3.5f, 1.0f, 1.0f,

	-3.5f, -3.5f, -3.5f, 0.0f, 1.0f,
	3.5f, -3.5f, -3.5f, 1.0f, 1.0f,
	3.5f, -3.5f, 3.5f, 1.0f, 0.0f,
	-3.5f, -3.5f, 3.5f, 0.0f, 0.0f,

	-3.5f, 3.5f, -3.5f, 0.0f, 0.0f,
	3.5f, 3.5f, -3.5f, 0.0f, 1.0f,
	3.5f, 3.5f, 3.5f, 1.0f, 1.0f,
	-3.5f, 3.5f, 3.5f, 1.0f, 0.0f,
};
GLuint indices[] = {
	0, 1, 2, 3,
	4, 5, 6, 7,
	8, 9, 10, 11,
	12, 13, 14, 15,
	16, 17, 18, 19,
	20, 21, 22, 23,
};

GLfloat vertices[] = {
// Back face
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
// Front face
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
// Left face
-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
// Right face
0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
// Bottom face
-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
// Top face
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left        
};
void Menu::init()
{
	beginSky.init(beginSkyTex, BIG);
	//b.init(vec3(32.0f, 6.0f, 0.0f), 0.5f, "datas/soccer.png");
	//b.enable(0.0f, 5.0f);
	goal.openFile("datas/goal/goal3.obj", 3, 3);     // 3 FaceUnit!	
	trophy.openFile("datas/trophy/trophy.obj", 3, 3);

	menuGoalList = glGenLists(1);   // for menu.goal
	menuTrophyList = glGenLists(1); // for menu.trophy

	glNewList(menuGoalList, GL_COMPILE);
	goal.draw();
	glEndList();

	glNewList(menuTrophyList, GL_COMPILE);
	trophy.draw();
	glEndList();

	penalty = generateTexId("datas/menu/penalty.jpg");
	corner = generateTexId("datas/menu/corner.jpg");
	random = generateTexId("datas/menu/random.jpg");
	
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
	
	//glGenVertexArrays(1, &cubeVAO);
	//glGenBuffers(1, &cubeVBO);
	//// Fill buffer
	//glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//// Link vertex attributes
	//glBindVertexArray(cubeVAO);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	cubeShader.init("shaders/cube.vert", "shaders/cube.frag");
	objectWithShadowShader.init("shaders/render_with_shadow.vert", 
								"shaders/render_with_shadow.frag");

	glGenFramebuffers(1, &depthMapFBO);
	// - Create depth texture
	glGenTextures(1, &depthMapTextureID);
	glBindTexture(GL_TEXTURE_2D, depthMapTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTextureID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	shadowShader.init("shaders/shadow.vert", "shaders/shadow.frag");
}

void Menu::renderOptions()
{
	GLfloat am[] = { 1.0f, 1.0f, 0.0f };
	GLfloat sp[] = { 1.0f, 1.0f, 1.0f };
	GLfloat di[] = { 1.0f, 1.0f, 0.0f };

	beginSky.render();
	glPushMatrix();
	//b.render();   
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(10.0, 7.0, 0.0);
	glCallList(menuTrophyList);
	glPopMatrix();
	
	glPushMatrix();
	glRotated(90.0, 0.0, 1.0, 0.0);
	glScaled(0.5, 0.5, 0.5);
	glCallList(menuGoalList);
	glPopMatrix();
}

void Menu::renderACube(GLuint i)
{
	GLfloat modelView[16], projection[16];

	glUseProgram(0);
	glPushAttrib(GL_TEXTURE_BIT);
	View::getModelViewMatrix(modelView);
	View::getProjectionMatrix(projection);

	cubeShader.work();
	glUniformMatrix4fv(glGetUniformLocation(cubeShader.prog, "modelView"), 1, GL_FALSE, modelView);
	glUniformMatrix4fv(glGetUniformLocation(cubeShader.prog, "projection"), 1, GL_FALSE, projection);
	glUniform1i(glGetUniformLocation(cubeShader.prog, "texture_diffuse1"), 0);

	// Cubes
	glBindVertexArray(cubeVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, i);

	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, indices);
	glBindVertexArray(0);

	glUseProgram(0);
	glPopAttrib();
}

void Menu::renderCubes()
{
	glPushMatrix();
	glTranslated(10.0, 3.5, 10.0);
	renderACube(penalty);
	glPopMatrix();

	glPushMatrix();
	glTranslated(10.0, 3.5, 0.0);
	renderACube(random);
	glPopMatrix();

	glPushMatrix();
	glTranslated(10.0, 3.5, -10.0);
	renderACube(corner);
	glPopMatrix();
}

void Menu::renderCubesWithShadow()
{
	glUseProgram(0);
	glPushAttrib(GL_TEXTURE_BIT);

	GLfloat viewMatrix[16], projectionMatrix[16];
	GLfloat eyePosition[3];
	View::getEye(eyePosition);
	View::getViewMatrix(viewMatrix);
	View::getProjectionMatrix(projectionMatrix);
	int SRC_WIDTH = View::getWidth();
	int SRC_HEIIGHT = View::getHeight();

	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	glm::vec3 lightPos = glm::vec3(lightPosition[0], lightPosition[1], lightPosition[2]);

	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f); // 1.0 7.5
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	glm::mat4 model;

	// 1. now render scene from light's point of view
	shadowShader.work();
	glUniformMatrix4fv(glGetUniformLocation(shadowShader.prog, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Render Cube
	glUniformMatrix4fv(glGetUniformLocation(shadowShader.prog, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. Render scene as normal 
	glViewport(0, 0, SRC_WIDTH, SRC_HEIIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	objectWithShadowShader.work();
	glUniformMatrix4fv(glGetUniformLocation(objectWithShadowShader.prog, "projection"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(objectWithShadowShader.prog, "view"), 1, GL_FALSE, viewMatrix);
	glUniform3fv(glGetUniformLocation(objectWithShadowShader.prog, "lightPos"), 1, &lightPos[0]);
	glUniform3fv(glGetUniformLocation(objectWithShadowShader.prog, "viewPos"), 1, eyePosition);
	glUniformMatrix4fv(glGetUniformLocation(objectWithShadowShader.prog, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	// render penalty
	glBindVertexArray(cubeVAO);
	glBindTexture(GL_TEXTURE_2D, penalty);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(objectWithShadowShader.prog, "diffuseTexture"), 0);

	glBindTexture(GL_TEXTURE_2D, depthMapTextureID);
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(objectWithShadowShader.prog, "shadowMap"), 1);
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(10.0f, 3.5f, 10.0f));
	glUniformMatrix4fv(glGetUniformLocation(objectWithShadowShader.prog, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, indices);
	glBindVertexArray(0);
	// render random
	/*
	model = glm::mat4();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, random);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMapTextureID);
	model = glm::translate(model, glm::vec3(10.0f, 3.5f, 0.0f));
	glBindVertexArray(cubeVAO);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, indices);
	glBindVertexArray(0);
	*/
	// render corner
	/*
	model = glm::mat4();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, corner);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMapTextureID);
	model = glm::translate(model, glm::vec3(10.0f, 3.5f, -10.0f));
	glBindVertexArray(cubeVAO);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, indices);
	glBindVertexArray(0);
	*/

	glUseProgram(0);
	glPopAttrib();
}

void Menu::render()
{
	renderOptions();
	renderCubes();
	//renderCubesWithShadow();
}
