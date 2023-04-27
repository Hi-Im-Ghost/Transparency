#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
Texture::Texture()
{
	width = 0;
	height = 0;
	channels = 0;
	textureID = 0;
	texData = NULL;
}
Texture::Texture(const std::string_view& path)
{
	width = 0;
	height = 0;
	channels = 0;
	textureID = 0;
	texData = NULL;
	//Obrocenie tekstury (os Y) bo opengl oczekuje osi y na gorze
	stbi_set_flip_vertically_on_load(true);
	//ladowanie tekstury
	texData = stbi_load(path.data(), &width, &height, &channels, 4);
	//Sprawdzanie czy ladowanie sie powiodlo
	if (stbi_failure_reason())
	{
		std::cout << "ERROR::FAIL_LOAD_TEXDATA" << std::endl;
		std::cout << stbi_failure_reason() <<std::endl;
	}
	else {
		//Generowanie tekstur i ustawienie ich identyfikatorow 
		glGenTextures(1, &textureID);
		//Powiazanie tekstury
		glBindTexture(GL_TEXTURE_2D, textureID);
		//Ustawienie filtrowania tekstur 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//Ustawienie mipmap
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//Generowanie obrazu tekstury
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		//Generowanie mipmap
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(texData);
}

Texture::~Texture()
{
	if (textureID != 0) {
		//stbi_image_free(this->texData);
	}
}

GLuint Texture::getTextureID() const
{
	return this->textureID;
}
