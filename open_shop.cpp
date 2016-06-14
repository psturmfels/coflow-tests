#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <limits>

using namespace std;

class COS_job {
public:
    vector<int> processing_times;
    double weight;
    bool is_scheduled;

    COS_job(const double w, &vector<int> pi) {
        processing_times = move(pi);
        weight = w;
        is_scheduled = false;
    }
};

class COS_solution {
public:
    vector<int> order;
    vector<int> completion_times;
    
    COS_solution(int num_jobs) {
        order.resize(num_jobs);
        completion_times.resize(num_jobs);
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
        
        for (int k = num_jobs - 1; k >= 0; --k) {
        
            //Determmine machine with max load
            int max_load = 0;
            int mu = 0;
            for (int i = 0; i < num_machines; ++i) {
                if (max_load < loads[i]) {
                    max_load = loads[i];
                    mu = i;
                }
            }
            
            
            //Determine which job has the smallest
            //weight-to-processing time ratio, schedule it last
            double min_ratio = numeric_limits<double>::max(); //theta
            int next_job = 0; //sigma
            for (int j = 0; j < num_jobs; ++j) {
                if (!jobs[j].is_scheduled) {
                    double ratio = jobs[j].weight / jobs[j].processing_times[mu];
                    if (min_ratio < ratio) {
                        min_ratio = ratio;
                        next_job = j;
                    }
                }
            }
            
            //Adjust remaining weights
            for(int j = 0; j < num_jobs; ++j) {
                if (!jobs[j].is_scheduled) {
                    jobs[j].weight -= (min_ratio * jobs[j].processing_times[mu]);
                }
            }
            
            //Record solution
            sol.order[k] = next_job;
            sol.completion_times[k] = max_load;
            
            //Update machine loads
            for (int i = 0; i < num_machines; ++i) {
                loads[i] -= jobs[next_job].processing_times[i];
            }
            
            //Update unscheduled jobs
            jobs[next_job].is_scheduled = true;
        }
        
        return sol;
    }
}

