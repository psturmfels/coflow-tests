#include <vector>
#include <utility>
#include "open_shop.h"

class Flow {
public:
    std::vector<std::vector<int> > processing_times;
    double weight;
    int p_sum;
    
    Flow(const double w, std::vector<std::vector<int> > &pi);
    
    Flow();
    
    void print() const;
    
    int get_delta() const;
    int get_delta_i(int i) const;
    int get_delta_o(int o) const;
};


class CF_solution {
public:
    std::vector<int> completion_times;
    std::vector<Flow> flows;
    double wct;
    int num_flows;
    
    CF_solution(const int N);
    CF_solution(const int N, const double w, std::vector<int> &ct);
    
    void print() const;
};

class CF_instance {
public:
    std::vector<Flow> flows;
    int num_ports;
    int num_flows;
    
    CF_instance(const int N, const int M, std::vector<Flow> &f);
    
    void print() const;
    
    std::pair<CF_solution, COS_solution> approx2() const;
};
