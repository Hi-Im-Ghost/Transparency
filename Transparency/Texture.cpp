#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture()
{
	this->width = 0;
	this->height = 0;
	this->channels = 0;
	this->textureID = 0;
	this->texData = NULL;
}

Texture::Texture(const std::string_view& path)
{
	if (this->textureID)
	{
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &textureID);
	}
	//Obrocenie tekstury (os Y) bo opengl oczekuje osi y na gorze
	stbi_set_flip_vertically_on_load(true);
	//ladowanie tekstury
	texData = stbi_load(path.data(), &width, &height, &channels, 0);

	if (texData) {
		GLenum format{};
		if (channels == 1)
			format = GL_RED;
		else if (channels == 3)
			format = GL_RGB;
		else if (channels == 4)
			format = GL_RGBA;

		//Sprawdzanie czy ladowanie sie powiodlo
		if (stbi_failure_reason())
		{
			std::cout << "ERROR::FAIL_LOAD_TEXDATA" << std::endl;
			std::cout << stbi_failure_reason() << std::endl;
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
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			//Generowanie obrazu tekstury
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
			//Generowanie mipmap
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(texData);
		
	}
}

Texture::~Texture()
{
	if (textureID != 0) {
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &textureID);
	}
}

GLuint Texture::getTextureID() const
{
	return textureID;
}

void Texture::genTextures()
{
	//Generowanie tekstur i ustawienie ich identyfikatorow 
	glGenTextures(1, &textureID);
}

void Texture::useTexture() const
{
	//Aktywacja tekstur i przypisanie nazwy tekstury do celu
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

}

void Texture::useTexture(GLuint textureNum) const
{
	//Aktywacja tekstur i przypisanie nazwy tekstury do celu
	glActiveTexture(GL_TEXTURE0 + textureNum);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() const
{
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}