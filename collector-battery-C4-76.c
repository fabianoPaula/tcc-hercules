#include "contiki.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "sys/node-id.h"
#include "simple-udp.h"
#include "servreg-hack.h"
#include <stdio.h>
#include <string.h>
#define UDP_PORT 1234
#define SERVICE_ID 190
#define SEND_INTERVAL    (10 * CLOCK_SECOND)
#define SEND_TIME    (random_rand() % (SEND_INTERVAL))
#define DATA_SIZE 100

int samples[100] = {
15688, 15388, 15288, 15385, 15521, 15465, 15464, 15538, 15256, 15790, 15566, 
15432, 15553, 15493, 15584, 15824, 15493, 15804, 15557, 15634, 15545, 
15552, 15481, 15674, 15597, 15448, 15537, 15862, 15516, 15335, 15511, 
15471, 15762, 15589, 15159, 15506, 15378, 15366, 15546, 15864, 15303, 
15296, 15769, 15706, 15880, 15564, 15555, 15491, 15713, 15380, 15483, 
15388, 15367, 15250, 15446, 15323, 15482, 15298, 15660, 15404, 15463, 
15426, 15237, 15504, 15449, 15406, 15585, 15392, 15648, 15879, 15432, 
15335, 15354, 15622, 15342, 15441, 15496, 15464, 15585, 15652, 15459, 
15415, 15425, 15535, 15653, 15499, 15383, 15490, 15369, 15366, 15824, 
15275, 15696, 15651, 15396, 15166, 15642, 15417, 15700, 15817, 
};

static struct simple_udp_connection unicast_connection;
PROCESS(unicast_sender_process, "Unicast sender example process");
AUTOSTART_PROCESSES(&unicast_sender_process);

static void
receiver(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  printf("Data received on port %d from port %d with length %d\n",
         receiver_port, sender_port, datalen);
}

static void
set_global_address(void)
{
  uip_ipaddr_t ipaddr;
  int i;
  uint8_t state;
  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
  printf("IPv6 addresses: ");  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(uip_ds6_if.addr_list[i].isused &&
       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
      uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr);
      printf("\n");
    }
  }
}

PROCESS_THREAD(unicast_sender_process, ev, data)
{
  static struct etimer periodic_timer;
  static struct etimer send_timer;
  uip_ipaddr_t *addr;
  PROCESS_BEGIN();
  servreg_hack_init();
  set_global_address();
  simple_udp_register(&unicast_connection, UDP_PORT,
                      NULL, UDP_PORT, receiver);
  etimer_set(&periodic_timer, SEND_INTERVAL);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_reset(&periodic_timer);
    etimer_set(&send_timer, SEND_TIME);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));
    addr = servreg_hack_lookup(SERVICE_ID);
    if(addr != NULL) {
      char buf[20];

      sprintf(buf, "c:1:%d", samples[random_rand() % DATA_SIZE]);
      simple_udp_sendto(&unicast_connection, buf, strlen(buf) + 1, addr);
          } else {
      printf("Service %d not found\n", SERVICE_ID);
    }
  }
  PROCESS_END();
}
