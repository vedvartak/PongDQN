#ifndef UNICODE
#define UNICODE
#endif 

#include <uilib/window.h>
#include<cstdio>
#include<string>
namespace UIlib{
    Window::Window(HINSTANCE hInstance,std::wstring classN,std::wstring title,int x,int y,int w,int h,LRESULT (CALLBACK *WindowProc)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)){
    this->title=title;
    this->hInstance=hInstance;
    className=classN;
    this->x=x;
    this->y=y;
    this->w=w;
    this->h=h;
    WNDCLASS wc = { };
    
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = className.c_str();
    //wc.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(290));
    wc.hCursor=LoadCursor(NULL,IDC_CROSS);
    wc.style=0;
    // wc.hbrBackground
    wc.hbrBackground=NULL;
    if(RegisterClass(&wc)==0)
    printf("error can't register wndclass\n");
    
    // Create the window.
    
    
    
    }
    int Window::create(){
        hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        className.c_str(),                     // Window class
        title.c_str(),    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        x, y,w, h,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }
    // SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)this);
    SetProp(hwnd,L"CKwindow",this);
    return 1;
    }
    int Window::show(bool isFull){
        
        ShowWindow(hwnd,isFull ? SW_MAXIMIZE : SW_SHOW);
        return 0;
    }
    int Window::close(){
        if(hwnd==NULL) return 1;
        
        return DestroyWindow(this->hwnd);
    }
    bool Window::isCreated() const{ 
        return ((hwnd==NULL) ? false:true);
    }
}

