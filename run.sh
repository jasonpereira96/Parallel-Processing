rm qs.out
mpic++ qs.cpp -o qs.out
mpiexec -n 16 ./qs.out test/A_vector_100000_16_4.txt test/pivot_vector_100000_16_4.txt 0 4

 
