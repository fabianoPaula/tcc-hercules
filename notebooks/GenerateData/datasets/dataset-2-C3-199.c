#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15721, 15721, 15435, 15721, 15435, 15957, 15721, 15435, 15957, 15130, 15721, 15435, 15957, 15130, 15193, 15721, 15435, 15957, 15130, 15193, 15717, 15721, 15435, 15957, 15130, 15193, 15717, 15741, 15721, 15435, 15957, 15130, 15193, 15717, 15741, 15441, 15721, 15435, 15957, 15130, 15193, 15717, 15741, 15441, 15307, 15721, 15435, 15957, 15130, 15193, 15717, 15741, 15441, 15307, 15540, 15721, 15435, 15957, 15130, 15193, 15717, 15741, 15441, 15307, 15540, 15824, 15037, 15037, 15707, 15037, 15707, 15968, 15037, 15707, 15968, 15455, 15037, 15707, 15968, 15455, 15369, 15037, 15707, 15968, 15455, 15369, 15662, 15037, 15707, 15968, 15455, 15369, 15662, 15413, 15037, 15707, 15968, 15455, 15369, 15662, 15413, 15497, 15037, 15707, 15968, 15455, 15369, 15662, 15413, 15497, 15760, 15037, 15707, 15968, 15455, 15369, 15662, 15413, 15497, 15760, 15481, 15782, 15782, 15585, 15782, 15585, 15424, 15782, 15585, 15424, 15281, 15782, 15585, 15424, 15281, 15316, 15782, 15585, 15424, 15281, 15316, 15476, 15782, 15585, 15424, 15281, 15316, 15476, 15735, 15782, 15585, 15424, 15281, 15316, 15476, 15735, 15593, 15782, 15585, 15424, 15281, 15316, 15476, 15735, 15593, 15516, 15782, 15585, 15424, 15281, 15316, 15476, 15735, 15593, 15516, 15636, 15420, 15420, 15427, 15420, 15427, 15449, 15420, 15427, 15449, 15478, 15420, 15427, 15449, 15478, 15558, 15420, 15427, 15449, 15478, 15558, 15835, 15420, 15427, 15449, 15478, 15558, 15835, 15580, 15420, 15427, 15449, 15478, 15558, 15835, 15580, 15681, 15420, 15427, 15449, 15478, 15558, 15835, 15580, 15681, 15437, 15420, 15427, 15449, 15478, 15558, 15835, 15580, 15681, 15437, 15524, 15681, 15681, 15649, 15681, 15649, 15404, 15681, 15649, 15404, 15449, 15681, 15649, 15404, 15449, 15620, 15681, 15649, 15404, 15449, 15620, 15269, 15681, 15649, 15404, 15449, 15620, 15269, 15514, 15681, 15649, 15404, 15449, 15620, 15269, 15514, 15464, 15681, 15649, 15404, 15449, 15620, 15269, 15514, 15464, 15349, 15681, 15649, 15404, 15449, 15620, 15269, 15514, 15464, 15349, 15145, 15622, 15622, 15369, 15622, 15369, 15401, 15622, 15369, 15401, 15463, 15622, 15369, 15401, 15463, 15432, 15622, 15369, 15401, 15463, 15432, 15794, 15622, 15369, 15401, 15463, 15432, 15794, 15466, 15622, 15369, 15401, 15463, 15432, 15794, 15466, 15447, 15622, 15369, 15401, 15463, 15432, 15794, 15466, 15447, 15187, 15622, 15369, 15401, 15463, 15432, 15794, 15466, 15447, 15187, 15431, 14990, 14990, 15808, 14990, 15808, 15314, 14990, 15808, 15314, 15498, 14990, 15808, 15314, 15498, 15628, 14990, 15808, 15314, 15498, 15628, 15597, 14990, 15808, 15314, 15498, 15628, 15597, 15759, 14990, 15808, 15314, 15498, 15628, 15597, 15759, 15194, 14990, 15808, 15314, 15498, 15628, 15597, 15759, 15194, 15678, 14990, 15808, 15314, 15498, 15628, 15597, 15759, 15194, 15678, 15765, 15144, 15144, 15458, 15144, 15458, 15448, 15144, 15458, 15448, 15362, 15144, 15458, 15448, 15362, 15348, 15144, 15458, 15448, 15362, 15348, 15294, 15144, 15458, 15448, 15362, 15348, 15294, 15085, 15144, 15458, 15448, 15362, 15348, 15294, 15085, 15453, 15144, 15458, 15448, 15362, 15348, 15294, 15085, 15453, 15702, 15144, 15458, 15448, 15362, 15348, 15294, 15085, 15453, 15702, 15749, 15146, 15146, 15507, 15146, 15507, 15444, 15146, 15507, 15444, 15587, 15146, 15507, 15444, 15587, 15526, 15146, 15507, 15444, 15587, 15526, 15524, 15146, 15507, 15444, 15587, 15526, 15524, 15363, 15146, 15507, 15444, 15587, 15526, 15524, 15363, 15345, 15146, 15507, 15444, 15587, 15526, 15524, 15363, 15345, 15775, 15146, 15507, 15444, 15587, 15526, 15524, 15363, 15345, 15775, 15501, 15535, 15535, 15522, 15535, 15522, 15597, 15535, 15522, 15597, 15577, 15535, 15522, 15597, 15577, 15632, 15535, 15522, 15597, 15577, 15632, 15825, 15535, 15522, 15597, 15577, 15632, 15825, 15521, 15535, 15522, 15597, 15577, 15632, 15825, 15521, 15646, 15535, 15522, 15597, 15577, 15632, 15825, 15521, 15646, 15812, };static struct simple_udp_connection unicast_connection;
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