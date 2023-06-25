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

// Definicja struktury Vertex, przechowuj¹cej informacje o wierzcho³kach obiektu
struct Vertex {
    glm::vec3 Position;   // Pozycja w przestrzeni 3D
    glm::vec3 Normal;     // Wektor normalny wierzcho³ka
    glm::vec2 TexCoords;  // Wspó³rzêdne tekstury
};

// Klasa Object reprezentuj¹ca obiekt na scenie
class Object
{
public:
    // Konstruktor przyjmuj¹cy œcie¿kê do pliku obiektu
    Object(const std::string& path);  

    // Identyfikatory buforów OpenGL
    GLuint VAO, VBO, EBO;          
    // Wektor wierzcho³ków obiektu
    std::vector <Vertex> vertices;  
    // Liczba wierzcho³ków
    size_t vertexCount;      
    // Wektor indeksów wierzcho³ków dla renderowania obiektu
    std::vector<unsigned int> indices; 
    // Macierz modelu
    glm::mat4 modelMatrix;                 

    // Funkcja wczytuj¹ca model obiektu z pliku
    bool loadModel(const std::string& path);      
    // Funkcja przetwarzaj¹ca siatkê (mesh) obiektu
    void processMesh(const aiMesh* mesh);   
    // Inicjalizacja buforów OpenGL
    void initBuffers();                              
    // Ustawianie wartoœci uniformów w shaderze
    void setShaderUniforms(GLuint shaderProgram, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& objectColor, const glm::vec3& lightColor, const glm::vec3& lightPos, const glm::vec3& viewPos);
    // Renderowanie obiektu
    void draw();    
    // Ustawianie macierzy modelu
    void setModelMatrix(const glm::mat4& model);     
    // Przesuniêcie obiektu
    void translate(const glm::vec3& translation);   
    // Obrót obiektu
    void rotate(float angle, const glm::vec3& axis);       
    // Skalowanie obiektu
    void scale(const glm::vec3& scale);                          
};

