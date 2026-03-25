#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::string loadShaderSource(const char* path);

// settings
const unsigned int width = 600;
const unsigned int height = 800;

// const char* vertexShaderSource = "#version 330 core\n"
//     "layout (location = 0) in vec3 aPos;\n"
//     "void main()"
//     "{\n"
//     "   gl_Position = vec4(aPos.x, aPos. y, aPos.z, 1.0);\n"
//     "}\0";

// const char* fragmentShaderSource = "#version 330 core\n"
//     "out vec4 FragColor;\n"
//     "void main()"
//     "{\n"
//     "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//     "}\n";

std::string vertCode = loadShaderSource("shaders/shader.vert");
std::string fragCode = loadShaderSource("Shaders/shader.frag");

const char* vertexShaderSource = vertCode.c_str();
const char* fragmentShaderSource = fragCode.c_str();

int main(){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(height, width, "physics-sim", NULL, NULL);
    if(!window){
        std::cout << "Failed to create window" << '\n';
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to init GLAD" << '\n';
        return -1;
    }

    // build and compile shader program

    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errirs
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
    }

    //fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
    }

    //link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking error
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data and buffers and config vertex attrs

    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[] = {
        0,1,3,  //first triangle
        1,2,3   //second traingle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind VAO first, then bind and set VBO(s) and then config vertex attribute(s)
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);

    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while(!glfwWindowShouldClose(window)){
        // input
        processInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw swapp buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // deallocate resources:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // glm test
    // glm::vec2 testVec = {1.0f, 2.0f};
    // std::cout << testVec.x << ' ' << testVec.y << '\n';
    
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width, height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

std::string loadShaderSource(const char* path){
    std::string content;
    std::ifstream filestream(path, std::ios::in);

    if(!filestream.is_open()){
        std::cerr << "Could not read file. " << path << " does not exist." << '\n';
        return "";
    }

    std::stringstream sstr;
    sstr << filestream.rdbuf();
    content = sstr.str();
    filestream.close();
    return content;
}