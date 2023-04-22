#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// Funkcja do skalowania okna do renderowania (uruchamia sie w momencie zmiany rozmiaru okna)
void scaleViewport(GLFWwindow* window, int width, int height);
// Funkcja do obs�ugu inputu 
void mappingInput(GLFWwindow* window);

// szerokosc okna
const unsigned int width = 800;
// wysokosc okna
const unsigned int height = 600;

// wierzcholki
float vertices[] = {
-0.5, -0.5, 0.0,
0.5, -0.5, 0.0,
0.0, 0.5, 0.0
};

const char *vertexShaderCode = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderCode = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.5f, 1.0f);\n"
"}\0";

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

    //Shadery
    //Vertex
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Dolaczenie kodu shadera do obiektu
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);
    //Fragment
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //Dolaczenie kodu shadera do obiektu
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);

    //Obiekt programu shaderow
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    //Laczenie shaderow
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //Konsola dla shaderow
    //zmienna informujaca o powodzeniu kompilacji 
    int success;
    //zmienna do przechowywania komunikatow
    char infoLog[512];
    //Funkcja sprawdzajaca czy kompilacja sie powiodla
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        //funkcja do pobrania komunikatu o bledzie
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //Usuwanie obiektow shaderow
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    //VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    //Funkcja do wiazania obiektu tablicy wierzcholkow
    glBindVertexArray(VAO);

    //VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    //Funkcja do wiazania bufora z typem 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Funkcja do przydzielenia pamieci dla bufora
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //Funkcja do okreslenia jak opengl ma interpretowac dane wierzcholkow
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Rozlaczenie buffora bo zostal zarejestrowany juz 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //Odlaczenie VAO
    glBindVertexArray(0);




    // Petla glowna
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        mappingInput(window);

        // czyszczenie okna kolorem
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Ustawienie aktywnego shaderu do rysowania
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        //Rysowanie 
        glDrawArrays(GL_TRIANGLES, 0, 3);



        // Podw�jne buforowanie 
        glfwSwapBuffers(window);
        //Funkcja do sprawdzenia czy zostalo wykonane jakies wydarzenie i aktualizowania stanu okna
        glfwPollEvents();
    }
    // Funkcje do zwalniania uzytych zasobow 
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    // Funkcja do usuwania i czyszczenia zasob�w 
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