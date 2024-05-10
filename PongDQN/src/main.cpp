#include <uilib/window.h>
#include<cstdio>
#include<timer.h>
// #include<matrix.h>
#include<neurallib/neuralnet.h>
#include<mathlib/vector.h>
#include<gymlab/pongenv.h>
#include <cstdio>
#include <GL/GL.h>
#include <renderlib/ogl_ren.h>
#include <renderlib/render.h>
#include <renderlib/camera.h>
#include <renderlib/shapes/circle.h>
#include <renderlib/shapes/rect.h>
#include <filesystem>
#include <fstream>
#include<cmath>
#include<rllib/dqn.h>
#define ROLLSPEED_G 0.01f

#define BOT_CHASER 0
#define BOT_EDGE 1
#define PLAYER 2
#define BOT_AGENT 3
//-- Config:
    #define OPPONENT BOT_CHASER
    #define MODEL_ID "model3"
    // #define TRAIN  // Uncomment to run in training mode
//--
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProcMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void initwindow(UIlib::Window & window);
bool render=false;
Render::Opengl::Context newContext;
Render::Shapes::Circle* ballp;
Render::Shapes::Rect* rectp0;
Render::Shapes::Rect* rectp1;
Render::Camera cam;
Gymlab::Pongenv* envp=NULL;
Rllib::DQN* agentp=NULL;
bool speedup=false;

