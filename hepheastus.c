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
#include <string.h>

#define UDP_PORT 1234
#define SERVICE_ID 190

#define SEND_INTERVAL	(60 * CLOCK_SECOND)
#define POWERTRACE_INTERVAL (2 * CLOCK_SECOND)

#define NUMBER_OF_SAMPLES 200

#define PERCENTIL(array, size, p) (array[p*size/100])

// static struct simple_udp_connection hepheastus_connection;

uint16_t locked = 0;

// Definições dos pontos de cisão
float split_points[NUMBER_OF_SAMPLES];
uint16_t split_counter = 0;

float samples[NUMBER_OF_SAMPLES];
uint16_t samples_counter = 0;

char message_buffer[100];

float myfloor(float x) {
  if(x >= 0.0f) {
    return (float)((int)x);
  } else { 
  	return (float)((int)x - 1);
  }
}

void printf_float(float number){
	printf("%ld.%02u", (long) number, (unsigned)((number-myfloor(number))*100));
}

float mysqrt(float number) {
	float sqrt = 0, i, upper, lower;

	if ( number < 62025){
		for( i = 0; i < 255; i++){
			lower = i*i;
			upper = (i+1)*(i+1);
			// printf("%ld.%02u - ", (long) lower, (unsigned)((lower-myfloor(lower))*1000));
			// printf("%ld.%02u\n", (long) upper, (unsigned)((upper-myfloor(upper))*1000));
			if( lower <= number && number < upper){
				sqrt = i;
				break;
			}
		}	
	}else{
		sqrt = 255;
	}
	
	
	while( sqrt*sqrt < number ){
		// printf("%ld.%02u - ", (long) number, (unsigned)((number-myfloor(number))*100));
		// printf("%ld.%02u\n", (long) sqrt, (unsigned)((sqrt-myfloor(sqrt))*100));
		sqrt = sqrt + 0.001;
	}
	
	return sqrt;
}

uint16_t select_maximun(float *data, uint16_t begin, uint16_t end, float value){
	uint16_t upinterval = begin;
	
	while( data[upinterval] < value && upinterval < (end - 1)){
		upinterval = upinterval + 1;
	}
	
	return upinterval;
}


float mean(float *y,uint16_t begin,uint16_t end){
	uint16_t number_of_elements = end - begin, i;
	float aux = 0;
	
	if( end <= begin ){
		return 0;
	}

	for(i = begin; i < end; i++){
		aux = aux + y[i]/number_of_elements;
	}

	return aux;
}

