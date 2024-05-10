#ifndef NEURALLIB_LOSS_H
#define NEURALLIB_LOSS_H
#include<vector>
#include<stdexcept>
#include<algorithm>
template<typename _ty>
_ty __absloss(const std::vector<_ty>& predicted,const std::vector<_ty>& expected){
    if(predicted.size() != expected.size()){
        throw std::logic_error("[Loss] Predicted and Excepted size doesnt match.\n");
    }
    _ty out=0;
    for (size_t i = 0; i < predicted.size(); i++)
    {
        out+=std::abs(predicted[i]-expected[i]);
    }
    return out;
}
template<typename _ty>
_ty __abslossderiv(const std::vector<_ty>& predicted,const std::vector<_ty>& expected,bool isexpec,size_t i){
    if(predicted.size() != expected.size()){
        throw std::logic_error("[Loss] Predicted and Excepted size doesnt match.\n");
    }
    
    // return 2*(predicted[i]-expected[i]);
    return (predicted[i]>=expected[i])?((!isexpec)?1:-1):((isexpec)?1:-1);
}
template<typename _ty>
_ty __mseloss(const std::vector<_ty>& predicted,const std::vector<_ty>& expected){
    if(predicted.size() != expected.size()){
        throw std::logic_error("[Loss] Predicted and Excepted size doesnt match.\n");
    }
    _ty out=0;
    for (size_t i = 0; i < predicted.size(); i++)
    {
        out+=(predicted[i]-expected[i])*(predicted[i]-expected[i]);
    }
    return out/predicted.size();
}
template<typename _ty>
_ty __mselossderiv(const std::vector<_ty>& predicted,const std::vector<_ty>& expected,bool isexpec,size_t i){
    if(predicted.size() != expected.size()){
        throw std::logic_error("[Loss] Predicted and Excepted size doesnt match.\n");
    }
    
    return 2*(predicted[i]-expected[i])/predicted.size();
    // return (predicted[i]>=expected[i])?((!isexpec)?1:-1):((isexpec)?1:-1);
}
template<typename _ty>
class Loss
{
private:
    typedef _ty (*lossfuncp)(const std::vector<_ty>& predicted,const std::vector<_ty>& expected);
    typedef _ty (*lossfuncderivp)(const std::vector<_ty>& predicted,const std::vector<_ty>& expected,bool isexpec,size_t i);
    lossfuncp m_funcp=NULL;
    lossfuncderivp m_deriv=NULL;
public:
    Loss()=default;
    Loss(lossfuncp funcp,lossfuncderivp funcdervp);
    _ty operator()(const std::vector<_ty>& predicted,const std::vector<_ty>& expected);
    _ty derivative(const std::vector<_ty>& predicted,const std::vector<_ty>& expected,bool isexpec,size_t i);
    ~Loss();
};
template<typename _ty>
Loss<_ty>::Loss(lossfuncp funcp,lossfuncderivp funcdervp):m_funcp(funcp),m_deriv(funcdervp)
{
}
template <typename _ty>
inline _ty Loss<_ty>::operator()(const std::vector<_ty>& predicted,const std::vector<_ty>& expected)
{
    return (m_funcp==NULL)?(__absloss(predicted,expected)):m_funcp(predicted,expected);
}
template <typename _ty>
inline _ty Loss<_ty>::derivative(const std::vector<_ty>& predicted,const std::vector<_ty>& expected,bool isexpec,size_t i)
{
    // return (m_deriv==NULL)?(1):m_deriv(x);
    return (m_deriv==NULL)?(__abslossderiv(predicted,expected,isexpec,i)):m_deriv(predicted,expected,isexpec,i);
}
template <typename _ty>
Loss<_ty>::~Loss()
{
}




#endif