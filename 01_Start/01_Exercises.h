#ifndef _01_EXERCISES_H_
#define _01_EXERCISES_H_



#include <glad/glad.h>
#include <01.h>
#include <cmath>


int draw_triangle() {
    GLFWwindow* window;
    unsigned int shaderProgram;
    draw_init(window, shaderProgram);

    // Set vertices
    float vertices[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.0f, -0.5f, 0.0f
    };
    unsigned int vertIndices[] = {
        0, 2, 3,
        0, 1, 3
    };

    // Set VAO, VBO and EBO
    unsigned int VAO, VBO, EBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertIndices), vertIndices, GL_STATIC_DRAW);
    
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    
    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input

        processInput(window);


        // rendering commands

        // clear current window
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // set color buffer bit
        glClear(GL_COLOR_BUFFER_BIT); // set the whole window to color buffer bit

        float timeValue = (float)glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glBindVertexArray(VAO);
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
#endif