#include "contiki.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "net/uip.h"
#include "net/uip-ds6.h"
#include "net/uip-debug.h"
#include "sys/node-id.h"
#include "simple-udp.h"
#include "servreg-hack.h"
#include <stdio.h>
#include <string.h>
#define UDP_PORT 1234
#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)
#define SEND_TIME    (random_rand() % (SEND_INTERVAL))
#define DATA_SIZE 100

int samples[100] = {
8539, 8111, 8642, 8632, 8454, 8554, 8494, 8148, 8341, 8624, 8531, 
8237, 8691, 8340, 8314, 8293, 8383, 9049, 8599, 8917, 8513, 
8554, 8306, 8708, 8791, 8616, 8627, 8542, 8560, 8662, 8706, 
8268, 8371, 8423, 8115, 8795, 8726, 8538, 8446, 8352, 8558, 
8582, 8422, 8449, 8616, 8355, 8510, 8227, 8509, 8332, 8441, 
8428, 8555, 8335, 8507, 8625, 8521, 8077, 8540, 8488, 8310, 
8273, 8458, 8948, 8188, 8379, 8476, 8722, 8415, 8867, 8464, 
8424, 8485, 8503, 8807, 8298, 8458, 8570, 8475, 8502, 8754, 
8372, 8309, 8293, 8659, 8238, 8515, 8189, 8643, 8573, 8461, 
8723, 8773, 8585, 8921, 8687, 8820, 8599, 8568, 8368, 
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
      printf("Sending unicast to ");
      uip_debug_ipaddr_print(addr);
      printf(": %s \n", buf);
    } else {
      printf("Service %d not found\n", SERVICE_ID);
    }
  }
  PROCESS_END();
}
