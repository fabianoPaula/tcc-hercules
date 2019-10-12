#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8623, 8623, 11162, 8623, 11162, 11951, 8623, 11162, 11951, 11677, 8623, 11162, 11951, 11677, 8648, 8623, 11162, 11951, 11677, 8648, 8611, 8623, 11162, 11951, 11677, 8648, 8611, 6612, 8623, 11162, 11951, 11677, 8648, 8611, 6612, 7886, 8623, 11162, 11951, 11677, 8648, 8611, 6612, 7886, 6689, 8623, 11162, 11951, 11677, 8648, 8611, 6612, 7886, 6689, 10768, 8623, 11162, 11951, 11677, 8648, 8611, 6612, 7886, 6689, 10768, 6393, 10862, 10862, 9326, 10862, 9326, 11996, 10862, 9326, 11996, 6684, 10862, 9326, 11996, 6684, 8970, 10862, 9326, 11996, 6684, 8970, 8597, 10862, 9326, 11996, 6684, 8970, 8597, 9153, 10862, 9326, 11996, 6684, 8970, 8597, 9153, 8097, 10862, 9326, 11996, 6684, 8970, 8597, 9153, 8097, 9823, 10862, 9326, 11996, 6684, 8970, 8597, 9153, 8097, 9823, 8389, 8393, 8393, 7568, 8393, 7568, 8762, 8393, 7568, 8762, 9020, 8393, 7568, 8762, 9020, 8996, 8393, 7568, 8762, 9020, 8996, 8017, 8393, 7568, 8762, 9020, 8996, 8017, 11841, 8393, 7568, 8762, 9020, 8996, 8017, 11841, 7693, 8393, 7568, 8762, 9020, 8996, 8017, 11841, 7693, 8310, 8393, 7568, 8762, 9020, 8996, 8017, 11841, 7693, 8310, 12075, 9040, 9040, 6016, 9040, 6016, 8463, 9040, 6016, 8463, 4637, 9040, 6016, 8463, 4637, 9082, 9040, 6016, 8463, 4637, 9082, 9423, 9040, 6016, 8463, 4637, 9082, 9423, 7659, 9040, 6016, 8463, 4637, 9082, 9423, 7659, 8304, 9040, 6016, 8463, 4637, 9082, 9423, 7659, 8304, 8151, 9040, 6016, 8463, 4637, 9082, 9423, 7659, 8304, 8151, 7930, 11376, 11376, 11407, 11376, 11407, 9774, 11376, 11407, 9774, 6455, 11376, 11407, 9774, 6455, 6129, 11376, 11407, 9774, 6455, 6129, 8433, 11376, 11407, 9774, 6455, 6129, 8433, 11440, 11376, 11407, 9774, 6455, 6129, 8433, 11440, 10089, 11376, 11407, 9774, 6455, 6129, 8433, 11440, 10089, 12570, 11376, 11407, 9774, 6455, 6129, 8433, 11440, 10089, 12570, 9084, 11725, 11725, 7631, 11725, 7631, 7766, 11725, 7631, 7766, 9113, 11725, 7631, 7766, 9113, 8158, 11725, 7631, 7766, 9113, 8158, 10672, 11725, 7631, 7766, 9113, 8158, 10672, 8435, 11725, 7631, 7766, 9113, 8158, 10672, 8435, 7575, 11725, 7631, 7766, 9113, 8158, 10672, 8435, 7575, 10045, 11725, 7631, 7766, 9113, 8158, 10672, 8435, 7575, 10045, 11207, 10828, 10828, 8290, 10828, 8290, 7540, 10828, 8290, 7540, 10086, 10828, 8290, 7540, 10086, 6116, 10828, 8290, 7540, 10086, 6116, 8812, 10828, 8290, 7540, 10086, 6116, 8812, 6807, 10828, 8290, 7540, 10086, 6116, 8812, 6807, 9293, 10828, 8290, 7540, 10086, 6116, 8812, 6807, 9293, 6253, 10828, 8290, 7540, 10086, 6116, 8812, 6807, 9293, 6253, 6495, 10197, 10197, 8483, 10197, 8483, 11538, 10197, 8483, 11538, 7482, 10197, 8483, 11538, 7482, 10568, 10197, 8483, 11538, 7482, 10568, 7001, 10197, 8483, 11538, 7482, 10568, 7001, 9349, 10197, 8483, 11538, 7482, 10568, 7001, 9349, 11107, 10197, 8483, 11538, 7482, 10568, 7001, 9349, 11107, 10655, 10197, 8483, 11538, 7482, 10568, 7001, 9349, 11107, 10655, 10890, 9938, 9938, 7723, 9938, 7723, 8046, 9938, 7723, 8046, 11517, 9938, 7723, 8046, 11517, 7841, 9938, 7723, 8046, 11517, 7841, 10998, 9938, 7723, 8046, 11517, 7841, 10998, 5637, 9938, 7723, 8046, 11517, 7841, 10998, 5637, 10051, 9938, 7723, 8046, 11517, 7841, 10998, 5637, 10051, 11192, 9938, 7723, 8046, 11517, 7841, 10998, 5637, 10051, 11192, 11098, 9972, 9972, 7462, 9972, 7462, 9449, 9972, 7462, 9449, 7970, 9972, 7462, 9449, 7970, 10489, 9972, 7462, 9449, 7970, 10489, 9342, 9972, 7462, 9449, 7970, 10489, 9342, 6901, 9972, 7462, 9449, 7970, 10489, 9342, 6901, 11443, 9972, 7462, 9449, 7970, 10489, 9342, 6901, 11443, 9547, };static struct simple_udp_connection unicast_connection;
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