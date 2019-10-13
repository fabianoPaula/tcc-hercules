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
4588, 4910, 5653, 4833, 4746, 4600, 5373, 5054, 5228, 4933, 5429, 
4708, 5173, 5079, 4934, 4591, 4121, 4153, 6315, 4851, 4572, 
4844, 5056, 4982, 5340, 5155, 5153, 4569, 5518, 4641, 5071, 
5498, 4427, 5095, 5577, 4677, 4313, 4584, 5147, 4060, 5366, 
5133, 4205, 4813, 5059, 4611, 5099, 5109, 4651, 4857, 4463, 
5335, 5187, 5418, 4551, 4859, 4534, 4593, 5028, 4648, 5173, 
4891, 5207, 5199, 4640, 4989, 4765, 5087, 5506, 5421, 4734, 
5500, 5105, 5655, 4619, 5062, 4897, 4015, 4844, 5733, 5410, 
4636, 5120, 5099, 4986, 4532, 5182, 5307, 5149, 4570, 4746, 
5217, 4702, 5257, 5912, 4965, 5093, 5629, 5345, 5251, 
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
