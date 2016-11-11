To run the file just run the ./main. To change the number of threads in the code change the n_threads to desired amount.

1. Compare the runtime of the serial and parallel versions considering the datasets with different
sizes. Calculate the speedup using 2, 4, and 8 threads.

Ans. For reference the time taken by the serial code on my system is:
Small dataset: 29 ms
Medium dataset: 10354 ms

Following is the average of 5 runs per dataset with the specified number of threads:
Small dataset:
2 threads - 20 ms
4 threads - 17 ms
8 threads - 13 ms

Medium dataset:
2 threads - 5047 ms
4 threads - 2635 ms
8 threads - 2096 ms

2. Estimate the proportion of parallel code by relating the speedup you get and the number of
threads employed. What’s the maximum speedup you would be able to obtain using an infinite
number of threads and cores?

Ans. By analysing the code and the speed up achieved we can assume that every time the number of threads are doubled 
the time taken is reduced by half. Also the speed up depends upon our dataset, therefore if we have unlimited number 
of threads and core our speed up will be as quick as our dataset->num_instances i.e. our outer loop in the program.

3. Let’s run using 256 threads. What’s the speedup now?

Ans. Following is the average of 5 runs per dataset with 256 threads:
Small dataset: 27 ms
Medium dataset: 2028 ms
