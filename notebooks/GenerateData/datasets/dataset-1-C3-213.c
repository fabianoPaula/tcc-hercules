#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {5053, 5053, 5237, 5053, 5237, 5316, 5053, 5237, 5316, 5374, 5053, 5237, 5316, 5374, 4609, 5053, 5237, 5316, 5374, 4609, 4860, 5053, 5237, 5316, 5374, 4609, 4860, 4755, 5053, 5237, 5316, 5374, 4609, 4860, 4755, 4894, 5053, 5237, 5316, 5374, 4609, 4860, 4755, 4894, 4823, 5053, 5237, 5316, 5374, 4609, 4860, 4755, 4894, 4823, 5394, 5053, 5237, 5316, 5374, 4609, 4860, 4755, 4894, 4823, 5394, 4925, 4718, 4718, 5364, 4718, 5364, 5613, 4718, 5364, 5613, 5748, 4718, 5364, 5613, 5748, 4686, 4718, 5364, 5613, 5748, 4686, 5141, 4718, 5364, 5613, 5748, 4686, 5141, 5277, 4718, 5364, 5613, 5748, 4686, 5141, 5277, 5238, 4718, 5364, 5613, 5748, 4686, 5141, 5277, 5238, 4882, 4718, 5364, 5613, 5748, 4686, 5141, 5277, 5238, 4882, 4517, 5217, 5217, 4717, 5217, 4717, 5095, 5217, 4717, 5095, 4572, 5217, 4717, 5095, 4572, 4357, 5217, 4717, 5095, 4572, 4357, 4932, 5217, 4717, 5095, 4572, 4357, 4932, 4758, 5217, 4717, 5095, 4572, 4357, 4932, 4758, 4864, 5217, 4717, 5095, 4572, 4357, 4932, 4758, 4864, 5140, 5217, 4717, 5095, 4572, 4357, 4932, 4758, 4864, 5140, 4874, 5116, 5116, 4842, 5116, 4842, 5355, 5116, 4842, 5355, 5287, 5116, 4842, 5355, 5287, 4900, 5116, 4842, 5355, 5287, 4900, 5301, 5116, 4842, 5355, 5287, 4900, 5301, 5034, 5116, 4842, 5355, 5287, 4900, 5301, 5034, 5324, 5116, 4842, 5355, 5287, 4900, 5301, 5034, 5324, 5204, 5116, 4842, 5355, 5287, 4900, 5301, 5034, 5324, 5204, 4737, 5315, 5315, 4289, 5315, 4289, 4888, 5315, 4289, 4888, 4859, 5315, 4289, 4888, 4859, 5321, 5315, 4289, 4888, 4859, 5321, 4379, 5315, 4289, 4888, 4859, 5321, 4379, 5326, 5315, 4289, 4888, 4859, 5321, 4379, 5326, 4673, 5315, 4289, 4888, 4859, 5321, 4379, 5326, 4673, 4545, 5315, 4289, 4888, 4859, 5321, 4379, 5326, 4673, 4545, 5524, 5090, 5090, 4938, 5090, 4938, 5359, 5090, 4938, 5359, 5880, 5090, 4938, 5359, 5880, 5224, 5090, 4938, 5359, 5880, 5224, 4620, 5090, 4938, 5359, 5880, 5224, 4620, 4731, 5090, 4938, 5359, 5880, 5224, 4620, 4731, 4853, 5090, 4938, 5359, 5880, 5224, 4620, 4731, 4853, 4875, 5090, 4938, 5359, 5880, 5224, 4620, 4731, 4853, 4875, 4877, 4821, 4821, 4769, 4821, 4769, 4660, 4821, 4769, 4660, 5322, 4821, 4769, 4660, 5322, 5212, 4821, 4769, 4660, 5322, 5212, 5203, 4821, 4769, 4660, 5322, 5212, 5203, 4510, 4821, 4769, 4660, 5322, 5212, 5203, 4510, 4845, 4821, 4769, 4660, 5322, 5212, 5203, 4510, 4845, 4212, 4821, 4769, 4660, 5322, 5212, 5203, 4510, 4845, 4212, 5193, 4540, 4540, 5150, 4540, 5150, 5360, 4540, 5150, 5360, 4904, 4540, 5150, 5360, 4904, 5164, 4540, 5150, 5360, 4904, 5164, 4669, 4540, 5150, 5360, 4904, 5164, 4669, 5891, 4540, 5150, 5360, 4904, 5164, 4669, 5891, 5147, 4540, 5150, 5360, 4904, 5164, 4669, 5891, 5147, 4350, 4540, 5150, 5360, 4904, 5164, 4669, 5891, 5147, 4350, 4953, 5134, 5134, 4629, 5134, 4629, 5343, 5134, 4629, 5343, 5697, 5134, 4629, 5343, 5697, 5457, 5134, 4629, 5343, 5697, 5457, 4682, 5134, 4629, 5343, 5697, 5457, 4682, 4617, 5134, 4629, 5343, 5697, 5457, 4682, 4617, 5103, 5134, 4629, 5343, 5697, 5457, 4682, 4617, 5103, 5261, 5134, 4629, 5343, 5697, 5457, 4682, 4617, 5103, 5261, 4406, 4996, 4996, 5220, 4996, 5220, 4121, 4996, 5220, 4121, 4608, 4996, 5220, 4121, 4608, 4800, 4996, 5220, 4121, 4608, 4800, 4053, 4996, 5220, 4121, 4608, 4800, 4053, 5233, 4996, 5220, 4121, 4608, 4800, 4053, 5233, 4320, 4996, 5220, 4121, 4608, 4800, 4053, 5233, 4320, 4467, };static struct simple_udp_connection unicast_connection;
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