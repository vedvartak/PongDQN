#include<rllib/dqn.h>
#include <random>
// #include <algorithm>
namespace Rllib
{
    float mseloss(const std::vector<float>& predicted,const std::vector<float>& expected){
        // if(predicted.size()==0)
        if(predicted.size() != expected.size()||predicted.size()==0){
        throw std::logic_error("[Loss] Predicted and Excepted size doesnt match.\n");
        }
        float out=0.0f;
        for (size_t i = 0; i < predicted.size(); i++)
        {
            out+=(predicted[i]-expected[i])*(predicted[i]-expected[i]);
        }
        return out/predicted.size();
    }
    float mselossder(const std::vector<float>& predicted,const std::vector<float>& expected,bool isexpec,size_t i){
        if(predicted.size() != expected.size()){
        throw std::logic_error("[Loss der] Predicted and Excepted size doesnt match.\n");
        }
        float out=(predicted[i]-expected[i])*2;
        return out/predicted.size()*((isexpec)?(-1.0f):(1.0f));
    }
    float huberloss(const std::vector<float>& predicted,const std::vector<float>& expected){
        // if(predicted.size()==0)
        if(predicted.size() != expected.size()||predicted.size()==0){
        throw std::logic_error("[Loss] Predicted and Excepted size doesnt match.\n");
        }
        float out=0.0f;
        for (size_t i = 0; i < predicted.size(); i++)
        {
            out+=(std::abs(predicted[i]-expected[i])<1.0f)?((predicted[i]-expected[i])*(predicted[i]-expected[i])/2.0f):(1.0f*(std::abs(predicted[i]-expected[i])-0.5f*1.0f));
        }
        return out/predicted.size();
    }
    float huberder(const std::vector<float>& predicted,const std::vector<float>& expected,bool isexpec,size_t i){
        if(predicted.size() != expected.size()){
        throw std::logic_error("[Loss der] Predicted and Excepted size doesnt match.\n");
        }
        float out=(std::abs(predicted[i]-expected[i])<1.0f)?((predicted[i]-expected[i])):(((predicted[i]-expected[i]>0)?1.0f:-1.0f));
        return out/predicted.size()*((isexpec)?(-1.0f):(1.0f));
    }
    float sigmoid(const float& x){
        // printf("sigmoid :%f %f\n",x,1.0f/(1.0f+std::exp(-x)));
        return 1.0f/(1+std::exp(-x));
    }
    float sigmoidder(const float& x){
        float out=1.0f/(1+std::exp(-x));
        return out*(1.0f-out);
    }
    float relu(const float& x){
        // printf("sigmoid :%f %f\n",x,1.0f/(1.0f+std::exp(-x)));
        return (x>0)?x:0;
    }
    float reluder(const float& x){
        return (x>0)?1:0;
    }
    Experience::Experience(std::vector<float> state, size_t action, float reward, std::vector<float> nextstate,bool done):m_state(state),m_action(action),m_reward(reward),m_nextstate(nextstate),m_done(done)
    {
    }

    Experience::~Experience()
    {
    }

    DQN::DQN(const std::vector<unsigned int>& layers,size_t replay_mem_size):network(Neuralnet<float>(layers)),replay_memory(std::vector<Experience>(replay_mem_size))
    {
        // network.setLayerActivation(4,Activation<float>(sigmoid,sigmoidder));
        // network.setLayerActivation(3,Activation<float>(sigmoid,sigmoidder));
        // network.setLayerActivation(3,Activation<float>(sigmoid,sigmoidder));
        // network.setLayerActivation(2,Activation<float>(sigmoid,sigmoidder));
        // network.setLayerActivation(1,Activation<float>(sigmoid,sigmoidder));
        network.setLayerActivation(3,Activation<float>(relu,reluder));
        network.setLayerActivation(2,Activation<float>(relu,reluder));
        network.setLayerActivation(1,Activation<float>(relu,reluder));
        // network.setLayerActivation(2,Activation<float>(relu,reluder));
        // network.setLayerActivation(1,Activation<float>(sigmoid,sigmoidder));
        network.randInit();
        target_network=network;
    }

    std::vector<float> DQN::predict(const std::vector<float>& input)
    {

        return network.predict(input);
    }
    std::vector<float> DQN::forward(const std::vector<float>& input)
    {
        return network.forward_propogate(input);
    }
    void DQN::storeExperience(const Experience &experience)
    {
        replay_memory[m_replay_mem_new]=experience;
        m_replay_mem_new++;
        if(m_replay_mem_new==replay_memory.size())
        ismemfull=true;
        m_replay_mem_new=m_replay_mem_new%replay_memory.size();
    }
    void DQN::train_step()
    {
        if(!ismemfull&&m_replay_mem_new<1000) return;
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution dist=std::uniform_int_distribution(0,(int)(ismemfull?replay_memory.size()-1:m_replay_mem_new-1));
        float closs=0;
        float cmaxloss=0;
        Experience exp=replay_memory[dist(mt)];
        // do{
        float target;
        if(exp.m_done){
            target=exp.m_reward;
        }
        else
        {
            auto pred=(target_network.predict(exp.m_nextstate));
            size_t max=0;
            for (size_t i = 1; i < pred.size(); i++)
            {
                if(pred[max]<pred[i])max=i;
            }
            
            target=exp.m_reward+discount_factor*pred[max];
        }
        auto pred=network.forward_propogate(exp.m_state);
        std::vector<float> expected=pred;
        expected[exp.m_action]=target;
        network.back_propogate(pred,expected,Loss<float>(mseloss,mselossder));
        // network.back_propogate(pred,expected,Loss<float>(huberloss,huberder));
        closs=mseloss(pred,expected);
        cmaxloss=std::max(cmaxloss,closs);
        if(std::isnan(mseloss(pred,expected))) {
            for (size_t i = 0; i < pred.size(); i++)
            {
                std::cout<<pred[i]<<" "<<expected[i]<<" "<<pred[i]-expected[i]<<"\n";
            }
            for (size_t i = 0; i < exp.m_state.size(); i++)
            {
                std::cout<<exp.m_state[i]<<" ";
            }
            
            exit(0);
            // printf("nan\n");
            // printf("loss : %f %f\n",mselossder(pred,expected,false,0),mselossder(pred,expected,false,1));
            // scanf("%*c");
        };
        network.learn(exp.m_state,0.0001f);
        // }while(closs>2.0f);
        // printf("loss : %f \n",closs);
        tstepi++;
        if(tstepi==tsteps){
            target_network=network;
            tstepi=0;
            printf("loss : %f maxloss %f\n",closs,cmaxloss);
            cmaxloss=0.0f;
            if(epsilon>0.004f)epsilon-=0.004f;
        }
    }
    // std::vector<float> DQN::backpropogate(const std::vector<float> &input)
    void DQN::save(std::string path)
    {
        network.save(path);
    }
    void DQN::load(std::string path)
    {
        if(!network.load(path)) {
            printf("[ERROR] could not load model.\n");
            exit(0);
        }
        // network.setLayerActivation(3,Activation<float>(sigmoid,sigmoidder));
        // network.setLayerActivation(2,Activation<float>(sigmoid,sigmoidder));
        // network.setLayerActivation(1,Activation<float>(sigmoid,sigmoidder));
        network.setLayerActivation(3,Activation<float>(relu,reluder));
        network.setLayerActivation(2,Activation<float>(relu,reluder));
        network.setLayerActivation(1,Activation<float>(relu,reluder));
        target_network=network;
    }
    // {
    //     return network.back_propogate();
    // }
    DQN::~DQN()
    {
    }
} // namespace Rllib
