#include <vector>

class Flow {
public:
    std::vector<std::vector<int> > processing_times;
    double weight;
    int p_sum;
    
    Flow(const double w, std::vector<std::vector<int> > &pi);
    
    Flow();
    
    void print();
    
    int get_delta();
    int get_delta_i(int i);
    int get_delta_o(int o);
};


class CF_solution {
public:
    std::vector<int> completion_times;
    double wct;
    int num_flows;
    
    CF_solution(const int N);
    CF_solution(const int N, const double w, std::vector<int> &ct);
    
    void print();
};

class CF_instance {
public:
    std::vector<Flow> flows;
    int num_ports;
    int num_flows;
    
    CF_instance(const int N, const int M, std::vector<Flow> &f);
    
    void print();
    
    CF_solution approx2(std::vector<int> &flow_order) const;

};
