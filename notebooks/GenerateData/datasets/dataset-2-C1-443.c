#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {9986, 9986, 9570, 9986, 9570, 9399, 9986, 9570, 9399, 7961, 9986, 9570, 9399, 7961, 6504, 9986, 9570, 9399, 7961, 6504, 10282, 9986, 9570, 9399, 7961, 6504, 10282, 7515, 9986, 9570, 9399, 7961, 6504, 10282, 7515, 10209, 9986, 9570, 9399, 7961, 6504, 10282, 7515, 10209, 7479, 9986, 9570, 9399, 7961, 6504, 10282, 7515, 10209, 7479, 5871, 9986, 9570, 9399, 7961, 6504, 10282, 7515, 10209, 7479, 5871, 9190, 10583, 10583, 8324, 10583, 8324, 13026, 10583, 8324, 13026, 6539, 10583, 8324, 13026, 6539, 9015, 10583, 8324, 13026, 6539, 9015, 8799, 10583, 8324, 13026, 6539, 9015, 8799, 10845, 10583, 8324, 13026, 6539, 9015, 8799, 10845, 5371, 10583, 8324, 13026, 6539, 9015, 8799, 10845, 5371, 9969, 10583, 8324, 13026, 6539, 9015, 8799, 10845, 5371, 9969, 9838, 10387, 10387, 10480, 10387, 10480, 6301, 10387, 10480, 6301, 10922, 10387, 10480, 6301, 10922, 7781, 10387, 10480, 6301, 10922, 7781, 9951, 10387, 10480, 6301, 10922, 7781, 9951, 8865, 10387, 10480, 6301, 10922, 7781, 9951, 8865, 6433, 10387, 10480, 6301, 10922, 7781, 9951, 8865, 6433, 6644, 10387, 10480, 6301, 10922, 7781, 9951, 8865, 6433, 6644, 11127, 5320, 5320, 7875, 5320, 7875, 6968, 5320, 7875, 6968, 9199, 5320, 7875, 6968, 9199, 9157, 5320, 7875, 6968, 9199, 9157, 9221, 5320, 7875, 6968, 9199, 9157, 9221, 6409, 5320, 7875, 6968, 9199, 9157, 9221, 6409, 8070, 5320, 7875, 6968, 9199, 9157, 9221, 6409, 8070, 8185, 5320, 7875, 6968, 9199, 9157, 9221, 6409, 8070, 8185, 9582, 8584, 8584, 11342, 8584, 11342, 9266, 8584, 11342, 9266, 9390, 8584, 11342, 9266, 9390, 12949, 8584, 11342, 9266, 9390, 12949, 9928, 8584, 11342, 9266, 9390, 12949, 9928, 11298, 8584, 11342, 9266, 9390, 12949, 9928, 11298, 9144, 8584, 11342, 9266, 9390, 12949, 9928, 11298, 9144, 10780, 8584, 11342, 9266, 9390, 12949, 9928, 11298, 9144, 10780, 7968, 8858, 8858, 9026, 8858, 9026, 10993, 8858, 9026, 10993, 8687, 8858, 9026, 10993, 8687, 10145, 8858, 9026, 10993, 8687, 10145, 7388, 8858, 9026, 10993, 8687, 10145, 7388, 10872, 8858, 9026, 10993, 8687, 10145, 7388, 10872, 11553, 8858, 9026, 10993, 8687, 10145, 7388, 10872, 11553, 10712, 8858, 9026, 10993, 8687, 10145, 7388, 10872, 11553, 10712, 9984, 9579, 9579, 9311, 9579, 9311, 13629, 9579, 9311, 13629, 10198, 9579, 9311, 13629, 10198, 9523, 9579, 9311, 13629, 10198, 9523, 7415, 9579, 9311, 13629, 10198, 9523, 7415, 9181, 9579, 9311, 13629, 10198, 9523, 7415, 9181, 8234, 9579, 9311, 13629, 10198, 9523, 7415, 9181, 8234, 10520, 9579, 9311, 13629, 10198, 9523, 7415, 9181, 8234, 10520, 10711, 7935, 7935, 8677, 7935, 8677, 9416, 7935, 8677, 9416, 10494, 7935, 8677, 9416, 10494, 8545, 7935, 8677, 9416, 10494, 8545, 11864, 7935, 8677, 9416, 10494, 8545, 11864, 8558, 7935, 8677, 9416, 10494, 8545, 11864, 8558, 11740, 7935, 8677, 9416, 10494, 8545, 11864, 8558, 11740, 7679, 7935, 8677, 9416, 10494, 8545, 11864, 8558, 11740, 7679, 10653, 5394, 5394, 8832, 5394, 8832, 5250, 5394, 8832, 5250, 9343, 5394, 8832, 5250, 9343, 9053, 5394, 8832, 5250, 9343, 9053, 7355, 5394, 8832, 5250, 9343, 9053, 7355, 9930, 5394, 8832, 5250, 9343, 9053, 7355, 9930, 7021, 5394, 8832, 5250, 9343, 9053, 7355, 9930, 7021, 9512, 5394, 8832, 5250, 9343, 9053, 7355, 9930, 7021, 9512, 10193, 9487, 9487, 10170, 9487, 10170, 7625, 9487, 10170, 7625, 7421, 9487, 10170, 7625, 7421, 3392, 9487, 10170, 7625, 7421, 3392, 7499, 9487, 10170, 7625, 7421, 3392, 7499, 6504, 9487, 10170, 7625, 7421, 3392, 7499, 6504, 8037, 9487, 10170, 7625, 7421, 3392, 7499, 6504, 8037, 8218, };static struct simple_udp_connection unicast_connection;
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