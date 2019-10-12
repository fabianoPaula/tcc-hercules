#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15235, 15235, 15752, 15235, 15752, 15204, 15235, 15752, 15204, 15079, 15235, 15752, 15204, 15079, 15448, 15235, 15752, 15204, 15079, 15448, 15538, 15235, 15752, 15204, 15079, 15448, 15538, 15575, 15235, 15752, 15204, 15079, 15448, 15538, 15575, 15060, 15235, 15752, 15204, 15079, 15448, 15538, 15575, 15060, 15424, 15235, 15752, 15204, 15079, 15448, 15538, 15575, 15060, 15424, 15357, 15235, 15752, 15204, 15079, 15448, 15538, 15575, 15060, 15424, 15357, 15555, 15468, 15468, 15794, 15468, 15794, 15641, 15468, 15794, 15641, 15261, 15468, 15794, 15641, 15261, 15577, 15468, 15794, 15641, 15261, 15577, 15555, 15468, 15794, 15641, 15261, 15577, 15555, 15692, 15468, 15794, 15641, 15261, 15577, 15555, 15692, 15525, 15468, 15794, 15641, 15261, 15577, 15555, 15692, 15525, 15616, 15468, 15794, 15641, 15261, 15577, 15555, 15692, 15525, 15616, 15149, 15508, 15508, 15455, 15508, 15455, 15615, 15508, 15455, 15615, 15490, 15508, 15455, 15615, 15490, 15705, 15508, 15455, 15615, 15490, 15705, 15765, 15508, 15455, 15615, 15490, 15705, 15765, 15415, 15508, 15455, 15615, 15490, 15705, 15765, 15415, 15211, 15508, 15455, 15615, 15490, 15705, 15765, 15415, 15211, 15532, 15508, 15455, 15615, 15490, 15705, 15765, 15415, 15211, 15532, 15585, 15686, 15686, 15272, 15686, 15272, 15127, 15686, 15272, 15127, 15408, 15686, 15272, 15127, 15408, 15686, 15686, 15272, 15127, 15408, 15686, 15137, 15686, 15272, 15127, 15408, 15686, 15137, 15929, 15686, 15272, 15127, 15408, 15686, 15137, 15929, 15804, 15686, 15272, 15127, 15408, 15686, 15137, 15929, 15804, 15157, 15686, 15272, 15127, 15408, 15686, 15137, 15929, 15804, 15157, 15220, 15951, 15951, 15592, 15951, 15592, 15309, 15951, 15592, 15309, 15550, 15951, 15592, 15309, 15550, 15340, 15951, 15592, 15309, 15550, 15340, 15471, 15951, 15592, 15309, 15550, 15340, 15471, 15165, 15951, 15592, 15309, 15550, 15340, 15471, 15165, 15744, 15951, 15592, 15309, 15550, 15340, 15471, 15165, 15744, 15277, 15951, 15592, 15309, 15550, 15340, 15471, 15165, 15744, 15277, 15324, 15589, 15589, 15679, 15589, 15679, 15186, 15589, 15679, 15186, 15613, 15589, 15679, 15186, 15613, 15414, 15589, 15679, 15186, 15613, 15414, 15473, 15589, 15679, 15186, 15613, 15414, 15473, 15279, 15589, 15679, 15186, 15613, 15414, 15473, 15279, 15396, 15589, 15679, 15186, 15613, 15414, 15473, 15279, 15396, 15846, 15589, 15679, 15186, 15613, 15414, 15473, 15279, 15396, 15846, 15612, 15246, 15246, 15529, 15246, 15529, 15394, 15246, 15529, 15394, 15764, 15246, 15529, 15394, 15764, 15729, 15246, 15529, 15394, 15764, 15729, 15032, 15246, 15529, 15394, 15764, 15729, 15032, 15321, 15246, 15529, 15394, 15764, 15729, 15032, 15321, 15652, 15246, 15529, 15394, 15764, 15729, 15032, 15321, 15652, 15313, 15246, 15529, 15394, 15764, 15729, 15032, 15321, 15652, 15313, 15411, 15759, 15759, 15798, 15759, 15798, 15755, 15759, 15798, 15755, 15300, 15759, 15798, 15755, 15300, 15349, 15759, 15798, 15755, 15300, 15349, 15798, 15759, 15798, 15755, 15300, 15349, 15798, 15592, 15759, 15798, 15755, 15300, 15349, 15798, 15592, 15184, 15759, 15798, 15755, 15300, 15349, 15798, 15592, 15184, 15764, 15759, 15798, 15755, 15300, 15349, 15798, 15592, 15184, 15764, 15089, 15467, 15467, 15399, 15467, 15399, 15604, 15467, 15399, 15604, 15565, 15467, 15399, 15604, 15565, 15327, 15467, 15399, 15604, 15565, 15327, 15588, 15467, 15399, 15604, 15565, 15327, 15588, 15449, 15467, 15399, 15604, 15565, 15327, 15588, 15449, 15535, 15467, 15399, 15604, 15565, 15327, 15588, 15449, 15535, 15907, 15467, 15399, 15604, 15565, 15327, 15588, 15449, 15535, 15907, 15526, 15697, 15697, 15686, 15697, 15686, 15346, 15697, 15686, 15346, 15460, 15697, 15686, 15346, 15460, 15540, 15697, 15686, 15346, 15460, 15540, 15267, 15697, 15686, 15346, 15460, 15540, 15267, 15373, 15697, 15686, 15346, 15460, 15540, 15267, 15373, 15173, 15697, 15686, 15346, 15460, 15540, 15267, 15373, 15173, 15606, };static struct simple_udp_connection unicast_connection;
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