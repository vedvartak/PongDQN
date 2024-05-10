#ifndef RENDERLIB
#define RENDERLIB

#include<Windows.h>
#include <string>
#include <GL/GL.h>
#define USE_OPENGL
#ifdef USE_OPENGL
#include <renderlib/ogl_ren.h>
#include <renderlib/camera.h>
#include <renderlib/shapes/circle.h>
#include <renderlib/shapes/rect.h>
#endif


namespace Render
{
    // extern wglChoosePixelFormatARB_type* wglChoosePixelFormatARB;
    
    HGLRC getcontext(HWND hwnd,const HDC hdc);
    void* loadfunction(std::string str);
    int getpixelformat(HWND dummy);
    int setpixelformat(HWND hwnd,HDC* hdcp=NULL);
    HGLRC init_opengl(HDC hdc);
    int reninit(unsigned int& VBO,unsigned int& VAO,unsigned int& shader,unsigned int&dT,Shader **shade);
    int ren(unsigned int VBO,unsigned int VAO,unsigned int shader);
    int render(Render::Camera& cam,Shapes::Circle& circle);
    int render(Render::Camera& cam,Shapes::Rect& rect);

    class Vec2
    {
    private:
        float x=0,y=0;
    public:
        Vec2(float x=0,float y=0);
        ~Vec2();
    };
    
    
    class Vertex
    {
    private:
        /* data */
    public:
        float x=0,y=0,z=0;
        Vertex(float xin=0,float yin=0,float zin=0);
        ~Vertex();
    };
    
    // class Triangle
    // {
    // private:
    //     Vertex vertices[3];
    // public:
    //     Triangle(const Vertex& a,const Vertex& b,const Vertex& c);
    //     ~Triangle();
    // };
    
    // Triangle::Triangle(const Vertex& a,const Vertex& b,const Vertex& c)
    // {
    //     vertices[0]=a;
    //     vertices[1]=b;
    //     vertices[2]=c;
    // }
    
    // Triangle::~Triangle()
    // {
    // }
    
} // namespace Render
#endif
