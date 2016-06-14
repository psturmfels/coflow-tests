#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

class COS_job {
public:
    vector<int> processing_times;
    double weight;
    bool scheduled;

    COS_job(const double w, &vector<int> pi) {
        processing_times = move(pi);
        weight = w;
        scheduled = false;
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
    
    COS_instance(const int N, const int M, &vector<COS_job> j) {
        num_jobs = N;
        num_machines = M;
        jobs = move(j);
    }
    
    
    COS_solution solve() {
        //Initialization step
        
        COS_solution sol(num_jobs);
        vector<double> adj_weights(num_jobs);
        vector<int> loads(num_machines, 0);
        for (int i = 0; i < num_machines; ++i) {
            for (int j = 0; j < num_jobs; ++j) {
                loads[i] += jobs[j].processing_times[i];
            }
        }
        for (int j = 0; j < num_jobs; ++j) {
            adj_weights[j] = jobs[j].weight;
        }
        
        //Solve step
        
        for (int k = 0; k < num_jobs; ++k) {
            int max_load = 0;
            int mu = 0;
            for (int i = 0; i < num_machines; ++i) {
                if (max_load < loads[i]) {
                    max_load = loads[i];
                    mu = i;
                }
            }
            
            int min_ratio = 0;
            int next_job = 0;
            for 
            
            
            
        }
        
    }
    
}

