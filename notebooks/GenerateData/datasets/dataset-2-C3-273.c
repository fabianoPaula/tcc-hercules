#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15669, 15669, 15505, 15669, 15505, 15648, 15669, 15505, 15648, 15706, 15669, 15505, 15648, 15706, 15488, 15669, 15505, 15648, 15706, 15488, 15618, 15669, 15505, 15648, 15706, 15488, 15618, 15500, 15669, 15505, 15648, 15706, 15488, 15618, 15500, 15402, 15669, 15505, 15648, 15706, 15488, 15618, 15500, 15402, 15479, 15669, 15505, 15648, 15706, 15488, 15618, 15500, 15402, 15479, 15296, 15669, 15505, 15648, 15706, 15488, 15618, 15500, 15402, 15479, 15296, 15197, 15459, 15459, 15379, 15459, 15379, 15689, 15459, 15379, 15689, 15797, 15459, 15379, 15689, 15797, 15307, 15459, 15379, 15689, 15797, 15307, 15561, 15459, 15379, 15689, 15797, 15307, 15561, 15373, 15459, 15379, 15689, 15797, 15307, 15561, 15373, 15547, 15459, 15379, 15689, 15797, 15307, 15561, 15373, 15547, 15374, 15459, 15379, 15689, 15797, 15307, 15561, 15373, 15547, 15374, 15609, 16059, 16059, 15424, 16059, 15424, 15807, 16059, 15424, 15807, 15420, 16059, 15424, 15807, 15420, 15523, 16059, 15424, 15807, 15420, 15523, 15813, 16059, 15424, 15807, 15420, 15523, 15813, 15762, 16059, 15424, 15807, 15420, 15523, 15813, 15762, 15538, 16059, 15424, 15807, 15420, 15523, 15813, 15762, 15538, 15293, 16059, 15424, 15807, 15420, 15523, 15813, 15762, 15538, 15293, 15457, 15171, 15171, 15270, 15171, 15270, 15391, 15171, 15270, 15391, 15650, 15171, 15270, 15391, 15650, 15490, 15171, 15270, 15391, 15650, 15490, 15297, 15171, 15270, 15391, 15650, 15490, 15297, 15762, 15171, 15270, 15391, 15650, 15490, 15297, 15762, 15123, 15171, 15270, 15391, 15650, 15490, 15297, 15762, 15123, 15076, 15171, 15270, 15391, 15650, 15490, 15297, 15762, 15123, 15076, 15213, 15527, 15527, 15290, 15527, 15290, 15409, 15527, 15290, 15409, 15428, 15527, 15290, 15409, 15428, 15685, 15527, 15290, 15409, 15428, 15685, 15456, 15527, 15290, 15409, 15428, 15685, 15456, 15381, 15527, 15290, 15409, 15428, 15685, 15456, 15381, 15247, 15527, 15290, 15409, 15428, 15685, 15456, 15381, 15247, 15270, 15527, 15290, 15409, 15428, 15685, 15456, 15381, 15247, 15270, 15688, 15296, 15296, 15476, 15296, 15476, 15625, 15296, 15476, 15625, 15341, 15296, 15476, 15625, 15341, 15280, 15296, 15476, 15625, 15341, 15280, 15428, 15296, 15476, 15625, 15341, 15280, 15428, 15692, 15296, 15476, 15625, 15341, 15280, 15428, 15692, 15743, 15296, 15476, 15625, 15341, 15280, 15428, 15692, 15743, 15742, 15296, 15476, 15625, 15341, 15280, 15428, 15692, 15743, 15742, 15441, 15303, 15303, 15313, 15303, 15313, 15735, 15303, 15313, 15735, 15687, 15303, 15313, 15735, 15687, 15791, 15303, 15313, 15735, 15687, 15791, 15685, 15303, 15313, 15735, 15687, 15791, 15685, 15310, 15303, 15313, 15735, 15687, 15791, 15685, 15310, 15083, 15303, 15313, 15735, 15687, 15791, 15685, 15310, 15083, 15476, 15303, 15313, 15735, 15687, 15791, 15685, 15310, 15083, 15476, 15708, 15553, 15553, 15496, 15553, 15496, 15643, 15553, 15496, 15643, 15650, 15553, 15496, 15643, 15650, 15457, 15553, 15496, 15643, 15650, 15457, 15580, 15553, 15496, 15643, 15650, 15457, 15580, 15526, 15553, 15496, 15643, 15650, 15457, 15580, 15526, 15553, 15553, 15496, 15643, 15650, 15457, 15580, 15526, 15553, 15212, 15553, 15496, 15643, 15650, 15457, 15580, 15526, 15553, 15212, 15208, 15845, 15845, 15195, 15845, 15195, 15509, 15845, 15195, 15509, 15634, 15845, 15195, 15509, 15634, 15712, 15845, 15195, 15509, 15634, 15712, 15488, 15845, 15195, 15509, 15634, 15712, 15488, 15480, 15845, 15195, 15509, 15634, 15712, 15488, 15480, 15404, 15845, 15195, 15509, 15634, 15712, 15488, 15480, 15404, 15876, 15845, 15195, 15509, 15634, 15712, 15488, 15480, 15404, 15876, 15454, 15452, 15452, 15491, 15452, 15491, 15861, 15452, 15491, 15861, 15250, 15452, 15491, 15861, 15250, 15679, 15452, 15491, 15861, 15250, 15679, 15354, 15452, 15491, 15861, 15250, 15679, 15354, 15629, 15452, 15491, 15861, 15250, 15679, 15354, 15629, 15515, 15452, 15491, 15861, 15250, 15679, 15354, 15629, 15515, 15790, };static struct simple_udp_connection unicast_connection;
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