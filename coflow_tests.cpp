#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include "coflow.h"
#include "open_shop.h"

using namespace std;

int main() {
    srand (time(NULL));
    
    int num_flows = 2;
    int num_ports = 2;
    int max_processing_time = 10;
    int max_weight = 3;
    vector<Flow> flows;
    flows.reserve(num_flows);
    
    for (int f = 0; f < num_flows; ++f) {
        double weight = rand() % max_weight + 1;
        
        vector<vector<int> > pi(num_ports, vector<int>(num_ports, 0));
        
        for (int i = 0; i < num_ports; ++i)
            for (int o = 0; o < num_ports; ++o)
                pi[i][o] = rand() % max_processing_time;
        
        Flow flow(weight, pi);
        flows.push_back(flow);
    }
    
    vector<COS_job> jobs;
    jobs.reserve(num_flows);
    for (int f = 0; f < num_flows; ++f) {
        vector<int> pi(2 * num_ports, 0);
        
        for (int i = 0; i < num_ports; ++i)
            pi[i] = flows[f].get_delta_i(i);
        
        for (int o = 0; o < num_ports; ++o)
            pi[num_ports + o] = flows[f].get_delta_o(o);
    
        COS_job job(flows[f].weight, pi);
        jobs.push_back(job);
    }
    
    COS_instance reduction(num_flows, 2 * num_ports, jobs);
    COS_solution reduced_apr = reduction.approximate();
    
    CF_instance coflow(num_flows, num_ports, flows);
    coflow.print();
    
    cout << "Reduction:\n";
    reduced_apr.print(jobs);
    CF_solution apr2 = coflow.approx2(reduced_apr.order);
    
    cout << endl; 
    cout << "Solution:";
    apr2.print();
    return 0;
}
