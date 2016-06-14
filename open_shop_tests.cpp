#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include "open_shop.h"

using namespace std;

int main() {
    srand (time(NULL));
    
    int num_jobs = 3;
    int num_machines = 3;
    int max_processing_time = 100;
    int max_weight = 25;
    vector<COS_job> jobs(num_jobs);
    
    for (int j = 0; j < num_jobs; ++j) {
        vector<int> pj(num_machines);
        for (int i = 0; i < num_machines; ++i) {
            pj[i] = rand() % max_processing_time;
        }
        double weight = (rand() % max_weight) * 1.0;
        
        jobs[j] = COS_job(weight, pj);
    }
    
    COS_instance open_shop(num_jobs, num_machines, jobs);
    open_shop.print();
    COS_solution apr = open_shop.approximate();
    COS_solution sol = open_shop.solve();
    
    cout << "\nApproximate solution: \n";
    apr.print(jobs);
    cout << "\nActual solution: \n";
    sol.print(jobs);
    
    return 0;
}
