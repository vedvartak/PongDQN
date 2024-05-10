#ifndef OGLLIB
#define OGLLIB
// #define WGL_CONTEXT_MAJOR_VERSION_ARB (0x2091)
// #define WGL_CONTEXT_MINOR_VERSION_ARB (0x2092)
// #define WGL_CONTEXT_PROFILE_MASK_ARB (0x9126)
// #define WGL_CONTEXT_CORE_PROFILE_BIT_ARB (0x01)

// #define WGL_DRAW_TO_WINDOW_ARB (0x2001)
// #define WGL_ACCELERATION_ARB (0x2003)
// #define WGL_SUPPORT_OPENGL_ARB (0x2010)
// #define WGL_DOUBLE_BUFFER_ARB (0x2011)
// #define WGL_PIXEL_TYPE_ARB (0x2013)
// #define WGL_COLOR_BITS_ARB (0x2014)
// #define WGL_DEPTH_BITS_ARB (0x2022)
// #define WGL_STENCIL_BITS_ARB (0x2023)
// #define WGL_TYPE_RGBA_ARB (0x202B)

// #define GL_ARRAY_BUFFER (0x8892)
// #define GL_STATIC_DRAW (0x88E4)
#include <windows.h>
#include <uilib/window.h>
#include <GL/gl.h>
#include <GL/wgl.h>
#include <GL/glext.h>
#include <GL/wglext.h>
// #include <GL/glcorearb.h>
#include <cctype>
#include <vector>

#include <renderlib/shader.h>
// typedef uint64_t GLsizeiptr;
// typedef BOOL (wglChoosePixelFormatARB_type)(HDC hdc,const int *piAttribIList,
//     const FLOAT *pfAttribFList,UINT nMaxFormats,int *piFormats,
//     UINT *nNumFormats);
// typedef HGLRC WINAPI (wglCreateContextAttribsARB_type) (HDC hdc,HGLRC context,const int *attribList);
// typedef void glGenBuffers_type(GLsizei n,GLuint *buffers);
// typedef void glBindBuffer_type(GLenum target,GLuint buffer);
// typedef void glBufferData_type(GLenum target,GLsizeiptr size,const void* data,GLenum usage);
// typedef GLuint glCreateShader_type(GLenum shaderType);
// typedef void glShaderSource_type(GLuint shader,GLsizei count,const GLchar **string,const GLint *length);
// typedef 
// typedef BOOL (wglChoosePixelFormatARB_type)(HDC hdc,const int *piAttribIList,
//     const FLOAT *pfAttribFList,UINT nMaxFormats,int *piFormats,
//     UINT *nNumFormats);
namespace Render
{
    namespace Opengl{
        extern PFNGLCREATESHADERPROC glCreateShader;
        class Context
        {
        private:
            HGLRC context=NULL;
            HDC currenthdc=NULL;
            PIXELFORMATDESCRIPTOR pfd;
            int makeCurrent(HDC hdc);
        public:
            PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
            PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs;
            PFNGLGENBUFFERSPROC glGenBuffers;
            PFNGLBINDBUFFERPROC glBindBuffer;
            PFNGLBUFFERDATAPROC glBufferData;
            PFNGLCREATESHADERPROC glCreateShader;
            PFNGLSHADERSOURCEPROC glShaderSource;
            PFNGLCOMPILESHADERPROC glCompileShader;
            PFNGLGETSHADERIVPROC glGetShaderiv;
            PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
            PFNGLCREATEPROGRAMPROC glCreateProgram;
            PFNGLATTACHSHADERPROC glAttachShader;
            PFNGLLINKPROGRAMPROC glLinkProgram;
            PFNGLDELETESHADERPROC glDeleteShader;
            PFNGLUSEPROGRAMPROC glUseProgram;
            PFNGLGETPROGRAMIVPROC glGetProgramiv;
            PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
            PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
            PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
            PFNGLDELETEBUFFERSPROC glDeleteBuffers;
            PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
            PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
            PFNGLDELETEPROGRAMPROC glDeleteProgram;
            PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
            PFNGLUNIFORM1FPROC glUniform1f;
            PFNGLUNIFORM2FPROC glUniform2f;
            PFNGLUNIFORM3FPROC glUniform3f;
            PFNGLUNIFORM4FPROC glUniform4f;
            PFNGLUNIFORM1IPROC glUniform1i;
            PFNGLUNIFORM2IPROC glUniform2i;
            PFNGLUNIFORM3IPROC glUniform3i;
            PFNGLUNIFORM4IPROC glUniform4i;
            PFNGLUNIFORM1UIPROC glUniform1ui;
            PFNGLUNIFORM2UIPROC glUniform2ui;
            PFNGLUNIFORM3UIPROC glUniform3ui;
            PFNGLUNIFORM4UIPROC glUniform4ui;
            PFNGLUNIFORM1FVPROC glUniform1fv;
            PFNGLUNIFORM2FVPROC glUniform2fv;
            PFNGLUNIFORM3FVPROC glUniform3fv;
            PFNGLUNIFORM4FVPROC glUniform4fv;
            PFNGLUNIFORM1IVPROC glUniform1iv;
            PFNGLUNIFORM2IVPROC glUniform2iv;
            PFNGLUNIFORM3IVPROC glUniform3iv;
            PFNGLUNIFORM4IVPROC glUniform4iv;
            PFNGLUNIFORM1UIVPROC glUniform1uiv;
            PFNGLUNIFORM2UIVPROC glUniform2uiv;
            PFNGLUNIFORM3UIVPROC glUniform3uiv;
            PFNGLUNIFORM4UIVPROC glUniform4uiv;
        public:
            Context(/* args */);
            ~Context();
            HGLRC getContextHandle() const;
            HDC getHDC() const;
            bool isCreated() const;
            int create(const ::UIlib::Window& window);
            bool isCurrent() const;
            int makeCurrent(const ::UIlib::Window& window);
            int detach();
            int destroy();
            int loadFunctions();
        };
        
        
        // class Program
        // {
        // private:
        //     Shader shader;
        //     std::vector<unsigned int> bufferObjects;
        // public:
        //     Program(/* args */);
        //     ~Program();
        // };
        
