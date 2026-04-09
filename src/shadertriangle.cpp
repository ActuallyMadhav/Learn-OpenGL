#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string.h>

// window dimensions + settings
GLuint width = 800, height = 600;
GLuint vao, vbo, shader;

// create window
GLFWwindow* createWindow();

// vertex shader
static const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"void main(){\n"
"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);\n"
"}\0";

// fragment shader
static const char* fragmentShaderSource = 
"#version 330 core\n"
"out vec4 colour;\n"
"void main(){\n"
"   colour  = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\0";

void createTriangle();
void addShader(GLuint program, const char* shaderCode, GLenum shaderType);
void compileShaders();

int main(){
    // window init
    GLFWwindow* window = createWindow();

    createTriangle();
    compileShaders();

    // rendering loop
    while(!glfwWindowShouldClose(window)){

        // get + handle user input
        glfwPollEvents();

        // clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

GLFWwindow* createWindow(){
    // glfw init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create window object
    GLFWwindow* window = glfwCreateWindow(width, height, "shader triangle", NULL, NULL);
    if(!window){
        std::cout << "Failed to create GLFW window" << '\n';
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    // glad init
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << '\n';
    }

    return window;
}

void createTriangle(){
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    //create and bind vertex array
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // create and bind vertex buffer
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void addShader(GLuint program, const char* shaderCode, GLenum shaderType){

    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[512] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result){
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        std::cout << "Error compiling " << shaderType << "shader: " << eLog << '\n';
        return;
    }

    glAttachShader(program, theShader);
}

void compileShaders(){
    shader = glCreateProgram();

    if(!shader){
        std::cout << "Error creating shader program!\n";
        return;
    }

    addShader(shader, vertexShaderSource, GL_VERTEX_SHADER);
    addShader(shader, fragmentShaderSource, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[512] = {0};

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cout << "Error linking program: " << eLog << '\n';
        return;
    }

    // this causes an error because createTriangle() unbinds the VAO at the end, so validation fails

    // glValidateProgram(shader);
    // glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    // if(!result){
    //     glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    //     std::cout << "Error linking program: " << eLog << '\n';
    //     return;
    // }
}
