#! /bin/bash
#source ./.bashrc

cd /home2/accts11/Project2/tp

# compilation 
echo 'compiling simple2Core-hw-matmul.c file'
cc -o3 simple2Core-hw-matmul.c -o simpledouble -I/opt/papi/5.6.0/include -L/opt/papi/5.6.0/lib64 -lpapi

echo 'Matrix size = 1024 for all iteration'
echo 'First iteration'

./simpledouble 1024

echo '2nd iteration'

./simpledouble 1024

echo '3rd iteration'

./simpledouble 1024

echo '4th iteration'

./simpledouble 1024

echo '5th iteration'

./simpledouble 1024

# compilation 
echo 'compiling simple-hw-matmul.c file'
cc -o3 simple-hw-matmul.c -o simplesingle -I/opt/papi/5.6.0/include -L/opt/papi/5.6.0/lib64 -lpapi

echo 'Matrix size = 1024 for all iteration'
echo 'First iteration'

./simplesingle 1024

echo '2nd iteration'

./simplesingle 1024

echo '3rd iteration'

./simplesingle 1024

echo '4th iteration'

./simplesingle 1024

echo '5th iteration'

./simplesingle 1024

exit 
