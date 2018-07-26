#ifndef _MESH_H_
#define _MASH_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

#include "glew.h"
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "MyShader.h"


struct vec2 {
    float x;
    float y;
    vec2(float x = 0.0f, float y = 0.0f):
        x(x), y(y)
    {

    }        
};

struct vec3 {
    float x;
    float y;
    float z;
    vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f):
        x(x), y(y), z(z)
    {

    }
    vec3(const vec2& xy, float z):
        x(xy.x), y(xy.y), z(z)
    {

    }
}; 

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texCoords;
};

struct Texture {
    GLuint id;         // texture id for glGenTex()
    string type;       // "texture_diffuse" / "texture_specular"
    aiString fileName; // ../../.png .dds .tga for redundant load texture files
};

enum Method {FIXEDPIPLINE, SHADER};

class Mesh {
private:
    GLuint VAO; 
    GLuint VBO; 
    GLuint EBO;

    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    void init();
public:
	Mesh(){ }
    Mesh(const vector<Vertex>& vertices, const vector<GLuint> indices, const vector<Texture> textures, Method method);
	void init(const vector<Vertex>& vertices, const vector<GLuint> indices, const vector<Texture> textures, Method method);
	void draw(const MyShader& shader);
	void draw(); // draw by fixed pipeline! Naive! However!
};


#endif