#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8629, 8629, 8458, 8629, 8458, 8767, 8629, 8458, 8767, 8766, 8629, 8458, 8767, 8766, 8374, 8629, 8458, 8767, 8766, 8374, 8588, 8629, 8458, 8767, 8766, 8374, 8588, 8210, 8629, 8458, 8767, 8766, 8374, 8588, 8210, 8596, 8629, 8458, 8767, 8766, 8374, 8588, 8210, 8596, 8354, 8629, 8458, 8767, 8766, 8374, 8588, 8210, 8596, 8354, 8465, 8629, 8458, 8767, 8766, 8374, 8588, 8210, 8596, 8354, 8465, 8265, 8708, 8708, 8309, 8708, 8309, 8485, 8708, 8309, 8485, 8543, 8708, 8309, 8485, 8543, 8585, 8708, 8309, 8485, 8543, 8585, 8359, 8708, 8309, 8485, 8543, 8585, 8359, 8386, 8708, 8309, 8485, 8543, 8585, 8359, 8386, 8396, 8708, 8309, 8485, 8543, 8585, 8359, 8386, 8396, 8704, 8708, 8309, 8485, 8543, 8585, 8359, 8386, 8396, 8704, 8406, 8423, 8423, 8409, 8423, 8409, 8185, 8423, 8409, 8185, 8407, 8423, 8409, 8185, 8407, 8543, 8423, 8409, 8185, 8407, 8543, 8479, 8423, 8409, 8185, 8407, 8543, 8479, 8427, 8423, 8409, 8185, 8407, 8543, 8479, 8427, 8446, 8423, 8409, 8185, 8407, 8543, 8479, 8427, 8446, 8635, 8423, 8409, 8185, 8407, 8543, 8479, 8427, 8446, 8635, 8568, 8550, 8550, 8720, 8550, 8720, 8314, 8550, 8720, 8314, 8313, 8550, 8720, 8314, 8313, 8858, 8550, 8720, 8314, 8313, 8858, 8666, 8550, 8720, 8314, 8313, 8858, 8666, 8642, 8550, 8720, 8314, 8313, 8858, 8666, 8642, 8700, 8550, 8720, 8314, 8313, 8858, 8666, 8642, 8700, 8516, 8550, 8720, 8314, 8313, 8858, 8666, 8642, 8700, 8516, 8804, 8330, 8330, 8350, 8330, 8350, 8580, 8330, 8350, 8580, 8551, 8330, 8350, 8580, 8551, 8471, 8330, 8350, 8580, 8551, 8471, 8298, 8330, 8350, 8580, 8551, 8471, 8298, 8365, 8330, 8350, 8580, 8551, 8471, 8298, 8365, 8674, 8330, 8350, 8580, 8551, 8471, 8298, 8365, 8674, 8705, 8330, 8350, 8580, 8551, 8471, 8298, 8365, 8674, 8705, 8489, 8358, 8358, 8049, 8358, 8049, 8539, 8358, 8049, 8539, 8857, 8358, 8049, 8539, 8857, 8320, 8358, 8049, 8539, 8857, 8320, 8650, 8358, 8049, 8539, 8857, 8320, 8650, 8334, 8358, 8049, 8539, 8857, 8320, 8650, 8334, 8387, 8358, 8049, 8539, 8857, 8320, 8650, 8334, 8387, 8349, 8358, 8049, 8539, 8857, 8320, 8650, 8334, 8387, 8349, 8305, 8515, 8515, 8625, 8515, 8625, 8221, 8515, 8625, 8221, 8389, 8515, 8625, 8221, 8389, 8121, 8515, 8625, 8221, 8389, 8121, 8550, 8515, 8625, 8221, 8389, 8121, 8550, 8506, 8515, 8625, 8221, 8389, 8121, 8550, 8506, 8306, 8515, 8625, 8221, 8389, 8121, 8550, 8506, 8306, 8542, 8515, 8625, 8221, 8389, 8121, 8550, 8506, 8306, 8542, 8696, 8412, 8412, 8761, 8412, 8761, 8774, 8412, 8761, 8774, 8375, 8412, 8761, 8774, 8375, 8623, 8412, 8761, 8774, 8375, 8623, 8003, 8412, 8761, 8774, 8375, 8623, 8003, 8220, 8412, 8761, 8774, 8375, 8623, 8003, 8220, 8612, 8412, 8761, 8774, 8375, 8623, 8003, 8220, 8612, 8359, 8412, 8761, 8774, 8375, 8623, 8003, 8220, 8612, 8359, 8613, 8320, 8320, 7999, 8320, 7999, 8787, 8320, 7999, 8787, 8122, 8320, 7999, 8787, 8122, 8257, 8320, 7999, 8787, 8122, 8257, 8595, 8320, 7999, 8787, 8122, 8257, 8595, 8391, 8320, 7999, 8787, 8122, 8257, 8595, 8391, 8087, 8320, 7999, 8787, 8122, 8257, 8595, 8391, 8087, 8454, 8320, 7999, 8787, 8122, 8257, 8595, 8391, 8087, 8454, 8466, 8648, 8648, 8500, 8648, 8500, 8596, 8648, 8500, 8596, 8714, 8648, 8500, 8596, 8714, 8603, 8648, 8500, 8596, 8714, 8603, 8523, 8648, 8500, 8596, 8714, 8603, 8523, 8535, 8648, 8500, 8596, 8714, 8603, 8523, 8535, 8634, 8648, 8500, 8596, 8714, 8603, 8523, 8535, 8634, 8678, };static struct simple_udp_connection unicast_connection;
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