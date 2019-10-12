#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {5126, 5126, 4802, 5126, 4802, 5487, 5126, 4802, 5487, 5174, 5126, 4802, 5487, 5174, 4708, 5126, 4802, 5487, 5174, 4708, 5172, 5126, 4802, 5487, 5174, 4708, 5172, 4725, 5126, 4802, 5487, 5174, 4708, 5172, 4725, 4818, 5126, 4802, 5487, 5174, 4708, 5172, 4725, 4818, 4706, 5126, 4802, 5487, 5174, 4708, 5172, 4725, 4818, 4706, 4698, 5126, 4802, 5487, 5174, 4708, 5172, 4725, 4818, 4706, 4698, 4666, 5179, 5179, 5723, 5179, 5723, 4804, 5179, 5723, 4804, 5352, 5179, 5723, 4804, 5352, 4720, 5179, 5723, 4804, 5352, 4720, 4980, 5179, 5723, 4804, 5352, 4720, 4980, 5763, 5179, 5723, 4804, 5352, 4720, 4980, 5763, 4868, 5179, 5723, 4804, 5352, 4720, 4980, 5763, 4868, 5460, 5179, 5723, 4804, 5352, 4720, 4980, 5763, 4868, 5460, 4820, 4564, 4564, 6261, 4564, 6261, 5172, 4564, 6261, 5172, 5293, 4564, 6261, 5172, 5293, 4764, 4564, 6261, 5172, 5293, 4764, 4894, 4564, 6261, 5172, 5293, 4764, 4894, 4714, 4564, 6261, 5172, 5293, 4764, 4894, 4714, 5267, 4564, 6261, 5172, 5293, 4764, 4894, 4714, 5267, 5423, 4564, 6261, 5172, 5293, 4764, 4894, 4714, 5267, 5423, 5424, 5410, 5410, 4877, 5410, 4877, 5263, 5410, 4877, 5263, 5117, 5410, 4877, 5263, 5117, 4947, 5410, 4877, 5263, 5117, 4947, 5111, 5410, 4877, 5263, 5117, 4947, 5111, 4762, 5410, 4877, 5263, 5117, 4947, 5111, 4762, 4740, 5410, 4877, 5263, 5117, 4947, 5111, 4762, 4740, 4608, 5410, 4877, 5263, 5117, 4947, 5111, 4762, 4740, 4608, 5482, 5333, 5333, 5218, 5333, 5218, 5285, 5333, 5218, 5285, 5605, 5333, 5218, 5285, 5605, 5206, 5333, 5218, 5285, 5605, 5206, 4932, 5333, 5218, 5285, 5605, 5206, 4932, 4067, 5333, 5218, 5285, 5605, 5206, 4932, 4067, 5306, 5333, 5218, 5285, 5605, 5206, 4932, 4067, 5306, 4760, 5333, 5218, 5285, 5605, 5206, 4932, 4067, 5306, 4760, 5727, 4794, 4794, 4998, 4794, 4998, 5298, 4794, 4998, 5298, 5093, 4794, 4998, 5298, 5093, 4765, 4794, 4998, 5298, 5093, 4765, 4913, 4794, 4998, 5298, 5093, 4765, 4913, 4475, 4794, 4998, 5298, 5093, 4765, 4913, 4475, 4961, 4794, 4998, 5298, 5093, 4765, 4913, 4475, 4961, 4950, 4794, 4998, 5298, 5093, 4765, 4913, 4475, 4961, 4950, 5579, 4739, 4739, 4979, 4739, 4979, 4586, 4739, 4979, 4586, 4703, 4739, 4979, 4586, 4703, 4875, 4739, 4979, 4586, 4703, 4875, 4730, 4739, 4979, 4586, 4703, 4875, 4730, 5449, 4739, 4979, 4586, 4703, 4875, 4730, 5449, 4955, 4739, 4979, 4586, 4703, 4875, 4730, 5449, 4955, 5411, 4739, 4979, 4586, 4703, 4875, 4730, 5449, 4955, 5411, 5027, 4036, 4036, 4950, 4036, 4950, 5432, 4036, 4950, 5432, 5262, 4036, 4950, 5432, 5262, 4652, 4036, 4950, 5432, 5262, 4652, 4716, 4036, 4950, 5432, 5262, 4652, 4716, 5441, 4036, 4950, 5432, 5262, 4652, 4716, 5441, 5109, 4036, 4950, 5432, 5262, 4652, 4716, 5441, 5109, 4807, 4036, 4950, 5432, 5262, 4652, 4716, 5441, 5109, 4807, 4865, 4890, 4890, 4770, 4890, 4770, 4759, 4890, 4770, 4759, 5289, 4890, 4770, 4759, 5289, 4432, 4890, 4770, 4759, 5289, 4432, 4744, 4890, 4770, 4759, 5289, 4432, 4744, 4794, 4890, 4770, 4759, 5289, 4432, 4744, 4794, 4413, 4890, 4770, 4759, 5289, 4432, 4744, 4794, 4413, 5400, 4890, 4770, 4759, 5289, 4432, 4744, 4794, 4413, 5400, 5057, 5503, 5503, 4198, 5503, 4198, 4636, 5503, 4198, 4636, 5365, 5503, 4198, 4636, 5365, 5001, 5503, 4198, 4636, 5365, 5001, 4986, 5503, 4198, 4636, 5365, 5001, 4986, 4800, 5503, 4198, 4636, 5365, 5001, 4986, 4800, 4962, 5503, 4198, 4636, 5365, 5001, 4986, 4800, 4962, 5431, };static struct simple_udp_connection unicast_connection;
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