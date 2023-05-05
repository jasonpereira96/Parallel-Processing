rm qs.out
mpic++ qs.cpp -o qs.out
mpiexec -n 16 ./qs.out A_vector_10000_16_4.txt pivot_vector_10000_16_4.txt 0 4

 
