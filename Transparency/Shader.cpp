#include "Shader.h"

Shader::Shader(const std::string_view& path, ShaderType shaderType, SourceType sourceType)
{
	//zwraca wskaznik na tablice znakow
	std::string  shaderCode = path.data();
	//Jesli typ shadera to plik
	if (sourceType == SourceType::FILE)
	{
		//utworz strumien wejsciowy dla shadera 
		std::ifstream shaderFile(path.data());
		//jesli strumien ma bledy odczytu/zapisu
		if (!shaderFile.good())
		{
			//zglos wyjatek
			throw std::runtime_error("SHADER_FILE_NOT_SUCCESFULLY_READ");
		}
		//przy pomocy iteratora pobierz kazdy znak po kolei
		shaderCode = std::string((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>()).data();
		//OGARNAC TEGO EOF
		//Jesli zostaly jakies bity to
		if (shaderFile.eof())
		{
			//zglos wyjatek
			throw std::runtime_error("SHADER_FILE_NOT_SUCCESFULLY_READ");
		}
		
	}
	const char* content = shaderCode.c_str();
	//Tworzenie shadera
	shaderID = glCreateShader(shaderType == ShaderType::VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
	//Dolaczenie kodu shadera do obiektu
	glShaderSource(shaderID, 1, &content, NULL);
	glCompileShader(shaderID);
	checkErrors(shaderID, "SHADER");

}

Shader::~Shader()
{
	//Jesli jest shader to 
	if (shaderID != 0)
	{
		//Wyczysc
		glDeleteShader(shaderID);
	}
}

GLuint Shader::getShaderID() const
{
	return shaderID;
}

void Shader::checkErrors(GLuint shader, std::string type)
{
	//Sprawdz typ 
	if (type != "PROGRAM")
	{
		shader = getShaderID();
		//Pobierz wybrany parametr (status komiplacji)
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			//Pobierz wybrany parametr (dlugosc logow)
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
			//Jesli nie ma logow
			if (len == 0)
			{
				//Usun obiekt shadera
				glDeleteShader(shader);
				shaderID = 0;
				shader = 0;
				throw std::runtime_error("SHADER_COMPILATION_ERROR_WITHOUT_ANY_LOG");
			}
			//Utworz zmienna vector char o dlugosci len
			std::vector<char> log(len);
			//Pobierz informacje z logow shadera
			glGetShaderInfoLog(shader, log.size(), &len, log.data());
			//Usun obiekt shadera
			glDeleteShader(shader);
			shaderID = 0;
			shader = 0;
			//TO DO SPRAWDZIC BY KONSOLA WYPISYWALA BLEDY SHADERA
			throw std::runtime_error({ log.begin(), log.end() });
		}
	}
	else if (type == "VERTEX")
	{
		//Pobierz informacje o bledach
		error = glGetError();
		//Jesli sa bledy
		if (error != GL_NO_ERROR)
		{
			//Usun obiekt programu
			glDeleteProgram(shader);
			shader = 0;
			//wyrzuc wyjatek
			throw std::runtime_error("ERROR::PROGRAM_ATTACHING_VERTEX with code: " + std::to_string(error));
		}
	}
	else if (type == "FRAGMENT")
	{
		//Pobierz informacje o bledach
		error = glGetError();
		//Jesli sa bledy
		if (error != GL_NO_ERROR)
		{
			//Usun obiekt programu
			glDeleteProgram(shader);
			shader = 0;
			//wyrzuc wyjatek
			throw std::runtime_error("ERROR::PROGRAM_ATTACHING_FRAGMENT with code: " + std::to_string(error));
		}
		else
		{
			//Pobierz wybrany parametr (status linkowania)
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				//Pobierz wybrany parametr (dlugosc logow)
				glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &len);
				//Jesli nie ma logow
				if (len == 0)
				{
					//Usun obiekt programu
					glDeleteProgram(shader);
					shader = 0;
					throw std::runtime_error("PROGRAM_LINK_ERROR_WITHOUT_ANY_LOG");
				}
				//Utworz zmienna vector char o dlugosci len
				std::vector<char> log(len);
				//Pobierz informacje z logow shadera
				glGetProgramInfoLog(shader, log.size(), &len, log.data());
				//Usun obiekt programu
				glDeleteProgram(shader);
				shader = 0;
				throw std::runtime_error({ log.begin(), log.end() });
			}
		}
	}
}


/*
void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}
*/