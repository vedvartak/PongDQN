#include<renderlib/shapes/circle.h>
#include<renderlib/render.h>
#include<renderlib/camera.h>
#include<vector.h>

namespace Render
{
    namespace Shapes
    {
        Circle::Circle(const Mathlib::Vec2<float> center, float r):m_center(center),m_r(r)
        {

        }
        void Circle::setCenter(const Mathlib::Vec2<float> center){
            m_center=center;
        }
        Circle::~Circle()
        {
        }
        Shader Circle::shader=Shader();
        bool Circle::isShaderready=false;
        void Circle::initShader(){
            shader.loadVertexShader("..\\res\\shaders\\circle\\circle.vert");
            shader.loadFragmentShader("..\\res\\shaders\\circle\\circle.frag");
            shader.compile();
            shader.buildProgram();
            isShaderready=true;
        }
        void Circle::update(Render::Camera& cam)
        {
            if(Circle::isShaderready!=true){
                
                Circle::initShader();
            }
            ::Mathlib::Vec2<float> ver[4]={
                {m_center.x-m_r,m_center.y-m_r},
                {m_center.x+m_r,m_center.y-m_r},
                {m_center.x-m_r,m_center.y+m_r},
                {m_center.x+m_r,m_center.y+m_r},
            };
            Mathlib::Vec2<float> t;
            for (size_t i = 0; i < 4; i++)
            {
                t=cam.transform(ver[i]);
                ver[i].set(t.x,t.y);
            }
            
            // ::Mathlib::Vec2<float> ver[4]={
            //     {-0.5f,-0.5f},
            //     {0.5f,-0.5f},
            //     {-0.5f,0.5f},
            //     {0.5f,0.5f}
            // };
            Vec2 textcord[4]={
                    {0.0f,0.0f},
                    {1.0f,0.0f},
                    {0.0f,1.0f},
                    {1.0f,1.0f},
            };
            if(VAO==0){
                printf("init circlw\n");
                
                // unsigned int VBO,VAO,EBO;
                unsigned int VTXBO=0;

                Openglengine::currentglc->glGenBuffers(1,&VBO);
                Openglengine::currentglc->glGenBuffers(1,&VTXBO);
                Openglengine::currentglc->glBindBuffer(GL_ARRAY_BUFFER,VBO);
                // Openglengine::glBindBuffer(GL_ARRAY_BUFFER,VTXBO);
                Openglengine::currentglc->glGenVertexArrays(1,&VAO);
                Openglengine::currentglc->glBindVertexArray(VAO);
                // Openglengine::currentglc->glBufferData(GL_ARRAY_BUFFER,sizeof(::Mathlib::Vec2<float>)*4,ver,GL_STATIC_DRAW);
                Openglengine::currentglc->glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,NULL);
                Openglengine::currentglc->glEnableVertexAttribArray(0);
                Openglengine::currentglc->glBindBuffer(GL_ARRAY_BUFFER,VTXBO);
                Openglengine::currentglc->glBufferData(GL_ARRAY_BUFFER,sizeof(Vec2)*4,textcord,GL_STATIC_DRAW);
                Openglengine::currentglc->glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,NULL);
                Openglengine::currentglc->glEnableVertexAttribArray(1);
                // Openglengine::currentglc->glVertexAttribPointer(2,1,GL_FLOAT,GL_FALSE,0,NULL);
                // Openglengine::currentglc->glEnableVertexAttribArray(2);
                printf("hiiii\n");

                // prev=std::chrono::steady_clock::now();
            }
            Openglengine::currentglc->glBindBuffer(GL_ARRAY_BUFFER,VBO);
            Openglengine::currentglc->glBufferData(GL_ARRAY_BUFFER,sizeof(::Mathlib::Vec2<float>)*4,ver,GL_STATIC_DRAW);
            
        }
    } // namespace Shapes

    

} // namespace Render
