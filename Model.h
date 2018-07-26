#ifndef MODEL_H_
#define MODEL_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

#include "glew.h" 
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include "SOIL.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Mesh.h"

class Model {
private:
    vector<Mesh> meshes;
    string directory; // .obj / .mtl
    vector<Texture> texturesLoaded; 

    void parseNode(aiNode* node, const aiScene* scene);
    Mesh parseMesh(aiMesh* mesh, const aiScene* scene);
    void parseTextures(aiMaterial* mat, aiTextureType type, const string& typeName, vector<Texture>&);

    // path from aiString!!! dir from ctor!!!
    GLint readTexture(const char* path);
public:
	Model() { }
    Model(const string& fileName);
	void init(const string& fileName);
    void draw(const MyShader& shader);
};


#endif