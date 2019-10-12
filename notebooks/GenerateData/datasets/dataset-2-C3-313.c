#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15537, 15537, 15252, 15537, 15252, 15565, 15537, 15252, 15565, 15699, 15537, 15252, 15565, 15699, 15517, 15537, 15252, 15565, 15699, 15517, 15628, 15537, 15252, 15565, 15699, 15517, 15628, 15470, 15537, 15252, 15565, 15699, 15517, 15628, 15470, 15533, 15537, 15252, 15565, 15699, 15517, 15628, 15470, 15533, 15785, 15537, 15252, 15565, 15699, 15517, 15628, 15470, 15533, 15785, 14977, 15537, 15252, 15565, 15699, 15517, 15628, 15470, 15533, 15785, 14977, 15787, 15435, 15435, 15171, 15435, 15171, 15650, 15435, 15171, 15650, 15358, 15435, 15171, 15650, 15358, 15509, 15435, 15171, 15650, 15358, 15509, 15598, 15435, 15171, 15650, 15358, 15509, 15598, 15790, 15435, 15171, 15650, 15358, 15509, 15598, 15790, 15054, 15435, 15171, 15650, 15358, 15509, 15598, 15790, 15054, 15337, 15435, 15171, 15650, 15358, 15509, 15598, 15790, 15054, 15337, 15324, 15386, 15386, 15227, 15386, 15227, 15622, 15386, 15227, 15622, 15007, 15386, 15227, 15622, 15007, 15632, 15386, 15227, 15622, 15007, 15632, 15310, 15386, 15227, 15622, 15007, 15632, 15310, 15935, 15386, 15227, 15622, 15007, 15632, 15310, 15935, 15668, 15386, 15227, 15622, 15007, 15632, 15310, 15935, 15668, 15587, 15386, 15227, 15622, 15007, 15632, 15310, 15935, 15668, 15587, 15468, 15546, 15546, 15338, 15546, 15338, 15095, 15546, 15338, 15095, 15030, 15546, 15338, 15095, 15030, 15482, 15546, 15338, 15095, 15030, 15482, 15569, 15546, 15338, 15095, 15030, 15482, 15569, 15472, 15546, 15338, 15095, 15030, 15482, 15569, 15472, 15325, 15546, 15338, 15095, 15030, 15482, 15569, 15472, 15325, 15356, 15546, 15338, 15095, 15030, 15482, 15569, 15472, 15325, 15356, 15391, 15661, 15661, 15115, 15661, 15115, 15551, 15661, 15115, 15551, 15538, 15661, 15115, 15551, 15538, 15788, 15661, 15115, 15551, 15538, 15788, 15606, 15661, 15115, 15551, 15538, 15788, 15606, 15380, 15661, 15115, 15551, 15538, 15788, 15606, 15380, 15553, 15661, 15115, 15551, 15538, 15788, 15606, 15380, 15553, 15425, 15661, 15115, 15551, 15538, 15788, 15606, 15380, 15553, 15425, 15572, 15821, 15821, 15396, 15821, 15396, 15486, 15821, 15396, 15486, 15524, 15821, 15396, 15486, 15524, 15782, 15821, 15396, 15486, 15524, 15782, 15457, 15821, 15396, 15486, 15524, 15782, 15457, 15335, 15821, 15396, 15486, 15524, 15782, 15457, 15335, 15436, 15821, 15396, 15486, 15524, 15782, 15457, 15335, 15436, 15487, 15821, 15396, 15486, 15524, 15782, 15457, 15335, 15436, 15487, 15727, 15572, 15572, 15233, 15572, 15233, 15835, 15572, 15233, 15835, 15802, 15572, 15233, 15835, 15802, 15412, 15572, 15233, 15835, 15802, 15412, 15515, 15572, 15233, 15835, 15802, 15412, 15515, 15706, 15572, 15233, 15835, 15802, 15412, 15515, 15706, 15754, 15572, 15233, 15835, 15802, 15412, 15515, 15706, 15754, 15672, 15572, 15233, 15835, 15802, 15412, 15515, 15706, 15754, 15672, 15640, 15384, 15384, 15651, 15384, 15651, 15595, 15384, 15651, 15595, 15018, 15384, 15651, 15595, 15018, 15456, 15384, 15651, 15595, 15018, 15456, 15374, 15384, 15651, 15595, 15018, 15456, 15374, 15295, 15384, 15651, 15595, 15018, 15456, 15374, 15295, 15627, 15384, 15651, 15595, 15018, 15456, 15374, 15295, 15627, 15535, 15384, 15651, 15595, 15018, 15456, 15374, 15295, 15627, 15535, 15787, 15504, 15504, 15537, 15504, 15537, 15659, 15504, 15537, 15659, 15554, 15504, 15537, 15659, 15554, 15821, 15504, 15537, 15659, 15554, 15821, 15768, 15504, 15537, 15659, 15554, 15821, 15768, 15669, 15504, 15537, 15659, 15554, 15821, 15768, 15669, 15581, 15504, 15537, 15659, 15554, 15821, 15768, 15669, 15581, 15438, 15504, 15537, 15659, 15554, 15821, 15768, 15669, 15581, 15438, 15475, 15145, 15145, 15611, 15145, 15611, 15680, 15145, 15611, 15680, 15304, 15145, 15611, 15680, 15304, 15593, 15145, 15611, 15680, 15304, 15593, 15843, 15145, 15611, 15680, 15304, 15593, 15843, 15283, 15145, 15611, 15680, 15304, 15593, 15843, 15283, 15538, 15145, 15611, 15680, 15304, 15593, 15843, 15283, 15538, 15364, };static struct simple_udp_connection unicast_connection;
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