#ifndef MATHLIB_MATRIX_H
#define MATHLIB_MATRIX_H
#include <iostream>
#include <vector>
#include <matstorage.h>
namespace Mathlib{
template <typename _ty,template<typename _stty> class _st>
class Matrix
{
private:
    unsigned int m_rows=0,m_columns=0;
    _st<_ty> data=_st<_ty>(0,0);
public:
    static Matrix<_ty,_st> add(const Matrix<_ty,_st>& mat1,const Matrix<_ty,_st>& mat2);
    static Matrix<_ty,_st> add(const Matrix<_ty, _st> &mat,const _ty& value);
    static Matrix<_ty,_st> mul(const Matrix<_ty, _st> &mat1,const Matrix<_ty, _st>& mat2);
    static Matrix<_ty,_st> mul(const Matrix<_ty, _st> &mat,const _ty& value);

public:
    Matrix()=default;
    Matrix(unsigned int rows,unsigned int columns);
    _ty& operator()(size_t i,size_t j);
    _ty get(unsigned int i,unsigned int j) const;
    void set(unsigned int i,unsigned int j,const _ty& value);
    unsigned int rows() const;
    unsigned int columns() const;
    void print() const;
    int add(const Matrix<_ty,_st>& mat);
    int add(const _ty& value);
    int mul(const Matrix<_ty, _st> &mat);
    int mul(const _ty& value);
    int div(const _ty& value);
    // int mul(const _ty& value);
    int func(_ty (*function)(const _ty& val));
    void set(std::vector<std::vector<_ty>> values);
    void transpose();
    ~Matrix();
public:
    Matrix<_ty,_st> operator+ (const Matrix<_ty,_st>& mat);
    Matrix<_ty,_st> operator* (const Matrix<_ty,_st>& mat);
    Matrix<_ty,_st> operator+ (const _ty& value);
    Matrix<_ty,_st> operator* (const _ty& value);
};

template <typename _ty,template<typename _stty> class _st>
Matrix<_ty,_st>::Matrix(unsigned int rows,unsigned int columns)
{
    data=_st<_ty>(rows,columns);
    this->m_rows=rows;
    this->m_columns=columns;
}
template <typename _ty, template<typename _stty> class _st>
inline _ty &Matrix<_ty, _st>::operator()(size_t i, size_t j)
{
    return data(i,j);
}
template <typename _ty, template<typename _stty> class _st>
inline _ty Matrix<_ty, _st>::get(unsigned int i, unsigned int j) const
{
    return data.get(i,j);
}
template <typename _ty, template<typename _stty>class _st>
inline void Matrix<_ty, _st>::set(unsigned int i, unsigned int j, const _ty &value)
{
    data.set(i,j,value);
}

template <typename _ty,template<typename _stty>class _st>
inline unsigned int Matrix<_ty,_st>::rows() const
{
    return this->m_rows;
}

template <typename _ty,template<typename _stty>class _st>
inline unsigned int Matrix<_ty,_st>::columns() const
{
    return this->m_columns;
}

template <typename _ty, template<typename _stty> class _st>
void Matrix<_ty, _st>::print() const
{
    for (unsigned int i = 0; i < m_rows; i++)
    {
        for (unsigned int j = 0; j < m_columns; j++)
        {
            std::cout <<data.get(i,j) << " "; 
        }
        std::cout << "\n";
    }
    
}

template <typename _ty, template<typename _stty> class _st>
inline int Matrix<_ty, _st>::add(const Matrix<_ty, _st> &mat)
{
    if(rows()!=mat.rows()||columns()!=mat.columns()) return -1;
    for (unsigned int i = 0; i < m_rows; i++)
    {
        for (unsigned int j = 0; j < m_columns; j++)
        {
            set(i,j,get(i,j)+mat.get(i,j));
        }
        
    }
    
    return 0;
}
template <typename _ty, template<typename _stty> class _st>
inline int Matrix<_ty, _st>::add(const _ty &value)
{
    for (unsigned int i = 0; i < m_rows; i++)
    {
        for (unsigned int j = 0; j < m_columns; j++)
        {
            // _ty temp=;
            set(i,j,get(i,j)+value);
        }
        
    }
    return 0;
} template <typename _ty, template <typename _stty> class _st>
inline int Matrix<_ty, _st>::mul(const Matrix<_ty, _st> &mat)
{

    if(columns()!=mat.rows()) return -1;
    Matrix<_ty, _st> out(rows(),mat.columns());
    for (unsigned int i = 0; i < m_rows; i++)
    {
        for (unsigned int j = 0; j < mat.columns(); j++)
        {
            _ty temp=0;
            for (unsigned int k = 0; k < m_columns; k++)
            {
                temp+=get(i,k)*mat.get(k,j);
            }
            out.set(i,j,temp);
        }
        
    }
    *this=out;
    return 0;
}

template <typename _ty, template<typename _stty> class _st>
inline int Matrix<_ty, _st>::mul(const _ty& value)
{
    for (unsigned int i = 0; i < m_rows; i++)
    {
        for (unsigned int j = 0; j < m_columns; j++)
        {
            set(i,j,get(i,j)*value);
        }
        
    }
    return 0;
}

template <typename _ty, template<typename _stty> class _st>
inline int Matrix<_ty, _st>::div(const _ty &value)
{
    for (unsigned int i = 0; i < m_rows; i++)
    {
        for (unsigned int j = 0; j < m_columns; j++)
        {
            set(i,j,get(i,j)/value);
        }
        
    }
    return 0;
}

// template <typename _ty, template<typename _stty> class _st>
// inline int Matrix<_ty, _st>::mul(const _ty &value)
// {
//     for (unsigned int i = 0; i < m_rows; i++)
//     {
//         for (unsigned int j = 0; j < m_columns; j++)
//         {
//             set(i,j,get(i,j)*value);
//         }
//
//     }
//     return 0;
// }

template <typename _ty, template <typename _stty> class _st>
inline int Matrix<_ty, _st>::func(_ty (*function)(const _ty &val))
{
    if(function==NULL) return -1;
    for (unsigned int i = 0; i < m_rows; i++)
    {
        for (unsigned int j = 0; j < m_columns; j++)
        {
            set(i,j,function(get(i,j)));
        }
        
    }
    return 0;
}

template <typename _ty, template <typename _stty> class _st>
inline void Matrix<_ty, _st>::set(std::vector<std::vector<_ty>> values)
{
    data.set(values);
    m_rows=values.size();
    if(m_rows==0){
        m_columns=0;
        return;
    }
    m_columns=values[0].size();
    
    return;
}

template <typename _ty, template <typename _stty> class _st>
inline void Matrix<_ty, _st>::transpose()
{
    data.transpose();
    unsigned int temp=m_rows;
    m_rows=m_columns;
    m_columns=temp;
}
// template <typename _ty, template<typename _stty> class _st>
// inline Matrix<_ty, _st> Matrix<_ty, _st>::add(Matrix<_ty, _st> mat1, Matrix<_ty, _st> mat2)
// {

//     Matrix<_ty, _st> out();
// }
template <typename _ty, template <typename _stty> class _st>
Matrix<_ty,_st> Matrix<_ty, _st>::add(const Matrix<_ty,_st>& mat1,const Matrix<_ty,_st>& mat2){
    Matrix<_ty,_st> out=mat1;
    out.add(mat2);
    return out;
}
template <typename _ty, template <typename _stty> class _st>
Matrix<_ty,_st> Matrix<_ty, _st>::add(const Matrix<_ty,_st>& mat1,const _ty& value){
    Matrix<_ty,_st> out=mat1;
    out.add(value);
    return out;
}
template <typename _ty, template <typename _stty> class _st>
Matrix<_ty,_st> Matrix<_ty, _st>::mul(const Matrix<_ty,_st>& mat1,const Matrix<_ty,_st>& mat2){
    Matrix<_ty,_st> out=mat1;
    out.mul(mat2);
    return out;
}
template <typename _ty, template <typename _stty> class _st>
Matrix<_ty,_st> Matrix<_ty, _st>::mul(const Matrix<_ty,_st>& mat1,const _ty& value){
    // unsigned int r=mat1.rows();
    // unsigned int c=mat1.columns();
    Matrix<_ty,_st> out=mat1;
    out.mul(value);
    // for (unsigned int i = 0; i < r; i++)
    // {
    //     for (unsigned int j = 0; j < c; j++)
    //     {
    //         out.set(i,j,mat1.get(i,j)*value);
    //     }
        
    // }
    return out;
}

template <typename _ty, template <typename _stty> class _st>
Matrix<_ty, _st>::~Matrix()
{
}
template <typename _ty, template <typename _stty> class _st>
Matrix<_ty,_st> Matrix<_ty, _st>::operator+ (const Matrix<_ty,_st>& mat){
    Matrix<_ty, _st> out=*this;
    out.add(mat);
    return out;
}
template <typename _ty, template <typename _stty> class _st>
Matrix<_ty,_st> Matrix<_ty, _st>::operator* (const Matrix<_ty,_st>& mat){
    Matrix<_ty, _st> out=*this;
    out.mul(mat);
    return out;
}
template <typename _ty, template <typename _stty> class _st>
Matrix<_ty,_st> Matrix<_ty, _st>::operator+ (const _ty& value){
    Matrix<_ty, _st> out=*this;
    out.add(value);
    return out;
}
template <typename _ty, template <typename _stty> class _st>
Matrix<_ty,_st> Matrix<_ty, _st>::operator* (const _ty& value){
    Matrix<_ty, _st> out=*this;
    out.mul(value);
    return out;
}
}


#endif