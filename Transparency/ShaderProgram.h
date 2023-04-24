#pragma once
#include <glad/glad.h>

#include "Shader.h"

class ShaderProgram : public Shader
{
public:
	ShaderProgram(const std::string_view& vertexFilePath, const std::string_view& fragmentFilePath, SourceType vertexSourceType , SourceType fragmentSourceType);
	~ShaderProgram();
	void useShaderProgram() const;
	GLuint getShaderProgram() const;
private:
	GLuint shaderProgramID;
};

