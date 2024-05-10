#include<renderlib/camera.h>
namespace Render{
Camera::Camera(/* args */)
{
}
Mathlib::Vec2<float> Camera::getPos(){
    return m_pos;
}
Mathlib::Vec2<float> Camera::getRes(){
    return m_res;
}
void Camera::setPos(const Mathlib::Vec2<float> &pos)
{
    m_pos=pos;
}

void Camera::setRes(const Mathlib::Vec2<float> &res)
{
    m_res=res;
}

Mathlib::Vec2<float> Camera::transform(const Mathlib::Vec2<float> &point)
{
    Mathlib::Vec2<float> out=point;
    out.x-=m_pos.x+m_res.x/2.0f;
    out.y-=m_pos.y-m_res.y/2.0f;
    out*=m_zoom;
    out.x+=m_res.x/2.0f;
    out.y+=-m_res.y/2.0f;
    out.x*=2.0f/m_res.x;
    out.y*=2.0f/m_res.y;

    out.x--;
    out.y++;
    // out.y=-out.y;
    return out;
}

Mathlib::Vec2<float> Camera::scale(const Mathlib::Vec2<float> &vec)
{
    Mathlib::Vec2<float> out=vec;
    out.x*=(2.0f/m_res.x);
    out.x--;
    out.y*=2.0f/m_res.y;
    out.y--;
    return out;
}

float Camera::getH()
{
    return m_res.y;
}

float Camera::getW()
{
    return m_res.x;
}

Camera::~Camera()
{
}
}
float Render::Camera::getZoom()
{
    return m_zoom;
}

void Render::Camera::setZoom(float zoom)
{
    m_zoom=zoom;
}

Mathlib::Matrix<float, Mathlib::Matstorage> Render::Camera::getTransform()
{
    std::vector<std::vector<float>> matscale={
        std::vector<float>({2.0f/getRes().x*m_zoom,0.0f,-1.0f}),
        std::vector<float>({0.0f,2.0f/getRes().y*m_zoom,-1.0f}),
        std::vector<float>({0.0f,0.0f,1.0f})
    };
    Mathlib::Matrix<float,Mathlib::Matstorage> mat(3,3);
    Mathlib::Matrix<float,Mathlib::Matstorage> mat2(3,3);
    std::vector<std::vector<float>> mattranslation={
        std::vector<float>({1.0f,0.0f,1}),
        std::vector<float>({0.0f,1.0f,1}),
        std::vector<float>({0.0f,0.0f,1.0f})
    };
    mat.set(mattranslation);
    mat2.set(matscale);
    mat.mul(mat2);
    mattranslation={
        std::vector<float>({1.0f,0.0f,-getW()/2.0f}),
        std::vector<float>({0.0f,1.0f,-getH()/2.0f}),
        std::vector<float>({0.0f,0.0f,1.0f})
    };
    mat2.set(mattranslation);
    mat.mul(mat2);
    
    mattranslation={
        std::vector<float>({1.0f,0.0f,-getPos().x}),
        std::vector<float>({0.0f,1.0f,-getPos().y}),
        std::vector<float>({0.0f,0.0f,1.0f})
    };
    mat2.set(mattranslation);
    mat.mul(mat2);
    return mat;
}
