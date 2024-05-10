#ifndef REN_CIRCLE_H
#define REN_CIRCLE_H
#include<vector.h>
#include<renderlib/shader.h>
#include<renderlib/camera.h>
namespace Render
{
    namespace Shapes
    {
        class Circle
        {
        private:
            Mathlib::Vec2<float> m_center;
            float m_r=1.0f;
        public:
            unsigned int VBO=0,VAO=0;
            static bool isShaderready;
            static Shader shader;
            static void initShader();
            void update(Render::Camera& cam);
            void setCenter(const Mathlib::Vec2<float> center);
            Circle(/* args */)=default;
            Circle(const Mathlib::Vec2<float> center,float r);
            ~Circle();
        };

        
        
    } // namespace Shapes
    
} // namespace Render
#endif