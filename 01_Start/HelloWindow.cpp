#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    //std::cout << "viewport size modified.\n";
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Vertex Shader code: vertex (vec3) -> out vertex (vec4)
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Frag Shader code: modify the &input
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor; \n"
"void main() { \n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
"} \n";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // When proper version of opengl is not found, set version to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile only

    // Instantiate glfw window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set shaders and shader program
    //  Vertex Shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Deal with failure
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Frag Shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link the shaders into a shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set rendering window position & size
    // should be smaller than glfw window
    glViewport(0, 0, 800, 600);
    // Registr callback function for adjusting window size
     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

     // Vertices for a triangle
     float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
     };
     unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
     };
     // Prepare data for vertex shader
     unsigned int VAO, VBO, EBO; // ID for a vertex array obj, vertex buffer object and element buf obj
     glGenVertexArrays(1, &VAO);
     glGenBuffers(1, &VBO); // return size(params[0]) buffer object names
     glGenBuffers(1, &EBO);
     glBindVertexArray(VAO);
     glBindBuffer(GL_ARRAY_BUFFER, VBO); // binds buffer object name (params[1]) to the target (params[0])
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
     // Copy vertices to buffer's memory
     // Static draw: the data is set only once, but used many times
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

     // tell OpenGL how to treat the input to vertex buffer, i.e., position data
     // param[0]: the location of the aPos vertex attribute
     // param[3]: whether or not to normalize
     // param[-1]: offset of where the position data begins. should be in void* type.
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
     glEnableVertexAttribArray(0);
     // Unbind 
     glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input

        processInput(window);


        // rendering commands

        // clear current window
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // set color buffer bit
        glClear(GL_COLOR_BUFFER_BIT); // set the whole window to color buffer bit

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // automatically bind the last bound EBO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // check events and swap buffer
        
        // checks if any events are triggered, updates window state, 
        // and calls corresponding func, e.g., callback
        glfwPollEvents();
        // Double buffer: render happens on the back buffer
        // Swap the back buffer to front when the render process is completed
        glfwSwapBuffers(window);
        
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}