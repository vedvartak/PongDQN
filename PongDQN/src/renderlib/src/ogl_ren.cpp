#include <renderlib/ogl_ren.h>
#include <GL/wgl.h>
#include <GL/glext.h>
#include <renderlib/render.h>
#include <uilib/window.h>
#include <memory>
#include <string>
#include <iostream>
namespace Render{
    namespace Opengl{
        Context::Context(/* args */)
        {
        }

        Context::~Context()
        {
            destroy();
        }
        HGLRC Context::getContextHandle() const{
            return context;
        }
        HDC Context::getHDC() const{
            return currenthdc;
        }
        bool Context::isCreated() const{
            return (context!=NULL) ? true:false;
        }
        int Context::create(const ::UIlib::Window& window){
            if(!window.isCreated()) return -1;
            int glattribs[]={
                WGL_CONTEXT_MAJOR_VERSION_ARB,3,
                WGL_CONTEXT_MINOR_VERSION_ARB,3,
                WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                0
            };
            //Improve
            HDC hdc=GetDC(window.hwnd);
            if(hdc==NULL) return -2;
            context=Openglengine::wglCreateContextAttribsARB(hdc,0,glattribs);
            int format=GetPixelFormat(hdc);
            DescribePixelFormat(hdc,format,sizeof(pfd),&pfd);
            if (!context)
            {
                printf("jdkjlajdsajd\n");
                ReleaseDC(window.hwnd,hdc);
                return -3;
            }

            makeCurrent(hdc);
            // ReleaseDC(window.hwnd,hdc);
            return 0;
        }
        bool Context::isCurrent() const{
            if(!isCreated()||Openglengine::currentglc==NULL||this!=Openglengine::currentglc) 
                return false;
            return true;
        }
        int Context::makeCurrent(HDC hdc){
            
            if(!(isCreated())) return -2;
            if(isCurrent()){
                printf("already current\n");
                printf("curr Lastkk %d %d\n",isCurrent(),detach());
                
            }
            if(wglMakeCurrent(hdc,context)==TRUE){
                Openglengine::currentglc=this;
                // printf("set %p %p\n",Openglengine::currentglc,this);
                currenthdc=hdc;
            }
            else{
                printf("couldnt make curr Last %u hdc:%p glc:%p\n",GetLastError(),hdc,context);
                return -3;
            }
            return 0;
        }
        int Context::makeCurrent(const ::UIlib::Window& window){
            if(!window.isCreated()) return -5;
            int ret=0;
            if(isCurrent()){
                // printf("jk hdc:%p hw:%p dw:%p\n",getHDC(),window.hwnd,WindowFromDC(getHDC()));
                if(window.hwnd==WindowFromDC(getHDC())){
                    // printf("came\n");
                    return 0;
                }
            }
            HDC hdc=GetDC(window.hwnd);
            if(hdc==NULL) return -4;
            if(ret=makeCurrent(hdc)){
                ReleaseDC(window.hwnd,hdc);
                printf("in %d\n",ret);
                return ret;
            }
            return 0;
        }
        int Context::detach(){
            if(!isCurrent()) return -1;
            if(wglMakeCurrent(NULL,NULL)){
                Openglengine::currentglc=NULL;
                // printf("currentdc:%p %p\n",currenthdc,WindowFromDC(currenthdc));
                ReleaseDC(WindowFromDC(currenthdc),currenthdc);
                currenthdc=NULL;
                return 0;
            }
            return -2;
        }
        int Context::destroy(){
            if(isCurrent()){
                if(detach()!=0) return -1;
            }
            if(isCreated()){
                if(!wglDeleteContext(context)) return -2;
                context=NULL;
                pfd={0};
            }

            return 0;
        }
        int Context::loadFunctions(){
            if(!isCurrent()) return -1;
            int fails=0;
            void* func=loadfunction("glGenBuffers");
            if(func!=NULL)
            glGenBuffers=(PFNGLGENBUFFERSPROC)func;
            else fails++;
            func=loadfunction("glBindBuffer");
            if(func!=NULL)
            glBindBuffer=(PFNGLBINDBUFFERPROC)func;
            else fails++;
            func=loadfunction("glBufferData");
            if(func!=NULL)
            glBufferData=(PFNGLBUFFERDATAPROC)func;
            else fails++;

            func=loadfunction("glCreateShader");
            if(func!=NULL){
            glCreateShader=(PFNGLCREATESHADERPROC)func;
            }
            else fails++;
            
            func=loadfunction("glShaderSource");
            if(func!=NULL)
            glShaderSource=(PFNGLSHADERSOURCEPROC)func;
            else fails++;
            func=loadfunction("glCompileShader");
            if(func!=NULL)
            glCompileShader=(PFNGLCOMPILESHADERPROC)func;
            else fails++;
            func=loadfunction("glGetShaderiv");
            if(func!=NULL)
            glGetShaderiv=(PFNGLGETSHADERIVPROC)func;
            else fails++;
            func=loadfunction("glGetShaderInfoLog");
            if(func!=NULL)
            glGetShaderInfoLog=(PFNGLGETSHADERINFOLOGPROC)func;
            else fails++;
            func=loadfunction("glCreateProgram");
            if(func!=NULL)
            glCreateProgram=(PFNGLCREATEPROGRAMPROC)func;
            else fails++;
            func=loadfunction("glAttachShader");
            if(func!=NULL)
            glAttachShader=(PFNGLATTACHSHADERPROC)func;
            else fails++;
            func=loadfunction("glLinkProgram");
            if(func!=NULL)
            glLinkProgram=(PFNGLLINKPROGRAMPROC)func;
            else fails++;
            func=loadfunction("glDeleteShader");
            if(func!=NULL)
            glDeleteShader=(PFNGLDELETESHADERPROC)func;
            else fails++;
            func=loadfunction("glUseProgram");
            if(func!=NULL)
            glUseProgram=(PFNGLUSEPROGRAMPROC)func;
            else fails++;
            func=loadfunction("glGetUniformLocation");
            if(func!=NULL)
            glGetUniformLocation=(PFNGLGETUNIFORMLOCATIONPROC)func;
            else fails++;
            func=loadfunction("glGetProgramiv");
            if(func!=NULL)
            glGetProgramiv=(PFNGLGETPROGRAMIVPROC)func;
            else fails++;
            func=loadfunction("glGetProgramiv");
            if(func!=NULL)
            glGetProgramInfoLog=(PFNGLGETPROGRAMINFOLOGPROC)func;
            else fails++;
            func=loadfunction("glVertexAttribPointer");
            if(func!=NULL)
            glVertexAttribPointer=(PFNGLVERTEXATTRIBPOINTERPROC)func;
            else fails++;
            func=loadfunction("glEnableVertexAttribArray");
            if(func!=NULL)
            glEnableVertexAttribArray=(PFNGLENABLEVERTEXATTRIBARRAYPROC)func;
            else fails++;
            func=loadfunction("glDeleteBuffers");
            if(func!=NULL)
            glDeleteBuffers=(PFNGLDELETEBUFFERSPROC)func;
            else fails++;
            func=loadfunction("glGenVertexArrays");
            if(func!=NULL)
            glGenVertexArrays=(PFNGLGENVERTEXARRAYSPROC)func;
            else fails++;
            func=loadfunction("glBindVertexArray");
            if(func!=NULL)
            glBindVertexArray=(PFNGLBINDVERTEXARRAYPROC)func;
            else fails++;
            func=loadfunction("glDeleteProgram");
            if(func!=NULL)
            glDeleteProgram=(PFNGLDELETEPROGRAMPROC)func;
            else fails++;
            func=loadfunction("glUniform1f");
            if(func!=NULL)
            glUniform1f=(PFNGLUNIFORM1FPROC)func;
            else fails++;
            func=loadfunction("glUniform2f");
            if(func!=NULL)
            glUniform2f=(PFNGLUNIFORM2FPROC)func;
            else fails++;
            func=loadfunction("glUniform3f");
            if(func!=NULL)
            glUniform3f=(PFNGLUNIFORM3FPROC)func;
            else fails++;
            func=loadfunction("glUniform4f");
            if(func!=NULL)
            glUniform4f=(PFNGLUNIFORM4FPROC)func;
            else fails++;
            func=loadfunction("glUniform1i");
            if(func!=NULL)
            glUniform1i=(PFNGLUNIFORM1IPROC)func;
            else fails++;
            func=loadfunction("glUniform2i");
            if(func!=NULL)
            glUniform2i=(PFNGLUNIFORM2IPROC)func;
            else fails++;
            func=loadfunction("glUniform3i");
            if(func!=NULL)
            glUniform3i=(PFNGLUNIFORM3IPROC)func;
            else fails++;
            func=loadfunction("glUniform4i");
            if(func!=NULL)
            glUniform4i=(PFNGLUNIFORM4IPROC)func;
            else fails++;
            func=loadfunction("glUniform1ui");
            if(func!=NULL)
            glUniform1ui=(PFNGLUNIFORM1UIPROC)func;
            else fails++;
            func=loadfunction("glUniform2ui");
            if(func!=NULL)
            glUniform2ui=(PFNGLUNIFORM2UIPROC)func;
            else fails++;
            func=loadfunction("glUniform3ui");
            if(func!=NULL)
            glUniform3ui=(PFNGLUNIFORM3UIPROC)func;
            else fails++;
            func=loadfunction("glUniform4ui");
            if(func!=NULL)
            glUniform4ui=(PFNGLUNIFORM4UIPROC)func;
            else fails++;
            func=loadfunction("glUniform1fv");
            if(func!=NULL)
            glUniform1fv=(PFNGLUNIFORM1FVPROC)func;
            else fails++;
            func=loadfunction("glUniform2fv");
            if(func!=NULL)
            glUniform2fv=(PFNGLUNIFORM2FVPROC)func;
            else fails++;
            func=loadfunction("glUniform3fv");
            if(func!=NULL)
            glUniform3fv=(PFNGLUNIFORM3FVPROC)func;
            else fails++;
            func=loadfunction("glUniform4fv");
            if(func!=NULL)
            glUniform4fv=(PFNGLUNIFORM4FVPROC)func;
            else fails++;
            func=loadfunction("glUniform1iv");
            if(func!=NULL)
            glUniform1iv=(PFNGLUNIFORM1IVPROC)func;
            else fails++;
            func=loadfunction("glUniform2iv");
            if(func!=NULL)
            glUniform2iv=(PFNGLUNIFORM2IVPROC)func;
            else fails++;
            func=loadfunction("glUniform3iv");
            if(func!=NULL)
            glUniform3iv=(PFNGLUNIFORM3IVPROC)func;
            else fails++;
            func=loadfunction("glUniform4iv");
            if(func!=NULL)
            glUniform4iv=(PFNGLUNIFORM4IVPROC)func;
            else fails++;
            func=loadfunction("glUniform1uiv");
            if(func!=NULL)
            glUniform1uiv=(PFNGLUNIFORM1UIVPROC)func;
            else fails++;
            func=loadfunction("glUniform2uiv");
            if(func!=NULL)
            glUniform2uiv=(PFNGLUNIFORM2UIVPROC)func;
            else fails++;
            func=loadfunction("glUniform3uiv");
            if(func!=NULL)
            glUniform3uiv=(PFNGLUNIFORM3UIVPROC)func;
            else fails++;
            func=loadfunction("glUniform4uiv");
            if(func!=NULL)
            glUniform4uiv=(PFNGLUNIFORM4UIVPROC)func;
            else fails++;
            return fails;
        }
    }
    bool Openglengine::contextStatus()
    {
        if(Openglengine::currentglc==NULL||!Openglengine::currentglc->isCurrent())
            return false;
        else 
            return true;
    }
    int Openglengine::clearError()
    {
        unsigned int i=0;
        while (glGetError()!=GL_NO_ERROR) i++;
        
        return i;
    }
    Openglengine::Openglengine(/* args */)
    {
    }

