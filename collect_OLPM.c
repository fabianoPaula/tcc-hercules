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

#define SEND_INTERVAL		(60 * CLOCK_SECOND)
#define SEND_TIME		(random_rand() % (SEND_INTERVAL))
#define DATA_SIZE 200

int[200] data = {
4598, 5310, 4949, 4944, 5241, 5098, 4515, 4682, 5588, 4648, 5026, 
4623, 4996, 4854, 4657, 4430, 5572, 5345, 4795, 4516, 4208, 
4736, 5277, 5220, 5961, 4580, 5464, 5075, 5134, 5657, 5502, 
4667, 4839, 5315, 4401, 4853, 4646, 5415, 5515, 5122, 5183, 
4604, 4400, 5742, 5793, 4950, 3930, 5347, 4957, 5257, 5135, 
4408, 5242, 4436, 5670, 4699, 4648, 4977, 5537, 5254, 5304, 
5172, 5160, 5585, 4677, 5238, 4679, 4984, 4704, 5218, 4387, 
5437, 4523, 4286, 5124, 5458, 4424, 5349, 4713, 5223, 4556, 
5614, 5602, 5432, 4948, 5291, 4702, 5482, 4894, 5050, 4411, 
5264, 5137, 5199, 4921, 4664, 4907, 5365, 5135, 5128, 5362, 
5449, 5030, 4809, 5600, 4882, 5196, 5122, 4584, 4307, 4275, 
4561, 5205, 5456, 5688, 4289, 4874, 5251, 4914, 4843, 4644, 
5626, 5030, 5168, 4700, 4054, 4740, 4726, 4906, 4851, 5506, 
5593, 5470, 5369, 5418, 5550, 5174, 4407, 4949, 4776, 5071, 
5230, 4753, 4851, 4322, 4966, 4745, 5335, 4911, 4450, 5636, 
5105, 5058, 5288, 5056, 4708, 5607, 4238, 5250, 5052, 5321, 
5211, 5429, 4982, 4887, 4816, 4989, 5211, 4807, 5078, 4489, 
5619, 5072, 4502, 4550, 5024, 5086, 4756, 4809, 4513, 4769, 
5262, 5043, 5195, 4756, 4416, 4516, 5648, 4823, 4961, 5233, 
5488, 4539, 5170, 4890, 5827, 5704, 4932, 5162, 4812, 
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
  printf("Data received on port %d from port %d with length %d
",
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

  printf("IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(uip_ds6_if.addr_list[i].isused &&
       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
      uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr);
      printf("
");
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
      static unsigned int message_number;
      char buf[20];

      printf("Sending unicast to ");
      uip_debug_ipaddr_print(addr);
      message_number = random_rand();
      sprintf(buf, "Message %d", message_number);
      simple_udp_sendto(&unicast_connection, buf, strlen(buf) + 1, addr);
    } else {
      printf("Service %d not found\n", SERVICE_ID);
    }
  }

  PROCESS_END();
}
