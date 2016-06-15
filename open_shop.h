#ifndef open_shop
#define open_shop
#include <vector>

class COS_job {
public:
    std::vector<int> processing_times;
    double weight;

    COS_job(const double w, std::vector<int> &pi);
    
    COS_job();
};

class COS_solution {
public:
    std::vector<int> order;
    std::vector<int> completion_times;
    double wct;
    int num_jobs;
    
    COS_solution(const int N);
    
    COS_solution(const int N, const double w, std::vector<int> &o, std::vector<int> &ct);
    
    void print() const;
};

class COS_instance {
public:
    std::vector<COS_job> jobs;
    int num_machines;
    int num_jobs;
    
    COS_instance(const int N, const int M, std::vector<COS_job> &j);
    
    void print() const;
    
    COS_solution solve() const;
    
    COS_solution approximate() const;
};

#endif

