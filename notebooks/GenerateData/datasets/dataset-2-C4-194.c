#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15793, 15793, 15939, 15793, 15939, 15732, 15793, 15939, 15732, 15670, 15793, 15939, 15732, 15670, 15544, 15793, 15939, 15732, 15670, 15544, 15260, 15793, 15939, 15732, 15670, 15544, 15260, 15705, 15793, 15939, 15732, 15670, 15544, 15260, 15705, 15751, 15793, 15939, 15732, 15670, 15544, 15260, 15705, 15751, 15405, 15793, 15939, 15732, 15670, 15544, 15260, 15705, 15751, 15405, 15541, 15793, 15939, 15732, 15670, 15544, 15260, 15705, 15751, 15405, 15541, 15085, 15451, 15451, 15449, 15451, 15449, 15643, 15451, 15449, 15643, 15684, 15451, 15449, 15643, 15684, 15461, 15451, 15449, 15643, 15684, 15461, 15218, 15451, 15449, 15643, 15684, 15461, 15218, 15350, 15451, 15449, 15643, 15684, 15461, 15218, 15350, 15732, 15451, 15449, 15643, 15684, 15461, 15218, 15350, 15732, 15682, 15451, 15449, 15643, 15684, 15461, 15218, 15350, 15732, 15682, 15422, 15474, 15474, 15703, 15474, 15703, 15447, 15474, 15703, 15447, 15613, 15474, 15703, 15447, 15613, 15735, 15474, 15703, 15447, 15613, 15735, 15498, 15474, 15703, 15447, 15613, 15735, 15498, 15333, 15474, 15703, 15447, 15613, 15735, 15498, 15333, 15289, 15474, 15703, 15447, 15613, 15735, 15498, 15333, 15289, 15285, 15474, 15703, 15447, 15613, 15735, 15498, 15333, 15289, 15285, 15134, 15332, 15332, 15708, 15332, 15708, 15554, 15332, 15708, 15554, 15589, 15332, 15708, 15554, 15589, 15581, 15332, 15708, 15554, 15589, 15581, 15573, 15332, 15708, 15554, 15589, 15581, 15573, 15759, 15332, 15708, 15554, 15589, 15581, 15573, 15759, 15507, 15332, 15708, 15554, 15589, 15581, 15573, 15759, 15507, 14716, 15332, 15708, 15554, 15589, 15581, 15573, 15759, 15507, 14716, 15867, 15305, 15305, 15680, 15305, 15680, 15186, 15305, 15680, 15186, 15518, 15305, 15680, 15186, 15518, 15798, 15305, 15680, 15186, 15518, 15798, 15420, 15305, 15680, 15186, 15518, 15798, 15420, 15528, 15305, 15680, 15186, 15518, 15798, 15420, 15528, 15715, 15305, 15680, 15186, 15518, 15798, 15420, 15528, 15715, 15221, 15305, 15680, 15186, 15518, 15798, 15420, 15528, 15715, 15221, 15139, 15652, 15652, 15693, 15652, 15693, 15262, 15652, 15693, 15262, 15554, 15652, 15693, 15262, 15554, 15282, 15652, 15693, 15262, 15554, 15282, 15419, 15652, 15693, 15262, 15554, 15282, 15419, 15526, 15652, 15693, 15262, 15554, 15282, 15419, 15526, 15478, 15652, 15693, 15262, 15554, 15282, 15419, 15526, 15478, 15332, 15652, 15693, 15262, 15554, 15282, 15419, 15526, 15478, 15332, 15381, 15136, 15136, 15457, 15136, 15457, 15691, 15136, 15457, 15691, 15359, 15136, 15457, 15691, 15359, 15525, 15136, 15457, 15691, 15359, 15525, 15435, 15136, 15457, 15691, 15359, 15525, 15435, 15536, 15136, 15457, 15691, 15359, 15525, 15435, 15536, 15228, 15136, 15457, 15691, 15359, 15525, 15435, 15536, 15228, 15240, 15136, 15457, 15691, 15359, 15525, 15435, 15536, 15228, 15240, 15206, 15509, 15509, 15680, 15509, 15680, 15299, 15509, 15680, 15299, 15488, 15509, 15680, 15299, 15488, 15698, 15509, 15680, 15299, 15488, 15698, 15829, 15509, 15680, 15299, 15488, 15698, 15829, 15285, 15509, 15680, 15299, 15488, 15698, 15829, 15285, 15340, 15509, 15680, 15299, 15488, 15698, 15829, 15285, 15340, 15346, 15509, 15680, 15299, 15488, 15698, 15829, 15285, 15340, 15346, 15365, 15051, 15051, 15453, 15051, 15453, 15700, 15051, 15453, 15700, 15429, 15051, 15453, 15700, 15429, 15523, 15051, 15453, 15700, 15429, 15523, 15696, 15051, 15453, 15700, 15429, 15523, 15696, 15776, 15051, 15453, 15700, 15429, 15523, 15696, 15776, 15404, 15051, 15453, 15700, 15429, 15523, 15696, 15776, 15404, 15411, 15051, 15453, 15700, 15429, 15523, 15696, 15776, 15404, 15411, 15592, 15406, 15406, 15427, 15406, 15427, 15477, 15406, 15427, 15477, 15426, 15406, 15427, 15477, 15426, 15380, 15406, 15427, 15477, 15426, 15380, 15591, 15406, 15427, 15477, 15426, 15380, 15591, 15696, 15406, 15427, 15477, 15426, 15380, 15591, 15696, 15615, 15406, 15427, 15477, 15426, 15380, 15591, 15696, 15615, 15468, };static struct simple_udp_connection unicast_connection;
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