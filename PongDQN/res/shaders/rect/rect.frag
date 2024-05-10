#version 330 core
out vec4 FragColor;
in vec2 coord;
void main(){
vec3 color=vec3(0.0f,0.2f,0.6f);
FragColor=vec4(color,1.0f);
}