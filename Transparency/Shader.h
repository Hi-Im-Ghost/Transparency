#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//Klasa do obslugi shaderow
class Shader
{
public:
	//ID programu
	unsigned int shaderProgramID;
	//Konstruktor do budowania shaderow
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	//funkcja do aktywacji shadera
	void activate();
	//funkcje dla uniformow
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
private:
	//zmienna informujaca o powodzeniu kompilacji 
	int success;
	//zmienna do przechowywania komunikatow
	char infoLog[512];
	//funkcja do sprawdzania kompilacji shaderow
	void checkErrors(unsigned int shader, std::string type);
};


