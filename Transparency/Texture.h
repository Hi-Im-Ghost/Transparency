#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>

//Klasa dla tekstur
class Texture
{
private:
	//Id tekstury
	GLuint textureID;
	//Parametry tekstury
	int width, height, channels;
	//Dane zaladowanej tekstury
	unsigned char* texData;
public:
	//Konstruktor do ustawienia zmiennych
	Texture();
	//Konstruktor, ktory laduje teksture o wskazanej sciezce 
	Texture(const std::string_view& path);
	//Destruktor
	~Texture();
	//Funkcja do pobrania id textury
	GLuint getTextureID() const;
};

