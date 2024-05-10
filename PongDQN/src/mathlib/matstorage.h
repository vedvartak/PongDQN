#ifndef MATHLIB_MATSTORAGE_H
#define MATHLIB_MATSTORAGE_H
#include <vector>
namespace Mathlib{
template<typename _ty>
class Matstorage
{
private:
    std::vector<std::vector<_ty>> data;

public:
    Matstorage()=default;
    Matstorage(unsigned int rows,unsigned int columns);
    void set(std::vector<std::vector<_ty>> values);
    void transpose();
    ~Matstorage();
    _ty& operator()(size_t i,size_t j);
    _ty get(unsigned int i,unsigned int j) const;
    
    void set(unsigned int i,unsigned int j,const _ty& value);
};
template<typename _ty>
Matstorage<_ty>::Matstorage(unsigned int rows,unsigned int columns)
{
    data=std::vector<std::vector<_ty>>(rows,std::vector<_ty>(columns));
    
}
template<typename _ty>
Matstorage<_ty>::~Matstorage()
{
}
template <typename _ty>
inline _ty &Matstorage<_ty>::operator()(size_t i, size_t j)
{
    return data[i][j];
}
template <typename _ty>
inline _ty Matstorage<_ty>::get(unsigned int i, unsigned int j) const
{
    return data[i][j];
}



template <typename _ty>
inline void Matstorage<_ty>::set(unsigned int i, unsigned int j, const _ty &value)
{
    data[i][j]=value;
    return;
}

template <typename _ty>
inline void Matstorage<_ty>::set(std::vector<std::vector<_ty>> values)
{
    data=values;
    for (size_t i = 1; i < values.size(); i++)
    {
        data[i].resize(data[0].size());
    }
    
    return;
}
template <typename _ty>
inline void Matstorage<_ty>::transpose()
{
    if(data.size()==0) return;
    std::vector<std::vector<_ty>> temp(data[0].size(),std::vector<_ty>(data.size()));
    // unsigned int maxcols=data[0].size();
    for(unsigned int i=0;i<data.size();i++){
        // if(data[i].size()>maxcols){
        
        // }
        for (unsigned int j = 0; j < data[i].size(); j++)
        {
            
            temp[j][i]=data[i][j];
        }
        
    }
    data=temp;
}
}
#endif