# Coflow Scheduling
### An implementation of the 4-approximation to the NP-hard co-flow problem done in C++.


The co-flow scheduling problem goes as follows: 
You are given M input ports and M output ports, and N jobs, called "flows".
Each job is an MxM matrix such that the ijth entry represents the amount of data
to be sent from input port i to output port j. Each job has a weight wk for 1 <= k <= N.
Each port (intput and output) has unit capacity, meaning that in a single time slot, 
a port can only be sending or receiving one packet of data. A job is called
complete when all of its data has been routed from each input to each output.

Given the above constraints, route each jobs' data from the input to the output ports
such that all jobs complete and the sum of the weighted completion times in minimized. 
That is, minimize the function: sum(wk * Ck), 1 <= k <= N, where Ck = completion time of job k.

More about the coflow problem can be seen in the [Original Coflow Paper](http://dl.acm.org/citation.cfm?id=2390237).

This problem is proven to be NP-hard, even when M = 2 and all jobs have unit weight.
The algorithm implemented here is a greedy 4-approximation algorithm based on 
[Minimizing the sum of weighted completion times in a concurrent open shop](http://www.sciencedirect.com/science/article/pii/S0167637710000556) 
and [Manish Purohit's](http://www.cs.umd.edu/~manishp/) thesis.

### Example
M = 2, N = 2  

| Flow 1, w1 = 3| out1          | out2  |
| ------------- | ------------- | ----- |
| in1		| 5 		|   5 	|
| in2     	| 0      	|   9 	|


| Flow 2, w2 = 2| out1          | out2  |
| ------------- | ------------- | ----- |
| in1		| 1 		|   3 	|
| in2     	| 3      	|   9 	|


The above defines an instance of the problem. In the example, 
Flow 1 has 9 units of data to send from input 2 to output 2.
Below is the optimal solution to the problem. The coflows have been rearranged,
and each can be completed in its delta time – the maximum column or row sum – using
a matching algorithm.

Solution:
Order: 1 2 
Completion Times: 14 26 
Sum of weighted completions times:  3 * 14 + 2 * 26 = 94

| Flow 1, w1 = 3| out1          | out2  |
| ------------- | ------------- | ----- |
| in1		| 6 		|   5 	|
| in2     	| 3      	|   9 	|


| Flow 2, w2 = 2| out1          | out2  |
| ------------- | ------------- | ----- |
| in1		| 0 		|   3 	|
| in2     	| 0      	|   9 	|


