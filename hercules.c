#include "contiki.h"
#include <stdio.h> /* For printf() */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NUMBER_OF_BINS 10
#define NUMBER_OF_INTERVALS 10


// Funções auxiliares
void sort_n2(int *data, int datasize) {
	int i,j, aux; 

	for(i = 0; i < datasize - 1; i++){
		for(j = i; j < datasize; j++){
			if( data[i] > data[j]){
				aux = data[i];
				data[i] = data[j];
				data[j] = aux;
			}
		}
	}
}

int discrete_mean(int *y,int begin,int end){
	int number_of_elements = end - begin, i, aux = 0;
    if( number_of_elements == 0){
        return 0;
    }

    for(i = begin; i < end; i++){
    	aux += y[i];
    }

    return aux/number_of_elements;
}

int weight_mean(int *x, int *y, int begin, int end){
	int sum_mean = 0, total_sample = 0, i;

    for(i = begin; i < end; i++){
    	sum_mean += x[i]*y[i];
        total_sample += y[i];
    }

    if (total_sample == 0){
        return 0;
    }

    return sum_mean/total_sample/100;
}

int minvalue(int *data, int data_size){
    int min_value = data[0];
    int i;

    for( i = 0; i < data_size; i++){
    	if( min_value > data[i]){
    		min_value = data[i];
    	}
    }

    return min_value;
}

int maxvalue(int *data, int data_size){
    int max_value = data[0];
    int i;

    for( i = 0; i < data_size; i++){
    	if( max_value < data[i]){
    		max_value = data[i];
    	}
    }

    return max_value;
}

// Definição da curva de frequência
int curve_frequency[NUMBER_OF_BINS];
int curve_data[NUMBER_OF_BINS];

// Definição da função de média
int function_frequency[NUMBER_OF_INTERVALS];
int function_data[NUMBER_OF_INTERVALS];
int function_status[NUMBER_OF_INTERVALS];

// Definições dos pontos de cisão
int split_points[NUMBER_OF_INTERVALS];
int split_counter = 0;

// Funções auxiliares ao processo principais

void set_histogram(int *data, int data_size){
    int min_value = minvalue(data, data_size);
    int max_value = maxvalue(data, data_size);
    int step = (max_value - min_value)/(NUMBER_OF_BINS - 1);
	
    int sup_data_range = min_value + step;
   
    curve_frequency[0] = 0;
    curve_data[0] = min_value + step/2;

    int index = 0, curve_index = 0;

    while( index < data_size ){
    	if( data[index] < sup_data_range ){
    		curve_frequency[curve_index] += 1;
    		index += 1;
    	}else{
    		curve_frequency[curve_index] = 0;
    		curve_data[curve_index] = sup_data_range + step/2;
    		sup_data_range += step;
    	}
    }
}

void set_average_function(int K, int step){
	int 	k;

	for(k = 0; k < K; k++ ){
		function_frequency[k] = discrete_mean(curve_frequency, k*step, (k + 1)*step);
		function_data[k]      = weight_mean(curve_data,curve_frequency,k*step, (k + 1)*step);
		function_status[k]    = 0;
	}
}

void classify_points(){
	int last_valley = 0, index, aux;
        
    // Classificando os pontos do meio
	for(index = 0; index < NUMBER_OF_INTERVALS; index++ ){
		if( function_frequency[index-1] < function_frequency[index]){
			if( function_status[index-1] < 1){
				function_status[index] = 1;
				function_status[last_valley] = 0;
				aux = last_valley + (index - last_valley)/2;
				function_status[aux] = -1;
			}else if( function_status[index-1] == 1){
				function_status[index-1] = 0;
				function_status[index] = 1;
			}
		} else if( function_frequency[index-1] > function_frequency[index]){
			if( function_status[index-1] > -1 ){
				function_status[index] = -1;
			}else if( function_status[index-1] == -1){
				function_status[index-1] = 0;
				function_status[index] = -1;
			}
			last_valley = index;
		}       
	}
}


void select_split_points(){
	int i, j;

    // removendo o vale no extremo direito
	for(i = 0; i < NUMBER_OF_INTERVALS; i++){
		if(function_status[i] == 1){
			for(j = 0; j < i; j++){
				function_status[j] = 0;
			}
			break;
		}
	}

	// removendo o vale no extremo esquerdo
	for(i = NUMBER_OF_INTERVALS; i > 0; i--){
		if(function_status[i] == 1){
			for(j = i; j < NUMBER_OF_INTERVALS; j++){
				function_status[j] = 0;
			}
			break;
		}
	}

	split_counter = 0;
	// selecionando os vales que foram identificados
	for(i = 0; i < NUMBER_OF_INTERVALS; i++){
		if(function_status[i] == -1){
			split_points[split_counter++] = function_status[i];
		}
	}
}


// Função processo principal
void hercules(int* data, int data_size){

	// Ordenando vertor de leitura 
	sort_n2(data, data_size);

	// Preencher os vetores da curva de frequência
	set_histogram(data,data_size);

	// Preencher os vetores da função de média
	set_average_function(NUMBER_OF_INTERVALS, NUMBER_OF_BINS/NUMBER_OF_INTERVALS);

	// Classificar os pontos da função de médias
	classify_points();

	// selecionar os vales, excluindo os vales do extremo
	select_split_points();

	// criar os vetores que serão so subconjuntos unimodais

}

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();

  printf("Hello, world\n");
  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
