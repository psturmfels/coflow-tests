#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <utility>
#include "coflow.h"
#include "open_shop.h"

using namespace std;

Flow::Flow(const double w, std::vector<vector<int> > &pi) {
    weight = w;
    processing_times = move(pi);
    p_sum = 0;
    for (unsigned i = 0; i < processing_times.size(); ++i)
        for (unsigned o = 0; o < processing_times.size(); ++o)
            p_sum += processing_times[i][o];
}

Flow::Flow() {
    weight = -1;
    p_sum = -1;
}

void Flow::print() const {
    cout << "\t";
    for (unsigned o = 0; o < processing_times.size(); ++o) {
        cout << "out" << (o + 1) << "\t";
    }
    cout << endl << "\t";
    for (unsigned o = 0; o < processing_times.size(); ++o) {
        cout << "–––––\t";
    }
    cout << endl;
    
    for (unsigned i = 0; i < processing_times.size(); ++i) {
        cout << "in" << (i + 1) << " |\t";
        for (unsigned o = 0; o < processing_times.size(); ++o) {
            cout << processing_times[i][o] << "\t";
        }
        cout << endl;
    }
}

int Flow::get_delta() const {
    int max_delta = 0;
    for (unsigned i = 0; i < processing_times.size(); ++i) {
        int delta_i = 0;
        int delta_o = 0;
        for(unsigned o = 0; o < processing_times.size(); ++o) {
            delta_i += processing_times[i][o];
            delta_o += processing_times[o][i];
        }
        if (max_delta < delta_i) {
            max_delta = delta_i;
        }
        if (max_delta < delta_o) {
            max_delta = delta_o;
        }
    }
    return max_delta;
}

int Flow::get_delta_i(int i) const {
    int delta = 0;
    for (unsigned o = 0; o < processing_times.size(); ++o)
        delta += processing_times[i][o];
    return delta;
}

int Flow::get_delta_o(int o) const {
    int delta = 0;
    for (unsigned i = 0; i < processing_times.size(); ++i)
        delta += processing_times[i][o];
    return delta;
}

CF_solution::CF_solution(const int N) {
    num_flows = N;
    completion_times.resize(N, 0);
    wct = 0;
}

CF_solution::CF_solution(const int N, const double w, std::vector<int> &ct) {
    num_flows = N;
    completion_times = move(ct);
    wct = w;
}

void CF_solution::print() const {
    cout << "\nCtimes: ";
    for (int j = 0; j < num_flows; ++j) {
        cout << completion_times[j] << " ";
    }
    cout << "\nSum of weighted completions times: " << wct << endl;
    for (int f = 0; f < num_flows; ++f) {
        cout << "Flow " << (f + 1) << " with weight " << flows[f].weight << " ";
        cout << "and max delta: " << flows[f].get_delta() << endl;
        flows[f].print();
        cout << endl;
    }
}

CF_instance::CF_instance(const int N, const int M, vector<Flow> &f) {
    num_ports = M;
    num_flows = N;
    flows = f;
}

void CF_instance::print() const {
    cout << "Outputs down, Inputs across\n";
    for (int f = 0; f < num_flows; ++f) {
        cout << "Flow " << (f + 1) << " with weight " << flows[f].weight << " ";
        cout << "and max delta: " << flows[f].get_delta() << endl;
        flows[f].print();
        cout << endl;
    }
}

pair<CF_solution, COS_solution> CF_instance::approx2() const {
    //Reduction
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
 
    //Initialization
    vector<Flow> f = flows;
    CF_solution apr2(num_flows);
    int time_so_far = 0;
    
    //Moving Back Edges
    for (int j = 0; j < num_flows; ++j) {
        if (f[reduced_apr.order[j]].p_sum > 0) {
        
            int delta_j_max = f[reduced_apr.order[j]].get_delta();
            
            //Run greedy pulling
            for (int k = j + 1; k < num_flows; ++k) {
                if (f[reduced_apr.order[k]].p_sum > 0) {
                    //Calculate maximum degree of j
                    for (int i = 0; i < num_ports; ++i) {
                        
                        //Calculate degree of input port to move to
                        int delta_ji = f[reduced_apr.order[j]].get_delta_i(i);
                        for (int o = 0; o < num_ports; ++o) {
                            
                            //Calculate degree of output port to move to
                            int delta_jo = f[reduced_apr.order[j]].get_delta_o(o);
                            
                            while (f[reduced_apr.order[k]].processing_times[i][o] > 0 &&
                                   delta_j_max > delta_ji && delta_j_max > delta_jo) {
                                f[reduced_apr.order[j]].processing_times[i][o] += 1;
                                f[reduced_apr.order[j]].p_sum += 1;
                                
                                f[reduced_apr.order[k]].processing_times[i][o] -= 1;
                                f[reduced_apr.order[k]].p_sum -= 1;
                                    
                                ++delta_ji;
                                ++delta_jo;
                            }
                            if (f[reduced_apr.order[k]].p_sum == 0) {
                                apr2.completion_times[reduced_apr.order[k]] = apr2.completion_times[reduced_apr.order[j]];
                            }
                        }
                    }
                }
            }
            
            
            //At this point, everything from 0 to j has been scheduled.
            time_so_far += delta_j_max;
            apr2.completion_times[j] = time_so_far;
            apr2.wct += time_so_far * f[reduced_apr.order[j]].weight;
        }
    }
    apr2.flows = move(f);
    return pair<CF_solution, COS_solution>(apr2, reduced_apr);
}
