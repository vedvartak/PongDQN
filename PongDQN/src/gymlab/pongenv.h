#ifndef GYMLAB_PONGENV_H
#define GYMLAB_PONGENV_H
#include<mathlib/vector.h>
#include<renderlib/shapes/circle.h>
namespace Gymlab
{
    // class Scene
    // {
    // private:
    //     Render::Shapes::Circle ball=Render::Shapes::Circle({0,0},1.0f);
    // public:
    //     Scene(/* args */);
    //     void draw();
    //     ~Scene();
    // };
    
    // Scene::Scene(/* args */)
    // {
    // }

    // inline void Scene::draw()
    // {
        
    // }

    // Scene::~Scene()
    // {
    // }
    class PongEvents
    {
    private:
    public:
        bool m_p0miss=false;
        bool m_p1miss=false;
        bool m_p0hit=false;
        bool m_p1hit=false;
        PongEvents(/* args */)=default;
        ~PongEvents()=default;
    };
    
    class Pad
    {
    private:
        float m_action=0;
        float m_speed=700.0f;//here300.0f;
    public:
        float m_height=500.0f;
        float m_width=80.0f;
        Mathlib::Vec2<float> m_pos;
        Pad(/* args */)=default;
        void setPos(const Mathlib::Vec2<float>& pos);
        float getAction();
        void setAction(float action);
        float getSpeed();
        Mathlib::Vec2<float> getPos();
        Mathlib::Vec2<float> getSize();
        // bool testCollisionBroad(Mathlib::Vec2<float>& ballpos,float ballsize);
        // void resolveCollision(Mathlib::Vec2<float>& ballpos,float ballsize);
        Pad(Mathlib::Vec2<float> pos);
        ~Pad();
    };

    
    
    class Pongenv
    {
    private:
        float m_targetdt=0.03f;//s
        PongEvents m_lastevent;
        void resolveBoardCollisions();
        bool resolvePadCollisions(Pad& pad);
        unsigned int score0=0;
        unsigned int score1=0;
    public:
        float m_boardw=2400.0f;
        float m_boardh=1800.0f;
        float m_edgepadding=70.0f;
        Mathlib::Vec2<float> m_ballpos;
        Mathlib::Vec2<float> m_ballvelocity;
        float m_ballsize=50.0f;
        float m_ballspeed=1800.0f;//1200.0f;//600.0f;

        Pad m_p0;
        Pad m_p1;

        Pongenv(/* args */);
        int init();
        int setAction(bool player,float action);
        float targetDelta();
        void step();
        float step(float delta);
        PongEvents getEvents();
        void clearEvents();
        void printScores();
        void clearScores();
        ~Pongenv();
    };
    
} // namespace gymlab
#endif