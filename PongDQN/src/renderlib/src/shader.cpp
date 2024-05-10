#include<string>
#include<fstream>
#include<sstream>
#include<renderlib/ogl_ren.h>
#include<renderlib/shader.h>
#include <renderlib/shader.h>

int Shader::setSrc(std::string &to, const std::string &src)
{
    to=src;
    return 0;
}

int Shader::loadSrc(std::string &to, const std::string &path)
{
    try{
    std::ifstream file(path,std::ios::in);
    if(!file.is_open()){
        printf("Error loading shader:\n");
        return -1;
    }
    std::stringstream stream;
    stream<<file.rdbuf();
    to=stream.str();
    // printf("shader src:\n%s\n",to.c_str());
    file.close();
    }
    catch(std::ifstream::failure e){
        printf("Error loading shader:%s\n",e.what());
        return -1;
    }
    return 0;
}
int Shader::setVertexShader(const std::string& src){
    return setSrc(vertexshadersrc,src);
}
int Shader::loadVertexShader(const std::string& path){

    return loadSrc(vertexshadersrc,path);
}
int Shader::setFragmentShader(const std::string& src){

    return setSrc(fragmentshadersrc,src);
}
int Shader::loadFragmentShader(const std::string& path){

    return loadSrc(fragmentshadersrc,path);
}
int Shader::compile(ShaderType type){
    // printf("out compile l:%p\n",__builtin_return_address(0));
    if(!Render::Openglengine::contextStatus()) return -1;
    bool flag=1;
    const char* tvertexsrc=vertexshadersrc.c_str();
    const char* tfragmentsrc=fragmentshadersrc.c_str();
    if(type==ShaderType::VERTEX||type==ShaderType::VERTEX_FRAGMENT){
        // glGetError();
        if(vertexshader==0)
        vertexshader=Render::Openglengine::currentglc->glCreateShader(GL_VERTEX_SHADER);
        // Render::Openglengine::currentglc->glDeleteShader(vertexshader);
        printf("last err %u\n",glGetError());
        Render::Openglengine::currentglc->glShaderSource(vertexshader,1,&tvertexsrc,NULL);
        Render::Openglengine::currentglc->glCompileShader(vertexshader);
        int success;
        char log[512];
        Render::Openglengine::currentglc->glGetShaderiv(vertexshader,GL_COMPILE_STATUS,&success);
        if(!success){
            Render::Openglengine::currentglc->glGetShaderInfoLog(vertexshader,512,NULL,log);
            printf("COMPILE ERROR VERT:%s\n",log);
            return -1;
        }
    }
    if(type==ShaderType::FRAGMENT||type==ShaderType::VERTEX_FRAGMENT){
        if(fragmentshader==0)
        fragmentshader=Render::Openglengine::currentglc->glCreateShader(GL_FRAGMENT_SHADER);
        Render::Openglengine::currentglc->glShaderSource(fragmentshader,1,&tfragmentsrc,NULL);
        Render::Openglengine::currentglc->glCompileShader(fragmentshader);

        int success;
        char log[512];
        Render::Openglengine::currentglc->glGetShaderiv(fragmentshader,GL_COMPILE_STATUS,&success);
        if(!success){
            Render::Openglengine::currentglc->glGetShaderInfoLog(fragmentshader,512,NULL,log);
            printf("COMPILE ERROR FRAG: %s\n",log);
            return -1;
        }
    }
    printf("out compile l:%p\n",__builtin_return_address(0));
    return 0;
}
int Shader::buildProgram()
{
    if(!Render::Openglengine::contextStatus()) return -1;
    if(vertexshader==0||fragmentshader==0) return -2;
    program=Render::Openglengine::currentglc->glCreateProgram();
    Render::Openglengine::currentglc->glAttachShader(program,vertexshader);
    Render::Openglengine::currentglc->glAttachShader(program,fragmentshader);
    Render::Openglengine::currentglc->glLinkProgram(program);
    int success;
    char log[512];
    Render::Openglengine::currentglc->glGetProgramiv(program,GL_LINK_STATUS,&success);
    if(!success){
        Render::Openglengine::currentglc->glGetProgramInfoLog(program,512,NULL,log);
        printf("LINK ERROR: %s\n",log);
        return -1;
    }
    Render::Openglengine::currentglc->glDeleteShader(vertexshader);
    Render::Openglengine::currentglc->glDeleteShader(fragmentshader);
    vertexshader=0;
    fragmentshader=0;
    return 0;
}
int Shader::setUniform(const std::string &name, float x)
{
    if (!Render::Openglengine::contextStatus()) return -1;
    Render::Openglengine::clearError();
    setUniform(Render::Openglengine::currentglc->glGetUniformLocation(program,name.c_str()),x);
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::setUniform(const std::string &name, float x, float y)
{
    setUniform(Render::Openglengine::currentglc->glGetUniformLocation(program,name.c_str()),x,y);
    return 0;
}
int Shader::setUniform(const std::string &name, float x, float y, float z)
{
    setUniform(Render::Openglengine::currentglc->glGetUniformLocation(program,name.c_str()),x,y,z);
    return 0;
}
int Shader::setUniform(const std::string &name, float x, float y, float z, float w)
{
    setUniform(Render::Openglengine::currentglc->glGetUniformLocation(program,name.c_str()),x,y,z,w);
    return 0;
}
int Shader::setUniform(const std::string &name, int x)
{
    setUniform(Render::Openglengine::currentglc->glGetUniformLocation(program,name.c_str()),x);
    return 0;
}
int Shader::setUniform(const std::string &name, int x, int y)
{
    setUniform(Render::Openglengine::currentglc->glGetUniformLocation(program,name.c_str()),x,y);
    return 0;
}
int Shader::setUniform(const std::string &name, int x, int y, int z)
{
    setUniform(Render::Openglengine::currentglc->glGetUniformLocation(program,name.c_str()),x,y,z);
    return 0;
}
int Shader::setUniform(const std::string &name, int x, int y, int z, int w)
{
    setUniform(Render::Openglengine::currentglc->glGetUniformLocation(program,name.c_str()),x,y,z,w);
    return 0;
}
int Shader::setUniform(const std::string &name, unsigned int x)
{
    setUniform(Render::Openglengine::currentglc->glGetUniformLocation(program,name.c_str()),x);
    return 0;
}
int Shader::setUniform(const std::string &name, unsigned int x, unsigned int y)
{
    setUniform(Render::Openglengine::currentglc->glGetUniformLocation(program,name.c_str()),x,y);
    return 0;
}
int Shader::setUniform(const std::string &name, unsigned int x, unsigned int y, unsigned int z)
{
    setUniform(Render::Openglengine::currentglc->glGetUniformLocation(program,name.c_str()),x,y,z);
    return 0;
}
int Shader::setUniform(const std::string &name, unsigned int x, unsigned int y, unsigned int z, unsigned int w)
{
    setUniform(Render::Openglengine::currentglc->glGetUniformLocation(program,name.c_str()),x,y,z,w);
    return 0;
}
int Shader::setUniform(int location, float x)
{
    if (!Render::Openglengine::contextStatus()) return -1;
    Render::Openglengine::clearError();
    Render::Openglengine::currentglc->glUniform1f(location,x);
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::setUniform(int location, float x, float y)
{
    if (!Render::Openglengine::contextStatus()) return -1;
    Render::Openglengine::clearError();
    Render::Openglengine::currentglc->glUniform2f(location,x,y);
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::setUniform(int location, float x, float y, float z)
{
    if (!Render::Openglengine::contextStatus()) return -1;
    Render::Openglengine::clearError();
    Render::Openglengine::currentglc->glUniform3f(location,x,y,z);
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::setUniform(int location, float x, float y, float z, float w)
{
    if (!Render::Openglengine::contextStatus()) return -1;
    Render::Openglengine::clearError();
    Render::Openglengine::currentglc->glUniform4f(location,x,y,z,w);
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::setUniform(int location, int x)
{
    if (!Render::Openglengine::contextStatus()) return -1;
    Render::Openglengine::clearError();
    Render::Openglengine::currentglc->glUniform1i(location,x);
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::setUniform(int location, int x, int y)
{
    if (!Render::Openglengine::contextStatus()) return -1;
    Render::Openglengine::clearError();
    Render::Openglengine::currentglc->glUniform2i(location,x,y);
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::setUniform(int location, int x, int y, int z)
{
    if (!Render::Openglengine::contextStatus()) return -1;
    Render::Openglengine::clearError();
    Render::Openglengine::currentglc->glUniform3i(location,x,y,z);
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::setUniform(int location, int x, int y, int z, int w)
{
    if (!Render::Openglengine::contextStatus()) return -1;
    Render::Openglengine::clearError();
    Render::Openglengine::currentglc->glUniform4i(location,x,y,z,w);
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::setUniform(int location, unsigned int x)
{
    if (!Render::Openglengine::contextStatus()) return -1;
    Render::Openglengine::clearError();
    Render::Openglengine::currentglc->glUniform1ui(location,x);
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::setUniform(int location, unsigned int x, unsigned int y)
{
    if (!Render::Openglengine::contextStatus()) return -1;
    Render::Openglengine::clearError();
    Render::Openglengine::currentglc->glUniform2ui(location,x,y);
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::setUniform(int location, unsigned int x, unsigned int y, unsigned int z)
{
    if (!Render::Openglengine::contextStatus()) return -1;
    Render::Openglengine::clearError();
    Render::Openglengine::currentglc->glUniform3ui(location,x,y,z);
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::setUniform(int location, unsigned int x, unsigned int y, unsigned int z, unsigned int w)
{
    if (!Render::Openglengine::contextStatus()) return -1;
    Render::Openglengine::clearError();
    Render::Openglengine::currentglc->glUniform4ui(location,x,y,z,w);
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::setUniform(int location, float *x, unsigned char n, int count)
{
    if (!Render::Openglengine::contextStatus()) return -1;
    Render::Openglengine::clearError();
    if(n==1)
    Render::Openglengine::currentglc->glUniform1fv(location,count,x);
    else if(n==2)
    Render::Openglengine::currentglc->glUniform2fv(location,count,x);
    else if(n==3)
    Render::Openglengine::currentglc->glUniform3fv(location,count,x);
    else if(n==4)
    Render::Openglengine::currentglc->glUniform4fv(location,count,x);
    else return -2;
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::setUniform(int location, int *x, unsigned char n, int count)
{
    if (!Render::Openglengine::contextStatus()) return -1;
    Render::Openglengine::clearError();
    if(n==1)
    Render::Openglengine::currentglc->glUniform1iv(location,count,x);
    else if(n==2)
    Render::Openglengine::currentglc->glUniform2iv(location,count,x);
    else if(n==3)
    Render::Openglengine::currentglc->glUniform3iv(location,count,x);
    else if(n==4)
    Render::Openglengine::currentglc->glUniform4iv(location,count,x);
    else return -2;
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::setUniform(int location, unsigned int *x, unsigned char n, int count)
{
    if (!Render::Openglengine::contextStatus()) return -1;
    Render::Openglengine::clearError();
    if(n==1)
    Render::Openglengine::currentglc->glUniform1uiv(location,count,x);
    else if(n==2)
    Render::Openglengine::currentglc->glUniform2uiv(location,count,x);
    else if(n==3)
    Render::Openglengine::currentglc->glUniform3uiv(location,count,x);
    else if(n==4)
    Render::Openglengine::currentglc->glUniform4uiv(location,count,x);
    else return -2;
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::use()
{
    if(!Render::Openglengine::contextStatus()) return -1;
    if(program==0) return -1;
    Render::Openglengine::clearError();
    Render::Openglengine::currentglc->glUseProgram(program);
    if(glGetError()!=GL_NO_ERROR) return -1;
    return 0;
}
int Shader::destroy()
{
    if(!Render::Openglengine::contextStatus()) return -1;
    if(vertexshader!=0){
        Render::Openglengine::clearError();
        Render::Openglengine::currentglc->glDeleteShader(vertexshader);
        if(glGetError()!=GL_NO_ERROR) return -1;
        vertexshader=0;
    }
    if(fragmentshader!=0){
        Render::Openglengine::clearError();
        Render::Openglengine::currentglc->glDeleteShader(fragmentshader);
        if(glGetError()!=GL_NO_ERROR) return -2;
        fragmentshader=0;
    }
    if(program!=0){
        Render::Openglengine::clearError();
        Render::Openglengine::currentglc->glDeleteProgram(program);
        if(glGetError()!=GL_NO_ERROR) return -2;
        program=0;
    }
    return 0;
}
Shader::Shader(/* args */)
{
    
}

// Shader::Shader(const std::string &vpath, const std::string &fpath)
// {
//     this->loadVertexShader(vpath);
//     this->loadFragmentShader(fpath);
// }
Shader::~Shader()
{
    destroy();
}