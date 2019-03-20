// hello triangle from learn opengl
/*
 for the legend concerning 1* etc see window.cpp
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
//#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h> // input output for shaders
#include <stdlib.h> // input output for shaders



boost::filesystem::path current_dir(boost::filesystem::current_path()); // bin
boost::filesystem::path mediaDir ("media");
boost::filesystem::path shaderDir ("shaders");
boost::filesystem::path fullShaderDir = current_dir / mediaDir / shaderDir;

const unsigned int WWIDTH = 800;
const unsigned int WHEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, 
        int width, int height);

void processInput(GLFWwindow* window);

void renderTrianglesOnWindow(GLFWwindow* window);
void checkShaderProgramCompilation(unsigned int program);
void checkShaderCompilation(unsigned int shader, std::string shaderType);
void readFileFromText(char * arr, FILE* inFile, size_t fileSize);

void triangleInit(unsigned int program, unsigned int VAO,
        unsigned int VBO);

void startGlfwMajorMinor(unsigned int majorVersion,
        unsigned int minorVersion) {
    // init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow * startGlfwWindowCoreProfile(std::string title, unsigned int majorVersion,
        unsigned int minorVersion, unsigned int windowWidth,
        unsigned int windowHeight, unsigned int viewportWidth,
        unsigned int viewportHeight) {
    // initialize the window with given major minor version,
    // with window height width, viewport width height
    GLFWwindow *window = glfwCreateWindow(windowWidth,
            windowHeight, title.c_str(), NULL, NULL);

    return window;
}

int main() {
    startGlfwMajorMinor(4, 2);
    // let's get that window going
    GLFWwindow* window = glfwCreateWindow(WWIDTH, WHEIGHT,
            "My Triangle", NULL, NULL);
    // sanity check
    if (window == NULL) {
        std::cout << "Failed to create the glfw window" << std::endl;
        // bye
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // on resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress) ) {
        // sanity check
        std::cout << "Failed to start glad" << std::endl;
    }

    glViewport(0, 0, WWIDTH, WHEIGHT);

   
    /* Start: Drawing Related Code Begins */
    GLuint shaderProgram, VBO, VAO;
    //triangleInit(shaderProgram, VAO, VBO);
    // initialization code
    // we need shaders first, they define the behaviour of our vertices
    // let's start with vertex shader
    GLuint vertexShader;
    boost::filesystem::path vshaderfile ("hello_triangle.vert");
    boost::filesystem::path vshaderf = fullShaderDir / vshaderfile;
    boost::filesystem::ifstream ifs(vshaderf);
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
            (std::istreambuf_iterator<char>()) );
    const char *vshader = content.c_str();
    //const char vshader[content.size()] = content.c_str();
            
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // lets source the shader
    glShaderSource(vertexShader, 1, &vshader, NULL);
    glCompileShader(vertexShader);
    checkShaderCompilation(vertexShader, "VERTEX");

    // let's see if we can manage a fragment shader
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // read the shader file
    boost::filesystem::path fragshaderfile ("hello_triangle.frag");
    boost::filesystem::path fragshaderf = fullShaderDir / fragshaderfile;
    boost::filesystem::ifstream iifs(fragshaderf);
    std::string content2( (std::istreambuf_iterator<char>(iifs) ),
            (std::istreambuf_iterator<char>()) );

    const char *fshader = content2.c_str();

    //const char fshader[content2.size()] = content2.c_str();

    // let's source the shader
    glShaderSource(fragmentShader, 1, &fshader, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompilation(fragmentShader, "FRAGMENT");

    // Now that the shaders are ready we need a program to use those shaders
    shaderProgram = glCreateProgram();
    // let's attach shaders to program bar
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // again sanity check for seeing the link was successful
    checkShaderProgramCompilation(shaderProgram);

    // now that the program is linked to opengl we don't need the
    // shader objects anymore, since they are already linked to opengl

    // so let's delete them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // let's start with defining the vertices of the 
    // triangle
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // x1, y1, z1
        0.5f, -0.5f, 0.0f,  // x2, y2, z2
        0.0f, 0.5f, 0.0f,  // x3, y3, z3
    };
    // three corners three vertices.
    // notice z is 0, so no depth, so triangle is 2d

    // this is all good

    // we need VAO, vertex array object and a VBO vertex buffer object
    // first one contains vertices, the second one holds them in memory
    // it is good practice to think of them together when you are dealing
    // with this stage
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 1 bind vertex array
    glBindVertexArray(VAO);

    // 2* Bind and set the buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  //
    // this binds the vertex buffer to opengl's Array_buffer
    // meaning that anything that targets GL_ARRAY_BUFFER
    // from this point on will be based on the data of VBO

    glBufferData(GL_ARRAY_BUFFER, 
            sizeof(vertices), // vertices size gives a size to buffer
            vertices,  // the data to be bound
            GL_STATIC_DRAW);  // type of memory that should be attributed
    // to the buffer data. GL_STATIC_DRAW means we don't expect a lot of
    // changes in the data.
    /*
    - GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
    - GL_DYNAMIC_DRAW: the data is likely to change a lot.
    - GL_STREAM_DRAW: the data will change every time it is drawn.
       */


    // now deal with vertex attributes, which link vertices to shaders
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 
            (void*)0);
    glEnableVertexAttribArray(0);

    /* lots of stuff happening here
     first parameter: 0, is the layout position that is specified in the 
     shader with layout ( location = 0 )

     second parameter: 3, the size of the vertex attribute.
     Since we have used a vec3 as the attribute of the vertex.

     Third parameter: GL_FLOAT the type of data inside the vertex attribute.
     Since we have used floats it is GL_FLOAT

     Fourth parameter: Use normalized coordinates for the vertex.
     If true, the coordinates are mapped to 0 - 1, or  -1 - 1 ranges.

     Fifth parameter: Stride: Given an array of vertices, it would be useful
     to know the space between a vertice and the next vertice.
     Since a vertex contains three float values, the next vertice is 
     3 * the size of a floating point away.
     V1 :: 0f, 1f, 2f -> 3 float values
     V2 :: 2f, 4f, 2f -> 3 float values
     So if we are at the beginning of V1, V2 is 3 * size of float,
     since each float has the same size in memory.

     Last parameter: Offset: Where does the data begin in the buffer.
     For our case it starts at the start of the buffer so it is 0.
     (void*) is just weird. I don't know why it exists

     */
    // now that everything is bind and ready to use in opengl

    // we can unbind them to keep things a bit organized
    glBindBuffer(GL_ARRAY_BUFFER, 0); // this unbinds the array buffer
    glBindVertexArray(0); // this unbinds the vertex array

    /* End: Drawing Related Code Begins */

    /* ---------- Main loop ----------------*/ 
    while (!glfwWindowShouldClose(window)) {
        // proces input
        processInput(window);
        // render window
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);

        // the actual drawing code
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // let's also unbind it as well since we are done
        glBindVertexArray(0);
        // swap buffers
        glfwSwapBuffers(window);
        // poll events
        glfwPollEvents();
    }

    // everything is done, let's destroy the objects
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window,
        int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        // user presses escape key and window closes
        glfwSetWindowShouldClose(window, true);

    }
}