float standard_desviation(float *x, uint16_t begin, uint16_t end){
	float sum_mean = 0, sum_square = 0;
	uint16_t i;

	if( end <= begin ){
		return 0;
	}

	sum_mean = mean(x, begin, end);

	for(i = begin; i < end; i++){
		sum_square = sum_square + (x[i] - sum_mean)*(x[i] - sum_mean);
	}

	return mysqrt(sum_square/(end - begin));
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

float skew(float *x, uint16_t begin, uint16_t end){
	float mean_value, p50, sd, aux;

	mean_value = mean(x, begin, end);

	if ( (end - begin) % 1 == 1){
		p50 = x[(begin+end)/2];
	}else{
		p50 = (x[(begin + end)/2] + x[(begin + end)/2 - 1])/2;
	}
	
	sd = standard_desviation(x, begin, end);

	return 3*(mean_value - p50)/sd;
}

void hepheastus(uint16_t begin, uint16_t end, uint16_t level){
	uint16_t upper;
	float skew_value, kurtosis_value, mean_value;

	mean_value = mean(samples, begin, end);
	kurtosis_value = kurtosis(samples, begin, end);
	skew_value = skew(samples, begin, end);

	if( skew_value < 1){
		if( kurtosis_value < 0.26 ){
			printf("Leptokurtic Set! : ");
		}else{
			printf("Platykurtic Set! : ");
		}

		// printf("(%u.%u,%u.%u,%u.%u)\n",
		// 	mean_value/100, mean_value % 100,
		// 	kurtosis_value/100, kurtosis_value % 100,
		// 	skew_value/100, skew_value % 100
		// );

	}else{
		split_points[split_counter++] = mean_value;

		upper = select_maximun(samples, begin, end, mean_value);

		hepheastus(begin, upper, level + 1);
		hepheastus(upper,   end, level + 1);
	}
}



// Função processo principal
void hepheastus_main(){
	uint begin, end, i, j, value;

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
	
	hepheastus(0, samples_counter, 0);

	begin = 0;
	sprintf(message_buffer, "H:(");
	// Gerar as mensagens que serão enviadas para as aplicações
	for(i = 0; i < split_counter; i++){
		end = select_maximun(samples, 0, samples_counter, split_points[i]);
		value = mean(samples,begin, end);

		printf("Data[%u] = %u\n", i, value);
		sprintf(
			message_buffer, 
			"%s%u, %ld.%02u),(", 
			message_buffer, 
			i,
			(long) value, 
			(unsigned)((value-myfloor(value))*100)
		);
		begin = end + 1;
	}

	value = mean(samples,begin, samples_counter);
	sprintf(
		message_buffer,
		"%s%u, %ld.%02u)",
		message_buffer,
		i, 
		(long) value, 
		(unsigned)((value-myfloor(value))*100)
	);

	printf("%s\n", message_buffer);	
}

/*---------------------------------------------------------------------------*/
PROCESS(unicast_receiver_process, "Hepheastus process");
AUTOSTART_PROCESSES(&unicast_receiver_process);
/*---------------------------------------------------------------------------*/
// static void
// receiver(struct simple_udp_connection *c,
// 		 const uip_ipaddr_t *sender_addr,
// 		 uint16_t sender_port,
// 		 const uip_ipaddr_t *receiver_addr,
// 		 uint16_t receiver_port,
// 		 const uint8_t *data,
// 		 uint16_t datalen)
// { 
//   uint16_t i, j;
//   uint16_t aux;
//   // printf("From ");
//   // uip_debug_ipaddr_print(sender_addr);
//   // printf(": '%s' && locked=%u\n",data,locked);

//   if( locked == 0){
//   	samples_counter = samples_counter % NUMBER_OF_SAMPLES;
// 	samples[samples_counter] = 0;
// 	for(i = 0; i < (datalen - 1); i++){
// 		aux = 1;
// 		for(j = 1; j < (datalen - 1 - i); j++) aux *= 10;

// 		samples[samples_counter] += (data[i] - '0')*aux;
// 	}
// 	samples_counter = (samples_counter + 1) % NUMBER_OF_SAMPLES;

// 	// printf("RawSample[%u]: [", samples_counter);
// 	// for(i = 0; i < samples_counter; i++){
// 	// 	printf("%u, ", samples[i]);  
// 	// }
// 	// printf("];\n");	
//   }
  
// }

// // /*---------------------------------------------------------------------------*/
// static uip_ipaddr_t *
// set_global_address(void)
// {
//   static uip_ipaddr_t ipaddr;
//   int i;
//   uint8_t state;

//   uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
//   uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
//   uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);

//   printf("IPv6 addresses: ");
//   for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
// 	state = uip_ds6_if.addr_list[i].state;
// 	if(uip_ds6_if.addr_list[i].isused &&
// 	   (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
// 	  uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr);
// 	  printf("\n");
// 	}
//   }

//   return &ipaddr;
// }
// /*---------------------------------------------------------------------------*/
// static void
// create_rpl_dag(uip_ipaddr_t *ipaddr)
// {
//   struct uip_ds6_addr *root_if;

//   root_if = uip_ds6_addr_lookup(ipaddr);
//   if(root_if != NULL) {
// 	rpl_dag_t *dag;
// 	uip_ipaddr_t prefix;
	
// 	rpl_set_root(RPL_DEFAULT_INSTANCE, ipaddr);
// 	dag = rpl_get_any_dag();
// 	uip_ip6addr(&prefix, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
// 	rpl_set_prefix(dag, &prefix, 64);
// 	PRINTF("created a new RPL dag\n");
//   } else {
// 	PRINTF("failed to create a new RPL DAG\n");
//   }
// }
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(unicast_receiver_process, ev, data)
{
  // static struct etimer periodic_timer;
  // static struct etimer send_timer;
  // uip_ipaddr_t *ipaddr;

  PROCESS_BEGIN();

  // float number10 = mysqrt(2.0);

  // Iniciando o powertrace
  //powertrace_start(POWERTRACE_INTERVAL);

  float data[8] = {1.0,1.0,1.0,1.0,2.0,2.0,2.0,2.0};
  float mean_value = mean(data, 0, 8);
  float sd_value = standard_desviation(data, 0, 8);
  float skew_value = skew(data, 0, 8);
  float kurtosis_value = kurtosis(data, 0, 8);

  printf("mean=");
  printf_float(mean_value);
  printf(", sd=");
  printf_float(sd_value);
  printf(", skew=");
  printf_float(skew_value);
  printf(", kurtosis=");
  printf_float(kurtosis_value);
  printf("\n");

  // Iniciando o serviço de rede
 //  servreg_hack_init();

 //  ipaddr = set_global_address();

 //  create_rpl_dag(ipaddr);

 //  servreg_hack_register(SERVICE_ID, ipaddr);

 //  simple_udp_register(&hepheastus_connection, UDP_PORT,NULL, UDP_PORT, receiver);

 //  // Iniciando o temporizador
 //  etimer_set(&periodic_timer, SEND_INTERVAL);

 //  printf("RTIMER_SECOND %u\n", RTIMER_SECOND);

 //  printf("Open collector window\n");
 //  while(1) {
	// // PROCESS_WAIT_EVENT();
	// PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

	// printf("Close collector window\n");
	// printf("Open fusion window\n");
	// locked = 1;

	// hepheastus_main();

	// printf("Close fusion window\n");
	// locked = 0;
	// printf("Open collector window\n");

	// etimer_reset(&periodic_timer);    
 //  }

  printf("End of process\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/