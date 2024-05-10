#ifndef REN_RECT_H
#define REN_RECT_H
#include<vector.h>
#include<renderlib/shader.h>
#include<renderlib/camera.h>
namespace Render
{
    namespace Shapes
    {
        class Rect
        {
        private:
            Mathlib::Vec2<float> m_pos;
            Mathlib::Vec2<float> m_size;
            // float m_height=1.0f;
            // float m_width=1.0f;
        public:
            unsigned int VBO=0,VAO=0;
            static bool isShaderready;
            static Shader shader;
            static void initShader();
            void update(Render::Camera& cam);
            void setPos(const Mathlib::Vec2<float> pos);
            void setSize(const Mathlib::Vec2<float> size);
            Rect(/* args */)=default;
            Rect(const Mathlib::Vec2<float> pos,float w,float h);
            ~Rect();
        };

        
        
    } // namespace Shapes
    
} // namespace Render
#endif