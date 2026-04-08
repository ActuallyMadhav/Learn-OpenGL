#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow* createWindow();

int main(){
    GLFWwindow* window = createWindow();
    if(!window) return -1;

    // rendering loop
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

GLFWwindow* createWindow(){
        // glfw initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // window creation
    GLFWwindow* window = glfwCreateWindow(800, 600, "test window", NULL, NULL);
    glfwMakeContextCurrent(window);

    // GLAD initialzation
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to init GLAD" << '\n';
    }

    return window;
}