#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include "coflow.h"

using namespace std;

Flow::Flow(const double w, std::vector<vector<int> > &pi) {
    weight = w;
    processing_times = move(pi);
}

Flow::Flow() {
    weight = -1;
}

CF_solution::CF_solution(const int N) {
    num_jobs = N;
    order.resize(N);
    completion_times.resize(N);
    wct = 0;
}

CF_solution(const int N, const double w, std::vector<int> &o, std::vector<int> &ct) {
    num_jobs = N;
    order = move(o);
    completion_times = move(ct);
    wct = w;
}

void print(const std::vector<Flow> &flows) {
    cout << "Orders: ";
    for (int j = 0; j < num_jobs; ++j) {
        cout << (order[j] + 1) << " ";
    }
    cout << "\nWeights: ";
    for (int j = 0; j < num_jobs; ++j) {
        cout << flows[order[j]].weight << " ";
    }
    cout << "\nCtimes: ";
    for (int j = 0; j < num_jobs; ++j) {
        cout << completion_times[j] << " ";
    }
    cout << "\nSum of weighted completions times: " << wct << endl;
}

