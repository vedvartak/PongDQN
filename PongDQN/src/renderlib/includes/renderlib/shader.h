#ifndef REN_SHADER_H
#define REN_SHADER_H
#include<string>
#include<fstream>
#include<sstream>
#include<renderlib/ogl_ren.h>

enum ShaderType{
    VERTEX=0,
    FRAGMENT,
    VERTEX_FRAGMENT
};
class Shader
{
private:
    /* data */
    std::string vertexshadersrc;
    std::string fragmentshadersrc;
    int setSrc(std::string& to,const std::string& src);
    int loadSrc(std::string& to,const std::string& path);
    int setUniform(int location,float x);
    int setUniform(int location,float x,float y);
    int setUniform(int location,float x,float y,float z);
    int setUniform(int location,float x,float y,float z,float w);
    int setUniform(int location,int x);
    int setUniform(int location,int x,int y);
    int setUniform(int location,int x,int y,int z);
    int setUniform(int location,int x,int y,int z,int w);
    int setUniform(int location,unsigned int x);
    int setUniform(int location,unsigned int x,unsigned int y);
    int setUniform(int location,unsigned int x,unsigned int y,unsigned int z);
    int setUniform(int location,unsigned int x,unsigned int y,unsigned int z,unsigned int w);
    int setUniform(int location,float *x,unsigned char n,int count);
    int setUniform(int location,int *x,unsigned char n,int count);
    int setUniform(int location,unsigned int *x,unsigned char n,int count);
public:
    unsigned int program=0,vertexshader=0,fragmentshader=0;
    Shader(/* args */);
    // Shader(const std::string& vpath,const std::string& fpath);
    int setVertexShader(const std::string& src);
    int loadVertexShader(const std::string& path);
    int setFragmentShader(const std::string& src);
    int loadFragmentShader(const std::string& path);
    int compile(ShaderType type=VERTEX_FRAGMENT);
    int buildProgram();
    int setUniform(const std::string& name,float x);
    int setUniform(const std::string& name,float x,float y);
    int setUniform(const std::string& name,float x,float y,float z);
    int setUniform(const std::string& name,float x,float y,float z,float w);
    int setUniform(const std::string& name,int x);
    int setUniform(const std::string& name,int x,int y);
    int setUniform(const std::string& name,int x,int y,int z);
    int setUniform(const std::string& name,int x,int y,int z,int w);
    int setUniform(const std::string& name,unsigned int x);
    int setUniform(const std::string& name,unsigned int x,unsigned int y);
    int setUniform(const std::string& name,unsigned int x,unsigned int y,unsigned int z);
    int setUniform(const std::string& name,unsigned int x,unsigned int y,unsigned int z,unsigned int w);
    int setUniform(const std::string& name,float *x,unsigned char n,int count);
    int setUniform(const std::string& name,int *x,unsigned char n,int count);
    int setUniform(const std::string& name,unsigned int *x,unsigned char n,int count);
    int use();
    int destroy();
    ~Shader();
};
#endif