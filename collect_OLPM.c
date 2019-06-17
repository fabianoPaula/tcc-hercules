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
4368, 4807, 5242, 4252, 5580, 5137, 4914, 4722, 5259, 4688, 5402, 
4628, 5478, 5084, 4920, 4558, 4722, 4745, 4228, 4985, 4460, 
4964, 5129, 5220, 4490, 4660, 5532, 4674, 4851, 5000, 4718, 
5187, 5189, 5853, 4740, 5236, 5331, 5015, 4793, 5464, 5816, 
5271, 5719, 5124, 4719, 4776, 4724, 5008, 5058, 5627, 4425, 
5301, 5719, 5479, 5447, 5397, 4665, 4552, 5443, 4911, 4467, 
5133, 4939, 5869, 5496, 4598, 4707, 5296, 4923, 5438, 4871, 
4581, 4696, 5265, 5036, 5227, 5235, 4428, 5138, 5204, 4840, 
5079, 4561, 5739, 5262, 5200, 5178, 5269, 4595, 5622, 4528, 
5695, 5579, 5314, 5629, 5186, 4452, 5050, 5638, 4158, 4727, 
5687, 4893, 5382, 6219, 5080, 5200, 5653, 5212, 5387, 5565, 
5015, 4950, 5547, 4513, 5076, 5505, 4657, 5046, 5077, 5530, 
5112, 5143, 4935, 4533, 5593, 4705, 4964, 4473, 4960, 4395, 
5036, 4491, 5366, 5421, 5452, 4661, 5046, 4598, 4918, 5453, 
5501, 4441, 4975, 5149, 4959, 5026, 4284, 5554, 5096, 4925, 
5346, 4330, 4487, 5483, 4986, 5032, 5591, 4869, 4832, 4862, 
5103, 5137, 4832, 5126, 5389, 4757, 5049, 4656, 4428, 4915, 
4869, 3931, 5995, 4948, 5340, 5028, 4794, 5487, 5529, 5206, 
5194, 5546, 4501, 4557, 5301, 4450, 4961, 4393, 5010, 4650, 
4667, 4879, 4892, 5268, 4551, 5077, 5599, 4522, 5668, 
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
      printf("
");
      message_number = random_rand();
      sprintf(buf, "Message %d", message_number);
      simple_udp_sendto(&unicast_connection, buf, strlen(buf) + 1, addr);
    } else {
      printf("Service %d not found
", SERVICE_ID);
    }
  }

  PROCESS_END();
}
