#include "MyShader.h"
#include <cstring>

MyShader::MyShader(const char *vertName, const char *fragName)
{
	if(std::strlen(vertName) > 0 && std::strlen(fragName) > 0) {
		init(vertName, fragName);
	}
}


void MyShader::init(const char *vertName, const char *fragName)
{
	std::ifstream vert, frag;
	vert.exceptions(std::ifstream::failbit);
	frag.exceptions(std::ifstream::failbit);
	std::string vertCode, fragCode;

	try {
		vert.open(vertName);
		frag.open(fragName);

		std::ostringstream vertTemp, fragTemp;
		vertTemp << vert.rdbuf();
		fragTemp << frag.rdbuf();

		vert.close();
		frag.close();

		//vertCode = const_cast<char *>(vertTemp.str().c_str());
		//fragCode = const_cast<char *>(fragTemp.str().c_str());
		vertCode = vertTemp.str();
		fragCode = fragTemp.str();
	}
	catch (const std::ifstream::failure& e) {
		std::cout << e.what() << "SAHDER_FILE_FAIL" << std::endl;
	}

	GLuint vertex, fragment;
	GLint success;
	char infoLog[512];
	const char *vertCodeC = vertCode.c_str();
	const char *fragCodeC = fragCode.c_str();

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertCodeC, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragCodeC, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	prog = glCreateProgram();
	glAttachShader(prog, vertex);
	glAttachShader(prog, fragment);
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(prog, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void MyShader::work()
{
	glUseProgram(prog);
}