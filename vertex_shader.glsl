#version 460 core
layout (location = 0) in vec3 aPos;
uniform float x;
uniform float y;
out vec3 vertices;
void main(){
   gl_Position = vec4(aPos.x + x,aPos.y + y, 0.0, 1.0);
   vertices = aPos;
}