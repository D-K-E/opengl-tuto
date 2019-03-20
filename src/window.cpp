// code of learn opengl
/*
 1*: line written at first pass
 2*: line written at second pass

 1*start ..code.. 1*end: lines written at first pass
 2*start ..code.. 2*end: lines written at second pass
 */
// 1*start
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

// 1*end

// 4*: declare the function here in order to 
// use it as a callback
// It's definition comes after the main function
void framebuffer_size_callback(GLFWwindow* window, 
        int width, int height);

void processInput(GLFWwindow* window);

void renderOnWindow(GLFWwindow* window);

// we start with writting a main function that loads the glfw 
int main() { // 1*
    glfwInit(); // 1*: loads the glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // 1*: specifies the
    // opengl major version we want to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // 1*: specifies the 
    // opengl minor version we want to use
    // These two lines state that we would like to use opengl 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 1*
    // We tell glfw to load the core profile of opengl

    // Now we need to create a window before we can actually show something
    // 2*start
    GLFWwindow* window = glfwCreateWindow(800, // width of the window
            600, // height of the window
            "My Window", // title of the window
            NULL,
            NULL);
    if (window == NULL) {
        std::cout << "Failed to create the window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // makes the window part of current
    // context. This means that the window would receive commands from
    // opengl, a context can have multiple windows, and a single applicationn
    // can have multiple contexts
    // 4*start
    // 4*
    glfwSetFramebufferSizeCallback(window,
            framebuffer_size_callback);

    // 4*end


    // 2*end
    // 3*start
    if (!gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress) ) {
        std::cout << "Glad failed to initialized" << std::endl;
        return -1;
    }
    // 3*end
    // 4*
    glViewport(0,  // x coord of the viewport
            0, // y coord of the viewport
            800, // width of the viewport
            600); // height of the viewport
    // the point is that the window and the viewport can have different
    // sizes. It is the viewport that opengl would use as a canvas
    // to which it will map the scene coordinates

    
    //5*: now let's write the main loop
    while (!glfwWindowShouldClose(window)){
        processInput(window); // 6* let's listen for inputs

        // 7*: rendering commands can start now
        renderOnWindow(window);

        glfwSwapBuffers(window); // 5* in accordance with the double buffer law
        // the idea is to have 2 buffers, one for computing scene,
        // the other for displaying the scene
        glfwPollEvents(); // 5* accomodates the events
    }

    // 1*
    glfwTerminate();
    return 0; // 1*: if everything went successfull we quit the application
}

// 4*
void framebuffer_size_callback(GLFWwindow* window,
        int width, int height) {
    // basically resize the viewport when the window is resized
    glViewport(0, 0, width, height);
}

// 6*: process some input on the window
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        // now the window should close when we press the escape key
        glfwSetWindowShouldClose(window, true);
    }
}

// 7*
void renderOnWindow(GLFWwindow* window) {
    // let's render stuff on window
    // for example let's set a clear color on window
    // change clear color to see different colors on the window
    glClearColor(1.0f, 0.3f, 0.3f, 1.0f);  // state setter function
    glClear(GL_COLOR_BUFFER_BIT);  // state user function
}
