#!/bin/bash

#SBATCH --partition normal
#SBATCH --ntasks 16
#SBATCH --mem 2900M
#SBATCH --time 00:05:00
#SBATCH --job-name gol_readin
#SBATCH --mail-user your_username@unm.edu
#SBATCH --mail-type ALL
#SBATCH --output=readin.out

module load openmpi

srun --mpi=pmi2 ../build/gol_mpi/gol_mpi -n_iter 5 -n_dims 64 -save_file save.txt -file_io_flag
