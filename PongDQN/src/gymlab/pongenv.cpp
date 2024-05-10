#include<pongenv.h>
#include<algorithm>
#include<random>
namespace Gymlab{
    void Pad::setPos(const Mathlib::Vec2<float> &pos)
    {
        m_pos=pos;
    }

    float Pad::getAction()
    {
        return m_action;
    }

    

    void Pad::setAction(float action)
    {

        m_action=std::max(std::min(action,1.0f),-1.0f);
    }

    float Pad::getSpeed()
    {
        return m_speed;
    }

    Mathlib::Vec2<float> Pad::getPos()
    {
        return m_pos;
    }

    Mathlib::Vec2<float> Pad::getSize()
    {
        return {m_width,m_height};
    }

    // bool Pad::testCollisionBroad(Mathlib::Vec2<float> &ballpos, float ballsize)
    // {
    //     return false;
    // }

    // void Pad::resolveCollision(Mathlib::Vec2<float> &ballpos, float ballsize)
    // {

    // }

    Pad::Pad(Mathlib::Vec2<float> pos) : m_pos(pos)
    {
    }
    
    Pad::~Pad()
    {
    }
    void Pongenv::resolveBoardCollisions()
    {
        if(m_ballpos.y-m_ballsize<0.0f){
            // printf("collided bottom\n");
            m_ballpos.y=m_ballsize;
            m_ballvelocity.y=-m_ballvelocity.y;
        }
        if(m_ballpos.y+m_ballsize>m_boardh){
            // printf("collided top %f\n",m_ballpos.y);
            m_ballpos.y=m_boardh-m_ballsize;
            m_ballvelocity.y=-m_ballvelocity.y;
        }
        if(m_ballpos.x-m_ballsize<0.0f){
            // printf("collided left\n");
            m_lastevent.m_p0miss=true;
            score1++;
            m_ballpos.x=m_ballsize;
            m_ballvelocity.x=-m_ballvelocity.x;
        }
        if(m_ballpos.x+m_ballsize>m_boardw){
            // printf("collided right\n");
            score0++;
            m_lastevent.m_p1miss=true;
            m_ballpos.x=m_boardw-m_ballsize;
            m_ballvelocity.x=-m_ballvelocity.x;
        }
    }

    bool Pongenv::resolvePadCollisions(Pad& pad)
    {
        float distX=std::abs(m_ballpos.x-pad.getPos().x);
        float distY=std::abs(m_ballpos.y-pad.getPos().y);
        if(distX>(pad.getSize().x/2.0f+m_ballsize)) return false;
        if(distY>(pad.getSize().y/2.0f+m_ballsize)) return false;
        
        if(distY<(pad.getSize().y/2.0f)) {
            // m_ballvelocity.x=-m_ballvelocity.x;
            m_ballvelocity.x=-m_ballvelocity.x;
            if(m_ballpos.x>pad.getPos().x){
                m_ballpos.x=pad.getPos().x+pad.getSize().x/2.0f+m_ballsize;
                
            }
            else
                m_ballpos.x=pad.getPos().x-pad.getSize().x/2.0f-m_ballsize;
            // printf("collision Y %f %f\n",pad.getPos().x+pad.getSize().x/2.0f,m_ballpos.x);
            // scanf("%*c");
            m_ballvelocity.y=(m_ballpos.y-pad.getPos().y)*2.0f/pad.getSize().y;
            m_ballvelocity.normalise();
            return true;
        }
        else if(distX<(pad.getSize().x/2.0f)) {
            
            if(m_ballpos.y>pad.getPos().y){
                m_ballpos.y=pad.getPos().y+pad.getSize().y/2.0f+m_ballsize;
                m_ballvelocity.y=std::abs(m_ballvelocity.y);
            }
            else if(m_ballpos.y<pad.getPos().y){
                m_ballpos.y=pad.getPos().y-pad.getSize().y/2.0f-m_ballsize;
                m_ballvelocity.y=-std::abs(m_ballvelocity.y);
            }
            // else
            //     exit(0);
            // m_ballpos+=m_ballvelocity*m_ballspeed*m_targetdt;
            // m_ballvelocity.x=-m_ballvelocity.x;
            // m_ballpos.x=pad.getPos().x+pad.getSize().x/2.0f+m_ballsize;
            printf("collision\n");
            // scanf("%*c");
            return true;
        }
        else{
            float dx=distX-pad.getSize().x/2.0f;
            float dy=distY-pad.getSize().y/2.0f;
            if(dx*dx+dy*dy<=(m_ballsize*m_ballsize)){
                
                // if(m_ballpos.x>pad.getPos().x)
                //     m_ballvelocity=m_ballpos-(pad.getPos()+pad.getSize()/2.0f);
                // else
                //     m_ballvelocity=m_ballpos-(pad.getPos()-pad.getSize()/2.0f);
                m_ballvelocity.x=-m_ballvelocity.x;
                m_ballvelocity.y=(m_ballpos.y-pad.getPos().y)/pad.getSize().y;
                // m_ballvelocity.normalise();
                m_ballvelocity.normalise();
                // if(m_ballvelocity.x<100)m_ballvelocity.x=1200.0f;
                // m_ballvelocity.normalise();

                // }
                return true;
            }
        }
        return false;
    }
    

