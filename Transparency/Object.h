#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <ObjLoader.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Definicja struktury Vertex, przechowuj�cej informacje o wierzcho�kach obiektu
struct Vertex {
    glm::vec3 Position;   // Pozycja w przestrzeni 3D
    glm::vec3 Normal;     // Wektor normalny wierzcho�ka
    glm::vec2 TexCoords;  // Wsp�rz�dne tekstury
};

// Klasa Object reprezentuj�ca obiekt na scenie
class Object
{
public:
    // Konstruktor przyjmuj�cy �cie�k� do pliku obiektu
    Object(const std::string& path);  

    // Identyfikatory bufor�w OpenGL
    GLuint VAO, VBO, EBO;          
    // Wektor wierzcho�k�w obiektu
    std::vector <Vertex> vertices;  
    // Liczba wierzcho�k�w
    size_t vertexCount;      
    // Wektor indeks�w wierzcho�k�w dla renderowania obiektu
    std::vector<unsigned int> indices; 
    // Macierz modelu
    glm::mat4 modelMatrix;                 

    // Funkcja wczytuj�ca model obiektu z pliku
    bool loadModel(const std::string& path);      
    // Funkcja przetwarzaj�ca siatk� (mesh) obiektu
    void processMesh(const aiMesh* mesh);   
    // Inicjalizacja bufor�w OpenGL
    void initBuffers();                              
    // Ustawianie warto�ci uniform�w w shaderze
    void setShaderUniforms(GLuint shaderProgram, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& objectColor, const glm::vec3& lightColor, const glm::vec3& lightPos, const glm::vec3& viewPos);
    // Renderowanie obiektu
    void draw();    
    // Ustawianie macierzy modelu
    void setModelMatrix(const glm::mat4& model);     
    // Przesuni�cie obiektu
    void translate(const glm::vec3& translation);   
    // Obr�t obiektu
    void rotate(float angle, const glm::vec3& axis);       
    // Skalowanie obiektu
    void scale(const glm::vec3& scale);                          
};

