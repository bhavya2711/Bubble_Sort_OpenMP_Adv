#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <chrono>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);

  
#ifdef __cplusplus
}
#endif



int main (int argc, char* argv[]) {

  //forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  }
  
  if (argc < 3) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

   //command line inputs
  int n = atoi(argv[1]);
  int numOfThreads = atoi(argv[2]);
  
  omp_set_num_threads(numOfThreads); //setting number of threads based on command line input
  int * arr = new int [n];

  generateMergeSortData (arr, n);
  
  //staring clock
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  
 for(int i = 0; i < n; i++)
  {
  	int hop = i%2;
	//openMP directive 
  	#pragma omp parallel for default(none) shared(arr,n, hop)
  	for(int j = hop; j < n-1; j+=2)
  	{
  		if(arr[j] > arr[j+1])  //swap function from algorithm library
  			std::swap(arr[j],arr[j+1]);  //this will compare two adjacent element and swap then  
  	}
  }
  //stopping clock
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  
  std::cerr<<elapsed_seconds.count()<<std::endl; //printing time taken in error stream
 
  
  checkMergeSortResult (arr, atoi(argv[1]));
  
  delete[] arr;
  

  return 0;
}
