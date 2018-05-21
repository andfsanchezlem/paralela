EXECS=mpi_blur_effect
MPICC?=mpiCC

all: ${EXECS}

mpi_blur_effect: mpi_blur_effect.c
	${MPICC} -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -o mpi_blur_effect mpi_blur_effect.c `pkg-config --cflags opencv` -lopencv_core -lopencv_imgproc -lopencv_highgui -fopenmp

clean:
	rm -f ${EXECS}
