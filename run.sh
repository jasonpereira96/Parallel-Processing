rm qs.out
mpic++ qs.cpp -o qs.out
mpiexec -n 8 ./qs.out A_vector_30_8_4.txt pivot_vector_30_8_4.txt max_index_30_8_4.txt
 