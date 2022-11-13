#!/bin/bash

#SBATCH --nodes=24
#SBATCH --ntasks-per-node=1
#SBATCH --time=0:10:00
#SBATCH --mail-type=END,FAIL
#SBATCH --account=PCS0268
#SBATCH --job-name=MPI_Matrix_Vector_Multiplication

module load openmpi 
module load boost

#module load intel
cd $SLURM_SUBMIT_DIR/Default
make clean && make OSC

mpiexec -n 1 MPI 6000 6000 >> results.csv
mpiexec -n 2 MPI 6000 6000 >> results.csv
mpiexec -n 3 MPI 6000 6000 >> results.csv
mpiexec -n 4 MPI 6000 6000 >> results.csv
mpiexec -n 6 MPI 6000 6000 >> results.csv
mpiexec -n 12 MPI 6000 6000 >> results.csv
mpiexec -n 24 MPI 6000 6000 >> results.csv

mpiexec -n 1 MPI 12000 6000 >> results.csv
mpiexec -n 2 MPI 12000 6000 >> results.csv
mpiexec -n 3 MPI 12000 6000 >> results.csv
mpiexec -n 4 MPI 12000 6000 >> results.csv
mpiexec -n 6 MPI 12000 6000 >> results.csv
mpiexec -n 12 MPI 12000 6000 >> results.csv
mpiexec -n 24 MPI 12000 6000 >> results.csv

mpiexec -n 1 MPI 12000 12000 >> results.csv
mpiexec -n 2 MPI 12000 12000 >> results.csv
mpiexec -n 3 MPI 12000 12000 >> results.csv
mpiexec -n 4 MPI 12000 12000 >> results.csv
mpiexec -n 6 MPI 12000 12000 >> results.csv
mpiexec -n 12 MPI 12000 12000 >> results.csv
mpiexec -n 24 MPI 12000 12000 >> results.csv

mpiexec -n 1 MPI 24000 12000 >> results.csv
mpiexec -n 2 MPI 24000 12000 >> results.csv
mpiexec -n 3 MPI 24000 12000 >> results.csv
mpiexec -n 4 MPI 24000 12000 >> results.csv
mpiexec -n 6 MPI 24000 12000 >> results.csv
mpiexec -n 12 MPI 24000 12000 >> results.csv
mpiexec -n 24 MPI 24000 12000 >> results.csv

mpiexec -n 1 MPI 24000 24000 >> results.csv
mpiexec -n 2 MPI 24000 24000 >> results.csv
mpiexec -n 3 MPI 24000 24000 >> results.csv
mpiexec -n 4 MPI 24000 24000 >> results.csv
mpiexec -n 6 MPI 24000 24000 >> results.csv
mpiexec -n 12 MPI 24000 24000 >> results.csv
mpiexec -n 24 MPI 24000 24000 >> results.csv