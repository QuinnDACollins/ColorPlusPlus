#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <Note.h>
#include <glad/glad.h> 
#include <KHR/khrplatform.h>
#include <GLFW/glfw3.h>
using namespace std;

const char *vertexShaderSrc = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main(){\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragShaderSrc =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"   FragColor = vec4(0.1f, 0.5f, 0.3f, 1.0f);\n"
"}\n\0";
Note generate_scale(string note, string quality){
    string allNotes[12] {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    int noteIdx;
    for(int i = 0; i < 12; i++){
        if(allNotes[i] == note){
            noteIdx = i;
        }
    }
    cout << noteIdx;

    int seq[6] {2, 4, 5, 7, 9, 11};

    string scale[7]{};
    scale[0] = note;
    for(int x = 0; x < 6; x++){
        int allNotesIdx = (noteIdx + seq[x]) % 12;
        cout << allNotesIdx;
        scale[x + 1] = allNotes[allNotesIdx];
        if (scale[x][0] == scale[x + 1][0]){
            if (scale[x + 1][1] == '#' || scale[x + 1][1] == 'b'){
                scale[x + 1] = allNotes[allNotesIdx + 1] + "b";
            }
        }
    }
    for (int i = 0; i < 7; i++)
    {cout << scale[i];}
    Note retNote(note, scale);
    return retNote;
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h){
    glViewport(0, 0, w, h);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);

    }
}
int main()
{
  
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "TEST!", NULL, NULL);
    if(window == NULL){
        cout << "Failiure";
        glfwTerminate();
        return -1;
    }
 
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
    }   

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };


    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);

    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3,GL_FLOAT, GL_FALSE,3* sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);







    while(!glfwWindowShouldClose(window)){

    processInput(window);

    glClearColor(0.2f, 0.6f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwPollEvents();    
    glfwSwapBuffers(window);
    
    }
    glfwTerminate();
    return 0;

    /*Note note = generate_scale("F", "major");
    return 0;*/
    
}
