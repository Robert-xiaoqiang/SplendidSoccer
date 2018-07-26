#include "Model.h"

//directory default ctor ""
Model::Model(const string& fileName)
{
	init(fileName);
}

void Model::draw(const MyShader& shader)
{

    for(GLuint i = 0; i < meshes.size(); i++)
        meshes[i].draw(shader);

}

void Model::init(const string& fileName)
{
	string::size_type pos;
	pos = fileName.rfind('/');
	if (pos != string::npos)
		directory = fileName.substr(0, pos + 1);
	// Read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);
    // Check for errors
    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }

    parseNode(scene->mRootNode, scene);
}

void Model::parseNode(aiNode* node, const aiScene* scene)
{
    for(GLuint i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(parseMesh(mesh, scene));         
    }

    for(GLuint i = 0; i < node->mNumChildren; i++) {
        this->parseNode(node->mChildren[i], scene);
    }
}

Mesh Model::parseMesh(aiMesh* mesh, const aiScene* scene)
{
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    for(GLuint i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vec3 tempVec; 
        // Positions
        tempVec.x = mesh->mVertices[i].x;
        tempVec.z = -mesh->mVertices[i].y;
        tempVec.y = mesh->mVertices[i].z;
        vertex.position = tempVec;
        // Normals
        tempVec.x = mesh->mNormals[i].x;
        tempVec.z = -mesh->mNormals[i].y;
        tempVec.y = mesh->mNormals[i].z;
        vertex.normal = tempVec;
        // Texture Coordinates
        if(mesh->mTextureCoords[0]) {
            vec2 tempVec;
            tempVec.x = mesh->mTextureCoords[0][i].x; 
            tempVec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = tempVec;
        } else {
            vertex.texCoords = vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }
    
    for(GLuint i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // Process materials
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // Diffuse: texture_diffuseN
        // Specular: texture_specularN
        // Normal: texture_normalN

        // 1. Diffuse maps
        vector<Texture> diffuseTextures;
        parseTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", diffuseTextures);
        textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
        
        // 2. Specular maps
        vector<Texture> specularTextures;
        parseTextures(material, aiTextureType_SPECULAR, "texture_specular", specularTextures);
        textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());
    }
    
    // Return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures, SHADER);
}

void Model::parseTextures(aiMaterial* mat, aiTextureType type, 
                          const string& typeName, vector<Texture>& ret)
{
    for(GLuint i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        GLboolean skip = false;
        for(GLuint j = 0; j < texturesLoaded.size(); j++) {
            if(!strcmp(texturesLoaded[j].fileName.C_Str(), str.C_Str())) {

                ret.push_back(texturesLoaded[j]);
                
                skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip) {   
            // If texture hasn't been loaded already, load it
            Texture texture;
            texture.id = readTexture(str.C_Str());
            texture.type = typeName;
            texture.fileName = str;

            ret.push_back(texture);
            
            texturesLoaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
}

GLint Model::readTexture(const char* path)
{
    //Generate texture ID and load texture data 
    string fileName = string(path);
    fileName = directory + fileName;
    //GLuint textureID;
    //glGenTextures(1, &textureID);
    //int width,height;
	GLuint textureID = SOIL_load_OGL_texture(fileName.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS | SOIL_FLAG_DDS_LOAD_DIRECT);
    //unsigned char* image = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    // Assign texture to ID
    //glBindTexture(GL_TEXTURE_2D, textureID);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    //glGenerateMipmap(GL_TEXTURE_2D);    

    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    //SOIL_free_image_data(image);
    return textureID;
}