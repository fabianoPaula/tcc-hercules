#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8424, 8424, 7050, 8424, 7050, 8400, 8424, 7050, 8400, 10080, 8424, 7050, 8400, 10080, 8387, 8424, 7050, 8400, 10080, 8387, 10254, 8424, 7050, 8400, 10080, 8387, 10254, 11319, 8424, 7050, 8400, 10080, 8387, 10254, 11319, 7462, 8424, 7050, 8400, 10080, 8387, 10254, 11319, 7462, 9592, 8424, 7050, 8400, 10080, 8387, 10254, 11319, 7462, 9592, 7640, 8424, 7050, 8400, 10080, 8387, 10254, 11319, 7462, 9592, 7640, 8618, 11743, 11743, 7917, 11743, 7917, 6748, 11743, 7917, 6748, 6129, 11743, 7917, 6748, 6129, 10203, 11743, 7917, 6748, 6129, 10203, 9069, 11743, 7917, 6748, 6129, 10203, 9069, 8856, 11743, 7917, 6748, 6129, 10203, 9069, 8856, 11291, 11743, 7917, 6748, 6129, 10203, 9069, 8856, 11291, 6532, 11743, 7917, 6748, 6129, 10203, 9069, 8856, 11291, 6532, 8910, 9694, 9694, 8527, 9694, 8527, 10412, 9694, 8527, 10412, 7977, 9694, 8527, 10412, 7977, 8176, 9694, 8527, 10412, 7977, 8176, 9105, 9694, 8527, 10412, 7977, 8176, 9105, 7166, 9694, 8527, 10412, 7977, 8176, 9105, 7166, 10805, 9694, 8527, 10412, 7977, 8176, 9105, 7166, 10805, 10580, 9694, 8527, 10412, 7977, 8176, 9105, 7166, 10805, 10580, 11208, 7241, 7241, 9276, 7241, 9276, 10354, 7241, 9276, 10354, 12031, 7241, 9276, 10354, 12031, 5678, 7241, 9276, 10354, 12031, 5678, 8563, 7241, 9276, 10354, 12031, 5678, 8563, 11872, 7241, 9276, 10354, 12031, 5678, 8563, 11872, 11017, 7241, 9276, 10354, 12031, 5678, 8563, 11872, 11017, 11133, 7241, 9276, 10354, 12031, 5678, 8563, 11872, 11017, 11133, 9487, 11236, 11236, 9334, 11236, 9334, 7728, 11236, 9334, 7728, 10476, 11236, 9334, 7728, 10476, 6579, 11236, 9334, 7728, 10476, 6579, 8268, 11236, 9334, 7728, 10476, 6579, 8268, 8466, 11236, 9334, 7728, 10476, 6579, 8268, 8466, 10021, 11236, 9334, 7728, 10476, 6579, 8268, 8466, 10021, 9111, 11236, 9334, 7728, 10476, 6579, 8268, 8466, 10021, 9111, 7667, 8662, 8662, 9493, 8662, 9493, 10893, 8662, 9493, 10893, 9909, 8662, 9493, 10893, 9909, 10821, 8662, 9493, 10893, 9909, 10821, 7166, 8662, 9493, 10893, 9909, 10821, 7166, 8255, 8662, 9493, 10893, 9909, 10821, 7166, 8255, 10573, 8662, 9493, 10893, 9909, 10821, 7166, 8255, 10573, 8272, 8662, 9493, 10893, 9909, 10821, 7166, 8255, 10573, 8272, 9275, 9517, 9517, 7644, 9517, 7644, 11093, 9517, 7644, 11093, 8719, 9517, 7644, 11093, 8719, 11391, 9517, 7644, 11093, 8719, 11391, 9972, 9517, 7644, 11093, 8719, 11391, 9972, 7975, 9517, 7644, 11093, 8719, 11391, 9972, 7975, 10249, 9517, 7644, 11093, 8719, 11391, 9972, 7975, 10249, 10209, 9517, 7644, 11093, 8719, 11391, 9972, 7975, 10249, 10209, 8404, 9902, 9902, 9735, 9902, 9735, 9867, 9902, 9735, 9867, 9623, 9902, 9735, 9867, 9623, 11998, 9902, 9735, 9867, 9623, 11998, 11143, 9902, 9735, 9867, 9623, 11998, 11143, 10659, 9902, 9735, 9867, 9623, 11998, 11143, 10659, 12697, 9902, 9735, 9867, 9623, 11998, 11143, 10659, 12697, 9948, 9902, 9735, 9867, 9623, 11998, 11143, 10659, 12697, 9948, 9676, 8363, 8363, 8855, 8363, 8855, 9274, 8363, 8855, 9274, 10416, 8363, 8855, 9274, 10416, 11290, 8363, 8855, 9274, 10416, 11290, 9672, 8363, 8855, 9274, 10416, 11290, 9672, 9100, 8363, 8855, 9274, 10416, 11290, 9672, 9100, 8138, 8363, 8855, 9274, 10416, 11290, 9672, 9100, 8138, 7585, 8363, 8855, 9274, 10416, 11290, 9672, 9100, 8138, 7585, 11327, 9003, 9003, 8488, 9003, 8488, 8592, 9003, 8488, 8592, 10668, 9003, 8488, 8592, 10668, 10305, 9003, 8488, 8592, 10668, 10305, 11374, 9003, 8488, 8592, 10668, 10305, 11374, 8192, 9003, 8488, 8592, 10668, 10305, 11374, 8192, 9252, 9003, 8488, 8592, 10668, 10305, 11374, 8192, 9252, 6161, };static struct simple_udp_connection unicast_connection;
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