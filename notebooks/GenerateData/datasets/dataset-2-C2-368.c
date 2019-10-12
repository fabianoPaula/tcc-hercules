#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {9549, 9549, 9454, 9549, 9454, 10237, 9549, 9454, 10237, 7515, 9549, 9454, 10237, 7515, 8766, 9549, 9454, 10237, 7515, 8766, 8001, 9549, 9454, 10237, 7515, 8766, 8001, 9914, 9549, 9454, 10237, 7515, 8766, 8001, 9914, 9983, 9549, 9454, 10237, 7515, 8766, 8001, 9914, 9983, 9838, 9549, 9454, 10237, 7515, 8766, 8001, 9914, 9983, 9838, 7215, 9549, 9454, 10237, 7515, 8766, 8001, 9914, 9983, 9838, 7215, 7328, 11115, 11115, 11197, 11115, 11197, 7609, 11115, 11197, 7609, 12479, 11115, 11197, 7609, 12479, 11312, 11115, 11197, 7609, 12479, 11312, 8582, 11115, 11197, 7609, 12479, 11312, 8582, 10289, 11115, 11197, 7609, 12479, 11312, 8582, 10289, 11345, 11115, 11197, 7609, 12479, 11312, 8582, 10289, 11345, 10606, 11115, 11197, 7609, 12479, 11312, 8582, 10289, 11345, 10606, 7359, 9192, 9192, 8044, 9192, 8044, 7477, 9192, 8044, 7477, 7018, 9192, 8044, 7477, 7018, 10901, 9192, 8044, 7477, 7018, 10901, 7076, 9192, 8044, 7477, 7018, 10901, 7076, 7994, 9192, 8044, 7477, 7018, 10901, 7076, 7994, 9590, 9192, 8044, 7477, 7018, 10901, 7076, 7994, 9590, 8232, 9192, 8044, 7477, 7018, 10901, 7076, 7994, 9590, 8232, 11209, 7736, 7736, 8652, 7736, 8652, 8497, 7736, 8652, 8497, 8419, 7736, 8652, 8497, 8419, 9117, 7736, 8652, 8497, 8419, 9117, 7525, 7736, 8652, 8497, 8419, 9117, 7525, 9845, 7736, 8652, 8497, 8419, 9117, 7525, 9845, 5948, 7736, 8652, 8497, 8419, 9117, 7525, 9845, 5948, 12054, 7736, 8652, 8497, 8419, 9117, 7525, 9845, 5948, 12054, 10138, 5948, 5948, 5424, 5948, 5424, 9535, 5948, 5424, 9535, 8521, 5948, 5424, 9535, 8521, 12003, 5948, 5424, 9535, 8521, 12003, 9593, 5948, 5424, 9535, 8521, 12003, 9593, 11015, 5948, 5424, 9535, 8521, 12003, 9593, 11015, 8622, 5948, 5424, 9535, 8521, 12003, 9593, 11015, 8622, 9924, 5948, 5424, 9535, 8521, 12003, 9593, 11015, 8622, 9924, 7794, 9455, 9455, 13126, 9455, 13126, 7870, 9455, 13126, 7870, 9002, 9455, 13126, 7870, 9002, 8683, 9455, 13126, 7870, 9002, 8683, 10422, 9455, 13126, 7870, 9002, 8683, 10422, 9164, 9455, 13126, 7870, 9002, 8683, 10422, 9164, 9396, 9455, 13126, 7870, 9002, 8683, 10422, 9164, 9396, 7774, 9455, 13126, 7870, 9002, 8683, 10422, 9164, 9396, 7774, 6995, 9709, 9709, 11015, 9709, 11015, 6260, 9709, 11015, 6260, 11661, 9709, 11015, 6260, 11661, 8587, 9709, 11015, 6260, 11661, 8587, 7364, 9709, 11015, 6260, 11661, 8587, 7364, 10560, 9709, 11015, 6260, 11661, 8587, 7364, 10560, 9418, 9709, 11015, 6260, 11661, 8587, 7364, 10560, 9418, 8405, 9709, 11015, 6260, 11661, 8587, 7364, 10560, 9418, 8405, 9919, 10661, 10661, 9079, 10661, 9079, 8680, 10661, 9079, 8680, 8057, 10661, 9079, 8680, 8057, 8821, 10661, 9079, 8680, 8057, 8821, 11615, 10661, 9079, 8680, 8057, 8821, 11615, 9083, 10661, 9079, 8680, 8057, 8821, 11615, 9083, 12970, 10661, 9079, 8680, 8057, 8821, 11615, 9083, 12970, 8994, 10661, 9079, 8680, 8057, 8821, 11615, 9083, 12970, 8994, 10838, 11642, 11642, 7613, 11642, 7613, 9872, 11642, 7613, 9872, 9127, 11642, 7613, 9872, 9127, 10751, 11642, 7613, 9872, 9127, 10751, 9845, 11642, 7613, 9872, 9127, 10751, 9845, 8818, 11642, 7613, 9872, 9127, 10751, 9845, 8818, 9949, 11642, 7613, 9872, 9127, 10751, 9845, 8818, 9949, 8199, 11642, 7613, 9872, 9127, 10751, 9845, 8818, 9949, 8199, 11191, 10655, 10655, 10851, 10655, 10851, 7472, 10655, 10851, 7472, 8104, 10655, 10851, 7472, 8104, 9204, 10655, 10851, 7472, 8104, 9204, 9153, 10655, 10851, 7472, 8104, 9204, 9153, 9858, 10655, 10851, 7472, 8104, 9204, 9153, 9858, 10509, 10655, 10851, 7472, 8104, 9204, 9153, 9858, 10509, 5024, };static struct simple_udp_connection unicast_connection;
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