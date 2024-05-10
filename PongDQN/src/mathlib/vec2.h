#ifndef MATHLIB_VEC2_H
#define MATHLIB_VEC2_H
#include<iostream>
#include<cmath>
namespace Mathlib
{
    template<typename _ty>
    class Vec2
    {
    private:
        /* data */
    public:
        _ty x,y;
        Vec2(/* args */);
        Vec2(_ty inx,_ty iny);
        void set(_ty x,_ty y);
        void normalise();
        Vec2<_ty>& operator*=(const _ty& constant);
        Vec2<_ty>& operator/=(const _ty& constant);
        
        Vec2<_ty> operator-();
        Vec2<_ty>& operator+=(const Vec2<_ty>& vec);
        Vec2<_ty>& operator-=(const Vec2<_ty>& vec);
        static _ty dot(const Vec2<_ty>& vec1,const Vec2<_ty>& vec2);
        ~Vec2();
    };
    template<typename _ty>
    Vec2<_ty> operator*(const Vec2<_ty>& vec,const _ty& constant){
        return Vec2<_ty>(vec.x*constant,vec.y*constant);
    }
    template<typename _ty>
    Vec2<_ty> operator/(const Vec2<_ty>& vec,const _ty& constant){
        if(constant==0.0f) return vec;
        return Vec2<_ty>(vec.x/constant,vec.y/constant);
    }
    template<typename _ty>
    Vec2<_ty> operator+(const Vec2<_ty>& vec1,const Vec2<_ty>& vec2){
        return Vec2<_ty>(vec1.x+vec2.x,vec1.y+vec2.y);
    }
    template<typename _ty>
    Vec2<_ty> operator-(const Vec2<_ty>& vec1,const Vec2<_ty>& vec2){
        return Vec2<_ty>(vec1.x-vec2.x,vec1.y-vec2.y);
    }
    template<typename _ty>
    std::ostream& operator<<(std::ostream& cout,const Vec2<_ty>& vec){
        cout<<"x:"<<vec.x<<" y:"<<vec.y<<"\n";
        return cout;
    }
    template<typename _ty>
    Vec2<_ty>::Vec2(/* args */)
    {
        
    }
    template <typename _ty>
    Vec2<_ty>::Vec2(_ty inx, _ty iny):x(inx),y(iny)
    {

    }
    template <typename _ty>
    void Vec2<_ty>::set(_ty x, _ty y)
    {
        this->x=x;
        this->y=y;
    }
    template <typename _ty>
    void Vec2<_ty>::normalise(){
        _ty mag=std::sqrt(x*x+y*y);
        if(mag==0.0f) return;
        this->set(x/mag,y/mag);
    }
    template <typename _ty>
    Vec2<_ty> Vec2<_ty>::operator-(){
        return {-x,-y};
    }
    template <typename _ty>
    Vec2<_ty> &Vec2<_ty>::operator*=(const _ty& constant)
    {
        this->set(x*constant,y*constant);
        return *this;
    }
    template <typename _ty>
    Vec2<_ty> &Vec2<_ty>::operator/=(const _ty& constant)
    {
        this->set(x/constant,y/constant);
        return *this;
    }
    
    template <typename _ty>
    Vec2<_ty> &Vec2<_ty>::operator+=(const Vec2<_ty> &vec)
    {
        this->set(x+vec.x,y+vec.y);
        return *this;
    }
    template <typename _ty>
    Vec2<_ty> &Vec2<_ty>::operator-=(const Vec2<_ty> &vec)
    {
        this->set(x-vec.x,y-vec.y);
        return *this;
    }
    template <typename _ty>
    _ty Vec2<_ty>::dot(const Vec2<_ty>& vec1,const Vec2<_ty>& vec2){
        return vec1.x*vec2.x+vec1.y*vec2.y;
    }
    template <typename _ty>
    Vec2<_ty>::~Vec2()
    {
    }
    
} // namespace Mathlib
#endif
