#include<chrono>
#include<cstdio>
#include<string>
#include<timer.h>

Timer::Timer(std::string name)
{
    this->name=name;
    
}

Timer::~Timer()
{
    
}

void Timer::begin()
{
    this->start=std::chrono::steady_clock::now();
}

void Timer::end()
{
    double t=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-start).count()/1000000000.0;
    printf("%s:%lfs\n",name.c_str(),t);
}
