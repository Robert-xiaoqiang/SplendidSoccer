#include "Mesh.h"

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

Mesh::Mesh(const vector<Vertex>& vertices, 
           const vector<GLuint> indices,
           const vector<Texture> textures,
		   Method method):
    vertices(vertices),
    indices(indices),
    textures(textures)
{
    if(method == SHADER) init();
}

void Mesh::init()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);   // location layout
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    
    glEnableVertexAttribArray(1);   // location layout
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
   
    glEnableVertexAttribArray(2);   // location layout
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::init(const vector<Vertex>& vertices, 
				const vector<GLuint> indices, 
				const vector<Texture> textures,
				Method method)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	if(method == SHADER) init();
}

void Mesh::draw(const MyShader& shader)
{
    GLuint diffuseNr = 1;   // sampler2D name 
    GLuint specularNr = 1;  // sampler2D name
    for(GLuint i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); 
        
        stringstream ss;
        string number;
        string name = this->textures[i].type;
        if(name == "texture_diffuse")
            ss << diffuseNr++; 
        else if(name == "texture_specular")
            ss << specularNr++; 
        number = ss.str(); 
        
        // i for sampler2D texture unit!
		std::cout << name + number << std::endl;
        glUniform1i(glGetUniformLocation(shader.prog, (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    
    glUniform1f(glGetUniformLocation(shader.prog, "material.shininess"), 16.0f);

    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    for (GLuint i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
void Mesh::draw()
{

}