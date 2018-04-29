#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "papi.h"
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>
#define NUM_EVENTS 2

//Array declaration
double *A, *B, *C;
int dimension;


int main(int argc, char **argv)
{
  int i,j;
  float MR_L2 = 0.0f, MR1_L2 = 0.0f;
  long long start_time, end_time;
  long long values[NUM_EVENTS];
  long long values1[NUM_EVENTS];
  int eventset = PAPI_NULL;
  int eventset1 = PAPI_NULL;
  PAPI_library_init(PAPI_VER_CURRENT);
  PAPI_create_eventset(&eventset);
  PAPI_create_eventset(&eventset1);
  PAPI_add_event(eventset, PAPI_L2_TCM);
  PAPI_add_event(eventset, PAPI_L2_TCA);
  PAPI_add_event(eventset1, PAPI_L2_TCM);
  PAPI_add_event(eventset1, PAPI_L2_TCA);
  printf("created and added events.\n");
  
  if ( argc < 2) {
    printf("Usage: matmul <dimension>\n");
    exit(-1);
  }
        
  dimension = atoi(argv[1]);
    
  A = (double*)malloc(dimension*dimension*sizeof(double));
  B = (double*)malloc(dimension*dimension*sizeof(double));
  C = (double*)malloc(dimension*dimension*sizeof(double));
  if ( NULL == A || NULL == B || NULL == C ) {
    printf("Could not allocate memory. Aborting \n");
    exit (-1);
  }

  //Initialize the arrays with random values
  for(i = 0; i < dimension; i++) {
    for(j = 0; j < dimension; j++) {
      A[dimension*i+j] = (rand()/(RAND_MAX + 1.0));
      B[dimension*i+j] = (rand()/(RAND_MAX + 1.0));
      C[dimension*i+j] = 0.0;
    }
  }

  
    int k, p, q, r;
    int block_i, block_j, block_k;
    int block = 16;
    int num_blocks = dimension / block;

    // binding to core 0
    int core_id = 0;
    unsigned int len = sizeof(cpu_set_t);
    cpu_set_t mask;
    pid_t pid = getpid();
    CPU_ZERO(&mask);
    CPU_SET(core_id, &mask);
    int res = sched_setaffinity(pid, len, &mask);
    if( res != 0 ){
	printf("Error in setaffinity...\n");
	exit(EXIT_FAILURE);
    }
    if(sched_getaffinity(pid, len, &mask) == 0){
	if(CPU_ISSET(core_id, &mask))
		printf("Process will run on CPU %d\n", sched_getcpu());
        if(!(CPU_ISSET(7, &mask)))
		printf("Process will not run on CPU 7\n");
    }

    printf("**************************Core 0 block size 16**************************************************\n");
    PAPI_start(eventset);
    start_time = PAPI_get_real_usec();

    for(i = 0; i < (num_blocks/2); i++)  {
        block_i = i * block;
        for(j = 0; j < num_blocks; j++)  {
            block_j = j * block;
            for(k = 0; k < num_blocks; k++) {
                block_k = k * block;

                for (p=block_i; p < block_i+block; p++)
                    for (q=block_j; q < block_j+block; q++)
                        for (r=block_k; r < block_k+block; r++)
                            C[dimension*p+q] += A[dimension*p+r] *
                                B[dimension*r+q];
	    }
        }
    }

    PAPI_read(eventset, values);
    PAPI_stop(eventset, values);
    PAPI_reset(eventset);
   

    // binding to core 7
    core_id = 7;
    len = sizeof(cpu_set_t);
    pid = getpid();
    CPU_ZERO(&mask);
    CPU_SET(core_id, &mask);
    res = sched_setaffinity(pid, len, &mask);
    if( res != 0 ){
	printf("Error in setaffinity...\n");
	exit(EXIT_FAILURE);
    }


    PAPI_start(eventset1);
    

    for(i = num_blocks/2 ; i < num_blocks; i++)  {
        block_i = i * block;
        for(j = 0; j < num_blocks; j++)  {
            block_j = j * block;
            for(k = 0; k < num_blocks; k++) {
                block_k = k * block;

                for (p=block_i; p < block_i+block; p++)
                    for (q=block_j; q < block_j+block; q++)
                        for (r=block_k; r < block_k+block; r++)
                            C[dimension*p+q] += A[dimension*p+r] *
                                B[dimension*r+q];
	    }
        }
    }

    end_time = PAPI_get_real_usec();
    PAPI_read(eventset1, values1);
    PAPI_stop(eventset1, values1);
    MR_L2 = (float) values[0]/values[1];
    printf("L2 cache misses = %lli\nL2 total cache access = %lli\nL2 cache miss rate = %f\n", values[0], values[1], MR_L2);
    printf("**************************Core 7 block size 16**************************************************\n");
    printf("Done with block multiply block size 16 using core %d\n", sched_getcpu());
    MR1_L2 = (float) values1[0]/values1[1];
    printf("L2 cache misses = %lli\nL2 total cache access = %lli\nL2 cache miss rate = %f\ntotal execution time = %lli\n", values1[0], values1[1], MR1_L2, end_time - start_time);
    PAPI_reset(eventset1);


  free(A);
  free(B);
  free(C);
  return 0;
}
