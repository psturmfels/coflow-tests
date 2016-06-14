#include<vector>

class Flow {
public:
    double weight;
    std::vector<std::vector<int> > processing_times;

    Flow(const double w, std::vector<vector<int> > &pi);
    
    Flow();
}


class CF_solution {
public:
    std::vector<int> order;
    std::vector<int> completion_times;
    double wct;
    int num_jobs;
    
    CF_solution(const int N);
    CF_solution(const int N, const double w, std::vector<int> &o, std::vector<int> &ct);
    
    void print(const std::vector<Flow> &flows);
}
