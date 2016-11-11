This is a parallel implementation of the Knn algorithm. I'm using CPU threading to use multiple threads to divide the Knn task.
To run the file just run the ./main. To change the number of threads in the code change the n_threads to desired amount.

Comparing the runtime of the serial and parallel versions considering the datasets with different
sizes. The speedup using 2, 4, and 8 threads are:

For reference the time taken by the serial code on my system is:
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

These evaluations are done on an i7 6700HQ 2.6GHz Quad-core Processor
