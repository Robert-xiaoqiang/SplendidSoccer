#ifndef _OBJPARSE_H_
#define _OBJPARSE_H_

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <cstring>

struct Vertex {
	float x;
	float y;
	float z;
	Vertex():
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{

	}
	void scan(std::istream& in)
	{
		in >> x >> y >> z;
	}
};

struct TextureCoord {
	float u;
	float v;
	float t;
	int N;
	TextureCoord(int N):
		u(0.0F),
		v(0.0F),
		t(0.0F),
		N(N)
	{

	}
	void scan(std::istream& in)
	{
		if(N == 2)
			in >> u >> v;
		else
			in >> u >> v >> t;
	}
};

struct Normal {
	float x;
	float y;
	float z;
	Normal():
		x(0.0F),
		y(0.0F),
		z(0.0F)
	{

	}
	void scan(std::istream& in)
	{
		in >> x >> y >> z;
	}
};

struct Face {
	struct FaceUnit {
		int vIndex;
		int tIndex;
		int nIndex; // 1 start 10 base -1 -> NULL
		FaceUnit():
			vIndex(-1),
			tIndex(-1),
			nIndex(-1)
		{
			// set -1 for null tIndex/nIndex
		}
		void scan(std::istream& in)
		{
			std::string item;
			in >> item;
			std::string::size_type pos = std::string::size_type(0), cur;
			while((cur = item.find('/', pos)) != std::string::npos) {
				int index;
				if(cur - pos > 0)
					index = std::stoi(item.substr(pos, cur - pos), 0, 0);
				else
					index = 0; // no tIndex;
				pos = cur + 1;
				if(vIndex == -1)
					vIndex = index;
				else
					tIndex = index;
			}
			nIndex = std::stoi(item.substr(pos), 0, 0);
		}
	};
	FaceUnit datas[4];
	int N;
	Face(int N) :
		N(N)
	{
		//default ctor FaceUnit
	}
	void scan(std::istream& in)
	{
		for(int i = 0; i < N; i++) {
			datas[i].scan(in);
		}
	}
};

struct Material {
	float Ka[3];
	float Kd[3];
	float Ks[3];
	float Ns = 0.0f;

	Material(float *Ka = nullptr, float *Kd = nullptr, 
			 float *Ks = nullptr, float Ns = 0.0f):
		Ns(Ns)
	{
		if(Ka != nullptr)
			std::memcpy(this->Ka, Ka, 3 * sizeof(float));
		if(Kd != nullptr)
			std::memcpy(this->Kd, Kd, 3 * sizeof(float));
		if(Ks != nullptr)
			std::memcpy(this->Ks, Ks, 3 * sizeof(float));
	}
};

class OBJParse {

	//std::vector<Group> groups;
	std::map<std::string, Material> materialMap;
	std::map<std::string, GLuint> textureMap;
	std::vector<Vertex> vertexs;
	std::vector<TextureCoord> textures;
	std::vector<Normal> normals;
	std::map<std::string, std::vector<Face>> renderUnit;

	std::string fileName;
	int faceN;
	int textureN;
	void scan();
	void parseTexture();
	
	GLuint OBJParse::readTexture(const std::string& texturePng);
public:
	OBJParse();
	virtual ~OBJParse();
	void openFile(const std::string& fileName, int faceN, int textureN);
	void draw();
};

#endif