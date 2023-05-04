rm qs.out
mpic++ qs.cpp -o qs.out
mpiexec -n 4 ./qs.out 1000 
