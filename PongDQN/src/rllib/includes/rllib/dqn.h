#ifndef RLLIB_DQN_H
#define RLLIB_DQN_H
#include<vector>
#include<neurallib/neuralnet.h>
namespace Rllib{
    
    class Experience
    {
    private:
    public:
        std::vector<float> m_state;
        size_t m_action;
        float m_reward;
        std::vector<float> m_nextstate;
        bool m_done;
        Experience()=default;
        Experience(std::vector<float> state,size_t action,float reward,std::vector<float> nextstate,bool done);
        ~Experience();
    };
    

    class DQN
    {
    private:
        Neuralnet<float> network;
        Neuralnet<float> target_network;
        // size_t m_replay_mem_size;
        bool ismemfull=false;
        std::vector<Experience> replay_memory;
        float discount_factor=0.99;
        size_t tsteps=2000;
        size_t tstepi=0;
    public:
        size_t m_replay_mem_new=0;
        float epsilon=0.9;
        DQN(const std::vector<unsigned int>& layers,size_t replay_mem_size);
        std::vector<float> predict(const std::vector<float>& input);
        std::vector<float> forward(const std::vector<float>& input);
        void storeExperience(const Experience& experience);
        void train_step();
        void save(std::string path);
        void load(std::string path);
        // std::vector<float> backpropogate(const std::vector<float>& prediction,const std::vector<float> &expected,Loss<float> lossfunc);
        ~DQN();
    };

    
}

#endif
