#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <utility>
#include "coflow.h"
#include "open_shop.h"

using namespace std;

void bad_test() {
    int num_flows = 2;
    int num_ports = 10;
    vector<Flow> flows;
    vector<vector<int> > pi_first(num_ports, vector<int>(num_ports, 0));
    
    for (int i = 1; i < num_ports; ++i) {
        pi_first[i][i] = num_ports - 1;
    }
    
    Flow first(1, pi_first);
    
    vector<vector<int> > pi_second(num_ports, vector<int>(num_ports, 0));
    
    for (int i = 0; i < num_ports; ++i) {
        pi_second[0][i] = 1;
    }
    
    Flow second(1, pi_second);
    
    flows.push_back(first);
    flows.push_back(second);
    
    CF_instance coflow(num_flows, num_ports, flows);
    coflow.print();
    
    pair<CF_solution, COS_solution> sol = coflow.approx2();
    CF_solution apr2 = sol.first;
    COS_solution reduced_apr = sol.second;
    
    cout << endl;
    cout << "Reduction:\n";
    reduced_apr.print();
    
    cout << "\nSolution: \n";
    apr2.print();
}

void all_tests() {
    srand (time(NULL));
    int num_flows = 2;
    int num_ports = 3;
    int max_processing_time = 4;
    int max_weight = 4;
    CF_instance CFImax;
    CF_solution CFSmax;
    COS_solution COSmax;
    double biggest_fold_change = 1.0;
    
    for (int i = 0; i < 10000000; ++i) {
        vector<Flow> flows;
        flows.reserve(num_flows);
        
        for (int f = 0; f < num_flows; ++f) {
            double weight = rand() % max_weight + 1;
            
            vector<vector<int> > pi(num_ports, vector<int>(num_ports, 0));
            
            for (int i = 0; i < num_ports; ++i)
                for (int o = 0; o < num_ports; ++o)
                    pi[i][o] = rand() % max_processing_time;
            
            Flow flow(weight, pi);
            
            if (flow.get_delta() == 0)
                flow.processing_times[0][0] = 1;
            
            flows.push_back(flow);
        }
        
        CF_instance coflow(num_flows, num_ports, flows);
        //coflow.print();
        
        //cout << "Reduction:\n";
        //reduced_apr.print(jobs);
        pair<CF_solution, COS_solution> sol = coflow.approx2();
        CF_solution apr2 = sol.first;
        COS_solution reduced_apr = sol.second;
        
        //cout << endl;
        //cout << "Solution:";
        //apr2.print();
        if (reduced_apr.wct < apr2.wct) {
            if ((apr2.wct / reduced_apr.wct) > biggest_fold_change)
            {
                biggest_fold_change = (apr2.wct / reduced_apr.wct);
                cout << biggest_fold_change << endl;
                CFImax = coflow;
                CFSmax = apr2;
                COSmax = reduced_apr;
            }
        }
    }
    
    CFImax.print();
    cout << "Reduction:\n";
    COSmax.print();
    cout << endl;
    cout << "Solution:\n";
    CFSmax.print();
}

void interactive_mode() {
    srand (time(NULL));
    char input = 'y';
    do {
        int num_flows;
        int num_ports;
        int max_processing_time;
        int max_weight;
        cout << "Enter integer number of flows: ";
        cin >> num_flows;
        cout << "Enter integer number of ports: ";
        cin >> num_ports;
        cout << "Enter integer max processing time on an edge: ";
        cin >> max_processing_time;
        cout << "Enter integer max flow weight: ";
        cin >> max_weight;
        
        vector<Flow> flows;
        flows.reserve(num_flows);
        
        for (int f = 0; f < num_flows; ++f) {
            double weight = rand() % max_weight + 1;
            
            vector<vector<int> > pi(num_ports, vector<int>(num_ports, 0));
            
            for (int i = 0; i < num_ports; ++i)
                for (int o = 0; o < num_ports; ++o)
                    pi[i][o] = rand() % max_processing_time + 1;
            
            Flow flow(weight, pi);
            flows.push_back(flow);
        }
        
        CF_instance coflow(num_flows, num_ports, flows);
        coflow.print();
        
        pair<CF_solution, COS_solution> sol = coflow.approx2();
        CF_solution apr2 = sol.first;
        COS_solution reduced_apr = sol.second;
        
        cout << "Reduction:\n";
        reduced_apr.print();
        
        
        cout << endl;
        cout << "Solution:\n";
        apr2.print();
        
        
        cout << "Enter y to continue, any other character to quit: ";
        cin >> input;
    } while (input == 'y');
}


int main() {
    char input;
    cout << "Enter i to run in interactive mode, or a to run randomized tests: ";
    cin >> input;
    if (input == 'i')
        interactive_mode();
    else if (input == 'a')
        all_tests();
    return 0;
}

