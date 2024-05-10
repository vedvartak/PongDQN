#version 330 core
out vec4 FragColor;
in vec2 coord;
void main(){
float r=1.0f;
vec2 uv=2*coord-1.0f;
float len=dot(uv,uv);
vec3 color=vec3(1.0f,0.0f,0.0f);
float temp=smoothstep(r*r-0.01f,r*r,len);
//float t=glow*(rg*rg-len)*((sin(timer)+1)/2.0f)*2.0f;
//t=t/(t+0.155)*1.019;
//t=clamp(t,0.0,1.0);
FragColor=vec4(color,1.0f-temp);
}