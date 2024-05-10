#include <renderlib/render.h>
#include <renderlib/ogl_ren.h>
#include <Windows.h>
#include<cstdio>
#include <GL/GL.h>
#include <GL/wgl.h>
#include <GL/glext.h>
#include<timer.h>
#include<renderlib/shader.h>
#include<renderlib/camera.h>
#include<cmath>
extern Timer timer;
namespace Render
{
    // wglChoosePixelFormatARB_type* wglChoosePixelFormatARB;
    HGLRC getcontext(HWND hwnd,const HDC hdc){
        PIXELFORMATDESCRIPTOR format={
        sizeof(PIXELFORMATDESCRIPTOR),
        PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0,0,0,0,0,0,
        0,
        0,
        0,
        0,0,0,0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0,0,0
        };
        // hdc=GetDC(hwnd);
        int fnum=ChoosePixelFormat(hdc,&format);
        printf("prev:%d\n",fnum);
        if(fnum==0){
            printf("error\n");
            return 0;
            }
        SetPixelFormat(hdc,fnum,&format);
        HGLRC context=wglCreateContext(hdc);
        wglMakeCurrent(hdc,context);
        return context;
    }
    void* loadfunction(std::string str){
        void* func=(void*)wglGetProcAddress(str.c_str());
        if (func==(void*)0||func==(void*)1||func==(void*)2||func==(void*)3||func==(void*)-1)
        {
            HMODULE module=LoadLibraryA("opengl32.dll");
            func=(void*)GetProcAddress(module,str.c_str());
        }
        return func;
    }
    int getpixelformat(HWND dummy){
        // Openglengine ogl;
        HDC hdc=GetDC(dummy);
        HGLRC context=Render::getcontext(dummy,hdc);
        // MessageBoxA(NULL,(char*)(glGetString(GL_VERSION)),"version",0);
        void* func=Render::loadfunction("wglChoosePixelFormatARB");
        // printf("pointer: %p\n",Render::loadfunction("WGL_DRAW_TO_WINDO_ARB"));
        Openglengine::wglChoosePixelFormatARB=(PFNWGLCHOOSEPIXELFORMATARBPROC)func;

        func=Render::loadfunction("wglCreateContextAttribsARB");
        Openglengine::wglCreateContextAttribsARB=(PFNWGLCREATECONTEXTATTRIBSARBPROC)func;
        

        wglDeleteContext(context);
        ReleaseDC(dummy,hdc);
        return 0;
    }
    int setpixelformat(HWND hwnd,HDC* hdcp){
        bool releasedc=false;
        HDC hdc=NULL;
        if(hdcp==NULL){
            hdc=GetDC(hwnd);
            releasedc=true;
        }
        else
            hdc=*hdcp;
        const int attrl[]={
            WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
            WGL_PIXEL_TYPE_ARB,WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB,32,
            WGL_DEPTH_BITS_ARB,24,
            WGL_STENCIL_BITS_ARB,8,0
        };
        int pixelform=0;
        UINT numformat=0;
        // Openglengine::wglChoosePixelFormatARB;
        Openglengine::wglChoosePixelFormatARB(hdc,attrl,0,1,&pixelform,&numformat);
        printf("new:%d %u\n",pixelform,numformat);
        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd,0,sizeof(pfd));
        
