//mpic++ -ggdb `pkg-config --cflags opencv` -o `basename blur-openmp.c .c` blur-openmp.c `pkg-config --libs opencv` -lm -l pthread -fopenmp
//time mpirun -np 1 ./blur-openmp 4k.jpg miImagen_blur.jpg 15 16

#include <stdio.h>
#include <highgui.h>
#include <stdlib.h>
#include <cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <omp.h>
#include <mpi.h>

using namespace cv;
using namespace std;

IplImage* original = 0;
IplImage* distorsion = 0;
double kernel;
uchar* data;
int n_threads, id;

void blur(int tid, int numprocs){
    int cols, rows, r, g, b, sum, x, y;
    int init,ending;
    tid = omp_get_thread_num();
    init = tid*(original->width/numprocs);
	ending = (tid+1)*original->width/numprocs;
	for(rows = init; rows < ending; rows++){
		for(cols = 0; cols < original->height; cols++){
			  r = g = b = sum = 0;
				for(x = rows; x < original->width && x < rows + kernel; x++){
				    for(y = cols; y < original->height && y < cols + kernel; y++){
							b += data[x*3 + y*original->width*3 + 0];
							g += data[x*3 + y*original->width*3 + 1];
							r += data[x*3 + y*original->width*3 + 2];
							sum++;
						}
			 	}
				b = b / sum;
				g = g / sum;
				r = r / sum;
				data[rows*3 + cols*original->width*3 + 0] = b;
				data[rows*3 + cols*original->width*3 + 1] = g;
				data[rows*3 + cols*original->width*3 + 2] = r;
		}
	}
}


int main(int argc, char *argv[]){
	
	int done, processId, numprocs, I, rc, i;
	done = 0;

	if(argc < 4){
		printf("Faltan argumentos para poder ejecutar el programa \n");
		return -1;
	}

	//abrir la imagen, junto con restriccion
	original = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
    if(!original){
		printf("No se pudo abrir el archivo %s \n", argv[1]);
		return -1;
	}

	MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
        MPI_Comm_rank(MPI_COMM_WORLD, &processId);
	if (processId =! 0) printf("\nLaunching with %i processes", numprocs);

	//inicializando un segundo objeto IplImage para la imagen distorsionada
	distorsion = cvCreateImage(cvSize(original->width,original->height), original->depth, original->nChannels);
	//generando copia de datos
	distorsion->imageData = original->imageData;
	data = (uchar *)distorsion->imageData;

	//cast de char a double
	kernel = atof(argv[3]);
	n_threads = atoi(argv[4]);

	#pragma omp parallel num_threads(n_threads)
	{
	  int threadId = omp_get_thread_num();
	  int threadsTotal = omp_get_num_threads();
	  printf("%d",threadsTotal);
	  int globalId = (processId * threadsTotal) + threadId;
	  blur(globalId, threadsTotal*numprocs);
	}

	//cvNamedWindow("Imagen distorsionada", CV_WINDOW_NORMAL);
	//cvMoveWindow("Imagen distorsionada", 200, 100);
	// mostrar imagen
	//cvShowImage("Imagen distorsionada", distorsion );
	/*Mat nuevo = cvarrToMat(distorsion);
	string nombre;
	nombre = "bluropenmpi-";
	nombre += argv[1];
	imwrite(nombre,nuevo);
	// esperar por llave
	cvWaitKey(0);
	// release de imagen
	cvReleaseImage(&distorsion );*/

  return 0;
  MPI_Finalize();
}
