rm qs.out
mpic++ qs.cpp -o qs.out
<<<<<<< HEAD
mpiexec -n 4 ./qs.out 1000 
=======
mpiexec -n 8 ./qs.out A_vector_30_8_4.txt pivot_vector_30_8_4.txt max_index_30_8_4.txt
 
>>>>>>> master
