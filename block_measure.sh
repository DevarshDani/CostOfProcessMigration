#! /bin/bash
#source ./.bashrc

cd /home2/accts11/Project2/tp

# compilation 
echo 'compiling block2Core-hw-matmul.c file'
cc -o3 block2Core-hw-matmul.c -o blockdouble -I/opt/papi/5.6.0/include -L/opt/papi/5.6.0/lib64 -lpapi

echo 'Matrix size = 1024 for all iteration'
echo 'First iteration'

./blockdouble 1024

echo '2nd iteration'

./blockdouble 1024

echo '3rd iteration'

./blockdouble 1024

echo '4th iteration'

./blockdouble 1024

echo '5th iteration'

./blockdouble 1024

# compilation 
echo 'compiling block-hw-matmul.c file'
cc -o3 block-hw-matmul.c -o blocksingle -I/opt/papi/5.6.0/include -L/opt/papi/5.6.0/lib64 -lpapi

echo 'Matrix size = 1024 for all iteration'
echo 'First iteration'

./blocksingle 1024

echo '2nd iteration'

./blocksingle 1024

echo '3rd iteration'

./blocksingle 1024

echo '4th iteration'

./blocksingle 1024

echo '5th iteration'

./blocksingle 1024

exit 
