#!/bin/bash

# qsub -cwd -q hp-sl.q -b n ./test.sh

module load compilers/gnu-5.4.0
module load libraries/openmpi-2.0.1-gcc-5.4.0

set -o xtrace

cd /export/home/acs/stud/a/alin.georgescu/Image-Blur/Serial
make
time ./blur ../images/landscape.pnm out.pnm 10
time ./blur ../images/landscape.pnm out.pnm 100
time ./blur ../images/landscape.pnm out.pnm 1000

time ./blur ../images/baby-yoda.pnm out.pnm 10
time ./blur ../images/baby-yoda.pnm out.pnm 100
time ./blur ../images/baby-yoda.pnm out.pnm 1000

time ./blur ../images/rorschach.pgm out.pnm 10
time ./blur ../images/rorschach.pgm out.pnm 100
time ./blur ../images/rorschach.pgm out.pnm 1000
make clean
rm out.pnm

cd /export/home/acs/stud/a/alin.georgescu/Image-Blur/Pthreads
make
time ./blur ../images/landscape.pnm out.pnm 10 2
time ./blur ../images/landscape.pnm out.pnm 100 2
time ./blur ../images/landscape.pnm out.pnm 1000 2

time ./blur ../images/landscape.pnm out.pnm 10 4
time ./blur ../images/landscape.pnm out.pnm 100 4
time ./blur ../images/landscape.pnm out.pnm 1000 4

time ./blur ../images/landscape.pnm out.pnm 10 8
time ./blur ../images/landscape.pnm out.pnm 100 8
time ./blur ../images/landscape.pnm out.pnm 1000 8
make clean
rm out.pnm

cd /export/home/acs/stud/a/alin.georgescu/Image-Blur/OpenMPI
make
time mpirun -np 2 ./blur ../images/landscape.pnm out.pnm 10
time mpirun -np 2 ./blur ../images/landscape.pnm out.pnm 100
time mpirun -np 2 ./blur ../images/landscape.pnm out.pnm 1000

time mpirun -np 4 ./blur ../images/landscape.pnm out.pnm 10
time mpirun -np 4 ./blur ../images/landscape.pnm out.pnm 100
time mpirun -np 4 ./blur ../images/landscape.pnm out.pnm 1000

time mpirun -np 8 ./blur ../images/landscape.pnm out.pnm 10
time mpirun -np 8 ./blur ../images/landscape.pnm out.pnm 100
time mpirun -np 8 ./blur ../images/landscape.pnm out.pnm 1000
make clean
rm out.pnm

cd /export/home/acs/stud/a/alin.georgescu/Image-Blur/OpenMP
make
export OMP_NUM_THREADS=2
time ./blur ../images/landscape.pnm out.pnm 10
time ./blur ../images/landscape.pnm out.pnm 100
time ./blur ../images/landscape.pnm out.pnm 1000

export OMP_NUM_THREADS=4
time ./blur ../images/landscape.pnm out.pnm 10
time ./blur ../images/landscape.pnm out.pnm 100
time ./blur ../images/landscape.pnm out.pnm 1000

export OMP_NUM_THREADS=8
time ./blur ../images/landscape.pnm out.pnm 10
time ./blur ../images/landscape.pnm out.pnm 100
time ./blur ../images/landscape.pnm out.pnm 1000
make clean
rm out.pnm

cd /export/home/acs/stud/a/alin.georgescu/Image-Blur/Hybrid
make
export OMP_NUM_THREADS=2
time mpirun -np 2 ./blur ../images/landscape.pnm out.pnm 10
time mpirun -np 2 ./blur ../images/landscape.pnm out.pnm 100
time mpirun -np 2 ./blur ../images/landscape.pnm out.pnm 1000

export OMP_NUM_THREADS=2
time mpirun -np 4 ./blur ../images/landscape.pnm out.pnm 10
time mpirun -np 4 ./blur ../images/landscape.pnm out.pnm 100
time mpirun -np 4 ./blur ../images/landscape.pnm out.pnm 1000

export OMP_NUM_THREADS=4
time mpirun -np 2 ./blur ../images/landscape.pnm out.pnm 10
time mpirun -np 2 ./blur ../images/landscape.pnm out.pnm 100
time mpirun -np 2 ./blur ../images/landscape.pnm out.pnm 1000
make clean
rm out.pnm

set +o xtrace
