#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {4431, 4431, 5827, 4431, 5827, 5097, 4431, 5827, 5097, 4318, 4431, 5827, 5097, 4318, 4573, 4431, 5827, 5097, 4318, 4573, 4377, 4431, 5827, 5097, 4318, 4573, 4377, 4552, 4431, 5827, 5097, 4318, 4573, 4377, 4552, 5348, 4431, 5827, 5097, 4318, 4573, 4377, 4552, 5348, 5704, 4431, 5827, 5097, 4318, 4573, 4377, 4552, 5348, 5704, 5308, 4431, 5827, 5097, 4318, 4573, 4377, 4552, 5348, 5704, 5308, 4998, 4737, 4737, 4706, 4737, 4706, 4462, 4737, 4706, 4462, 5151, 4737, 4706, 4462, 5151, 4127, 4737, 4706, 4462, 5151, 4127, 5113, 4737, 4706, 4462, 5151, 4127, 5113, 5164, 4737, 4706, 4462, 5151, 4127, 5113, 5164, 5205, 4737, 4706, 4462, 5151, 4127, 5113, 5164, 5205, 5069, 4737, 4706, 4462, 5151, 4127, 5113, 5164, 5205, 5069, 5263, 4607, 4607, 4999, 4607, 4999, 5074, 4607, 4999, 5074, 4812, 4607, 4999, 5074, 4812, 5862, 4607, 4999, 5074, 4812, 5862, 5275, 4607, 4999, 5074, 4812, 5862, 5275, 5222, 4607, 4999, 5074, 4812, 5862, 5275, 5222, 5016, 4607, 4999, 5074, 4812, 5862, 5275, 5222, 5016, 5445, 4607, 4999, 5074, 4812, 5862, 5275, 5222, 5016, 5445, 4818, 4692, 4692, 5810, 4692, 5810, 5229, 4692, 5810, 5229, 4986, 4692, 5810, 5229, 4986, 5514, 4692, 5810, 5229, 4986, 5514, 4317, 4692, 5810, 5229, 4986, 5514, 4317, 5255, 4692, 5810, 5229, 4986, 5514, 4317, 5255, 4970, 4692, 5810, 5229, 4986, 5514, 4317, 5255, 4970, 5860, 4692, 5810, 5229, 4986, 5514, 4317, 5255, 4970, 5860, 4687, 4820, 4820, 4154, 4820, 4154, 5552, 4820, 4154, 5552, 5684, 4820, 4154, 5552, 5684, 3774, 4820, 4154, 5552, 5684, 3774, 4942, 4820, 4154, 5552, 5684, 3774, 4942, 5669, 4820, 4154, 5552, 5684, 3774, 4942, 5669, 5104, 4820, 4154, 5552, 5684, 3774, 4942, 5669, 5104, 5097, 4820, 4154, 5552, 5684, 3774, 4942, 5669, 5104, 5097, 4555, 6040, 6040, 4455, 6040, 4455, 5111, 6040, 4455, 5111, 4867, 6040, 4455, 5111, 4867, 5014, 6040, 4455, 5111, 4867, 5014, 5283, 6040, 4455, 5111, 4867, 5014, 5283, 4367, 6040, 4455, 5111, 4867, 5014, 5283, 4367, 5079, 6040, 4455, 5111, 4867, 5014, 5283, 4367, 5079, 5268, 6040, 4455, 5111, 4867, 5014, 5283, 4367, 5079, 5268, 5409, 5154, 5154, 5344, 5154, 5344, 5440, 5154, 5344, 5440, 6133, 5154, 5344, 5440, 6133, 4922, 5154, 5344, 5440, 6133, 4922, 5035, 5154, 5344, 5440, 6133, 4922, 5035, 5227, 5154, 5344, 5440, 6133, 4922, 5035, 5227, 4634, 5154, 5344, 5440, 6133, 4922, 5035, 5227, 4634, 4794, 5154, 5344, 5440, 6133, 4922, 5035, 5227, 4634, 4794, 5167, 4198, 4198, 4542, 4198, 4542, 4970, 4198, 4542, 4970, 5213, 4198, 4542, 4970, 5213, 4616, 4198, 4542, 4970, 5213, 4616, 4840, 4198, 4542, 4970, 5213, 4616, 4840, 4724, 4198, 4542, 4970, 5213, 4616, 4840, 4724, 4485, 4198, 4542, 4970, 5213, 4616, 4840, 4724, 4485, 4503, 4198, 4542, 4970, 5213, 4616, 4840, 4724, 4485, 4503, 4407, 5675, 5675, 4381, 5675, 4381, 4616, 5675, 4381, 4616, 4895, 5675, 4381, 4616, 4895, 5587, 5675, 4381, 4616, 4895, 5587, 5703, 5675, 4381, 4616, 4895, 5587, 5703, 4810, 5675, 4381, 4616, 4895, 5587, 5703, 4810, 4992, 5675, 4381, 4616, 4895, 5587, 5703, 4810, 4992, 4388, 5675, 4381, 4616, 4895, 5587, 5703, 4810, 4992, 4388, 4848, 5062, 5062, 4428, 5062, 4428, 4687, 5062, 4428, 4687, 5668, 5062, 4428, 4687, 5668, 5523, 5062, 4428, 4687, 5668, 5523, 4436, 5062, 4428, 4687, 5668, 5523, 4436, 5200, 5062, 4428, 4687, 5668, 5523, 4436, 5200, 5316, 5062, 4428, 4687, 5668, 5523, 4436, 5200, 5316, 5730, };static struct simple_udp_connection unicast_connection;
PROCESS(unicast_sender_process, "Unicast sender example process");AUTOSTART_PROCESSES(&unicast_sender_process);
static voidreceiver(struct simple_udp_connection *c,         const uip_ipaddr_t *sender_addr,         uint16_t sender_port,         const uip_ipaddr_t *receiver_addr,         uint16_t receiver_port,         const uint8_t *data,         uint16_t datalen){  printf("Data received on port %d from port %d with length %d\n",         receiver_port, sender_port, datalen);}
static voidset_global_address(void){  uip_ipaddr_t ipaddr;  int i;  uint8_t state;
  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
  printf("IPv6 addresses: ");  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {    state = uip_ds6_if.addr_list[i].state;    if(uip_ds6_if.addr_list[i].isused &&       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {      uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr);      printf("\n");    }  }}
PROCESS_THREAD(unicast_sender_process, ev, data){  static struct etimer periodic_timer;  static struct etimer send_timer;  uip_ipaddr_t *addr;
  PROCESS_BEGIN();
  powertrace_start(POWERTRACE_INTERVAL);  servreg_hack_init();
  set_global_address();
  simple_udp_register(&unicast_connection, UDP_PORT,                      NULL, UDP_PORT, receiver);
  etimer_set(&periodic_timer, SEND_INTERVAL);  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));    etimer_reset(&periodic_timer);    etimer_set(&send_timer, SEND_TIME);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));    addr = servreg_hack_lookup(SERVICE_ID);    if(addr != NULL) {      char buf[20];
      sprintf(buf, "c:1:%d", samples[random_rand() % DATA_SIZE]);      simple_udp_sendto(&unicast_connection, buf, strlen(buf) + 1, addr);      printf("Sending unicast to ");      uip_debug_ipaddr_print(addr);      printf(": %s \n", buf);    } else {      printf("Service %d not found\n", SERVICE_ID);    }  }
  PROCESS_END();}