#include <renderlib/shapes/rect.h>
#include <renderlib/shader.h>
namespace Render
{
    namespace Shapes
    {
        Shader Rect::shader=Shader();
        bool Rect::isShaderready=false;
        void Rect::initShader()
        {
            shader.loadVertexShader("..\\res\\shaders\\rect\\rect.vert");
            shader.loadFragmentShader("..\\res\\shaders\\rect\\rect.frag");
            shader.compile();
            shader.buildProgram();
            isShaderready = true;
        }
        void Rect::update(Render::Camera &cam)
        {
            if (Rect::isShaderready != true)
            {

                Rect::initShader();
            }
            ::Mathlib::Vec2<float> ver[4] = {
                {m_pos.x, m_pos.y},
                {m_pos.x + m_size.x, m_pos.y},
                {m_pos.x, m_pos.y + m_size.y},
                {m_pos.x+m_size.x, m_pos.y + m_size.y},
            };
            Mathlib::Vec2<float> t;
            for (size_t i = 0; i < 4; i++)
            {
                t = cam.transform(ver[i]);
                ver[i].set(t.x, t.y);
            }
            Mathlib::Vec2<float> textcord[4] = {
                {0.0f, 0.0f},
                {1.0f, 0.0f},
                {0.0f, 1.0f},
                {1.0f, 1.0f},
            };
            if (VAO == 0)
            {
                printf("init rect\n");

                // unsigned int VBO,VAO,EBO;
                unsigned int VTXBO = 0;

                Openglengine::currentglc->glGenBuffers(1, &VBO);
                Openglengine::currentglc->glGenBuffers(1, &VTXBO);
                Openglengine::currentglc->glBindBuffer(GL_ARRAY_BUFFER, VBO);
                // Openglengine::glBindBuffer(GL_ARRAY_BUFFER,VTXBO);
                Openglengine::currentglc->glGenVertexArrays(1, &VAO);
                Openglengine::currentglc->glBindVertexArray(VAO);
                // Openglengine::currentglc->glBufferData(GL_ARRAY_BUFFER,sizeof(::Mathlib::Vec2<float>)*4,ver,GL_STATIC_DRAW);
                Openglengine::currentglc->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
                Openglengine::currentglc->glEnableVertexAttribArray(0);
                Openglengine::currentglc->glBindBuffer(GL_ARRAY_BUFFER, VTXBO);
                Openglengine::currentglc->glBufferData(GL_ARRAY_BUFFER, sizeof(Mathlib::Vec2<float>) * 4, textcord, GL_STATIC_DRAW);
                Openglengine::currentglc->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
                Openglengine::currentglc->glEnableVertexAttribArray(1);
                // Openglengine::currentglc->glVertexAttribPointer(2,1,GL_FLOAT,GL_FALSE,0,NULL);
                // Openglengine::currentglc->glEnableVertexAttribArray(2);
                // printf("hiiii\n");

                // prev=std::chrono::steady_clock::now();
            }
            Openglengine::currentglc->glBindBuffer(GL_ARRAY_BUFFER, VBO);
            Openglengine::currentglc->glBufferData(GL_ARRAY_BUFFER, sizeof(::Mathlib::Vec2<float>) * 4, ver, GL_STATIC_DRAW);
        }

        void Rect::setPos(const Mathlib::Vec2<float> pos)
        {
            m_pos = pos;
        }

        void Rect::setSize(const Mathlib::Vec2<float> size)
        {
            m_size=size;
        }
        Rect::Rect(const Mathlib::Vec2<float> pos, float w, float h) : m_pos(pos), m_size({w, h})
        {
        }

        Rect::~Rect()
        {
        }
    }
}
