#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>

#include "camera.h"
#include "scene.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

//void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Constants
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 800;

// Camera
Camera camera(glm::vec3(0.0f, 2.0f, 25.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;

bool firstMouse = true;

// Timings
float deltaTime = 0.0f;	// time between the current frame and the next frame
float lastFrame = 0.0f;

int main() {

    // glfw: initise and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Share the code when using macOS
    /*
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    */

    // glfw: window creation
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "--Uzunig--", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed  to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // tell GLFW to captur our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: oad all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // ������ ������� ��������� ������� ���� "������ ��� �����" ��� ����� �� ����������
    
    Scene scene(SCREEN_WIDTH, SCREEN_HEIGHT, camera);
    scene.Init(window);


    while (!glfwWindowShouldClose(window)) {
        //������ �� �������� ��� ������� �����
        scene.currentTime = glfwGetTime();
        deltaTime = scene.currentTime - lastFrame;
        lastFrame = scene.currentTime;

        // ��������� �����
        scene.ProcessInput(window, deltaTime);

        // ���������
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if (lastFrame > 30.0f) {
            scene.Update(deltaTime);
        }
        scene.Render();
        
        // glfw: ����� ���������� front- � back-�������. ������������ ������� �����/������ (���� �� ������/�������� ������, ��������� ������ ���� � �.�.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: ����������, ������������ ���� ����� ��������������� GLFW-��������
    glfwTerminate();
    return 0;
}

// glfw: ������ ���, ����� ���������� ������� ���� (������������� ��� ������������ ��������), ���������� ������ callback-�������
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    
    if (!window) {};// Warnings
    // ����������, ��� ���� ��������� ������������� ����� �������� ����.
    // �������� ��������, ������ ���� �� Retina-�������� ����� ����������� ������, ��� ������� � ���������    
    glViewport(0, 0, width, height);
}

// glfw: ������ ���, ����� ������������ ����, ���������� ������ callback-�������
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    
    if (!window) {}; //Warnings
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;  //������� �������� ���� �� ������ �� ���������� ������ �������  mouse_callback �� ���������� 
    float yoffset = lastY - ypos; // �����������, ��� ��� y-���������� ��� ���� ���� ������ ���� � ��� OpenGL ����� �����

    lastX = xpos;     //�������������� ��� ���������� ������
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
