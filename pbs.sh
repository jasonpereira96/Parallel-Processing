#!/bin/bash
#PBS -N qs_jvum
#PBS -l walltime=00:00:15
#PBS -l nodes=4:ppn=2
#PBS -q edu_shared
#PBS -j oe
#
## Move to the directory where the job was submitted
#
cd $PBS_O_WORKDIR
#
#  setup MPI programming environment
#
module load MPICH/3.3.2-GCC-9.3.0
#
# Build mpi job
#
rm output.txt
rm qs.out
mpic++ qs.cpp -o qs.out
#
# Run mpi job
#
mpirun ./qs.out A_vector_30_8_4.txt pivot_vector_30_8_4.txt