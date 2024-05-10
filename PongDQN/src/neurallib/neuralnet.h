#ifndef NEURALLIB_NET_H
#define NEURALLIB_NET_H
#include<vector>
#include<cstdio>
#include<mathlib/matrix.h>
#include <random>
#include<neurallib/activation.h>
#include<neurallib/loss.h>
#include<stdexcept>
#include<fstream>

template <typename _ty>
class Gradient
{
private:
public:
    std::vector<Mathlib::Matrix<_ty,Mathlib::Matstorage>> m_weights;
    std::vector<Mathlib::Matrix<_ty,Mathlib::Matstorage>> m_biases;
    Gradient(std::vector<Mathlib::Matrix<_ty,Mathlib::Matstorage>> weights,
    std::vector<Mathlib::Matrix<_ty,Mathlib::Matstorage>> biases);
    Gradient<_ty>& operator +=(const Gradient<_ty>& other);
    Gradient<_ty>& operator /=(const _ty& constant);
    void zero();
    ~Gradient();
};
template <typename _ty>
Gradient<_ty>::Gradient(std::vector<Mathlib::Matrix<_ty,Mathlib::Matstorage>> weights,
    std::vector<Mathlib::Matrix<_ty,Mathlib::Matstorage>> biases):m_weights(weights),m_biases(biases)
{
}
template <typename _ty>
inline Gradient<_ty> &Gradient<_ty>::operator+=(const Gradient<_ty> &other)
{
    for (size_t i = 0; i < m_weights.size(); i++)
    {
        m_weights[i].add(other.m_weights[i]);
        m_biases[i].add(other.m_biases[i]);
        
    }

    return *this;
}
template <typename _ty>
inline Gradient<_ty> &Gradient<_ty>::operator/=(const _ty &constant)
{
    for (size_t i = 0; i < m_weights.size(); i++)
    {
        m_weights[i].div(constant);
        m_biases[i].div(constant);
        
    }
    return *this;
}
template <typename _ty>
inline void Gradient<_ty>::zero()
{
    for (size_t i = 0; i < m_weights.size(); i++)
    {
        for (size_t k = 0; k < m_weights[i].rows(); k++)
        {
            for (size_t j = 0; j < m_weights[i].columns(); j++)
            {
                
                m_weights[i](k,j)=0.0f;
            }
            m_biases[i](k,0)=0.0f;
            
        }
        
    }
}
template <typename _ty>
Gradient<_ty>::~Gradient()
{
}

