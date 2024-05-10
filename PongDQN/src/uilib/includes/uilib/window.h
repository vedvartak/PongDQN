#ifndef UNICODE
#define UNICODE
#endif
#ifndef WINDOWLIB
#define WINDOWLIB
#include<windows.h>
#include<string>
namespace UIlib{
    // class windowBase
    // {
    // private:
    //     /* data */
    // public:
    //     windowBase(/* args */);
    //     ~windowBase();
    // };
    
    // windowBase::windowBase(/* args */)
    // {
    // }
    
    // windowBase::~windowBase()
    // {
    // }
    
class Window{
    public:
    HINSTANCE hInstance; 
    HWND hwnd;
    std::wstring title;
    std::wstring className;
    int x,y,w,h;
    Window(HINSTANCE hInstance,std::wstring classN,std::wstring title,int x,int y,int w,int h,LRESULT (CALLBACK *WindowProc)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam));
    int create();
    int show(bool isFull);
    int close();
    bool isCreated() const;
};
// class Button
// {
// private:
    
// public:

//     Button(/* args */);
//     ~Button();
// };

// Button::Button(/* args */)
// {
// }

// Button::~Button()
// {
// }
}
#endif