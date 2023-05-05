rm qs.out
mpic++ qs.cpp -o qs.out
mpiexec -n 8 ./qs.out test/A_vector_30_8_4.txt test/pivot_vector_30_8_4.txt 8 0 4 