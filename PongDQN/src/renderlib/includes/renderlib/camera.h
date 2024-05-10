#ifndef REN_CAMERA_H
#define REN_CAMERA_H
#include<Mathlib/vector.h>
#include<Mathlib/matrix.h>
#include<Mathlib/matstorage.h>
namespace Render{
class Camera
{
private:
    Mathlib::Vec2<float> m_pos;
    Mathlib::Vec2<float> m_res=Mathlib::Vec2<float>(600,600);
public:
    float m_zoom=1.0f;
    Camera(/* args */);
    Mathlib::Vec2<float> getPos();
    Mathlib::Vec2<float> getRes();
    float getZoom();
    void setPos(const Mathlib::Vec2<float>& pos);
    void setRes(const Mathlib::Vec2<float>& res);
    void setZoom(float zoom);
    Mathlib::Matrix<float,Mathlib::Matstorage> getTransform();
    Mathlib::Vec2<float> transform(const Mathlib::Vec2<float>& point);
    Mathlib::Vec2<float> scale(const Mathlib::Vec2<float>& vec);
    float getH();
    float getW();
    ~Camera();
};


}
#endif