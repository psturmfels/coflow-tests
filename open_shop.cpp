#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

class COS_job {
public:
    vector<int> processing_times;
    int weight;

    COS_job(const int w, const &vector<int> pi) {
        processing_times = move(pi);
        weight = w;
    }
};

class COS_solution {
public:
    vector<int> order;
    vector<int> completion_times;
    
    COS_solution(int num_jobs) {
        order.reserve(num_jobs);
        completion_times.reserve(num_jobs);
    }
}

class COS_instance {
public:
    vector<COS_job> jobs;
    int num_machines;
    int num_jobs;
    
    COS_solution solve() {
        
        
    }
    
}