    int Pongenv::init()
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution dist=std::uniform_real_distribution(-1.0f,1.0f);
        m_p0.setPos({m_edgepadding,m_boardh/2.0f});
        m_p1.setPos({m_boardw-m_edgepadding,m_boardh/2.0f});
        // m_ballpos.set(0,0);
        m_ballpos.set(m_boardw/2.0f,m_boardh/2.0f);
        // m_ballvelocity.set(0.0f,0.0f);
        auto rx=dist(mt);
        m_ballvelocity.set((rx+2)*(dist(mt)>0?1.0f:-1.0f),dist(mt));
        // m_ballvelocity.set(dist(mt)-2,dist(mt));
        m_ballvelocity.normalise();
        return 0;
    }

    Pongenv::Pongenv(/* args */)
    {

    }
    PongEvents Pongenv::getEvents()
    {
        // PongEvents temp=m_lastevent;
        // m_lastevent=PongEvents();
        return m_lastevent;
    }
    void Pongenv::clearEvents()
    {
        m_lastevent=PongEvents();
    }
    void Pongenv::printScores()
    {
        std::cout<<"score : "<<score0<<" | "<<score1<<'\n';
    }
    int Pongenv::setAction(bool player, float action)
    {
        if(!player) m_p0.setAction(action);
        else m_p1.setAction(action);
        return 0;
    }
    float Pongenv::targetDelta(){
        return m_targetdt;
    }
    void Pongenv::step()
    {
        
        m_ballpos+=m_ballvelocity*m_ballspeed*m_targetdt;
        m_p0.m_pos+=Mathlib::Vec2<float>(0,1.0f)*(float)(m_p0.getAction())*(m_p0.getSpeed())*m_targetdt;
        m_p1.m_pos+=Mathlib::Vec2<float>(0,1.0f)*(float)(m_p1.getAction())*(m_p1.getSpeed())*m_targetdt;
        //p0
        if((m_p0.m_pos.y-m_p0.m_height/2.0f)<0.0f) m_p0.setPos({m_p0.m_pos.x,m_p0.m_height/2.0f});
        if((m_p0.m_pos.y+m_p0.m_height/2.0f)>m_boardh) m_p0.setPos({m_p0.m_pos.x,m_boardh-m_p0.m_height/2.0f});
        //p1
        if((m_p1.m_pos.y-m_p1.m_height/2.0f)<0.0f) m_p1.m_pos.set(m_p1.m_pos.x,m_p1.m_height/2.0f);
        if((m_p1.m_pos.y+m_p1.m_height/2.0f)>m_boardh) m_p1.m_pos.set(m_p1.m_pos.x,m_boardh-m_p1.m_height/2.0f);
        //board collision detection
        //top
        resolveBoardCollisions();
        if(resolvePadCollisions(m_p0)){
            m_lastevent.m_p0hit=true;
        }
        if(resolvePadCollisions(m_p1)){
            m_lastevent.m_p1hit=true;
        }
        // resolvePadCollisions(m_p1);

    }

    float Pongenv::step(float delta)
    {
        // printf("step %f",delta);
        // m_lastevent=PongEvents();
        while (delta>m_targetdt)
        {
            step();
            delta-=m_targetdt;
        }
        // printf("returned");
        return delta;
    }

    void Pongenv::clearScores()
    {
        score0=0;
        score1=0;
    }

    Pongenv::~Pongenv()
    {
    }
}