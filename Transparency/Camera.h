#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
//Klasa do obslugi kamery
class Camera
{
private:
	//Pozycja kamery
	glm::vec3 _position;
	//Kierunek patrzenia kamery
	glm::vec3 _front;
	//Wektor w gore
	glm::vec3 _up;
	//Wektor prawostrony
	glm::vec3 _right;
	//Wektor w gore swiata
	glm::vec3 _worldUp;
	//kat obrotu w poziomie
	float _yaw;
	//kat obrotu w pionie
	float _pitch;
	//pole widzenia
	float _fov;
	//stosunek szerokosci do wysokosci
	float _aspectRatio;
	//szerokosc ekranu
	const int _screenWidth;
	//wysokosc ekranu
	const int _screenHeight;
	//czulosc myszy
	float _mouseSensitivity;
	//predkosc poruszania
	float _movementSpeed;
	//Metoda do aktualizowania wektorow kamery
	void updateCameraVectors();
public:
	enum class Direction {
		Forward,
		Backward,
		Left,
		Right
	};
	//Konstruktor kamery
	Camera(int screenWidth, int screenHeight, glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f));
	//Metoda do przetwarzania ruchu myszka
	void setMouseMovement(float xOffset, float yOffset, float sensitivity = 0.2f);
	//Metoda do obslugi poruszania przy pomocy klawiszy
	void setKeyboardMovement(Direction direction, float deltaTime);
	//Metoda do przetwarzania kolka myszy
	void setMouseScroll(float yOffset);
	//Metoda do zwracania macierzy widoku
	glm::mat4 getViewMatrix() const;
	//Metoda do zwracania macierzy projekcji
	glm::mat4 getProjectionMatrix() const;
};

