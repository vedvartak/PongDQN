#ifndef NEURALLIB_ACTIVATION_H
#define NEURALLIB_ACTIVATION_H
template<typename _ty>
class Activation
{
private:
    typedef _ty (*activationfuncp)(const _ty& x);
    activationfuncp m_funcp=NULL;
    activationfuncp m_deriv=NULL;
public:
    Activation()=default;
    Activation(activationfuncp funcp,activationfuncp funcdervp);
    _ty operator()(const _ty& x) const;
    _ty derivative(const _ty& x) const;
    ~Activation();
};
template<typename _ty>
Activation<_ty>::Activation(activationfuncp funcp,activationfuncp funcdervp):m_funcp(funcp),m_deriv(funcdervp)
{
}
template <typename _ty>
inline _ty Activation<_ty>::operator()(const _ty &x) const
{
    return (m_funcp==NULL)?(x):m_funcp(x);
}
template <typename _ty>
inline _ty Activation<_ty>::derivative(const _ty &x) const
{
    return (m_deriv==NULL)?(1):m_deriv(x);
}
template <typename _ty>
Activation<_ty>::~Activation()
{
}




#endif