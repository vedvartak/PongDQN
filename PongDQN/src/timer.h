#ifndef TIMER_H
#define TIMER_H
#include<chrono>
#include<cstdio>
#include<string>
class Timer
{
private:
    std::string name;
    std::chrono::time_point<std::chrono::steady_clock> start;

public:

    Timer(std::string name);
    void begin();
    void end();
    ~Timer();
};
#endif