#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15616, 15616, 15746, 15616, 15746, 15664, 15616, 15746, 15664, 15340, 15616, 15746, 15664, 15340, 15582, 15616, 15746, 15664, 15340, 15582, 15464, 15616, 15746, 15664, 15340, 15582, 15464, 15485, 15616, 15746, 15664, 15340, 15582, 15464, 15485, 15386, 15616, 15746, 15664, 15340, 15582, 15464, 15485, 15386, 15481, 15616, 15746, 15664, 15340, 15582, 15464, 15485, 15386, 15481, 15671, 15616, 15746, 15664, 15340, 15582, 15464, 15485, 15386, 15481, 15671, 15327, 15503, 15503, 15813, 15503, 15813, 15406, 15503, 15813, 15406, 15421, 15503, 15813, 15406, 15421, 15663, 15503, 15813, 15406, 15421, 15663, 15515, 15503, 15813, 15406, 15421, 15663, 15515, 15479, 15503, 15813, 15406, 15421, 15663, 15515, 15479, 15824, 15503, 15813, 15406, 15421, 15663, 15515, 15479, 15824, 15232, 15503, 15813, 15406, 15421, 15663, 15515, 15479, 15824, 15232, 15472, 15404, 15404, 15181, 15404, 15181, 15326, 15404, 15181, 15326, 15493, 15404, 15181, 15326, 15493, 15194, 15404, 15181, 15326, 15493, 15194, 15475, 15404, 15181, 15326, 15493, 15194, 15475, 15447, 15404, 15181, 15326, 15493, 15194, 15475, 15447, 15699, 15404, 15181, 15326, 15493, 15194, 15475, 15447, 15699, 15563, 15404, 15181, 15326, 15493, 15194, 15475, 15447, 15699, 15563, 15492, 15486, 15486, 15411, 15486, 15411, 15404, 15486, 15411, 15404, 15510, 15486, 15411, 15404, 15510, 15305, 15486, 15411, 15404, 15510, 15305, 15213, 15486, 15411, 15404, 15510, 15305, 15213, 15428, 15486, 15411, 15404, 15510, 15305, 15213, 15428, 15590, 15486, 15411, 15404, 15510, 15305, 15213, 15428, 15590, 15338, 15486, 15411, 15404, 15510, 15305, 15213, 15428, 15590, 15338, 14961, 15600, 15600, 15499, 15600, 15499, 15421, 15600, 15499, 15421, 15468, 15600, 15499, 15421, 15468, 15589, 15600, 15499, 15421, 15468, 15589, 15737, 15600, 15499, 15421, 15468, 15589, 15737, 15433, 15600, 15499, 15421, 15468, 15589, 15737, 15433, 15476, 15600, 15499, 15421, 15468, 15589, 15737, 15433, 15476, 15867, 15600, 15499, 15421, 15468, 15589, 15737, 15433, 15476, 15867, 15645, 15273, 15273, 15216, 15273, 15216, 15932, 15273, 15216, 15932, 15148, 15273, 15216, 15932, 15148, 15642, 15273, 15216, 15932, 15148, 15642, 15361, 15273, 15216, 15932, 15148, 15642, 15361, 15551, 15273, 15216, 15932, 15148, 15642, 15361, 15551, 15709, 15273, 15216, 15932, 15148, 15642, 15361, 15551, 15709, 15115, 15273, 15216, 15932, 15148, 15642, 15361, 15551, 15709, 15115, 15470, 15772, 15772, 15691, 15772, 15691, 15912, 15772, 15691, 15912, 15328, 15772, 15691, 15912, 15328, 15420, 15772, 15691, 15912, 15328, 15420, 15463, 15772, 15691, 15912, 15328, 15420, 15463, 15554, 15772, 15691, 15912, 15328, 15420, 15463, 15554, 15385, 15772, 15691, 15912, 15328, 15420, 15463, 15554, 15385, 15398, 15772, 15691, 15912, 15328, 15420, 15463, 15554, 15385, 15398, 15414, 15529, 15529, 15553, 15529, 15553, 15390, 15529, 15553, 15390, 15524, 15529, 15553, 15390, 15524, 15855, 15529, 15553, 15390, 15524, 15855, 15243, 15529, 15553, 15390, 15524, 15855, 15243, 15715, 15529, 15553, 15390, 15524, 15855, 15243, 15715, 15542, 15529, 15553, 15390, 15524, 15855, 15243, 15715, 15542, 15415, 15529, 15553, 15390, 15524, 15855, 15243, 15715, 15542, 15415, 15471, 15292, 15292, 15469, 15292, 15469, 15098, 15292, 15469, 15098, 15497, 15292, 15469, 15098, 15497, 15482, 15292, 15469, 15098, 15497, 15482, 15781, 15292, 15469, 15098, 15497, 15482, 15781, 15431, 15292, 15469, 15098, 15497, 15482, 15781, 15431, 15352, 15292, 15469, 15098, 15497, 15482, 15781, 15431, 15352, 15368, 15292, 15469, 15098, 15497, 15482, 15781, 15431, 15352, 15368, 15252, 15748, 15748, 15526, 15748, 15526, 15623, 15748, 15526, 15623, 15736, 15748, 15526, 15623, 15736, 15376, 15748, 15526, 15623, 15736, 15376, 15438, 15748, 15526, 15623, 15736, 15376, 15438, 15565, 15748, 15526, 15623, 15736, 15376, 15438, 15565, 15536, 15748, 15526, 15623, 15736, 15376, 15438, 15565, 15536, 15308, };static struct simple_udp_connection unicast_connection;
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