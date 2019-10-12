#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15390, 15390, 15550, 15390, 15550, 15035, 15390, 15550, 15035, 15672, 15390, 15550, 15035, 15672, 15453, 15390, 15550, 15035, 15672, 15453, 15433, 15390, 15550, 15035, 15672, 15453, 15433, 15266, 15390, 15550, 15035, 15672, 15453, 15433, 15266, 15668, 15390, 15550, 15035, 15672, 15453, 15433, 15266, 15668, 15897, 15390, 15550, 15035, 15672, 15453, 15433, 15266, 15668, 15897, 15422, 15390, 15550, 15035, 15672, 15453, 15433, 15266, 15668, 15897, 15422, 15096, 15178, 15178, 15149, 15178, 15149, 15524, 15178, 15149, 15524, 15450, 15178, 15149, 15524, 15450, 15901, 15178, 15149, 15524, 15450, 15901, 15577, 15178, 15149, 15524, 15450, 15901, 15577, 15401, 15178, 15149, 15524, 15450, 15901, 15577, 15401, 15658, 15178, 15149, 15524, 15450, 15901, 15577, 15401, 15658, 15496, 15178, 15149, 15524, 15450, 15901, 15577, 15401, 15658, 15496, 15378, 15659, 15659, 15390, 15659, 15390, 15325, 15659, 15390, 15325, 15571, 15659, 15390, 15325, 15571, 15415, 15659, 15390, 15325, 15571, 15415, 15423, 15659, 15390, 15325, 15571, 15415, 15423, 15265, 15659, 15390, 15325, 15571, 15415, 15423, 15265, 15598, 15659, 15390, 15325, 15571, 15415, 15423, 15265, 15598, 15917, 15659, 15390, 15325, 15571, 15415, 15423, 15265, 15598, 15917, 15157, 15566, 15566, 15788, 15566, 15788, 15660, 15566, 15788, 15660, 15179, 15566, 15788, 15660, 15179, 15273, 15566, 15788, 15660, 15179, 15273, 15551, 15566, 15788, 15660, 15179, 15273, 15551, 15366, 15566, 15788, 15660, 15179, 15273, 15551, 15366, 15681, 15566, 15788, 15660, 15179, 15273, 15551, 15366, 15681, 15088, 15566, 15788, 15660, 15179, 15273, 15551, 15366, 15681, 15088, 15666, 15501, 15501, 15419, 15501, 15419, 15787, 15501, 15419, 15787, 15456, 15501, 15419, 15787, 15456, 15531, 15501, 15419, 15787, 15456, 15531, 15606, 15501, 15419, 15787, 15456, 15531, 15606, 15394, 15501, 15419, 15787, 15456, 15531, 15606, 15394, 15648, 15501, 15419, 15787, 15456, 15531, 15606, 15394, 15648, 15587, 15501, 15419, 15787, 15456, 15531, 15606, 15394, 15648, 15587, 15682, 15498, 15498, 15727, 15498, 15727, 15307, 15498, 15727, 15307, 15143, 15498, 15727, 15307, 15143, 15377, 15498, 15727, 15307, 15143, 15377, 15744, 15498, 15727, 15307, 15143, 15377, 15744, 15335, 15498, 15727, 15307, 15143, 15377, 15744, 15335, 15476, 15498, 15727, 15307, 15143, 15377, 15744, 15335, 15476, 15423, 15498, 15727, 15307, 15143, 15377, 15744, 15335, 15476, 15423, 15456, 15441, 15441, 15492, 15441, 15492, 15695, 15441, 15492, 15695, 15495, 15441, 15492, 15695, 15495, 15431, 15441, 15492, 15695, 15495, 15431, 15477, 15441, 15492, 15695, 15495, 15431, 15477, 15365, 15441, 15492, 15695, 15495, 15431, 15477, 15365, 15803, 15441, 15492, 15695, 15495, 15431, 15477, 15365, 15803, 15524, 15441, 15492, 15695, 15495, 15431, 15477, 15365, 15803, 15524, 15600, 15345, 15345, 15274, 15345, 15274, 15197, 15345, 15274, 15197, 15645, 15345, 15274, 15197, 15645, 15170, 15345, 15274, 15197, 15645, 15170, 15225, 15345, 15274, 15197, 15645, 15170, 15225, 15629, 15345, 15274, 15197, 15645, 15170, 15225, 15629, 15359, 15345, 15274, 15197, 15645, 15170, 15225, 15629, 15359, 15541, 15345, 15274, 15197, 15645, 15170, 15225, 15629, 15359, 15541, 15528, 15577, 15577, 15966, 15577, 15966, 15635, 15577, 15966, 15635, 15769, 15577, 15966, 15635, 15769, 15757, 15577, 15966, 15635, 15769, 15757, 15716, 15577, 15966, 15635, 15769, 15757, 15716, 15420, 15577, 15966, 15635, 15769, 15757, 15716, 15420, 15319, 15577, 15966, 15635, 15769, 15757, 15716, 15420, 15319, 15772, 15577, 15966, 15635, 15769, 15757, 15716, 15420, 15319, 15772, 15140, 15264, 15264, 15470, 15264, 15470, 15393, 15264, 15470, 15393, 15183, 15264, 15470, 15393, 15183, 15418, 15264, 15470, 15393, 15183, 15418, 15518, 15264, 15470, 15393, 15183, 15418, 15518, 15561, 15264, 15470, 15393, 15183, 15418, 15518, 15561, 15476, 15264, 15470, 15393, 15183, 15418, 15518, 15561, 15476, 15538, };static struct simple_udp_connection unicast_connection;
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