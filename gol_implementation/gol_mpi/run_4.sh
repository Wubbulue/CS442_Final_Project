#!/bin/bash

#SBATCH --partition normal
#SBATCH --ntasks 4
#SBATCH --mem 2900M
#SBATCH --time 00:05:00
#SBATCH --job-name mat_mult
#SBATCH --mail-user your_username@unm.edu
#SBATCH --mail-type ALL
#SBATCH --output=4.out

module load openmpi

srun --mpi=pmi2 ../build/gol_mpi/gol_mpi
