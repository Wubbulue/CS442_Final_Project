#!/bin/bash

#SBATCH --partition normal
#SBATCH --nodes 1
#SBATCH --ntasks 4
#SBATCH --ntasks-per-node=4
#SBATCH --mem 2900M
#SBATCH --time 02:00:00
#SBATCH --job-name Game_Of_Life_Shared
#SBATCH --mail-user jewicks@unm.edu
#SBATCH --mail-type ALL
#SBATCH --output=shared.out
module load openmpi
srun --mpi=pmi2 ../build/gol_mpi/gol_shared -n_iter 1000 -n_dims 8192
srun --mpi=pmi2 ../build/gol_mpi/gol_shared -n_iter 1000 -n_dims 4096
srun --mpi=pmi2 ../build/gol_mpi/gol_shared -n_iter 1000 -n_dims 2048
srun --mpi=pmi2 ../build/gol_mpi/gol_shared -n_iter 1000 -n_dims 1024
srun --mpi=pmi2 ../build/gol_mpi/gol_shared -n_iter 1000 -n_dims 512
srun --mpi=pmi2 ../build/gol_mpi/gol_shared -n_iter 1000 -n_dims 256