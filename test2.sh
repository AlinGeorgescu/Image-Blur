#!/bin/bash

# qsub -cwd -q hp-sl.q -b n ./test2.sh

module load compilers/gnu-5.4.0
module load libraries/openmpi-2.0.1-gcc-5.4.0
module load compilers/solarisstudio-12.5

set -o xtrace

cd /export/home/acs/stud/a/alin.georgescu/Image-Blur/Serial
make
collect ./blur ../images/landscape.pnm out.pnm 100
make clean
rm out.pnm

cd /export/home/acs/stud/a/alin.georgescu/Image-Blur/Pthreads
make
collect ./blur ../images/landscape.pnm out.pnm 100 2

collect ./blur ../images/landscape.pnm out.pnm 100 4

collect ./blur ../images/landscape.pnm out.pnm 100 8
make clean
rm out.pnm

cd /export/home/acs/stud/a/alin.georgescu/Image-Blur/OpenMPI
make
collect mpirun -np 2 ./blur ../images/landscape.pnm out.pnm 100

collect mpirun -np 4 ./blur ../images/landscape.pnm out.pnm 100

collect mpirun -np 8 ./blur ../images/landscape.pnm out.pnm 100
make clean
rm out.pnm

cd /export/home/acs/stud/a/alin.georgescu/Image-Blur/OpenMP
make
export OMP_NUM_THREADS=2
collect ./blur ../images/landscape.pnm out.pnm 100

export OMP_NUM_THREADS=4
collect ./blur ../images/landscape.pnm out.pnm 100

export OMP_NUM_THREADS=8
collect ./blur ../images/landscape.pnm out.pnm 100
make clean
rm out.pnm

cd /export/home/acs/stud/a/alin.georgescu/Image-Blur/Hybrid
make
export OMP_NUM_THREADS=2
collect mpirun -np 2 ./blur ../images/landscape.pnm out.pnm 100

export OMP_NUM_THREADS=2
collect mpirun -np 4 ./blur ../images/landscape.pnm out.pnm 100

export OMP_NUM_THREADS=4
collect mpirun -np 2 ./blur ../images/landscape.pnm out.pnm 100
make clean
rm out.pnm

set +o xtrace
