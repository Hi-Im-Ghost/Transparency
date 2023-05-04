#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

#include <ShaderProgram.h>
#include <Texture.h>
#include <Camera.h>

// Funkcja do skalowania okna do renderowania (uruchamia sie w momencie zmiany rozmiaru okna)
void scaleViewport(GLFWwindow* window, int width, int height);
//Funkcja do przechtywytwania pozycji myszy
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//Funkcja do przechwytywania kolka myszy 
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// Funkcja do obs³ugu inputu 
void mappingInput(GLFWwindow* window);


// szerokosc okna
const unsigned int width = 800;
// wysokosc okna
const unsigned int height = 600;
// czas
float deltaTime = 0.0f;	
float lastFrame = 0.0f;
//Kamera
Camera camera(width, height);
bool firstMouse = true;
float lastX = width / 2.0f;
float lastY = height / 2.0f;

//Obiekty
float square[] = {
    // positions          // colors           // texture
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};
unsigned int indices_square[] = {
    0, 1, 3, 
    1, 2, 3 
};

float cube[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
};
//cube with tex
float cubeTex[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
//kostka z normalnymi
float cubeNormals[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
    //Ustawienie funkcji do wywolania w momencie zmiany pozycji myszy
    glfwSetCursorPosCallback(window, mouse_callback);
    //Ustawienie funkcji do wywolania w momencie uzycia kolka myszy
    glfwSetScrollCallback(window, scroll_callback);
    //Wlacz pobieranie pozycji myszy i wylacz widocznosc kursora
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // glad inicjalizacja 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //Wlaczenie bufora glebokosci
    glEnable(GL_DEPTH_TEST);

    //SHADERS
    ShaderProgram LightingObject("Shaders/vertex.vert", "Shaders/fragment.frag", Shader::SourceType::FILE, Shader::SourceType::FILE);
    ShaderProgram SunObject("Shaders/light.vert", "Shaders/light.frag", Shader::SourceType::FILE, Shader::SourceType::FILE);
    //VAO
    GLuint VAO, sunVAO;
    glGenVertexArrays(1, &VAO);
    //Funkcja do wiazania obiektu tablicy wierzcholkow
    glBindVertexArray(VAO);

    //VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    //Funkcja do wiazania bufora z typem 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Funkcja do przydzielenia pamieci dla bufora
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormals), cubeNormals, GL_STATIC_DRAW);
    /*
    //EBO
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    */
    //Funkcja do okreslenia jak opengl ma interpretowac dane wierzcholkow
    //pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    /*//color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);*/
    //tex
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
   
    //SUN
    glGenVertexArrays(1, &sunVAO);
    //Funkcja do wiazania obiektu tablicy wierzcholkow
    glBindVertexArray(sunVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Funkcja do okreslenia jak opengl ma interpretowac dane wierzcholkow
    //pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //TEXTURES
    //Texture texture1("Textures/brick3.png");
    //Texture texture2("Textures/face.png");

    //Aktywacja shaderow
    //shaderProgram.useShaderProgram(); 
    //Przeslanie tekstur do shaderow jako uniform
    //glUniform1i(glGetUniformLocation(shaderProgram.getShaderProgram(), "texture1"), 0);
    //glUniform1i(glGetUniformLocation(shaderProgram.getShaderProgram(), "texture2"), 1);

    //Petla glowna
    while (!glfwWindowShouldClose(window))
    {
        //Pobieranie czasu ostatniej klatki
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //input
        mappingInput(window);

        //czyszczenie okna kolorem
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Zmiana pozycji swiatla w czasie
        lightPos.x = 1.0f + sin(glfwGetTime()) * 5.0f;
        lightPos.y = sin(glfwGetTime() / 2.0f) * 3.0f;
        //Aktywacja tekstur
        //texture1.useTexture();
        //texture2.useTexture(1);

        //Ustawienie aktywnego shaderu do rysowania
        LightingObject.useShaderProgram();

        //Przeslanie koloru do shadera
        glUniform3f(glGetUniformLocation(LightingObject.getShaderProgram(), "objectColor"), 1, GL_FALSE, (1.0f, 0.5f, 0.31f));
        glUniform3f(glGetUniformLocation(LightingObject.getShaderProgram(), "lightColor"), 1, GL_FALSE, (1.0f, 1.0f, 1.0f));
        //Pozycja swiatla
        glUniform3fv(glGetUniformLocation(LightingObject.getShaderProgram(), "lightPos"), 1, glm::value_ptr(lightPos));
        //Camera pos
        glUniform3fv(glGetUniformLocation(LightingObject.getShaderProgram(), "viewPos"), 1, glm::value_ptr(camera.getCameraPosition()));

        //Macierz projekcji
        glm::mat4 proj = camera.getProjectionMatrix();
        glUniformMatrix4fv(glGetUniformLocation(LightingObject.getShaderProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(proj));
        //Camera transform
        glm::mat4 view = camera.getViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(LightingObject.getShaderProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));

        //Rysowanie 
        
        for (unsigned int i = 1; i < 11; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i - 1]);
            //Obrot  wokol osi Z
            //model = glm::rotate(model, (float)glfwGetTime() * i, glm::vec3(1.0f, 0.3f, 0.5f));

            glUniformMatrix4fv(glGetUniformLocation(LightingObject.getShaderProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        
        //Dla slonca
        SunObject.useShaderProgram();
        //Macierz projekcji
        glUniformMatrix4fv(glGetUniformLocation(SunObject.getShaderProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(proj));
        //Camera transform
        glUniformMatrix4fv(glGetUniformLocation(SunObject.getShaderProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.3f));
        glUniformMatrix4fv(glGetUniformLocation(SunObject.getShaderProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        
        glBindVertexArray(sunVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        //Podwójne buforowanie 
        glfwSwapBuffers(window);
        //Funkcja do sprawdzenia czy zostalo wykonane jakies wydarzenie i aktualizowania stanu okna
        glfwPollEvents();
    }
    //Funkcje do zwalniania uzytych zasobow 
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &sunVAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    //Funkcja do usuwania i czyszczenia zasobów 
    glfwTerminate();
    return 0;
}


void mappingInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    //Obsluga poruszania kamery za pomoca klawiatury 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.setKeyboardMovement(Camera::Direction::Forward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.setKeyboardMovement(Camera::Direction::Backward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.setKeyboardMovement(Camera::Direction::Left, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.setKeyboardMovement(Camera::Direction::Right, deltaTime);
}

void scaleViewport(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    //Oblicz ruch myszy wzglêdem poprzedniej pozycji
    float xpos = static_cast<float>(xPos);
    float ypos = static_cast<float>(yPos);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 

    lastX = xpos;
    lastY = ypos;
    //Ustaw kierunek ruchu myszy w kamerze
    camera.setMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.setMouseScroll(static_cast<float>(yoffset));
}