#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

//Klasa do obslugi shaderow
class Shader
{
public:
	enum ShaderType {
		VERTEX,
		FRAGMENT
	};
	enum SourceType {
		TEXT,
		FILE
	};
	Shader() = default;
	//Konstruktor do budowania shaderow
	Shader(const std::string_view &path, ShaderType shaderType, SourceType sourceType);
	//Destruktor
	~Shader();
	//Funkcja do pobrania id shadera
	GLuint getShaderID() const;
	//funkcje dla uniformow
	//void setBool(const std::string& name, bool value) const;
	//void setInt(const std::string& name, int value) const;
	//setFloat(const std::string& name, float value) const;
protected:
	//zmienna informujaca o powodzeniu kompilacji 
	int success;
	//zmienna do dlugosci komunikatu
	int len;
	//zmienna przechowujace info o bledach
	int error;
	//funkcja do sprawdzania kompilacji shaderow
	void checkErrors(GLuint shader, std::string type);
private:
	//ID shadera
	GLuint shaderID;
};


