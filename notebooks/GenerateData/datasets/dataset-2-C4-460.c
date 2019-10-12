#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15570, 15570, 15605, 15570, 15605, 15418, 15570, 15605, 15418, 15301, 15570, 15605, 15418, 15301, 15497, 15570, 15605, 15418, 15301, 15497, 15279, 15570, 15605, 15418, 15301, 15497, 15279, 15554, 15570, 15605, 15418, 15301, 15497, 15279, 15554, 15513, 15570, 15605, 15418, 15301, 15497, 15279, 15554, 15513, 15295, 15570, 15605, 15418, 15301, 15497, 15279, 15554, 15513, 15295, 15549, 15570, 15605, 15418, 15301, 15497, 15279, 15554, 15513, 15295, 15549, 15317, 15591, 15591, 15635, 15591, 15635, 15543, 15591, 15635, 15543, 15775, 15591, 15635, 15543, 15775, 15489, 15591, 15635, 15543, 15775, 15489, 15100, 15591, 15635, 15543, 15775, 15489, 15100, 15669, 15591, 15635, 15543, 15775, 15489, 15100, 15669, 15842, 15591, 15635, 15543, 15775, 15489, 15100, 15669, 15842, 15872, 15591, 15635, 15543, 15775, 15489, 15100, 15669, 15842, 15872, 15671, 15623, 15623, 15497, 15623, 15497, 15800, 15623, 15497, 15800, 15782, 15623, 15497, 15800, 15782, 15865, 15623, 15497, 15800, 15782, 15865, 15476, 15623, 15497, 15800, 15782, 15865, 15476, 15584, 15623, 15497, 15800, 15782, 15865, 15476, 15584, 15484, 15623, 15497, 15800, 15782, 15865, 15476, 15584, 15484, 15242, 15623, 15497, 15800, 15782, 15865, 15476, 15584, 15484, 15242, 15498, 15608, 15608, 15804, 15608, 15804, 15784, 15608, 15804, 15784, 15475, 15608, 15804, 15784, 15475, 15446, 15608, 15804, 15784, 15475, 15446, 15587, 15608, 15804, 15784, 15475, 15446, 15587, 15354, 15608, 15804, 15784, 15475, 15446, 15587, 15354, 15072, 15608, 15804, 15784, 15475, 15446, 15587, 15354, 15072, 15566, 15608, 15804, 15784, 15475, 15446, 15587, 15354, 15072, 15566, 15441, 14957, 14957, 15300, 14957, 15300, 15476, 14957, 15300, 15476, 15417, 14957, 15300, 15476, 15417, 15686, 14957, 15300, 15476, 15417, 15686, 15547, 14957, 15300, 15476, 15417, 15686, 15547, 14938, 14957, 15300, 15476, 15417, 15686, 15547, 14938, 15813, 14957, 15300, 15476, 15417, 15686, 15547, 14938, 15813, 15465, 14957, 15300, 15476, 15417, 15686, 15547, 14938, 15813, 15465, 15159, 15558, 15558, 16052, 15558, 16052, 15052, 15558, 16052, 15052, 15651, 15558, 16052, 15052, 15651, 15727, 15558, 16052, 15052, 15651, 15727, 15603, 15558, 16052, 15052, 15651, 15727, 15603, 15485, 15558, 16052, 15052, 15651, 15727, 15603, 15485, 15167, 15558, 16052, 15052, 15651, 15727, 15603, 15485, 15167, 15760, 15558, 16052, 15052, 15651, 15727, 15603, 15485, 15167, 15760, 15425, 15096, 15096, 15765, 15096, 15765, 15482, 15096, 15765, 15482, 15622, 15096, 15765, 15482, 15622, 15539, 15096, 15765, 15482, 15622, 15539, 15594, 15096, 15765, 15482, 15622, 15539, 15594, 15845, 15096, 15765, 15482, 15622, 15539, 15594, 15845, 15816, 15096, 15765, 15482, 15622, 15539, 15594, 15845, 15816, 15353, 15096, 15765, 15482, 15622, 15539, 15594, 15845, 15816, 15353, 15201, 15297, 15297, 15423, 15297, 15423, 15954, 15297, 15423, 15954, 15231, 15297, 15423, 15954, 15231, 15113, 15297, 15423, 15954, 15231, 15113, 15727, 15297, 15423, 15954, 15231, 15113, 15727, 15274, 15297, 15423, 15954, 15231, 15113, 15727, 15274, 15276, 15297, 15423, 15954, 15231, 15113, 15727, 15274, 15276, 15698, 15297, 15423, 15954, 15231, 15113, 15727, 15274, 15276, 15698, 15496, 15372, 15372, 15533, 15372, 15533, 15786, 15372, 15533, 15786, 15689, 15372, 15533, 15786, 15689, 15450, 15372, 15533, 15786, 15689, 15450, 15431, 15372, 15533, 15786, 15689, 15450, 15431, 15595, 15372, 15533, 15786, 15689, 15450, 15431, 15595, 15639, 15372, 15533, 15786, 15689, 15450, 15431, 15595, 15639, 15491, 15372, 15533, 15786, 15689, 15450, 15431, 15595, 15639, 15491, 15296, 15317, 15317, 15284, 15317, 15284, 15639, 15317, 15284, 15639, 15151, 15317, 15284, 15639, 15151, 15284, 15317, 15284, 15639, 15151, 15284, 15392, 15317, 15284, 15639, 15151, 15284, 15392, 15681, 15317, 15284, 15639, 15151, 15284, 15392, 15681, 15319, 15317, 15284, 15639, 15151, 15284, 15392, 15681, 15319, 15456, };static struct simple_udp_connection unicast_connection;
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