    Openglengine::~Openglengine()
    {
    }
    
    PFNWGLCHOOSEPIXELFORMATARBPROC Openglengine::wglChoosePixelFormatARB;
    PFNWGLCREATECONTEXTATTRIBSARBPROC Openglengine::wglCreateContextAttribsARB;
    // PFNGLGENBUFFERSPROC Openglengine::glGenBuffers;
    // PFNGLBINDBUFFERPROC Openglengine::glBindBuffer;
    // PFNGLBUFFERDATAPROC Openglengine::glBufferData;
    // PFNGLCREATESHADERPROC Openglengine::glCreateShader;
    // PFNGLSHADERSOURCEPROC Openglengine::glShaderSource;
    // PFNGLCOMPILESHADERPROC Openglengine::glCompileShader;
    // PFNGLGETSHADERIVPROC Openglengine::glGetShaderiv;
    // PFNGLGETSHADERINFOLOGPROC Openglengine::glGetShaderInfoLog;
    // PFNGLCREATEPROGRAMPROC Openglengine::glCreateProgram;
    // PFNGLATTACHSHADERPROC Openglengine::glAttachShader;
    // PFNGLLINKPROGRAMPROC Openglengine::glLinkProgram;
    // PFNGLDELETESHADERPROC Openglengine::glDeleteShader;
    // PFNGLUSEPROGRAMPROC Openglengine::glUseProgram;
    // PFNGLGETPROGRAMIVPROC Openglengine::glGetProgramiv;
    // PFNGLGETPROGRAMINFOLOGPROC Openglengine::glGetProgramInfoLog;
    // PFNGLVERTEXATTRIBPOINTERPROC Openglengine::glVertexAttribPointer;
    // PFNGLENABLEVERTEXATTRIBARRAYPROC Openglengine::glEnableVertexAttribArray;
    // PFNGLDELETEBUFFERSPROC Openglengine::glDeleteBuffers;
    // PFNGLGENVERTEXARRAYSPROC Openglengine::glGenVertexArrays;
    // PFNGLBINDVERTEXARRAYPROC Openglengine::glBindVertexArray;
    // glGenBuffers_type* Openglengine::glGenBuffers;
    Opengl::Context *Openglengine::currentglc=NULL;
}