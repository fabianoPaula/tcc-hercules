#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {5433, 5433, 4625, 5433, 4625, 5032, 5433, 4625, 5032, 5196, 5433, 4625, 5032, 5196, 5237, 5433, 4625, 5032, 5196, 5237, 4643, 5433, 4625, 5032, 5196, 5237, 4643, 5750, 5433, 4625, 5032, 5196, 5237, 4643, 5750, 5251, 5433, 4625, 5032, 5196, 5237, 4643, 5750, 5251, 4733, 5433, 4625, 5032, 5196, 5237, 4643, 5750, 5251, 4733, 4612, 5433, 4625, 5032, 5196, 5237, 4643, 5750, 5251, 4733, 4612, 5275, 4997, 4997, 5199, 4997, 5199, 4888, 4997, 5199, 4888, 4289, 4997, 5199, 4888, 4289, 4935, 4997, 5199, 4888, 4289, 4935, 5289, 4997, 5199, 4888, 4289, 4935, 5289, 4854, 4997, 5199, 4888, 4289, 4935, 5289, 4854, 5136, 4997, 5199, 4888, 4289, 4935, 5289, 4854, 5136, 5006, 4997, 5199, 4888, 4289, 4935, 5289, 4854, 5136, 5006, 4550, 4359, 4359, 4719, 4359, 4719, 5130, 4359, 4719, 5130, 5159, 4359, 4719, 5130, 5159, 5407, 4359, 4719, 5130, 5159, 5407, 5173, 4359, 4719, 5130, 5159, 5407, 5173, 5291, 4359, 4719, 5130, 5159, 5407, 5173, 5291, 4113, 4359, 4719, 5130, 5159, 5407, 5173, 5291, 4113, 5152, 4359, 4719, 5130, 5159, 5407, 5173, 5291, 4113, 5152, 4415, 4874, 4874, 4524, 4874, 4524, 5093, 4874, 4524, 5093, 4927, 4874, 4524, 5093, 4927, 4495, 4874, 4524, 5093, 4927, 4495, 4547, 4874, 4524, 5093, 4927, 4495, 4547, 5602, 4874, 4524, 5093, 4927, 4495, 4547, 5602, 4880, 4874, 4524, 5093, 4927, 4495, 4547, 5602, 4880, 4360, 4874, 4524, 5093, 4927, 4495, 4547, 5602, 4880, 4360, 5511, 4977, 4977, 4855, 4977, 4855, 4836, 4977, 4855, 4836, 4924, 4977, 4855, 4836, 4924, 5430, 4977, 4855, 4836, 4924, 5430, 4734, 4977, 4855, 4836, 4924, 5430, 4734, 5115, 4977, 4855, 4836, 4924, 5430, 4734, 5115, 4597, 4977, 4855, 4836, 4924, 5430, 4734, 5115, 4597, 5728, 4977, 4855, 4836, 4924, 5430, 4734, 5115, 4597, 5728, 5645, 5180, 5180, 4833, 5180, 4833, 5716, 5180, 4833, 5716, 4677, 5180, 4833, 5716, 4677, 5021, 5180, 4833, 5716, 4677, 5021, 4549, 5180, 4833, 5716, 4677, 5021, 4549, 4930, 5180, 4833, 5716, 4677, 5021, 4549, 4930, 4911, 5180, 4833, 5716, 4677, 5021, 4549, 4930, 4911, 4331, 5180, 4833, 5716, 4677, 5021, 4549, 4930, 4911, 4331, 5497, 5457, 5457, 4839, 5457, 4839, 5163, 5457, 4839, 5163, 4364, 5457, 4839, 5163, 4364, 4829, 5457, 4839, 5163, 4364, 4829, 5245, 5457, 4839, 5163, 4364, 4829, 5245, 5179, 5457, 4839, 5163, 4364, 4829, 5245, 5179, 5146, 5457, 4839, 5163, 4364, 4829, 5245, 5179, 5146, 5449, 5457, 4839, 5163, 4364, 4829, 5245, 5179, 5146, 5449, 4515, 4740, 4740, 4852, 4740, 4852, 4517, 4740, 4852, 4517, 4887, 4740, 4852, 4517, 4887, 4369, 4740, 4852, 4517, 4887, 4369, 5063, 4740, 4852, 4517, 4887, 4369, 5063, 5224, 4740, 4852, 4517, 4887, 4369, 5063, 5224, 5163, 4740, 4852, 4517, 4887, 4369, 5063, 5224, 5163, 5241, 4740, 4852, 4517, 4887, 4369, 5063, 5224, 5163, 5241, 4870, 5249, 5249, 5684, 5249, 5684, 4782, 5249, 5684, 4782, 5718, 5249, 5684, 4782, 5718, 5168, 5249, 5684, 4782, 5718, 5168, 4770, 5249, 5684, 4782, 5718, 5168, 4770, 5584, 5249, 5684, 4782, 5718, 5168, 4770, 5584, 5360, 5249, 5684, 4782, 5718, 5168, 4770, 5584, 5360, 5426, 5249, 5684, 4782, 5718, 5168, 4770, 5584, 5360, 5426, 4590, 4936, 4936, 5496, 4936, 5496, 5472, 4936, 5496, 5472, 5277, 4936, 5496, 5472, 5277, 5603, 4936, 5496, 5472, 5277, 5603, 4992, 4936, 5496, 5472, 5277, 5603, 4992, 4501, 4936, 5496, 5472, 5277, 5603, 4992, 4501, 4710, 4936, 5496, 5472, 5277, 5603, 4992, 4501, 4710, 4777, };static struct simple_udp_connection unicast_connection;
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