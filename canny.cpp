#include "./header_files/canny.hpp"
#include "./header_files/main.hpp"

#define GAUSSIAN_SIZE 5 
#define SIGMA 1
const double pi = 3.1415926;
using namespace std;

void noise_reduce(double ** image, double** output_img){
	
	static int center = GAUSSIAN_SIZE / 2;
	//gaussian filter kernel 	
	double x, y;
	double sum = 0;
	double gaussian_filter[GAUSSIAN_SIZE][GAUSSIAN_SIZE];
	for(int i = 0; i < GAUSSIAN_SIZE; ++i){
		x = pow(i - center, 2);
		for (int j = 0; j < GAUSSIAN_SIZE; ++j){
			y = pow( j - center , 2);
			gaussian_filter[i][j] = exp(-(x + y) / (2 * SIGMA * SIGMA)) / (2*pi*SIGMA);
			sum += gaussian_filter[i][j];
		}
	
	}

	for(int i =0; i < GAUSSIAN_SIZE; ++i){
		for(int j = 0; j < GAUSSIAN_SIZE; ++j){
			gaussian_filter[i][j] /= sum;
		}
	}

	
	for(int i = 0; i < GAUSSIAN_SIZE; ++i){
		for (int j = 0; j < GAUSSIAN_SIZE; ++j){
			printf("%lf ", gaussian_filter[i][j]);
		}
		printf("\n");
	}
	double sumx;
	for (uint i = 0 ; i < height; ++i){
		for(uint j = 0; j < width; j++){
			sumx = 0;
			for(int p = -center; p <= center; p++){
				for(int q = -center; q <= center; q++){
					if((i+p) < 0 || (j+q) <0 || (i+p) >= height || (j+q)>= width)
						continue;
					sumx += image[i+p][j+q] * gaussian_filter[p+center][q+center];
			}
		}
	
		output_img[i][j] = sumx;	
		}
	}

}

void intensity_gradient(double** image){

	double kx[3][3] = {{-1.0, 0.0, 1.0}, {-2.0, 0.0, 2.0}, {-1.0, 0.0, 1.0}}; 
	double ky[3][3] = {{1.0, 2.0, 1.0}, {0.0, 0.0, 0.0}, {-1.0, -2.0, -1.0}}; 

	double partial_sumx;
	double partial_sumy;
	double max_val = 0; // used to normilize
	double grad;
	
	double **tempx = (double **)malloc(height*sizeof(double*));

	for (uint i = 0; i < height; ++i){
		tempx[i] = (double*)malloc(width*sizeof(double));
	}
	double **tempy = (double **)malloc(height*sizeof(double*));

	for (uint i = 0; i < height; ++i){
		tempy[i] = (double*)malloc(width*sizeof(double));
	}
	//convolution
	for(uint i = 0; i < height; ++i){
		for(uint j = 0; j < width; ++j){
			partial_sumx = 0;
			partial_sumy = 0;
			for(int p = -1; p <= 1; ++p){
				for(int q = -1; q <= 1; ++q){
					//conor control
					if((i+p) < 0 || (j+q) <0 || (i+p) >= height || (j+q)>= width)
						continue;
					
					partial_sumx += image[i+p][j+q] * kx[p+1][q+1];
					partial_sumy += image[i+p][j+q] * ky[p+1][q+1];	
				}
			}
			//write one element	
			tempx[i][j] = partial_sumx;
			tempy[i][j] = partial_sumy;
		}	
	}

	
	
	for(uint i = 0; i < height; ++i){
		for(uint j = 0; j < width; ++j){
			grad = sqrt((tempx[i][j] * tempx[i][j] + (tempy[i][j] * tempy[i][j])));

			if(grad > max_val)
				max_val = grad;
			image[i][j] = grad;
		}
	}	
	
	for(uint i = 0; i < height; ++i){
		for(uint j = 0; j < width; ++j){
			image[i][j] = image[i][j] / max_val * 255;
		}
	}
	for(uint i = 0; i < height; ++i){
		free(tempx[i]);
		free(tempy[i]);
	}	

	free(tempx);
	free(tempy);



}
