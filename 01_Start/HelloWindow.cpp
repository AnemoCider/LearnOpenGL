#include <glad/glad.h>
#include <01.h>
#include <01_Exercises.h>

int draw_rectangle() {
    GLFWwindow* window;
    unsigned int shaderProgram;
    draw_init(window, shaderProgram);

     // Vertices for a triangle
     float vertices[] = {
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top left 
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
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
     glEnableVertexAttribArray(0);
     glEnableVertexAttribArray(1);
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
    glDeleteBuffers(1, &EBO);
    draw_terminate(shaderProgram);
    return 0;
}

int main() {
    return draw_rectangle();
}