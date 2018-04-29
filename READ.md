block-hw-matmul.c - program for block size 16 matrix multiplication on single core i.e core 0.
simple-hw-matmul.c - program for simple matrix multiplication on single core i.e core 0.
block2Core-hw-matmul.c - program for block size 16 matrix multiplication on core 0 and core 7.
simple2Core-hw-matmul.c - program for simple matrix multiplication on core 0 and core 7.
singlecore_measure.sh - shell script that compiles and runs the program simple-hw-matmul.c and block-hw-matmul.c 5 times each.
doublecore_measure.sh - shell script that compiles and runs the program simple2Core-hw-matmul.c and block2Core-hw-matmul.c 5 times each.

NOTE:
Please run the below command before running the program:
salloc -N 1 -n 8 --exclusive

Please let me know in case of any concerns.