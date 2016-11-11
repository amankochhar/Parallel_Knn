#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <iostream>
#include <pthread.h>
#include "libarff/arff_parser.h"
#include "libarff/arff_data.h"

using namespace std;

// making the dataset global
ArffData *dataset;
// size of each dataset
int size;
//no. of threads
int n_threads = 8;
int totalThreads = n_threads - 1;
int * results;

void* KNNthread(void *param){

    int *thisThread = ((int*)&param);
    //printf("Thread No. - %d\n", ((int*)param));
    int startPoint, endPoint;

    //dividing the dataset for each thread
    if(totalThreads != *thisThread){
        startPoint = ((*thisThread) * size);
        endPoint = ((*thisThread) + 1) * size; 
        //printf(" For Thread %d the starting point is %d and end point is %d\n", ((int*)param), startPoint, endPoint);
    }
    else{
        startPoint = ((*thisThread) * size);
        endPoint = dataset->num_instances();
        //printf(" For Thread %d the starting point is %d and end point is %d\n", ((int*)param), startPoint, endPoint);
    }
    // we now have start and end point for all our threads

    for(int i = startPoint; i < endPoint; i++) // for each instance in the dataset
    {
        float smallestDistance = FLT_MAX;
        int smallestDistanceClass;

        for(int j = 0; j < dataset->num_instances(); j++) // target each other instance
        {
            if(i == j) continue;
            
            float distance = 0;
            
            for(int k = 0; k < dataset->num_attributes() - 1; k++) // compute the distance between the two instances
            {
                float diff = dataset->get_instance(i)->get(k)->operator float() - dataset->get_instance(j)->get(k)->operator float();
                distance += diff * diff;
            }
            
            distance = sqrt(distance);
            
            if(distance < smallestDistance) // select the closest one
            {
                smallestDistance = distance;
                smallestDistanceClass = dataset->get_instance(j)->get(dataset->num_attributes() - 1)->operator int32();
            }
        }
        results[i] = smallestDistanceClass;
    }
}


int* KNN(int n_threads) 
{
    //create threads and call KNNthread
    int i;
    pthread_t *threads;

    threads = (pthread_t*)malloc(n_threads * sizeof(pthread_t));

    for(i = 0; i < n_threads; i++)
        pthread_create(&threads[i],NULL,KNNthread, ((int*) i));
    
    for(i = 0; i < n_threads; i++)
        pthread_join(threads[i], NULL);

    return results;
}

int* computeConfusionMatrix(int* predictions, ArffData* dataset)
{
    int* confusionMatrix = (int*)calloc(dataset->num_classes() * dataset->num_classes(), sizeof(int)); // matriz size numberClasses x numberClasses
    
    for(int i = 0; i < dataset->num_instances(); i++) // for each instance compare the true class and predicted class
    {
        int trueClass = dataset->get_instance(i)->get(dataset->num_attributes() - 1)->operator int32();
        int predictedClass = predictions[i];
        
        confusionMatrix[trueClass*dataset->num_classes() + predictedClass]++;
    }
    
    return confusionMatrix;
}

float computeAccuracy(int* confusionMatrix, ArffData* dataset)
{
    int successfulPredictions = 0;
    
    for(int i = 0; i < dataset->num_classes(); i++)
    {
        successfulPredictions += confusionMatrix[i*dataset->num_classes() + i]; // elements in the diagnoal are correct predictions
    }
    
    return successfulPredictions / (float) dataset->num_instances();
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        cout << "Usage: ./main datasets/small.arff" << endl;
        exit(0);
    }
    
    ArffParser parser(argv[1]);
    dataset = parser.parse();
    // we'll save the result in this and collect it when joining the threads
    results = (int*)malloc(dataset->num_instances() * sizeof(int));
    struct timespec start, end;
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);


    // based on the no. of threads divide the dataset into further parts
    int num = dataset->num_instances();
    size = ceil(num/n_threads);
    //printf("size for each thread = %d\n", size);

    int* predictions = KNN(n_threads);
    int* confusionMatrix = computeConfusionMatrix(predictions, dataset);
    float accuracy = computeAccuracy(confusionMatrix, dataset);
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    uint64_t diff = (1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec) / 1e6;
  
    printf("The 1NN classifier for %lu instances required %llu ms CPU time, accuracy was %.4f\n", dataset->num_instances(), (long long unsigned int) diff, accuracy);
}