float constf(const float& x){
    return x;
}
float constderf(const float& x){
    return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Timer timer("time passed");
    timer.begin();
    #ifdef TRAIN
    std::ofstream logfile(std::string("../training/models/historylog_")+MODEL_ID+".csv");
    logfile<<"episode,netreward,epsilon\n";
    if(!logfile.is_open()) return -1;
    #else
    std::ofstream logfile(std::string("../training/models/testlog_")+MODEL_ID+".csv");
    logfile<<"episode,netreward,epsilon\n";
    if(!logfile.is_open()) return -1;
    #endif
    ballp=new Render::Shapes::Circle({300,300},50.0f);
    rectp0=new Render::Shapes::Rect({300,300},30.0f,30.0f);
    rectp1=new Render::Shapes::Rect({300,300},30.0f,30.0f);
    cam.setPos({0.0f,1800.0f});
    cam.setRes({2400,1800});
    printf("name:  %s\n",std::filesystem::current_path().string().c_str());
    UIlib::Window wind1(hInstance,L"class1",L"Window",0,0,800,600,WindowProc);
    HDC context=NULL;
    wind1.create();
    wind1.close();
    UIlib::Window mainwnd(hInstance,L"class2",L"PongDQN",0,0,800,600,WindowProcMain);
    mainwnd.create();
    
    initwindow(mainwnd);
    mainwnd.show(false);
    render=true;
    int fps=0;
    float timepassed=0;
    MSG msg = { };
    std::chrono::time_point<std::chrono::steady_clock> prevtime=std::chrono::steady_clock::now();
    
    #define OFFEP 1

    Rllib::DQN agent({6,12,12,12,2},100000);
    Rllib::DQN agent2({6,12,12,12,2},100000);

    agentp=&agent;
    //Load model
    // Agent 1
    // agent.load("../res/models/modelb.data");
    agent.load("../res/models/network_model3.data");
    // Agent 2
    agent2.load("../res/models/modelb.data");
    // agent2.load("../bin/modelb.dat");
    #ifndef TRAIN
    agent.epsilon=0;
    #endif
    agent2.epsilon=0;
    // exit(0);
    Gymlab::Pongenv env;
    envp=&env;
    env.init();
    // std::cout<<env.m_ballpos;
    float t=0.0f;
    auto state=std::vector<float>({env.m_ballpos.x/env.m_boardw,env.m_ballpos.y/env.m_boardh,env.m_ballvelocity.x,env.m_ballvelocity.y,
        env.m_p0.getPos().y/env.m_boardh,env.m_p1.getPos().y/env.m_boardh});
    auto state2=std::vector<float>({1-env.m_ballpos.x/env.m_boardw,env.m_ballpos.y/env.m_boardh,-env.m_ballvelocity.x,env.m_ballvelocity.y,
        env.m_p1.getPos().y/env.m_boardh,env.m_p0.getPos().y/env.m_boardh});
    auto pred=agent.predict(state);
    auto pred2=agent2.predict(state2);
    float netreward=0.0f;
    float maxreward=-100000.0f;
    size_t epoach=0;
    size_t episode=0;
    size_t fdropi=0;
    size_t fdrop=10;
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution dist=std::uniform_real_distribution(0.0f,1.0f);
    std::uniform_int_distribution dista=std::uniform_int_distribution(0,1);
    while (true)
    {
        // printf("loop\n");
        if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
            // printf("message\n");
            if(msg.message==WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            // MessageBoxA(NULL,"quit normally","version",0);
        }
        
        else{
        std::chrono::time_point<std::chrono::steady_clock> currt=std::chrono::steady_clock::now();
        float tempt=std::chrono::duration_cast<std::chrono::milliseconds>(currt-prevtime).count()/1000.0f;
        // printf("delta :%f\n",t);
        if(tempt+t>env.targetDelta()||speedup/* ||epoach<OFFEP */){
            size_t max=0;
            if(dist(mt)<agent.epsilon){
                max=dista(mt);
            }
            else{
                for (size_t i = 1; i < pred.size(); i++)
                {
                    
                    if(pred[max]<pred[i])max=i;
                }
            }
            size_t max2=0;
            if(dist(mt)<agent2.epsilon){
                max2=dista(mt);
            }
            else{
                for (size_t i = 1; i < pred2.size(); i++)
                {
                    
                    if(pred2[max2]<pred2[i])max2=i;
                }
            }
        for (size_t i = 0; i < pred.size(); i++)
        {
            if(std::isnan(pred[i])) throw 12;
        }
        env.m_p0.setAction((max==0)?1:-1);
        // env.m_p1.setAction((max2==0)?1:-1);
        #if OPPONENT==BOT_CHASER
            if(env.m_ballpos.y>env.m_p1.getPos().y)env.m_p1.setAction(1);
            else if(env.m_ballpos.y<env.m_p1.getPos().y)env.m_p1.setAction(-1);
            else env.m_p1.setAction(0);
        #elif OPPONENT==BOT_EDGE
            if(dist(mt)>0.5){
            if(env.m_ballpos.y>env.m_p1.getPos().y)env.m_p1.setAction(1);
            else if(env.m_ballpos.y<env.m_p1.getPos().y)env.m_p1.setAction(-1);
            else env.m_p1.setAction(0);
            }
            else{
                int temp=dista(mt);
                if(temp==1)
                env.m_p1.setAction(env.m_ballpos.y-(env.m_p1.getPos().y+env.m_p1.getSize().y/2.0f));
                else
                env.m_p1.setAction(env.m_ballpos.y-(env.m_p1.getPos().y-env.m_p1.getSize().y/2.0f));
            }
        #elif OPPONENT==PLAYER
            POINT currsor;
            RECT rect;
            GetWindowRect(mainwnd.hwnd,&rect);
            GetCursorPos(&currsor);
            // env.m_p1.setAction((1-(currsor.y/(float)(rect.bottom-rect.top)))*1200.0f-env.m_p1.getPos().y);
            env.m_p1.setPos({env.m_p1.getPos().x,(1-((currsor.y-rect.top)/(float)(rect.bottom-rect.top)))*1800.0f});
        #elif OPPONENT==BOT_AGENT
            env.m_p1.setAction((max2==0)?1:-1);
        #endif

        
        t+=tempt;
        // if(epoach>OFFEP)
        if(!speedup)
        t=env.step(t);
        else{
        
        for (size_t i = 0; i < 1; i++)
        {
            env.step();
        }
        t=0;
        }
        
        prevtime=currt;
        //render start
        ballp->setCenter(env.m_ballpos);
        rectp0->setPos(env.m_p0.getPos()-env.m_p0.getSize()/2.0f);
        rectp0->setSize(env.m_p0.getSize());
        rectp1->setPos(env.m_p1.getPos()-env.m_p1.getSize()/2.0f);
        rectp1->setSize(env.m_p1.getSize());
        // std::cout<< "cam "<<cam.getPos();
        // if(epoach>OFFEP)
        InvalidateRect(mainwnd.hwnd,NULL,FALSE);

        //render end
        float reward=0.0f;
        auto event=env.getEvents();
        // if(env.m_p0.getPos().y>600.0f)
        //     reward+=400;
        if(event.m_p0hit){
            reward+=1;

        }
        if(event.m_p1hit){
            reward-=1;

        }
        if(event.m_p0miss){
            reward-=50;

        }
        if(event.m_p1miss){
            reward+=50;

        }
        else{
            reward-=0;
        }
        // if(env.m_p0.getPos().y>700.0f&&env.m_p0.getPos().y<800.0f)
        //     reward+=400;
        // if(event.m_p0hit){
        //     reward+=5;

        // }
        // if(event.m_p0miss){
        //     reward-=50;

        // }
        // if(event.m_p1miss){
        //     reward+=100;

        // }
        // if(event.m_p1hit){
        //     reward-=5;

        // }
        // else{
        //     reward-=0;
        // }
        // if(event.m_p0hit){
        //     reward+=10;

        // }
        // if(event.m_p0miss){
        //     reward-=50;

        // }
        // if(event.m_p1miss){
        //     reward+=100;

        // }
        // else{
        //     reward-=0;
        // }
        // if(event.m_p0hit){
        //     reward+=0;//10;

        // }
        // if(event.m_p0miss){
        //     reward-=1;//50;

        // }
        // if(event.m_p1miss){
        //     reward+=1;//100;

        // }
        // else{
        //     reward-=0;
        // }
        netreward+=reward;
        auto nextstate=std::vector<float>({env.m_ballpos.x/env.m_boardw,env.m_ballpos.y/env.m_boardh,env.m_ballvelocity.x,env.m_ballvelocity.y,
        env.m_p0.getPos().y/env.m_boardh,env.m_p1.getPos().y/env.m_boardh});
        auto nextstate2=std::vector<float>({1-env.m_ballpos.x/env.m_boardw,env.m_ballpos.y/env.m_boardh,-env.m_ballvelocity.x,env.m_ballvelocity.y,
        env.m_p1.getPos().y/env.m_boardh,env.m_p0.getPos().y/env.m_boardh});
        #ifdef TRAIN
        agent.storeExperience(Rllib::Experience(state,max,reward,nextstate,env.getEvents().m_p0miss||env.getEvents().m_p1miss));
        for (size_t i = 0; i < 10; i++)
        {
            agent.train_step();
        }
        #endif
        // if(epoach<=OFFEP)
        epoach+=1;
        #ifdef TRAIN
        // if(epoach==OFFEP){
            
        //     t=0;
        //     prevtime=std::chrono::steady_clock::now();
        // }
        #endif
        state=nextstate;
        state2=nextstate2;
        // printf("noooo\n");
        pred=agent.predict(state);
        // printf("noooo\n");
        pred2=agent2.predict(state2);
        env.clearEvents();
        if(event.m_p0miss||event.m_p1miss){
            env.init();
            state=std::vector<float>({env.m_ballpos.x/env.m_boardw,env.m_ballpos.y/env.m_boardh,env.m_ballvelocity.x,env.m_ballvelocity.y,
        env.m_p0.getPos().y/env.m_boardh,env.m_p1.getPos().y/env.m_boardh});
            state2=std::vector<float>({1-env.m_ballpos.x/env.m_boardw,env.m_ballpos.y/env.m_boardh,-env.m_ballvelocity.x,env.m_ballvelocity.y,
        env.m_p1.getPos().y/env.m_boardh,env.m_p0.getPos().y/env.m_boardh});
            pred=agent.predict(state);
            // printf("noooo\n");
            pred2=agent2.predict(state2);
            // printf("noooo\n");
            episode++;
            if(epoach>0)
            printf("[%lu]:%lu net reward: %f max reward:%f %f\n",episode,epoach,netreward,maxreward,agent.epsilon);
            env.printScores();
            #ifdef TRAIN
            logfile<<episode<<','<<netreward<<','<<agent.epsilon<<'\n';
            if(episode%10==0) logfile.flush();
            // if(epoach>100)
            // printf("%lu mem: \n",agent.m_replay_mem_new);
            if(episode%10==0){
                agentp->save(std::string("../training/models/network_")+MODEL_ID+".data");
                timer.end();
            }
            #else
            logfile<<episode<<','<<netreward<<','<<agent.epsilon<<'\n';
            if(episode%10==0) logfile.flush();
            #endif
            if(netreward>maxreward)maxreward=netreward;
            netreward=0.0f;
        }
        
        }
        
        

        }
    }
    #ifdef TRAIN
    if(logfile.is_open()) logfile.close();
    #endif
    // exit(0);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:{
    // glGetString(GL_VERSION);
    Render::getpixelformat(hwnd);

    return 0;
    }
    case WM_DESTROY:{
        
        return 0;
    }
        

    case WM_PAINT:
        {
            return 0;
        }
        
    case WM_COMMAND:
    
        return 0;
    case WM_KEYDOWN:
    {

    }
    case WM_KEYUP:
       //bkey=false;
        // InvalidateRect(hwnd,NULL,false);
    return 0;
    case WM_CLOSE:
    DestroyWindow(hwnd);
    return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK WindowProcMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:{
    
    return 0;
    }
    case WM_DESTROY:

        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        {
            RECT rect;
            GetClientRect(hwnd,&rect);
            if(render){
                UIlib::Window *window=(UIlib::Window*)GetProp(hwnd,L"CKwindow");//(UIlib::Window*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
                int ret=newContext.makeCurrent(*window);
                if(ret) return 0;
                glViewport(0.0f,0.0f,rect.right-rect.left,rect.bottom-rect.top);
                // cam.setRes({(float)(rect.right-rect.left),(float)(rect.bottom-rect.top)});
                // printf("h %f w %f",(float)(rect.right-rect.left),(float)(rect.bottom-rect.top));
                newContext.detach();
            }
            return 0;
        }
    case WM_MOUSEWHEEL:
        {
            // cam.setZoom(cam.getZoom()-0.1);
            float rot=(float)(GET_WHEEL_DELTA_WPARAM(wParam))*WHEEL_DELTA;
            if(rot>0){
                (cam.m_zoom)+=ROLLSPEED_G;
            }
            else if((cam.m_zoom)>ROLLSPEED_G){
                (cam.m_zoom)-=ROLLSPEED_G;
            }
            InvalidateRect(hwnd,NULL,FALSE);
            return 0;
        }
    case WM_MOUSEMOVE:
        {
            return 0;
        }
    case WM_PAINT:
        {
            // printf("painted\n");
            // MessageBoxA(hwnd,(char*)(glGetString(GL_VERSION)),"version",0);
            if(render){
            PAINTSTRUCT ps;
            UIlib::Window *window=(UIlib::Window*)GetProp(hwnd,L"CKwindow");//(UIlib::Window*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
            if(window->hwnd!=hwnd) printf("got it\n");
            if(newContext.makeCurrent(*window)){
                return 0;
            }
            // printf("succ %u ogl:%p %p\n",oglc,hwnd,WindowFromDC(hdc));
            glGetError();
            glClearColor(0.0f,0.30f,0.1f,1.0f);
            
            glClear(GL_COLOR_BUFFER_BIT);

            Render::render(cam,*rectp0);
            Render::render(cam,*rectp1);
            Render::render(cam,*ballp);
            SwapBuffers(newContext.getHDC());
            ValidateRect(hwnd,NULL);
            newContext.detach();
            }
            
        }
        return 0;
    case WM_COMMAND:

    
        return 0;
    case WM_KEYDOWN:
    {
    // printf("down\n");
    //    bkey=true;
        if(GetAsyncKeyState(VK_UP)&0x8000){
            // printf("moved\n\n");
            envp->setAction(1,1.0f);
        }
        else if(GetAsyncKeyState(VK_DOWN)&0x8000){
            // printf("moved\n\n");
            envp->setAction(1,-1.0f);
        }
        if(GetAsyncKeyState('W')&0x8000){
            // printf("moved\n\n");
            envp->setAction(0,1.0f);
        }
        else if(GetAsyncKeyState('S')&0x8000){
            // printf("moved\n\n");
            envp->setAction(0,-1.0f);
        }
        if(GetAsyncKeyState('L')&0x8000){
            #ifdef TRAIN
            agentp->save(std::string("../training/models/networklog_")+MODEL_ID+".data");
            #endif
        }
        if(GetAsyncKeyState('P')&0x8000){
            scanf("%*c");
        }
        if(GetAsyncKeyState('M')&0x8000){
            speedup=!speedup;
            if(speedup==0){

            }
        }
        // else{
        //     envp->setAction(0,0.0f);
        // }
            // InvalidateRect(hwnd,NULL,false);
        return 0;
    }
    case WM_KEYUP:
    {   //bkey=false;
        // InvalidateRect(hwnd,NULL,false);
        if(GetAsyncKeyState(VK_UP)&0x8000){
            // printf("moved\n\n");
            envp->setAction(1,1.0f);
        }
        else if(GetAsyncKeyState(VK_DOWN)&0x8000){
            // printf("moved\n\n");
            envp->setAction(1,-1.0f);
        }
        else{
            envp->setAction(1,0.0f);
        }
        if(GetAsyncKeyState('W')&0x8000){
            // printf("moved\n\n");
            envp->setAction(0,1.0f);
        }
        else if(GetAsyncKeyState('S')&0x8000){
            // printf("moved\n\n");
            envp->setAction(0,-1.0f);
        }
        else{
            envp->setAction(0,0.0f);
        }
    return 0;
    }
    case WM_CLOSE:
    // newContext.destroy();
    DestroyWindow(hwnd);
    render=false;
    return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void initwindow(UIlib::Window &window)
{
    wprintf(L"title: %s\n",window.title.c_str());
    // wind1.close();
    HDC context=GetDC(window.hwnd);
    Render::setpixelformat(window.hwnd,&context);
    ReleaseDC(window.hwnd,context);
    // oglc=Render::init_opengl(context);//here1
    int ret=newContext.create(window);
    if(ret) {
        printf("context creation failed\n");
        return ;
    }
    printf("is crea %p %p\n",Render::Openglengine::currentglc,&newContext);
    // printf("is current %d\n",(int)newContext.isCurrent());
    int fa=newContext.loadFunctions(); //Render::Openglengine::loadfunctions();
    printf("fails:%d\n",fa);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
    newContext.detach();
}
