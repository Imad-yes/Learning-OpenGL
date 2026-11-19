#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>

typedef int ERROR_IMAD; //Trouver un nom de type


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width, height);
}

void process_input(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

GLFWwindow* init_gl_window(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    GLFWwindow* window = glfwCreateWindow(600, 600, "hello triangle", NULL, NULL);

    if (window == NULL){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return NULL;
    }

    return window;
}


/// @brief Checks the compile status of shader shader and prints error message if a failure occured
/// @param shader 
/// @return the succes state, 1 if compiling was succesfful, else 0
int check_shader_compilestat(unsigned int shader){
    int successState;
    char infolog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &successState);
    if (!successState){
        glGetShaderInfoLog(shader, 512, NULL, infolog);
        std::cout << "Failed to compile shader" << infolog <<std::endl;
    }
    return successState;
}

/// @brief Declares shaders and compiles them then checks for compile errors. 
///        Returns 1 if an error occured, else 0. Calls check_shader_compilestat
///        which prints any shader compiling error message.
/// @param result_shprogram The variable where the shader program should be stored
/// @return an error type
ERROR_IMAD init_shaders(unsigned int* result_shprogram){
    const char* vertShaderSource = 
    "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main(){\n"
        "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    const char* fragShaderSource = 
    "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertShaderSource, NULL);
    glCompileShader(vertexShader);

    if (!check_shader_compilestat(vertexShader)){
        return 1;
    }


    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragShader);

    if (!check_shader_compilestat(fragShader)){
        return 1;
    }

    unsigned int shprogram = glCreateProgram();
    glAttachShader(shprogram, vertexShader);
    glAttachShader(shprogram, fragShader);
    glLinkProgram(shprogram);

    *result_shprogram = shprogram;

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    return 0;
}



/// @brief Checks the linking status of shprogram
/// @param shprogram a shader program
/// @return the linking success state, 1 if the linking was successulf, else 0
int check_link_status(unsigned int shprogram){
    int successState;
    char infolog[512];

    glGetProgramiv(shprogram, GL_LINK_STATUS, &successState);
    if (!successState){
        glGetProgramInfoLog(shprogram, 512, NULL, infolog);
        std::cout<<"linking error"<<infolog<<std::endl;
    }
    return successState;

}

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};


int main(){
    
    /// Create and init window 
    GLFWwindow* window = init_gl_window();

    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 1;
    }

    glViewport(0,0, 600, 600); 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    /// Compile shaders, create shader program, link program
    unsigned int shprogram;
    if (init_shaders(&shprogram) != 0){
        std::cout << "shader initialisation wasn't successful" << std::endl;
        return 1;
    }

    if (!check_link_status(shprogram)){
        std::cout << "shader program linking wasn't successful" << std::endl;
        return 1;
    }

    /// Créer le buffer qu'on va utiliser comme data store pour les vertices
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /// Créer le VAO qui stock les appelles et la configuration vertex attrib
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window)){
        //First check for inputs
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shprogram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //Then check the event queue and call associated callbacks, then swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shprogram);

    glfwTerminate();
    return 0;
}