void checkShaderCompilation(unsigned int shader, std::string shaderType) {
    // check whether the compilation of a certain shader has been successful
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        //
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << shaderType 
            << "::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }
}
void checkShaderProgramCompilation(unsigned int program) {
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        //
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << "PROGRAM"
            << "::LINK_FAILED\n"
            << infoLog << std::endl;
    }
}


//#include <glad/glad.h>
//#include <GLFW/glfw3.h>

//#include <iostream>

//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow *window);

//// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;

//const char *vertexShaderSource = "#version 330 core\n"
//    "layout (location = 0) in vec3 aPos;\n"
//    "void main()\n"
//    "{\n"
//    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//    "}\0";
//const char *fragmentShaderSource = "#version 330 core\n"
//    "out vec4 FragColor;\n"
//    "void main()\n"
//    "{\n"
//    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//    "}\n\0";

//int main()
//{
//    // glfw: initialize and configure
//    // ------------------------------
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
//#endif

//    // glfw window creation
//    // --------------------
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

//    // glad: load all OpenGL function pointers
//    // ---------------------------------------
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }


//    // build and compile our shader program
//    // ------------------------------------
//    // vertex shader
//    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//    // check for shader compile errors
//    int success;
//    char infoLog[512];
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//    // fragment shader
//    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//    // check for shader compile errors
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//    // link shaders
//    int shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//    // check for linking errors
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//    }
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);

//    // set up vertex data (and buffer(s)) and configure vertex attributes
//    // ------------------------------------------------------------------
//    float vertices[] = {
//        -0.5f, -0.5f, 0.0f, // left  
//         0.5f, -0.5f, 0.0f, // right 
//         0.0f,  0.5f, 0.0f  // top   
//    }; 

//    unsigned int VBO, VAO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//    glBindVertexArray(VAO);

//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);

//    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//    glBindBuffer(GL_ARRAY_BUFFER, 0); 

//    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
//    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//    glBindVertexArray(0); 


//    // uncomment this call to draw in wireframe polygons.
//    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//    // render loop
//    // -----------
//    while (!glfwWindowShouldClose(window))
//    {
//        // input
//        // -----
//        processInput(window);

//        // render
//        // ------
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);

//        // draw our first triangle
//        glUseProgram(shaderProgram);
//        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//        // glBindVertexArray(0); // no need to unbind it every time 
 
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }

//    // optional: de-allocate all resources once they've outlived their purpose:
//    // ------------------------------------------------------------------------
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);

//    // glfw: terminate, clearing all previously allocated GLFW resources.
//    // ------------------------------------------------------------------
//    glfwTerminate();
//    return 0;
//}

//// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow *window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}

//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    // make sure the viewport matches the new window dimensions; note that width and 
//    // height will be significantly larger than specified on retina displays.
//    glViewport(0, 0, width, height);
//}
