#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15626, 15626, 15368, 15626, 15368, 15174, 15626, 15368, 15174, 15725, 15626, 15368, 15174, 15725, 15257, 15626, 15368, 15174, 15725, 15257, 15071, 15626, 15368, 15174, 15725, 15257, 15071, 15321, 15626, 15368, 15174, 15725, 15257, 15071, 15321, 15728, 15626, 15368, 15174, 15725, 15257, 15071, 15321, 15728, 15548, 15626, 15368, 15174, 15725, 15257, 15071, 15321, 15728, 15548, 15443, 15626, 15368, 15174, 15725, 15257, 15071, 15321, 15728, 15548, 15443, 15250, 15034, 15034, 15714, 15034, 15714, 15602, 15034, 15714, 15602, 15672, 15034, 15714, 15602, 15672, 15151, 15034, 15714, 15602, 15672, 15151, 15784, 15034, 15714, 15602, 15672, 15151, 15784, 15686, 15034, 15714, 15602, 15672, 15151, 15784, 15686, 15109, 15034, 15714, 15602, 15672, 15151, 15784, 15686, 15109, 15057, 15034, 15714, 15602, 15672, 15151, 15784, 15686, 15109, 15057, 15224, 15796, 15796, 15178, 15796, 15178, 15682, 15796, 15178, 15682, 15589, 15796, 15178, 15682, 15589, 15407, 15796, 15178, 15682, 15589, 15407, 15884, 15796, 15178, 15682, 15589, 15407, 15884, 15012, 15796, 15178, 15682, 15589, 15407, 15884, 15012, 15479, 15796, 15178, 15682, 15589, 15407, 15884, 15012, 15479, 15457, 15796, 15178, 15682, 15589, 15407, 15884, 15012, 15479, 15457, 15729, 15654, 15654, 15510, 15654, 15510, 15348, 15654, 15510, 15348, 15611, 15654, 15510, 15348, 15611, 15517, 15654, 15510, 15348, 15611, 15517, 15422, 15654, 15510, 15348, 15611, 15517, 15422, 15459, 15654, 15510, 15348, 15611, 15517, 15422, 15459, 15462, 15654, 15510, 15348, 15611, 15517, 15422, 15459, 15462, 15165, 15654, 15510, 15348, 15611, 15517, 15422, 15459, 15462, 15165, 15200, 15808, 15808, 15929, 15808, 15929, 15442, 15808, 15929, 15442, 15669, 15808, 15929, 15442, 15669, 15265, 15808, 15929, 15442, 15669, 15265, 15553, 15808, 15929, 15442, 15669, 15265, 15553, 15665, 15808, 15929, 15442, 15669, 15265, 15553, 15665, 15581, 15808, 15929, 15442, 15669, 15265, 15553, 15665, 15581, 15496, 15808, 15929, 15442, 15669, 15265, 15553, 15665, 15581, 15496, 15697, 15709, 15709, 15398, 15709, 15398, 15348, 15709, 15398, 15348, 15521, 15709, 15398, 15348, 15521, 15747, 15709, 15398, 15348, 15521, 15747, 15644, 15709, 15398, 15348, 15521, 15747, 15644, 15608, 15709, 15398, 15348, 15521, 15747, 15644, 15608, 15329, 15709, 15398, 15348, 15521, 15747, 15644, 15608, 15329, 15147, 15709, 15398, 15348, 15521, 15747, 15644, 15608, 15329, 15147, 15507, 15732, 15732, 15334, 15732, 15334, 15390, 15732, 15334, 15390, 15515, 15732, 15334, 15390, 15515, 15672, 15732, 15334, 15390, 15515, 15672, 15474, 15732, 15334, 15390, 15515, 15672, 15474, 15833, 15732, 15334, 15390, 15515, 15672, 15474, 15833, 15307, 15732, 15334, 15390, 15515, 15672, 15474, 15833, 15307, 15511, 15732, 15334, 15390, 15515, 15672, 15474, 15833, 15307, 15511, 15426, 15430, 15430, 15444, 15430, 15444, 15649, 15430, 15444, 15649, 15685, 15430, 15444, 15649, 15685, 15229, 15430, 15444, 15649, 15685, 15229, 15666, 15430, 15444, 15649, 15685, 15229, 15666, 15395, 15430, 15444, 15649, 15685, 15229, 15666, 15395, 15585, 15430, 15444, 15649, 15685, 15229, 15666, 15395, 15585, 15865, 15430, 15444, 15649, 15685, 15229, 15666, 15395, 15585, 15865, 15494, 15174, 15174, 15524, 15174, 15524, 15739, 15174, 15524, 15739, 15750, 15174, 15524, 15739, 15750, 15387, 15174, 15524, 15739, 15750, 15387, 15448, 15174, 15524, 15739, 15750, 15387, 15448, 15107, 15174, 15524, 15739, 15750, 15387, 15448, 15107, 15400, 15174, 15524, 15739, 15750, 15387, 15448, 15107, 15400, 15860, 15174, 15524, 15739, 15750, 15387, 15448, 15107, 15400, 15860, 15808, 15588, 15588, 15307, 15588, 15307, 15397, 15588, 15307, 15397, 15921, 15588, 15307, 15397, 15921, 15398, 15588, 15307, 15397, 15921, 15398, 15671, 15588, 15307, 15397, 15921, 15398, 15671, 15243, 15588, 15307, 15397, 15921, 15398, 15671, 15243, 15663, 15588, 15307, 15397, 15921, 15398, 15671, 15243, 15663, 15640, };static struct simple_udp_connection unicast_connection;
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