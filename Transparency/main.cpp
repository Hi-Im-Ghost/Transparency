#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

#include <iostream>

// Funkcja do skalowania okna do renderowania (uruchamia sie w momencie zmiany rozmiaru okna)
void scaleViewport(GLFWwindow* window, int width, int height);
// Funkcja do obs³ugu inputu 
void mappingInput(GLFWwindow* window);

// szerokosc okna
const unsigned int width = 800;
// wysokosc okna
const unsigned int height = 600;

// wierzcholki
float vertices[] = {
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f,  // top right
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top left 
};
//indeksy 
unsigned int indices[] = {  
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

/*
float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
};
*/
int main()
{
    glfwInit();
    // Wersja glowna opengl 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // Wersja pomocnicza opengl
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Tryb rdzenia (bez wczesniejszych funkcji)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Tworzenie okna
    GLFWwindow* window = glfwCreateWindow(width, height, "Transparency", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //Ustawienie funkcji do wywolania w momencie zmiany rozmiaru okna
    glfwSetFramebufferSizeCallback(window, scaleViewport);

    // glad inicjalizacja 
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //SHADERS
    Shader shader("Shaders/vertex.glsl", "Shaders/fragment.glsl");
  
    //VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    //VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    //Funkcja do wiazania obiektu tablicy wierzcholkow
    glBindVertexArray(VAO);
    //Funkcja do wiazania bufora z typem 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Funkcja do przydzielenia pamieci dla bufora
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //Funkcja do okreslenia jak opengl ma interpretowac dane wierzcholkow
    //pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Rozlaczenie buffora bo zostal zarejestrowany juz 
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //Odlaczenie VAO
    //glBindVertexArray(0);


    // Petla glowna
    while (!glfwWindowShouldClose(window))
    {
        // input
        mappingInput(window);

        // czyszczenie okna kolorem
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Ustawienie aktywnego shaderu do rysowania
        shader.activate();

        //Pobieranie czasu
        //float timer = glfwGetTime();
        //Zmiana koloru 
        //float greenColor = (sin(timer) / 2.0f) + 0.5f;
        //Pobranie lokalizacji uniformu
        //int colorUniform = glGetUniformLocation(shaderProgram, "color");
        //Przesylanie wartosci do uniformu
        //glUniform4f(colorUniform, 0.0f, greenColor, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        //Rysowanie 
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // Podwójne buforowanie 
        glfwSwapBuffers(window);
        //Funkcja do sprawdzenia czy zostalo wykonane jakies wydarzenie i aktualizowania stanu okna
        glfwPollEvents();
    }
    // Funkcje do zwalniania uzytych zasobow 
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // Funkcja do usuwania i czyszczenia zasobów 
    glfwTerminate();
    return 0;
}


void mappingInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void scaleViewport(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}