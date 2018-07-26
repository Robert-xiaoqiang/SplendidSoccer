#define FREEGLUT_STATIC

#include "glew.h"
#include "GL\freeglut.h"
#include "SOIL.h"
#include "OBJParse.h"
#include "General.h"
#include <exception>
#include <fstream>

OBJParse::OBJParse()
{
	vertexs.resize(1);
	normals.resize(1);
	textures.push_back(TextureCoord(3));
}

OBJParse::~OBJParse()
{

}

void OBJParse::openFile(const std::string& fileName, int faceN, int textureN)
{
	this->fileName = fileName;
	this->faceN = faceN;
	this->textureN = textureN;
	scan();
	return;
}

void OBJParse::parseTexture()
{
	std::string mtlFile = fileName.substr(0, fileName.rfind('.') + 1) + "mtl";
	std::ifstream in(mtlFile.c_str()); // text format
	std::string flag;
	std::string dummy;
	while (in >> flag) {
		if (flag == "newmtl") {
			std::string mtlName, texturePng;
			in >> mtlName;
			Material cur;
			while(true) {
				in >> flag;
				if(flag == "Ns") {
					in >> cur.Ns;
				} else if(flag == "Ka" || flag == "ka") {
					in >> cur.Ka[0] >> cur.Ka[1] >> cur.Ka[2];
				} else if(flag == "Kd" || flag == "kd") {
					in >> cur.Kd[0] >> cur.Kd[1] >> cur.Kd[2];
				} else if(flag == "Ks" || flag == "ks") {
					in >> cur.Ks[0] >> cur.Ks[1] >> cur.Ks[2];
				} else if(flag == "map_Kd" || flag == "map_kd") {
					in >> texturePng;
					textureMap[mtlName] = readTexture(texturePng);
					//textureMap[mtlName] = generateTexId(texturePng.c_str());
					std::getline(in, dummy);
				} else if(flag == "illum"){ 
					std::getline(in, dummy);
					materialMap[mtlName] = cur;
					break;
				} else {
					std::getline(in, dummy);
				}
			}
		} else {
			std::getline(in, dummy);
		}
	}

	int debug = 1;

}

GLuint OBJParse::readTexture(const std::string& texturePng)
{
	GLuint textureID = 
	SOIL_load_OGL_texture((texturePng).c_str(), 
							SOIL_LOAD_AUTO, 
							SOIL_CREATE_NEW_ID,
		                    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y |
							SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}

void OBJParse::scan()
{
	if(fileName == "datas/cola/cola.obj") {
		int debug = 1;
	}
	parseTexture();
	std::ifstream in; // text
	in.open(this->fileName);
	std::string tag, dummy, currentMtl;
	std::vector<Face> tempVec;
	int line = 0;

	while(in >> tag) {
		if(line == 187) {
			int debug = 1;
		}
		if(tag == "usemtl"){
			if(!tempVec.empty()) {
				if(renderUnit.find(currentMtl) == renderUnit.end())
					renderUnit.insert(std::make_pair(currentMtl, tempVec));
				else
					renderUnit[currentMtl].insert(renderUnit[currentMtl].end(), 
												  tempVec.begin(), tempVec.end());
			}
			in >> currentMtl;
			tempVec.clear();
		} else if(tag == "v"){
			Vertex v;
			v.scan(in);
			vertexs.push_back(v);
		} else if(tag == "vt"){
			TextureCoord t(textureN);
			t.scan(in);
			textures.push_back(t);
		} else if(tag == "vn"){
			Normal n;
			n.scan(in);
			normals.push_back(n);
		} else if(tag == "f"){
			try {
				Face f(faceN);
				f.scan(in);
				tempVec.push_back(f);
			} catch(const std::exception& e) {
				std::cout << line << std::endl;
			}

		} else if(tag == "g" || tag == "s" || tag == "o" || tag == "l") {
			//in >> dummy;
			//Group g;
			//g.scan(in);
			//groups.push_back(g);
			std::getline(in, dummy);
		} else {
			std::getline(in, dummy); 
			// EOL
			// usemtl mtllib o 
			// omitted
		}
		line++;
		//std::cout << line << std::endl;
	}
	if(!tempVec.empty()) {
		if(renderUnit.find(currentMtl) == renderUnit.end())
			renderUnit.insert(std::make_pair(currentMtl, tempVec));
		else
			renderUnit[currentMtl].insert(renderUnit[currentMtl].end(),
				tempVec.begin(), tempVec.end());
	}
	in.close();
}

void OBJParse::draw()
{
	glBlendFunc(GL_ONE, GL_ZERO);

	for(const auto& r : renderUnit) {
		bool isPng = false;
		if(textureMap.find(r.first) != textureMap.end()) {
			glPushAttrib(GL_TEXTURE_BIT);
			isPng = true;
			glBindTexture(GL_TEXTURE_2D, textureMap[r.first]);
		} 
		glPushAttrib(GL_LIGHTING_BIT);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialMap[r.first].Ka);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialMap[r.first].Kd);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialMap[r.first].Ks);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialMap[r.first].Ns);
		int vIndex;
		int nIndex;
		int tIndex;
		try {
			for(const auto& f : r.second) {
				if(faceN == 3)
					glBegin(GL_TRIANGLES);
				else
					glBegin(GL_QUADS);
				for(int j = 0; j < faceN; j++) {
					vIndex = f.datas[j].vIndex;
					nIndex = f.datas[j].nIndex;
					tIndex = f.datas[j].tIndex;
					if(isPng)
						glTexCoord2f(textures[tIndex].u, textures[tIndex].v);
				    glNormal3f(normals[nIndex].x, normals[nIndex].y, normals[nIndex].z);
					glVertex3f(vertexs[vIndex].x, vertexs[vIndex].y, vertexs[vIndex].z);
				}
				glEnd();
			}
		} catch(...) {
			std::cout << "GG" << std::endl;
			;;;;;;;
			;;;;;;;
			;;;;;;;
		}
		if(isPng) {
			glPopAttrib();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glPopAttrib();
	}
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return;
}