template <typename _ty>
class Neuralnet
{
private:
    std::vector<unsigned int> m_layers;
    std::vector<Mathlib::Matrix<_ty,Mathlib::Matstorage>> weights;
    std::vector<Mathlib::Matrix<_ty,Mathlib::Matstorage>> biases;
    std::vector<std::vector<Activation<_ty>>> activations;
    static std::random_device dev;
    static std::mt19937 rng;
    static std::uniform_real_distribution<float> dist;
    static _ty random(const _ty& val);
    std::vector<std::vector<_ty>> m_outputs;//m_activationcache; unactive
    std::vector<std::vector<_ty>> m_inputs;//m_beforeactivationcache;
    std::vector<std::vector<_ty>> m_deltas;
    // std::vector<_ty> m_predictions;
public:
    Neuralnet(/* args */);
    Neuralnet(const std::vector<unsigned int>& layers);
    void randInit();
    void setLayerActivation(size_t i,const Activation<_ty>& activation);
    std::vector<_ty> predict(const std::vector<_ty>& input);
    std::vector<_ty> forward_propogate(const std::vector<_ty>& input);
    void back_propogate(const std::vector<_ty>& prediction,const std::vector<_ty>& expected,Loss<_ty> lossfunc=Loss<_ty>());
    Gradient<_ty> getGradients();
    void applyGradients(const Gradient<_ty>& gradient,float learning_rate);
    void learn(const std::vector<_ty>& input,const _ty& learning_rate);
    void train(const std::vector<std::vector<_ty>>& datasetX,const std::vector<std::vector<_ty>> &datasetY,const size_t epochs,const _ty& learning_rate);
    bool save(std::string path);
    bool load(std::string path);
    ~Neuralnet();
};
template <typename _ty>
inline _ty Neuralnet<_ty>::random(const _ty& val)
{
    return Neuralnet::dist(rng);
}
template <typename _ty>
std::random_device Neuralnet<_ty>::dev=std::random_device();
template <typename _ty>
std::mt19937 Neuralnet<_ty>::rng=std::mt19937(dev());
template <typename _ty>
std::uniform_real_distribution<float> Neuralnet<_ty>::dist=std::uniform_real_distribution<float>(-1.0f,1.0f);
template <typename _ty>
Neuralnet<_ty>::Neuralnet()
{

}
template <typename _ty>
Neuralnet<_ty>::Neuralnet(const std::vector<unsigned int>& layers):m_layers(layers)
{
    size_t layerscount=layers.size();
    activations=std::vector<std::vector<Activation<_ty>>>(layerscount-1);
    m_outputs=std::vector<std::vector<_ty>>(layerscount-1);
    m_deltas=std::vector<std::vector<_ty>>(layerscount-1);
    m_inputs=std::vector<std::vector<_ty>>(layerscount-1);
    weights=std::vector<Mathlib::Matrix<_ty,Mathlib::Matstorage>>(layerscount-1);
    biases=std::vector<Mathlib::Matrix<_ty,Mathlib::Matstorage>>(layerscount-1);
    // m_predictions=std::vector<_ty>(layers[layerscount-1]);
    for (size_t i = 0; i < layerscount-1; i++)
    {
        weights[i]=(Mathlib::Matrix<_ty,Mathlib::Matstorage>(layers[i+1],layers[i]));
        biases[i]=(Mathlib::Matrix<_ty,Mathlib::Matstorage>(layers[i+1],1));
        activations[i]=std::vector<Activation<_ty>>(layers[i+1]);
        m_outputs[i]=std::vector<_ty>(layers[i+1]);
        m_deltas[i]=std::vector<_ty>(layers[i+1]);
        m_inputs[i]=std::vector<_ty>(layers[i]);

    }
    
}
template <typename _ty>
inline void Neuralnet<_ty>::randInit()
{
    for (unsigned int i = 0; i < weights.size(); i++)
    {
        // printf("%u weights:\n",i);
        weights[i].func(random);
        biases[i].func(random);
        // weights[i].print();
        // printf("biases:\n");
        // biases[i].print();
        // printf("\n");
        // exit(0);
    }
    
}
template <typename _ty>
inline void Neuralnet<_ty>::setLayerActivation(size_t i,const Activation<_ty>& activation)
{
    if(i==0||i>=activations.size()) return;
    
    for (size_t j = 0; j < activations[i-1].size(); j++)
    {
        activations[i-1][j]=activation;
    }
    // printf("print-- %f :\n",activations[i-1][0](-0.4));
    // exit(-1);
}
template <typename _ty>
std::vector<_ty> Neuralnet<_ty>::predict(const std::vector<_ty> &input)
{
    Mathlib::Matrix<_ty,Mathlib::Matstorage> inmat;
    if(input.size()==m_layers[0])
    inmat.set(std::vector<std::vector<_ty>>(1,input));
    else{
        // std::vector<_ty> temp=input;
        throw std::logic_error("Input does not match network inputs.\n");
    }
    inmat.transpose();
    // printf("input :\n");
    // inmat.print();
    // // printf("\n");
    // printf("w :\n");
    // weights[0].print();
    // printf("b :\n");
    // biases[0].print();
    Mathlib::Matrix<_ty,Mathlib::Matstorage> output;//(baises[baises.size()-1].rows(),1);
    // output=inmat;
    size_t layerscount=m_layers.size();
    for (unsigned int i = 0; i < layerscount-1; i++)
    {
        
        output=weights[i];
        output.mul(inmat);
        output.add(biases[i]);
        for (size_t j = 0; j < output.rows(); j++)
        {
            
            output(j,0)=activations[i][j](output(j,0));
        }
        
        inmat=output;
    }
    std::vector<_ty> out=std::vector<_ty>(output.rows());
    for (unsigned int i = 0; i < output.rows(); i++)
    {
        out[i]=output.get(i,0);
    }
    return out;
}
template <typename _ty>
inline std::vector<_ty> Neuralnet<_ty>::forward_propogate(const std::vector<_ty> &input)
{
    Mathlib::Matrix<_ty,Mathlib::Matstorage> inmat;
    if(input.size()==m_layers[0])
    inmat.set(std::vector<std::vector<_ty>>(1,input));
    else{
        // std::vector<_ty> temp=input;
        std::cout<<"size :"<<input.size()<<'\n';
        throw std::logic_error("Input does not match network inputs.\n");
    }
    inmat.transpose();
    // printf("input :\n");
    // inmat.print();
    // printf("\n");
    Mathlib::Matrix<_ty,Mathlib::Matstorage> output;//(baises[baises.size()-1].rows(),1);
    size_t layerscount=m_layers.size();
    for (unsigned int i = 0; i < layerscount-1; i++)
    {
        for (size_t j = 0; j < inmat.rows(); j++)
        {
            m_inputs[i][j]=inmat(j,0);
        }

        output=weights[i];
        // printf("w %u :\n",i);
        // weights[i].print();
        // printf("\n");
        output.mul(inmat);
        output.add(biases[i]);
        for (size_t j = 0; j < output.rows(); j++)
        {
            m_outputs[i][j]=output(j,0);
            // printf("in-- %f :\n",output(j,0));
            output(j,0)=activations[i][j](output(j,0));
            // printf("%u out-- %f :\n",i,output(j,0));
            // if(i==layerscount-2){
            //     m_predictions[j]=output(j,0);
            // }
        }
        
        inmat=output;
        // printf("out %u :\n",i);
        // inmat.print();
        // printf("\n");
    }
    // printf("output :\n");
    // inmat.print();
    // printf("\n");
    std::vector<_ty> out=std::vector<_ty>(output.rows());
    for (unsigned int i = 0; i < output.rows(); i++)
    {
        out[i]=output(i,0);
    }
    return out;
}
template <typename _ty>
inline void Neuralnet<_ty>::back_propogate(const std::vector<_ty>& prediction,const std::vector<_ty> &expected,Loss<_ty> lossfunc)
{
    std::vector<_ty> errors(m_layers[m_layers.size()-1]);
    for (size_t i = 0; i < m_layers[m_layers.size()-1]; i++)
    {
        // errors[i]=2*(prediction[i]-expected[i]);//TODO 
        errors[i]=lossfunc.derivative(prediction,expected,0,i);//TODO 
        // std::cout<<"error: "<<errors[i]<<"\n";
        m_deltas[m_layers.size()-2][i]=errors[i]*activations[m_layers.size()-2][i].derivative(m_outputs[m_layers.size()-2][i]);
    }
    
    for (size_t i = m_layers.size()-2; i > 0; i--)
    {
        errors.resize(m_layers[i]);
        for (size_t j = 0; j < m_layers[i]; j++)
        {
            _ty error=0.0;
            for (size_t k = 0; k < m_layers[i+1]; k++)
            {
                error+=weights[i](k,j)*m_deltas[i][k];
            }
            errors[j]=error;
            m_deltas[i-1][j]=errors[j]*activations[i-1][j].derivative(m_outputs[i-1][j]);
        }
        
    }
    // std::cout<<"deltas: "<<"\n";
    // for (size_t i = 0; i < m_deltas.size(); i++)
    // {
    //     for (size_t j = 0; j < m_deltas[i].size(); j++)
    //     {
    //         std::cout<<m_deltas[i][j]<<" ";
    //     }
    //     std::cout<<"\n";
    // }
    
    return ;
}
template <typename _ty>
inline Gradient<_ty> Neuralnet<_ty>::getGradients()
{
    auto grad_weights=weights;
    auto grad_biases=biases;
    for (size_t i = 0; i < m_layers.size()-1; i++)
    {
        for (size_t k = 0; k < m_layers[i+1]; k++)
        {
            for (size_t j = 0; j < m_layers[i]; j++)
            {
                
                grad_weights[i](k,j)=m_deltas[i][k]*m_inputs[i][j];
            }
            grad_biases[i](k,0)=m_deltas[i][k];
            
        }
        
    }
    return {grad_weights,grad_biases};
}
template <typename _ty>
inline void Neuralnet<_ty>::applyGradients(const Gradient<_ty>& gradient,float learning_rate)
{
    for (size_t i = 0; i < m_layers.size()-1; i++)
    {
        for (size_t k = 0; k < m_layers[i+1]; k++)
        {
            for (size_t j = 0; j < m_layers[i]; j++)
            {
                
                weights[i](k,j)-=learning_rate*gradient.m_weights[i].get(k,j);
            }
            biases[i](k,0)-=learning_rate*gradient.m_biases[i].get(k,0);
            
        }
        
    }
}
template <typename _ty>
inline void Neuralnet<_ty>::learn(const std::vector<_ty> &input, const _ty &learning_rate)
{
    for (size_t i = 0; i < m_layers.size()-1; i++)
    {
        for (size_t k = 0; k < m_layers[i+1]; k++)
        {
            for (size_t j = 0; j < m_layers[i]; j++)
            {
                
                weights[i](k,j)-=learning_rate*m_deltas[i][k]*m_inputs[i][j];
            }
            biases[i](k,0)-=learning_rate*m_deltas[i][k];
            
        }
        
    }
    
}
template <typename _ty>
inline void Neuralnet<_ty>::train(const std::vector<std::vector<_ty>> &datasetX,const std::vector<std::vector<_ty>> &datasetY, const size_t epochs, const _ty &learning_rate)
{
    Loss<float> loss(__mseloss,__mselossderiv);
    float netloss=0.0f;
    for (size_t i = 0; i < datasetX.size(); i++)
    {
        netloss+=loss(predict(datasetX[i]),datasetY[i]);
    }
    std::cout<<"loss: "<<netloss<<" avg: "<<netloss/datasetX.size()<<'\n';
    for (size_t i = 0; i < epochs; i++)
    {
        for (size_t j = 0; j < datasetX.size(); j++)
        {
            back_propogate(forward_propogate(datasetX[j]),datasetY[j],Loss<float>(__mseloss,__mselossderiv));
            learn(datasetX[j],learning_rate);
        }
        if(i%100==0){
            
            float netloss=0.0f;
            for (size_t i = 0; i < datasetX.size(); i++)
            {
                netloss+=loss(predict(datasetX[i]),datasetY[i]);
            }
            std::cout<<"loss: "<<netloss<<" avg: "<<netloss/datasetX.size()<<'\n';
        }
    }
    
}
template <typename _ty>
inline bool Neuralnet<_ty>::save(std::string path)
{
    printf("saved:\n");
    std::ofstream file(path,std::ios::binary);
    if(!file.is_open()) return false;
    size_t layerscount=m_layers.size();
    file.write((char*)(&layerscount),sizeof(size_t));
    for (size_t i = 0; i < layerscount; i++)
    {
        file.write((char*)(&m_layers[i]),sizeof(unsigned int));
    }
    
    for (size_t i = 0; i < layerscount-1; i++)
    {
        // printf("w %lu:\n",i);
        // weights[i].print();
        for (size_t j = 0; j < weights[i].rows(); j++)
        {
            for (size_t k = 0; k < weights[i].columns(); k++)
            {
                file.write((char*)(&weights[i](j,k)),sizeof(_ty));
            }
            
        }
        // printf("b %lu:\n",i);
        // biases[i].print();
        for (size_t j = 0; j < biases[i].rows(); j++)
        {
            
            file.write((char*)(&biases[i](j,0)),sizeof(float));
            
            
        }
        
    }
    file.close();
    return true;
    
}
template <typename _ty>
inline bool Neuralnet<_ty>::load(std::string path)
{
    printf("loaded:\n");
    std::ifstream file(path,std::ios::binary);
    if(!file.is_open()) return false;
    size_t layerscount;
    file.read((char*)(&layerscount),sizeof(size_t));
    std::vector<unsigned int> layers(layerscount);
    // printf("here\n");
    for (size_t i = 0; i < layerscount; i++)
    {
        file.read((char*)&layers[i],sizeof(unsigned int));
    }
    
    *this=Neuralnet<_ty>(layers);
    for (size_t i = 0; i < m_layers.size()-1; i++)
    {
        
        for (size_t j = 0; j < weights[i].rows(); j++)
        {
            for (size_t k = 0; k < weights[i].columns(); k++)
            {
                file.read((char*)(&weights[i](j,k)),sizeof(float));
            }
            
        }
        // printf("w %lu:\n",i);
        // weights[i].print();
        
        for (size_t j = 0; j < biases[i].rows(); j++)
        {
            
            file.read((char*)(&biases[i](j,0)),sizeof(float));
            
            
        }
        // printf("b %lu:\n",i);
        // biases[i].print();
    }
    file.close();
    return true;
}
template <typename _ty>
Neuralnet<_ty>::~Neuralnet()
{
}


#endif