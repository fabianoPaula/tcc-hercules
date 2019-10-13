#include "contiki.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "net/uip.h"
#include "net/uip-ds6.h"
#include "net/uip-debug.h"

#include "simple-udp.h"
#include "servreg-hack.h"
#include "powertrace.h"

#include "net/rpl/rpl.h"

#include <stdio.h>

#define UDP_PORT 1234
#define SERVICE_ID 190

#define SEND_INTERVAL	(60 * CLOCK_SECOND)
#define POWERTRACE_INTERVAL (2 * CLOCK_SECOND)

#define NUMBER_OF_SAMPLES 200
#define NUMBER_OF_SPLIT_POINTS 10

static struct simple_udp_connection hepheastus_connection;

uint16_t locked = 0;

// Definições dos pontos de cisão
float split_points[NUMBER_OF_SPLIT_POINTS];
uint16_t split_counter = 0;

float samples[NUMBER_OF_SAMPLES];
uint16_t samples_counter = 0;

char message_buffer[100];

// void printf_float(float number){
// 	printf("%ld.%02u", (long) number, (unsigned)((number-myfloor(number))*100));
// }

float myfloor(float x) {
  if(x >= 0.0f) {
    return (float)((int)x);
  } else { 
  	return (float)((int)x - 1);
  }
}

float mysqrt(float number) {
	float sqrt = 0, i, upper, lower;

	if ( number < 62025){
		for( i = 0; i < 255; i++){
			lower = i*i;
			upper = (i+1)*(i+1);
			if( lower <= number && number < upper){
				sqrt = i;
				break;
			}
		}	
	}else{
		sqrt = 255;
	}
	
	while( sqrt*sqrt < number )
		sqrt = sqrt + 0.001;
	
	return sqrt;
}

float kurtosis(float *x, uint16_t begin, uint16_t end){
	float p10, p25, p75, p90;

	p10 = x[(9*begin + end)/10];
	p90 = x[(begin + 9*end)/10];

	if ( (end - begin) % 1 == 1){
		p25 = x[(3*begin + end)/4];
		p75 = x[(begin + 3*end)/4];
	}else{
		p25 = (x[(3*begin + end)/4] + x[(3*begin + end)/4 - 1])/2;
		p75 = (x[(begin + 3*end)/4] + x[(begin + 3*end)/4 - 1])/2;
	}

	return (p75 - p25)/2*(p90 - p10);
}


void skew_mean(float *x, uint16_t begin, uint16_t end, float *mean, float *skew){
	float mean_value = 0, p50, sd;
	uint16_t number_of_elements = end - begin, i;

	if( number_of_elements == 0 ) return;

	mean_value = 0;

	for(i = begin; i < end; i++)
		mean_value = mean_value + x[i]/number_of_elements;

	sd = 0;
	for(i = begin; i < end; i++)
		sd = sd + (x[i] - mean_value)*(x[i] - mean_value);

	sd = mysqrt(sd/number_of_elements);

	if ( number_of_elements % 1 == 1){
		p50 = x[(begin+end)/2];
	}else{
		p50 = (x[(begin + end)/2] + x[(begin + end)/2 - 1])/2;
	}
	
	*mean = mean_value;
	*skew = 3*(mean_value - p50)/sd;
}

