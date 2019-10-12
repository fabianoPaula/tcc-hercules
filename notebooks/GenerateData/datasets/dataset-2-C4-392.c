#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15629, 15629, 15458, 15629, 15458, 15767, 15629, 15458, 15767, 15766, 15629, 15458, 15767, 15766, 15374, 15629, 15458, 15767, 15766, 15374, 15588, 15629, 15458, 15767, 15766, 15374, 15588, 15210, 15629, 15458, 15767, 15766, 15374, 15588, 15210, 15596, 15629, 15458, 15767, 15766, 15374, 15588, 15210, 15596, 15354, 15629, 15458, 15767, 15766, 15374, 15588, 15210, 15596, 15354, 15465, 15629, 15458, 15767, 15766, 15374, 15588, 15210, 15596, 15354, 15465, 15265, 15708, 15708, 15309, 15708, 15309, 15485, 15708, 15309, 15485, 15543, 15708, 15309, 15485, 15543, 15585, 15708, 15309, 15485, 15543, 15585, 15359, 15708, 15309, 15485, 15543, 15585, 15359, 15386, 15708, 15309, 15485, 15543, 15585, 15359, 15386, 15396, 15708, 15309, 15485, 15543, 15585, 15359, 15386, 15396, 15704, 15708, 15309, 15485, 15543, 15585, 15359, 15386, 15396, 15704, 15406, 15423, 15423, 15409, 15423, 15409, 15185, 15423, 15409, 15185, 15407, 15423, 15409, 15185, 15407, 15543, 15423, 15409, 15185, 15407, 15543, 15479, 15423, 15409, 15185, 15407, 15543, 15479, 15427, 15423, 15409, 15185, 15407, 15543, 15479, 15427, 15446, 15423, 15409, 15185, 15407, 15543, 15479, 15427, 15446, 15635, 15423, 15409, 15185, 15407, 15543, 15479, 15427, 15446, 15635, 15568, 15550, 15550, 15720, 15550, 15720, 15314, 15550, 15720, 15314, 15313, 15550, 15720, 15314, 15313, 15858, 15550, 15720, 15314, 15313, 15858, 15666, 15550, 15720, 15314, 15313, 15858, 15666, 15642, 15550, 15720, 15314, 15313, 15858, 15666, 15642, 15700, 15550, 15720, 15314, 15313, 15858, 15666, 15642, 15700, 15516, 15550, 15720, 15314, 15313, 15858, 15666, 15642, 15700, 15516, 15804, 15330, 15330, 15350, 15330, 15350, 15580, 15330, 15350, 15580, 15551, 15330, 15350, 15580, 15551, 15471, 15330, 15350, 15580, 15551, 15471, 15298, 15330, 15350, 15580, 15551, 15471, 15298, 15365, 15330, 15350, 15580, 15551, 15471, 15298, 15365, 15674, 15330, 15350, 15580, 15551, 15471, 15298, 15365, 15674, 15705, 15330, 15350, 15580, 15551, 15471, 15298, 15365, 15674, 15705, 15489, 15358, 15358, 15049, 15358, 15049, 15539, 15358, 15049, 15539, 15857, 15358, 15049, 15539, 15857, 15320, 15358, 15049, 15539, 15857, 15320, 15650, 15358, 15049, 15539, 15857, 15320, 15650, 15334, 15358, 15049, 15539, 15857, 15320, 15650, 15334, 15387, 15358, 15049, 15539, 15857, 15320, 15650, 15334, 15387, 15349, 15358, 15049, 15539, 15857, 15320, 15650, 15334, 15387, 15349, 15305, 15515, 15515, 15625, 15515, 15625, 15221, 15515, 15625, 15221, 15389, 15515, 15625, 15221, 15389, 15121, 15515, 15625, 15221, 15389, 15121, 15550, 15515, 15625, 15221, 15389, 15121, 15550, 15506, 15515, 15625, 15221, 15389, 15121, 15550, 15506, 15306, 15515, 15625, 15221, 15389, 15121, 15550, 15506, 15306, 15542, 15515, 15625, 15221, 15389, 15121, 15550, 15506, 15306, 15542, 15696, 15412, 15412, 15761, 15412, 15761, 15774, 15412, 15761, 15774, 15375, 15412, 15761, 15774, 15375, 15623, 15412, 15761, 15774, 15375, 15623, 15003, 15412, 15761, 15774, 15375, 15623, 15003, 15220, 15412, 15761, 15774, 15375, 15623, 15003, 15220, 15612, 15412, 15761, 15774, 15375, 15623, 15003, 15220, 15612, 15359, 15412, 15761, 15774, 15375, 15623, 15003, 15220, 15612, 15359, 15613, 15320, 15320, 14999, 15320, 14999, 15787, 15320, 14999, 15787, 15122, 15320, 14999, 15787, 15122, 15257, 15320, 14999, 15787, 15122, 15257, 15595, 15320, 14999, 15787, 15122, 15257, 15595, 15391, 15320, 14999, 15787, 15122, 15257, 15595, 15391, 15087, 15320, 14999, 15787, 15122, 15257, 15595, 15391, 15087, 15454, 15320, 14999, 15787, 15122, 15257, 15595, 15391, 15087, 15454, 15466, 15648, 15648, 15500, 15648, 15500, 15596, 15648, 15500, 15596, 15714, 15648, 15500, 15596, 15714, 15603, 15648, 15500, 15596, 15714, 15603, 15523, 15648, 15500, 15596, 15714, 15603, 15523, 15535, 15648, 15500, 15596, 15714, 15603, 15523, 15535, 15634, 15648, 15500, 15596, 15714, 15603, 15523, 15535, 15634, 15678, };static struct simple_udp_connection unicast_connection;
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