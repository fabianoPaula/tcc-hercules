#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {5455, 5455, 5119, 5455, 5119, 4630, 5455, 5119, 4630, 5344, 5455, 5119, 4630, 5344, 5310, 5455, 5119, 4630, 5344, 5310, 4207, 5455, 5119, 4630, 5344, 5310, 4207, 5834, 5455, 5119, 4630, 5344, 5310, 4207, 5834, 5093, 5455, 5119, 4630, 5344, 5310, 4207, 5834, 5093, 5112, 5455, 5119, 4630, 5344, 5310, 4207, 5834, 5093, 5112, 5715, 5455, 5119, 4630, 5344, 5310, 4207, 5834, 5093, 5112, 5715, 4716, 5388, 5388, 5102, 5388, 5102, 4831, 5388, 5102, 4831, 5121, 5388, 5102, 4831, 5121, 4899, 5388, 5102, 4831, 5121, 4899, 4834, 5388, 5102, 4831, 5121, 4899, 4834, 5385, 5388, 5102, 4831, 5121, 4899, 4834, 5385, 5188, 5388, 5102, 4831, 5121, 4899, 4834, 5385, 5188, 4711, 5388, 5102, 4831, 5121, 4899, 4834, 5385, 5188, 4711, 5152, 4478, 4478, 5523, 4478, 5523, 5204, 4478, 5523, 5204, 5451, 4478, 5523, 5204, 5451, 4977, 4478, 5523, 5204, 5451, 4977, 4877, 4478, 5523, 5204, 5451, 4977, 4877, 4958, 4478, 5523, 5204, 5451, 4977, 4877, 4958, 4935, 4478, 5523, 5204, 5451, 4977, 4877, 4958, 4935, 5081, 4478, 5523, 5204, 5451, 4977, 4877, 4958, 4935, 5081, 4276, 4530, 4530, 4697, 4530, 4697, 5167, 4530, 4697, 5167, 4689, 4530, 4697, 5167, 4689, 4466, 4530, 4697, 5167, 4689, 4466, 4147, 4530, 4697, 5167, 4689, 4466, 4147, 5396, 4530, 4697, 5167, 4689, 4466, 4147, 5396, 4700, 4530, 4697, 5167, 4689, 4466, 4147, 5396, 4700, 4674, 4530, 4697, 5167, 4689, 4466, 4147, 5396, 4700, 4674, 4655, 4977, 4977, 4837, 4977, 4837, 5400, 4977, 4837, 5400, 5167, 4977, 4837, 5400, 5167, 4912, 4977, 4837, 5400, 5167, 4912, 4804, 4977, 4837, 5400, 5167, 4912, 4804, 4492, 4977, 4837, 5400, 5167, 4912, 4804, 4492, 5316, 4977, 4837, 5400, 5167, 4912, 4804, 4492, 5316, 5175, 4977, 4837, 5400, 5167, 4912, 4804, 4492, 5316, 5175, 5479, 4780, 4780, 4832, 4780, 4832, 5362, 4780, 4832, 5362, 4293, 4780, 4832, 5362, 4293, 4427, 4780, 4832, 5362, 4293, 4427, 4917, 4780, 4832, 5362, 4293, 4427, 4917, 4556, 4780, 4832, 5362, 4293, 4427, 4917, 4556, 4893, 4780, 4832, 5362, 4293, 4427, 4917, 4556, 4893, 4316, 4780, 4832, 5362, 4293, 4427, 4917, 4556, 4893, 4316, 5329, 5223, 5223, 5611, 5223, 5611, 4880, 5223, 5611, 4880, 5618, 5223, 5611, 4880, 5618, 5459, 5223, 5611, 4880, 5618, 5459, 4670, 5223, 5611, 4880, 5618, 5459, 4670, 5061, 5223, 5611, 4880, 5618, 5459, 4670, 5061, 4930, 5223, 5611, 4880, 5618, 5459, 4670, 5061, 4930, 4957, 5223, 5611, 4880, 5618, 5459, 4670, 5061, 4930, 4957, 4842, 5266, 5266, 4927, 5266, 4927, 4674, 5266, 4927, 4674, 4835, 5266, 4927, 4674, 4835, 5105, 5266, 4927, 4674, 4835, 5105, 4397, 5266, 4927, 4674, 4835, 5105, 4397, 4650, 5266, 4927, 4674, 4835, 5105, 4397, 4650, 4395, 5266, 4927, 4674, 4835, 5105, 4397, 4650, 4395, 5026, 5266, 4927, 4674, 4835, 5105, 4397, 4650, 4395, 5026, 4474, 5164, 5164, 4557, 5164, 4557, 5405, 5164, 4557, 5405, 5223, 5164, 4557, 5405, 5223, 5234, 5164, 4557, 5405, 5223, 5234, 5500, 5164, 4557, 5405, 5223, 5234, 5500, 5854, 5164, 4557, 5405, 5223, 5234, 5500, 5854, 4952, 5164, 4557, 5405, 5223, 5234, 5500, 5854, 4952, 5303, 5164, 4557, 5405, 5223, 5234, 5500, 5854, 4952, 5303, 4851, 4734, 4734, 4523, 4734, 4523, 3919, 4734, 4523, 3919, 4262, 4734, 4523, 3919, 4262, 5294, 4734, 4523, 3919, 4262, 5294, 5360, 4734, 4523, 3919, 4262, 5294, 5360, 5033, 4734, 4523, 3919, 4262, 5294, 5360, 5033, 4242, 4734, 4523, 3919, 4262, 5294, 5360, 5033, 4242, 4788, };static struct simple_udp_connection unicast_connection;
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