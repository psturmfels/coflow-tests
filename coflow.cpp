#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include "coflow.h"

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

void Flow::print() {
    cout << "  ";
    for (unsigned o = 0; o < processing_times.size(); ++o) {
        cout << (o + 1) << " ";
    }
    
    for (unsigned i = 0; i < processing_times.size(); ++i) {
        cout << (i + 1) << " ";
        for (unsigned o = 0; o < processing_times.size(); ++o) {
            cout << processing_times[i][o] << " ";
        }
        cout << endl;
    }
}

int Flow::get_delta() {
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

int Flow::get_delta_i(int i) {
    int delta = 0;
    for (unsigned o = 0; o < processing_times.size(); ++o)
        delta += processing_times[i][o];
    return delta;
}

int Flow::get_delta_o(int o) {
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

void CF_solution::print() {
    cout << "\nCtimes: ";
    for (int j = 0; j < num_flows; ++j) {
        cout << completion_times[j] << " ";
    }
    cout << "\nSum of weighted completions times: " << wct << endl;
}

CF_instance::CF_instance(const int N, const int M, vector<Flow> &f) {
    num_ports = M;
    num_flows = N;
    flows = f;
}

void CF_instance::print() {
    cout << "Outputs down, Inputs across\n";
    for (int f = 0; f < num_flows; ++f) {
        cout << "Flow " << f << " with weight " << flows[f].weight << " ";
        cout << "and max delta: " << get_delta() << endl;
        flows[f].print();
        cout << endl;
    }
}

CF_solution CF_instance::approx2(vector<int> &flow_order) const {
    //Initialization
    vector<Flow> f = flows;
    CF_solution apr2(num_flows);
    int time_so_far = 0;
    
    //Moving Back Edges
    for (int j = 0; j < num_flows - 1; ++j) {
        if (f[flow_order[j]].p_sum > 0) {
            int delta_j_max = f[flow_order[j]].get_delta;
            for (int k = j + 1; k < num_flows) {
                if (f[flow_order[k]].p_sum > 0) {
                    //Calculate maximum degree of j
                    for (int i = 0; i < num_ports; ++i) {
                        
                        //Calculate degree of input port to move to
                        int delta_ji = f[flow_order[j]].get_delta_i(i);
                        for (int o = 0; o < num_port; ++o) {
                            
                            //Calculate degree of output port to move to
                            int delta_jo = f[flow_order[j]].get_delta_o(o);
                            
                            while (f[flow_order[k]].processing_times[i][o] > 0 &&
                                   delta_j_max > delta_ji && delta_j_max > delta_jo) {
                                f[flow_order[j]].processing_times[i][o] += 1;
                                f[flow_order[j]].p_sum += 1;
                                
                                f[flow_order[k]].processing_times[i][o] -= 1;
                                f[flow_order[k]].p_sum -= 1;
                                    
                                    ++delta_ji;
                                ++delta_jo;
                            }
                            if (f[flow_order[k]].p_sum == 0) {
                                apr2.completion_times[flow_order[k]] = apr2.completion_times[flow_order[j]];
                            }
                        }
                    }
                }
            }
            //At this point, everything from 0 to j has been scheduled.
            time_so_far += delta_j_max;
            apr2.completion_times[flow_order[j]] = time_so_far;
            apr2.wct += time_so_far * f[flow_order[j]].weight;
        }
    }
}




