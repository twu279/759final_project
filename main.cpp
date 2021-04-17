#include "header_files/main.hpp"
#include "header_files/canny.hpp"

using namespace std;
/*
 * Global variable
 */
char type[256];
uint width;
uint height;
uint intensity;

void error_check(char* error_message){

	fprintf(stderr, "%s\n", error_message);
	exit(-1);
}


int main(int argc, char** argv){
	
	if(argc != 2){
		error_check((char*)"Not correct Argument\n");
	}

			
	//OPEN image
	ifstream infile(argv[1], ios::binary);
	if(infile.good() != true)
	{
		error_check((char*)"Cant read the image\n");	
	}
	
	ofstream out_img("./output_img.pgm", ios::binary);
	//read the input image metadata	
	infile >> type >> width >> height >> intensity;	
	//write output metadata
	out_img << type << "\n" << width << "\t" << height << "\n" << intensity << "\n";

	//initial image row
	float **image = (float**) malloc (height*sizeof(float*));
	//initial image column
	for(uint i = 0; i < height; ++i){
		image[i] = (float*) malloc (width*sizeof(float));
	}
	
	//initial output_img row
	float **output_img = (float**) malloc (height*sizeof(float*));
	//initial outputimg column
	for(uint i = 0; i < height; ++i){
		output_img[i] = (float*) malloc (width*sizeof(float));
	}


	//Read input image to image array
	for(uint i = 0 ; i < height; ++i){
		for(uint j = 0; j < width; ++j){
			//read in as int but the computation will make it float 
			//therefore using float** image
			image[i][j] = (int)infile.get();
		}
	}


	// The algorithme find in https://en.wikipedia.org/wiki/Canny_edge_detector
	// 1. Gaussian filter to smooth the image to remove noise
	noise_reduce(image, output_img);
	//2. find the intensity gradients of the image
	intensity_gradient(output_img);

	

	//Store output file
	for(uint i = 0 ; i < height; ++i){
		for(uint j = 0; j < width; ++j){
			out_img << (char)(int)output_img[i][j];
		}
	}	
	

	/****************************************************
	 *******************FREE SECTION ********************
	 ***************************************************/
	/*
	for(uint i = 0; i < height; ++i){
		free(image[i]);
	}	
	
	free(image);	
	*/
	return 0;
}
