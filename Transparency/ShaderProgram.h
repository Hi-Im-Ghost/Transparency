#pragma once
#include <glad/glad.h>

#include "Shader.h"

//Klasa do tworzenia shader programow 
class ShaderProgram : public Shader
{
public:
	//Konstruktor do utworzenia shader programu na podstawie podanej sciezki do i typu shadera
	ShaderProgram(const std::string_view& vertexFilePath, const std::string_view& fragmentFilePath, SourceType vertexSourceType , SourceType fragmentSourceType);
	//Destruktor 
	~ShaderProgram();
	//Metoda do uruchomienia shader programu dla aktualnej sceny
	void useShaderProgram() const;
	//Metoda do pobrania identyfikatora shader programu
	GLuint getShaderProgram() const;
private:
	//Id shader programu
	GLuint shaderProgramID;
};

