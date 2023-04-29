#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const std::string_view& vertexFilePath, const std::string_view& fragmentFilePath, SourceType vertexSourceType, SourceType fragmentSourceType)
{
	Shader vertexShader(vertexFilePath, ShaderType::VERTEX, vertexSourceType);
	Shader fragmentShader(fragmentFilePath, ShaderType::FRAGMENT, fragmentSourceType);

	//Obiekt programu shaderow
	this->shaderProgramID = glCreateProgram();

	if (shaderProgramID == 0)
	{
		throw std::runtime_error("ERROR::PROGRAM_SHADER_CREATE_ERROR");
	}

	//Laczenie shaderow
	glAttachShader(shaderProgramID, vertexShader.getShaderID());
	checkErrors(shaderProgramID, "VERTEX");
	glAttachShader(shaderProgramID, fragmentShader.getShaderID());
	checkErrors(shaderProgramID, "FRAGMENT");
	glLinkProgram(shaderProgramID);
	checkErrors(shaderProgramID, "PROGRAM");
}

ShaderProgram::~ShaderProgram()
{
	//Jesli jest program to 
	if (shaderProgramID != 0)
	{
		//Wyczysc
		glDeleteProgram(this->shaderProgramID);
	}
}

void ShaderProgram::useShaderProgram() const
{
	//Jesli nie ma program to 
	if (shaderProgramID == 0)
	{
		throw std::runtime_error("ERROR::PROGRAM_CANNOT_USE");
	}
	else
	{
		glUseProgram(this->shaderProgramID);
	}
}

GLuint ShaderProgram::getShaderProgram() const
{
	return this->shaderProgramID;
}
