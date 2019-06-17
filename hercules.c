#include <stdlib.h>
#include <stdio.h> // For printf()
#include <math.h>

#include "contiki.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "net/uip.h"
#include "net/uip-ds6.h"
#include "net/uip-debug.h"

#include "simple-udp.h"
#include "servreg-hack.h"

#include "net/rpl/rpl.h"

#include <stdio.h>
#include <string.h>

#define UDP_PORT 1234
#define SERVICE_ID 190

#define SEND_INTERVAL		(10 * CLOCK_SECOND)
#define SEND_TIME		(random_rand() % (SEND_INTERVAL))

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
PROCESS(unicast_receiver_process, "Unicast receiver example process");
AUTOSTART_PROCESSES(&unicast_receiver_process);
/*---------------------------------------------------------------------------*/
static void receiver(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen) 
{
  printf("Data received from ");
  uip_debug_ipaddr_print(sender_addr);
  printf(" on port %d from port %d with length %d: '%s'\n",
         receiver_port, sender_port, datalen, data);
}
/*---------------------------------------------------------------------------*/
static uip_ipaddr_t * set_global_address(void) {
  static uip_ipaddr_t ipaddr;
  int i;
  uint8_t state;

  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);

  printf("IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(uip_ds6_if.addr_list[i].isused &&
       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
      uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr);
      printf("\n");
    }
  }

  return &ipaddr;
}
/*---------------------------------------------------------------------------*/
static void create_rpl_dag(uip_ipaddr_t *ipaddr) {
  struct uip_ds6_addr *root_if;

  root_if = uip_ds6_addr_lookup(ipaddr);
  if(root_if != NULL) {
    rpl_dag_t *dag;
    uip_ipaddr_t prefix;
    
    rpl_set_root(RPL_DEFAULT_INSTANCE, ipaddr);
    dag = rpl_get_any_dag();
    uip_ip6addr(&prefix, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
    rpl_set_prefix(dag, &prefix, 64);
    PRINTF("created a new RPL dag\n");
  } else {
    PRINTF("failed to create a new RPL DAG\n");
  }
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(unicast_receiver_process, ev, data)
{
  uip_ipaddr_t *ipaddr;

  PROCESS_BEGIN();

  servreg_hack_init();

  ipaddr = set_global_address();

  create_rpl_dag(ipaddr);

  servreg_hack_register(SERVICE_ID, ipaddr);

  simple_udp_register(&unicast_connection, UDP_PORT, NULL, UDP_PORT, receiver);

  while(1) {
    PROCESS_WAIT_EVENT();
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