void hepheastus(uint16_t begin, uint16_t end, uint16_t level){
	uint16_t i, j, value, number_of_elements, upper;
	float skew_value, kurtosis_value, mean_value;

	if( level == 0){
		// Ordenando vertor de leitura 
		for(i = 0; i < samples_counter - 1; i++){
			for(j = i; j < samples_counter; j++){
				if( samples[i] > samples[j]){
					value = samples[i];
					samples[i] = samples[j];
					samples[j] = value;
				}
			}
		}
		split_counter = 0;
	}

	skew_mean(samples, begin, end, &mean_value, &skew_value);
	kurtosis_value = kurtosis(samples, begin, end);
	
	if( skew_value < 1){
		if( kurtosis_value < 0.26 ){
			printf("Leptokurtic Set! : ");
		}else{
			printf("Platykurtic Set! : ");
		}

	}else{
		split_points[split_counter++] = mean_value;

		upper = begin;
		for(; samples[upper] < mean_value && upper < (end - 1); upper++);

		hepheastus(begin, upper, level + 1);
		hepheastus(upper,   end, level + 1);
	}

	if( level == 0){
		begin = 0;
		sprintf(message_buffer, "He:(");
		// Gerar as mensagens que serão enviadas para as aplicações
		for(i = 0; i < split_counter; i++){
			end = begin;
			for(; samples[end] < split_points[i] && end < (samples_counter - 1); end++);

			value = 0;
			number_of_elements = end - begin;

			for(i = begin; i < end; i++)
				value = value + samples[i]/number_of_elements;

			// printf("Data[%u] = %u\n", i, value);
			sprintf(message_buffer,"%s%u, %ld.%02u),(", message_buffer,  i,
				(long) value, (unsigned)((value-myfloor(value))*100)
			);
			begin = end + 1;
		}

		value = 0;
		number_of_elements = samples_counter - begin;

		for(i = begin; i < samples_counter; i++)
			value = value + samples[i]/number_of_elements;
		
		sprintf(message_buffer,"%s%u, %ld.%02u)", message_buffer, i, 
			(long) value, (unsigned)((value-myfloor(value))*100)
		);
		printf("%s\n", message_buffer);	
	}
}

/*---------------------------------------------------------------------------*/
PROCESS(unicast_receiver_process, "Hepheastus process");
AUTOSTART_PROCESSES(&unicast_receiver_process);
/*---------------------------------------------------------------------------*/
static void
receiver(struct simple_udp_connection *c,
		 const uip_ipaddr_t *sender_addr,
		 uint16_t sender_port,
		 const uip_ipaddr_t *receiver_addr,
		 uint16_t receiver_port,
		 const uint8_t *data,
		 uint16_t datalen)
{ 
  uint16_t i, j, k;
  uint16_t aux, localnumbersamples, pointer;

  if( locked == 0){
  	if( data[0] == 'c'){
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
				for(k = j; k < (i - 1); k++) aux *= 10.0;

				samples[samples_counter] += (data[j] - '0')*aux;
			}
			samples[samples_counter] = samples[samples_counter]/100;
			samples_counter = (samples_counter + 1) % NUMBER_OF_SAMPLES;
		}
	}
  }
  
}

// /*---------------------------------------------------------------------------*/
static uip_ipaddr_t *
set_global_address(void)
{
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
static void
create_rpl_dag(uip_ipaddr_t *ipaddr)
{
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
  static struct etimer periodic_timer;
  uip_ipaddr_t *ipaddr;

  PROCESS_BEGIN();

  // Iniciando o powertrace
  // powertrace_start(POWERTRACE_INTERVAL);

  // Iniciando o serviço de rede
  servreg_hack_init();

  ipaddr = set_global_address();

  create_rpl_dag(ipaddr);

  servreg_hack_register(SERVICE_ID, ipaddr);

  simple_udp_register(&hepheastus_connection, UDP_PORT,NULL, UDP_PORT, receiver);

  // Iniciando o temporizador
  etimer_set(&periodic_timer, SEND_INTERVAL);

  // printf("RTIMER_SECOND %u\n", RTIMER_SECOND);

  printf("Open collector window\n");
  while(1) {
	// PROCESS_WAIT_EVENT();
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

	printf("Close collector window\n");
	printf("Open fusion window\n");
	locked = 1;

	hepheastus(0, samples_counter, 0);

	printf("Close fusion window\n");
	locked = 0;
	printf("Open collector window\n");

	etimer_reset(&periodic_timer);    
  }

  printf("End of process\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/