        int de=DescribePixelFormat(hdc,pixelform,sizeof(pfd),&pfd);
        // printf("desc %d\n",de);
        SetLastError(0);
        GetLastError();
        if(!SetPixelFormat(hdc,pixelform,&pfd)){

            printf("errorkk:%u\n",GetLastError());
        }
        else{
            
        }
        if(releasedc) ReleaseDC(hwnd,hdc);
        return 0;
    }
    HGLRC init_opengl(HDC hdc){
        int glattribs[]={
            WGL_CONTEXT_MAJOR_VERSION_ARB,3,
            WGL_CONTEXT_MINOR_VERSION_ARB,3,
            WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };
        HGLRC oglcontext=Openglengine::wglCreateContextAttribsARB(hdc,0,glattribs);
        
        if (!oglcontext)
        {
            printf("error glc\n");
            return oglcontext;
        }
        printf("ready\n");
        wglMakeCurrent(hdc,oglcontext);

        return oglcontext;
    }
    // std::chrono::time_point<std::chrono::steady_clock> prev;
    int reninit(unsigned int& VBO,unsigned int& VAO,unsigned int& shader,unsigned int& dT,Shader **shade){
        Vertex ver[4]={
            {-0.5f,-0.5f,0.0f},
            {0.5f,-0.5f,0.0f},
            {-0.5f,0.5f,0.0f},
            {0.5f,0.5f,0.0f}
        };
        Vec2 textcord[4]={
            {0.0f,0.0f},
            {1.0f,0.0f},
            {0.0f,1.0f},
            {1.0f,1.0f},
        };
        unsigned int indices[]={0,1,2};
        // unsigned int VBO,VAO,EBO;
        unsigned int VTXBO=0;
        float initT=std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count()/1000.0f;
        float tim[]={initT,initT,initT,initT};
        Openglengine::currentglc->glGenBuffers(1,&VBO);
        Openglengine::currentglc->glGenBuffers(1,&VTXBO);
        Openglengine::currentglc->glGenBuffers(1,&dT);
        Openglengine::currentglc->glBindBuffer(GL_ARRAY_BUFFER,VBO);
        // Openglengine::glBindBuffer(GL_ARRAY_BUFFER,VTXBO);
        Openglengine::currentglc->glGenVertexArrays(1,&VAO);
        Openglengine::currentglc->glBindVertexArray(VAO);
        Openglengine::currentglc->glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*4,ver,GL_STATIC_DRAW);
        Openglengine::currentglc->glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
        Openglengine::currentglc->glEnableVertexAttribArray(0);
        Openglengine::currentglc->glBindBuffer(GL_ARRAY_BUFFER,VTXBO);
        Openglengine::currentglc->glBufferData(GL_ARRAY_BUFFER,sizeof(Vec2)*4,textcord,GL_STATIC_DRAW);
        Openglengine::currentglc->glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,NULL);
        Openglengine::currentglc->glEnableVertexAttribArray(1);
        Openglengine::currentglc->glBindBuffer(GL_ARRAY_BUFFER,dT);
        Openglengine::currentglc->glBufferData(GL_ARRAY_BUFFER,sizeof(float)*4,&tim,GL_STATIC_DRAW);
        Openglengine::currentglc->glVertexAttribPointer(2,1,GL_FLOAT,GL_FALSE,0,NULL);
        Openglengine::currentglc->glEnableVertexAttribArray(2);
        // printf("hiiii\n");
        *shade=new Shader();
        (*shade)->loadVertexShader("..\\res\\shaders\\vert.glsl");
        (*shade)->loadFragmentShader("..\\res\\shaders\\frag.glsl");
        (*shade)->compile();
        (*shade)->buildProgram();
        (*shade)->setUniform("timer",initT);
        (*shade)->use();
        // prev=std::chrono::steady_clock::now();
        shader=(*shade)->program;
        return 0;
    }
    
    int ren(unsigned int VBO,unsigned int VAO,unsigned int shader){
        if(VBO==0||VAO==0||shader==0){
            // printf("whatttt\n");
            return -3;
        }
        int i=0;
        unsigned int e=0;
        Vertex ver[4]={
        {-0.5f,-0.5f,0.0f},
        {0.5f,-0.5f,0.0f},
        {-0.5f,0.5f,0.0f},
        {0.5f,0.5f,0.0f}
        };
        // float t=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()/1000.0f;
        // // printf("dt:%f\n",t);
        // for(int i=0;i<4;i++){
        //     float old=ver[i].x;
        //     ver[i].x+=sinf(t);
        //     // printf("vertex %d:%f %f\n",i,ver[i].x,ver[i].y);
        // }
        // Openglengine::glBindBuffer(GL_ARRAY_BUFFER,VBO);
        // Openglengine::glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*4,ver,GL_STATIC_DRAW);
        glGetError();
        
        Openglengine::currentglc->glUseProgram(shader);
        Openglengine::currentglc->glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES,3,)
        // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
        if((e=glGetError())!=GL_NO_ERROR) {
            // printf("draw error %x\n",e);
            return -1;
        }
        return 0;
    }
    int render(Render::Camera& cam,Shapes::Circle& circle){
        // if (!Render::Openglengine::contextStatus()){
        //     printf("no context\n--------------");
        // }
        
        // Shader s=bshader;
        // s.loadVertexShader("..\\res\\shaders\\circle\\circle.vert");
        // s.loadFragmentShader("..\\res\\shaders\\circle\\circle.frag");
        // s.compile();
        // s.buildProgram();
        circle.update(cam);
        Shapes::Circle::shader.use();
        Openglengine::currentglc->glBindVertexArray(circle.VAO);
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
        unsigned int e=0;
        if((e=glGetError())!=GL_NO_ERROR) {
            printf("draw error %x\n",e);
            return -1;
        }
        // printf("drawn\n");
        return 0;
    }
    int render(Render::Camera& cam,Shapes::Rect& rect){
        rect.update(cam);
        Shapes::Rect::shader.use();
        Openglengine::currentglc->glBindVertexArray(rect.VAO);
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
        unsigned int e=0;
        if((e=glGetError())!=GL_NO_ERROR) {
            printf("draw error %x\n",e);
            return -1;
        }
        // printf("drawn\n");
        return 0;
    }
    Vec2::Vec2(float x,float y):x(x),y(y)
    {
    }
    
    Vec2::~Vec2()
    {
    }
    Vertex::Vertex(float xin,float yin,float zin):x(xin),y(yin),z(zin)
    {
    }
    
    Vertex::~Vertex()
    {
    }
} // namespace Render
