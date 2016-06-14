#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include "coflow.h"
#include "open_shop.h"

using namespace std;

int main() {
    srand (time(NULL));
    
    int num_flows = 3;
    int num_ports = 3;
    int max_processing_time = 50;
    int max_weight = 10;
    vector<Flow> flows;
    flows.reserve(num_flows);
    
    vector<COS_job>
    
    for(int f = 0; f < num_flows; ++f) {
        double weight = rand() % max_weight + 1;
        
        vector<vector<int> > pi(num_ports, vector<int>(num_ports, 0));
        
        for (int i = 0; i < num_ports; ++i)
            for (int o = 0; o < num_ports; ++o)
                pi[i][o] = rand() % max_processing_time;
        
        Flow flow(weight, pi);
        flows.push_back(flow);
    }
    
    
    
    CF_instance coflow(num_flows, num_ports, flows);
    coflow.print();
    
    
    
    CF_solution apr2 = coflow.approx2();
    
    return 0;
}
