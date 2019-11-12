#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <array>
#include <cmath>
#include <utility>
#include <Note.h>
#include <thread>
#include <chrono>
#include <algorithm>
#include <glad/glad.h> 
#include <KHR/khrplatform.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include "RtMidi.h"
#include "MidiFile.h"
#if defined(WIN32)
  #include <windows.h>
  #define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds ) 
#else // Unix variants
  #include <unistd.h>
  #define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif


using namespace std;
using namespace smf;

const char *vertexShaderSrc = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main(){\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";
const char *fragShaderSrc =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main(){\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";

const int w = 2400;
const int h = 1600;
string allNotes[12] {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
std::vector<unsigned char> message;
GLfloat nowPlaying[108] = {0.0};
RtMidiOut *midiOut = new RtMidiOut();

std::vector<Note> generate_scale(string note, string tonic, string quality){
    int noteIdx;
    for(int i = 0; i < 12; i++){
        if(allNotes[i] == note){
            noteIdx = i;
        }
    }

    int seq[6] {2, 4, 5, 7, 9, 11};

    std::vector<Note> scale;
    scale.push_back(Note(note, 1, noteIdx));
    for(int x = 0; x < 6; x++){
        int allNotesIdx = (noteIdx + seq[x]) % 12;

        scale.push_back(Note(allNotes[allNotesIdx], x+2, allNotesIdx));
        if (scale.at(x).noteName[0] == scale.at(x + 1).noteName[0]){
            if (scale.at(x + 1).noteName[1] == '#' || scale.at(x + 1).noteName[1] == 'b'){
                scale.at(x + 1).noteName = allNotes[allNotesIdx + 1] + "b";
            }
        }
    }
    for(int i = 0; i < scale.size(); i++){
        cout << scale.at(i).idx;
    }
    return scale;
}


void framebuffer_size_callback(GLFWwindow* window, int w, int h){
    glViewport(0, 0, w, h);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        
    }
}

float valMap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

std::map<int, pair<std::vector<unsigned char>, double>> handleMidi(){
    std::map<int, pair<std::vector<unsigned char>, double>> keyEvents;
    generate_scale("D", "C", "major");
    MidiFile song;
    song.read("battle-theme-3-.mid");
    

    song.joinTracks();
    song.doTimeAnalysis();
    song.linkNotePairs();
    for(int i = 0; i < song[0].size(); i++){
        //vector<int> vect{song[0][i].getP0, song[0][i].getP1, song[0][i].getP2}; 
        std::vector<unsigned char> messagevec = std::vector<unsigned char>({(unsigned char)song[0][i].getP0(), 
            (unsigned char)song[0][i].getP1(), 
            (unsigned char)song[0][i].getP2()});
        keyEvents.insert(pair<int, pair<std::vector<unsigned char>, double>>(i, pair<std::vector<unsigned char>, double>(messagevec, song[0][i].seconds * 1000)));
    }      
    return keyEvents;
}

    void messageToUniform(const std::vector<unsigned char> *message){
        //Grab info from the message
        int noteNum = message->at(1) % 12;
        string noteName = allNotes[noteNum];
        int octave = message->at(1) / 12;
        int vel = message->at(2);
        //Adjust our fatass boolean array with whether note is being pressed or not.

        //if it is an ON message
        if(vel > 0){
            nowPlaying[noteNum * octave] = 1.0f;
        } else {
            nowPlaying[noteNum * octave] = 0.0f;
        }
        


    }

int main(){
    midiOut->openPort(0);
    int idxCounter = -1;
    std::vector<std::vector<float>> teddy = {{1.0,1.0,0.72}, {1.0,1.0,0.82}, {0.72,1.0,0.75}, {0.82,1.0,0.85}, {0.84,0.93,0.97},{0.28,0.43,0.74}, {0.28,0.43,0.74}, {0.73,0.66,0.89}, {1.0, 1.0, 1.0}, {1,0.8,0.95}, {1.0,1.0, 1.0}, {0.87,0.69,0.48}}; 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(w, h, "TEST!", NULL, NULL);
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    auto start = chrono::steady_clock::now();


    GLfloat squareW = (GLfloat)2/ 12;
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    float vertices[] = {
        0.0f, 0.25f, 0.0f, 
        0.0f, 0.0f, 0.0f,
        0.16666666666, 0.0f, 0.0f,
        0.16666666666, 0.25f, 0.0f,
    };

    unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

    //Develop array
    pair<float, float> xyArr[8][12];
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 12; x++){
            xyArr[y][x].first = (x*(w/12));
            xyArr[y][x].second = (y*(h/8));
        }
    }
    



    Shader ourShader("vertex_Shader.glsl", "fragment_shader.glsl");

    unsigned int VBO;
    unsigned int VAO[96];
   
    glGenVertexArrays(96, VAO);
    glGenBuffers(1, &VBO);
    unsigned int EBO;
    glGenBuffers(1, &EBO);

        for(int y = 0; y < 96; y++){
       

        glBindVertexArray(VAO[y]);
    
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
        
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 3* sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        }

    int i = 0;
    std::vector<int> played;
    std::map<int, pair<std::vector<unsigned char>, double>> keyEvents = handleMidi();
    while(!glfwWindowShouldClose(window)){
    auto end = chrono::steady_clock::now();
    processInput(window);
    //cout << glfwGetTime() << "\n";
    
    
    glClearColor(0.0f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
     ourShader.use();
     //Generate midi sounds
    for (auto it = keyEvents.cbegin(); it != keyEvents.cend() /* not hoisted */; /* no increment */) {
        auto elapsed = (double)chrono::duration_cast<chrono::milliseconds>(end - start).count();
        if (count(played.begin(), played.end(), it->first)){
            it = keyEvents.erase(it);
        }else if(elapsed >= it->second.second){
            midiOut->sendMessage(&it->second.first);
            played.push_back(it->first);
            messageToUniform(&it->second.first);
            it++;
        } else {
            it++;
        }
            
    }  
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 12; x++){
            glBindVertexArray(VAO[(x)*(y)]);
            glUniform1f(glGetUniformLocation(ourShader.ID, "x"), valMap(xyArr[y][x].first, 0, w, -1., 1.));
            glUniform1f(glGetUniformLocation(ourShader.ID, "y"), -(valMap(xyArr[y][x].second, 0, h, 1., -1.)));
            glUniform1i(glGetUniformLocation(ourShader.ID, "idx"), (x + 1) * (y + 1) );
            glUniform2f(glGetUniformLocation(ourShader.ID, "u_location"), xyArr[y][x].first, xyArr[y][x].second);
            glUniform2f(glGetUniformLocation(ourShader.ID, "u_resolution"), w, h);
            glUniform1f(glGetUniformLocation(ourShader.ID, "u_time"), sin(glfwGetTime()));
            glUniform1fv(glGetUniformLocation(ourShader.ID, "notes"),108,nowPlaying);
            glUniform3f(glGetUniformLocation(ourShader.ID, "u_color"), teddy.at(x).at(0), teddy.at(x).at(1), teddy.at(x).at(2));
            double xPos, yPos;
            glfwGetCursorPos(window, &xPos, &yPos);
            glUniform2f(glGetUniformLocation(ourShader.ID, "u_mouse"), xPos, yPos);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }
    
        
    
    
    

    glfwSwapBuffers(window);
    glfwPollEvents();    
    i =  i + 1;
    
    
    }
    glfwTerminate();
    return 0;

    /*Note note = generate_scale("F", "major");
    return 0;*/
    
}

