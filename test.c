#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_SAMPLES 200

uint samples[NUMBER_OF_SAMPLES];
uint samples_counter = 0;

int main(int argc,char** argv ){
	uint i, j, k;
	uint aux, localnumbersamples, pointer;


	char* data = "c:1:4861";
	uint datalen = 8;

	for(i = 2; data[i] != ':'; i++);

	localnumbersamples = 0;
	for(j = 2; j < i; j++){
		aux = 1;
		for(k = 1; k < (i - 1 - j); k++) aux *= 10;
		localnumbersamples += (data[j] - '0')*aux;
	}

	for(; localnumbersamples > 0; localnumbersamples--){
	  	samples_counter = samples_counter % NUMBER_OF_SAMPLES;
		samples[samples_counter] = 0;
		pointer = ++i;
		for(; data[i] != ',' && i < datalen; i++);
		for(j = pointer; j < i; j++){
			aux = 1;
			for(k = j; k < (i - 1); k++) aux *= 10;
			printf("%d - %c\n", aux, data[j]);

			samples[samples_counter] += (data[j] - '0')*aux;
		}
		printf("Received %u\n",samples[samples_counter]);
		samples_counter = (samples_counter + 1) % NUMBER_OF_SAMPLES;
	}
	

	return 0;
}