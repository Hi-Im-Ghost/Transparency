#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <map>

#include <ShaderProgram.h>
#include <Texture.h>
#include <Camera.h>
#include <Object.h>


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

//Pozycja slonca
glm::vec3 lightPos(-20.0f, 18.0f, -15.0f);

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
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //SHADERS
    ShaderProgram LightingObject("Shaders/vertex.vert", "Shaders/fragment.frag", Shader::SourceType::FILE, Shader::SourceType::FILE);
    ShaderProgram SunObject("Shaders/light.vert", "Shaders/light.frag", Shader::SourceType::FILE, Shader::SourceType::FILE);

    //Inicjalizacja i wczytanie modeli
    Object house("Models/house.obj");
    Object tree("Models/tree.obj");
    Object floor("Models/plane.obj");
    Object window_object("Models/plane.obj");
    Object sun("Models/circle.obj");

    //TEXTURES
    Texture texture1("Textures/brick3.png");
    Texture texture2("Textures/floor.png");
    Texture texture3("Textures/window.png");
    Texture texture4("Textures/tree.png");
    Texture texture5("Textures/house2.png");
    Texture texture6("Textures/grassflat.png"); 
    //Pozycja dla okien
    std::vector<glm::vec3> windows
    {

        glm::vec3(-24.5f, 34.0f, -12.0f),
        glm::vec3(-55.0f, 34.0f, -12.0f),
        glm::vec3(-39.5f, 50.0f, -12.0f),
        glm::vec3(-34.5f, 17.5f, -12.0f)
    };

    std::vector<glm::vec3> trees
    {

        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(-5.0f, 0.0f, -6.0f),
        glm::vec3(6.5f, 0.0f, 6.0f)
    };
    window_object.scale(glm::vec3(0.15f));
    window_object.rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    //Aktywacja shaderow
    LightingObject.useShaderProgram();

    //Przeslanie tekstur do shaderow jako uniform
    glUniform1i(glGetUniformLocation(LightingObject.getShaderProgram(), "texture1"), 0);

    //mapa do przechowywania okien w odpowiedniej kolejnoœci
    std::map<float, glm::vec3> sorted;
    
    //Transformacje dla obiektach statycznych
    house.translate(glm::vec3(-5.0f, 0.0f, 5.0));
    floor.scale(glm::vec3(2.0f));

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

        //Aktywacja tekstur
        texture4.useTexture();
        // sortowanie okien przed renderowaniem tak aby by³y renderowane od najdalszych do najbli¿szych 
        for (unsigned int i = 0; i < trees.size(); i++)
        {
            tree.setModelMatrix(glm::mat4(1.0f));
            tree.translate(trees[i]);
            //Ustawienie uniformow i shadera
            tree.setShaderUniforms(LightingObject.getShaderProgram(), camera.getViewMatrix(), camera.getProjectionMatrix(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), lightPos, camera.getCameraPosition());
            tree.draw();
        }
        


        //Ustawienie uniformow i shadera
        house.setShaderUniforms(LightingObject.getShaderProgram(), camera.getViewMatrix(), camera.getProjectionMatrix(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), lightPos, camera.getCameraPosition());
        texture5.bindTexture();
        house.draw();

        //Ustawienie uniformow i shadera
        floor.setShaderUniforms(LightingObject.getShaderProgram(), camera.getViewMatrix(), camera.getProjectionMatrix(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), lightPos, camera.getCameraPosition());
        texture6.bindTexture();
        floor.draw();


        //Dla slonca
        sun.setModelMatrix(glm::mat4(1.0f));
        sun.scale(glm::vec3(0.5f));
        //Zmiana pozycji swiatla w czasie
        //lightPos.x = 3.0f + sin(glfwGetTime()) * 5.0f;
        //lightPos.y = 5.0f + sin(glfwGetTime() / 1.0f) * 5.0f;
        sun.translate(lightPos);
        sun.setShaderUniforms(SunObject.getShaderProgram(), camera.getViewMatrix(), camera.getProjectionMatrix(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), lightPos, camera.getCameraPosition());
        sun.draw();
                
        sorted.clear();
        // sortowanie okien przed renderowaniem tak aby by³y renderowane od najdalszych do najbli¿szych 
        for (unsigned int i = 0; i < windows.size(); i++)
        {
            // obliczamy odleg³oœæ od kamery poprzez obliczenie d³ugoœci wektora miêdzy pozycj¹ kamery a okna
            float distance = glm::length(camera.getCameraPosition() - windows[i]);
            // przypisanie tej odleg³oœci jako klucz do mapy a jako wartoœæ pozycje okna
            sorted[distance] = windows[i];
        }

        texture3.bindTexture();
        // iteracja przez posortowana mapê, która przechowuje odleg³oœæ od kamery do okien i renderowanie w odwrotnej kolejnosci  zaczyna od najdalszych i konczy na najblizszych
        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
        {
            window_object.setModelMatrix(glm::mat4(1.0f));
            window_object.scale(glm::vec3(0.15f));
            window_object.rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            // ustawiamy macierz dla obiektu przesuwaj¹c go do po³o¿enia w przestrzeni œwiata
            window_object.translate(it->second);
            // Sprawdzamy, czy to jest okno, które chcemy obróciæ
            if (it->second == windows[0] || it->second == windows[1]) {
                // Obrót okna o 90 stopni wokó³ osi Y
                window_object.rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            }
            // przesy³anie macierzy model do shadera
            window_object.setShaderUniforms(LightingObject.getShaderProgram(), camera.getViewMatrix(), camera.getProjectionMatrix(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), lightPos, camera.getCameraPosition());
            window_object.draw();
        }
        

        //Podwójne buforowanie 
        glfwSwapBuffers(window);
        //Funkcja do sprawdzenia czy zostalo wykonane jakies wydarzenie i aktualizowania stanu okna
        glfwPollEvents();
    }

    //Funkcje do zwalniania uzytych zasobow 
    glDeleteVertexArrays(1, &house.VAO);
    glDeleteVertexArrays(1, &tree.VAO);
    glDeleteVertexArrays(1, &floor.VAO);
    glDeleteVertexArrays(1, &window_object.VAO);
    glDeleteVertexArrays(1, &sun.VAO);
    glDeleteBuffers(1, &house.VBO);
    glDeleteBuffers(1, &house.VBO);
    glDeleteBuffers(1, &tree.VBO);
    glDeleteBuffers(1, &floor.VBO);
    glDeleteBuffers(1, &window_object.VBO);
    glDeleteBuffers(1, &sun.VBO);
    glDeleteBuffers(1, &house.EBO);
    glDeleteBuffers(1, &house.EBO);
    glDeleteBuffers(1, &tree.EBO);
    glDeleteBuffers(1, &floor.EBO);
    glDeleteBuffers(1, &window_object.EBO);
    glDeleteBuffers(1, &sun.EBO);
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