#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include "open_shop.h"

using namespace std;


COS_job::COS_job(const double w, vector<int> &pi) {
    processing_times = move(pi);
    weight = w;
}

COS_job::COS_job() {
    weight = -1;
}

COS_solution::COS_solution() {
    num_jobs = 0;
    wct = 0;
}

COS_solution::COS_solution(const int N) {
    num_jobs = N;
    order.resize(num_jobs);
    completion_times.resize(num_jobs);
    wct = 0;
}

COS_solution::COS_solution(const int N, const double w, vector<int> &o, vector<int> &ct) {
    num_jobs = N;
    wct = w;
    order = move(o);
    completion_times = move(ct);
}

void COS_solution::print() const {
    cout << "Orders: ";
    for (int j = 0; j < num_jobs; ++j) {
        cout << (order[j] + 1) << " ";
    }
    cout << "\nCtimes: ";
    for (int j = 0; j < num_jobs; ++j) {
        cout << completion_times[j] << " ";
    }
    cout << "\nSum of weighted completions times: " << wct << endl;
}


COS_instance::COS_instance(const int N, const int M, vector<COS_job> &j) {
    num_jobs = N;
    num_machines = M;
    jobs = j;
}

void COS_instance::print() const {
    cout << "Jobs across, machines down\n";
    cout << "  ";
    
    for (int i = 0; i < num_machines; ++i)
    {
        cout << (i + 1) << "  ";
    }
    cout << endl;
    for (int j = 0; j < num_jobs; ++j) {
        cout << (j + 1) << " ";
        for (int i = 0; i < num_machines; ++i) {
            cout << jobs[j].processing_times[i] << " ";
        }
        cout << endl;
    }
    cout << "Weights: ";
    for (int j = 0; j < num_jobs; ++j) {
        cout << jobs[j].weight << " ";
    }
    cout << endl;
    
}

COS_solution COS_instance::solve() const {
    int best_wct = numeric_limits<int>::max();
    vector<int> job_orders(num_jobs);
    vector<int> completion_times(num_jobs);
    
    for(int j = 0; j < num_jobs; ++j) {
        job_orders[j] = j;
    }
    vector<int> best_order = job_orders;
    vector<int> best_ct = completion_times;
    
    //Try every permutation
    do {
        int wct = 0;
        for (int j = 0; j < num_jobs; ++j) {
            int max_ct = 0;
            for (int i = 0; i < num_machines; ++i) {
                int cti = 0;
                for (int k = 0; k <= j; ++k) {
                    cti += jobs[job_orders[k]].processing_times[i];
                }
                if (cti > max_ct) {
                    max_ct = cti;
                }
            }
            
            completion_times[j] = max_ct;
            wct += max_ct * jobs[job_orders[j]].weight;
        }
        
        if (wct < best_wct) {
            best_wct = wct;
            best_order = job_orders;
            best_ct = completion_times;
        }
    } while(next_permutation(job_orders.begin(), job_orders.end()));
    
    return COS_solution(num_jobs, best_wct, best_order, best_ct);
}

COS_solution COS_instance::approximate() const {
    //Initialization step
    
    COS_solution sol(num_jobs);
    vector<double> adj_weights(num_jobs);
    vector<int> loads(num_machines, 0);
    vector<bool> is_scheduled(num_jobs, false);
    
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
            if (!is_scheduled[j]) {
                double ratio = adj_weights[j] / jobs[j].processing_times[mu];
                if (min_ratio > ratio) {
                    min_ratio = ratio;
                    next_job = j;
                }
            }
        }
        
        //Adjust remaining weights
        for(int j = 0; j < num_jobs; ++j) {
            if (!is_scheduled[j]) {
                adj_weights[j] -= (min_ratio * jobs[j].processing_times[mu]);
            }
        }
        
        //Record solution
        sol.order[k] = next_job;
        sol.completion_times[k] = max_load;
        sol.wct += sol.completion_times[k] * jobs[next_job].weight;
        
        //Update machine loads
        for (int i = 0; i < num_machines; ++i) {
            loads[i] -= jobs[next_job].processing_times[i];
        }
        
        //Update unscheduled jobs
        is_scheduled[next_job] = true;
    }
    
    return sol;
}