        // Program::Program(/* args */)
        // {
        // }
        
        // Program::~Program()
        // {
        // }
        
    }
    
    class Openglengine
    {
    private:
        /* data */
    public:
        // wglChoosePixelFormatARB_type* wglChoosePixelFormatARB;
        static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
        static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
        // static PFNGLGENBUFFERSPROC glGenBuffers;
        // static PFNGLBINDBUFFERPROC glBindBuffer;
        // static PFNGLBUFFERDATAPROC glBufferData;
        // static PFNGLCREATESHADERPROC glCreateShader;
        // static PFNGLSHADERSOURCEPROC glShaderSource;
        // static PFNGLCOMPILESHADERPROC glCompileShader;
        // static PFNGLGETSHADERIVPROC glGetShaderiv;
        // static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
        // static PFNGLCREATEPROGRAMPROC glCreateProgram;
        // static PFNGLATTACHSHADERPROC glAttachShader;
        // static PFNGLLINKPROGRAMPROC glLinkProgram;
        // static PFNGLDELETESHADERPROC glDeleteShader;
        // static PFNGLUSEPROGRAMPROC glUseProgram;
        // static PFNGLGETPROGRAMIVPROC glGetProgramiv;
        // static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
        // static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
        // static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
        // static PFNGLDELETEBUFFERSPROC glDeleteBuffers;
        // static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
        // static PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
        static Opengl::Context *currentglc;
        static bool contextStatus();
        static int clearError();
        // static int loadfunctions();
        Openglengine(/* args */);
        ~Openglengine();
    };
} // namespace Render
#endif
