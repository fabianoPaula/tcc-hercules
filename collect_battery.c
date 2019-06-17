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
8197, 7034, 6975, 7296, 9514, 8067, 9367, 11602, 7835, 9241, 8428, 
10520, 12177, 9921, 10896, 9836, 10036, 10160, 9497, 9087, 7394, 
12479, 10586, 6487, 8930, 11043, 12443, 8447, 9112, 9906, 13763, 
8550, 9009, 9412, 11706, 4220, 11363, 12317, 9328, 10626, 13207, 
10541, 11533, 11758, 7845, 9181, 5722, 5442, 10050, 9750, 8422, 
12623, 7838, 10141, 6856, 8790, 10783, 10475, 8528, 10517, 8189, 
11211, 9298, 9964, 12216, 11012, 12961, 7508, 8319, 11727, 8335, 
7434, 11473, 10807, 8259, 10115, 8222, 9177, 10710, 6528, 7996, 
7729, 9265, 9541, 9078, 10423, 10005, 8923, 10791, 10290, 9704, 
10242, 7944, 9305, 7609, 8840, 7583, 4526, 11254, 11401, 8565, 
9611, 8325, 10502, 8616, 7783, 9135, 9488, 11949, 12207, 11566, 
11916, 9971, 9277, 9514, 9731, 9597, 5015, 7601, 6039, 8027, 
6249, 12126, 9975, 6416, 9463, 10969, 11211, 7669, 6290, 8324, 
10937, 8419, 11237, 7757, 7578, 8538, 10497, 7208, 8286, 7073, 
11823, 10276, 11351, 9999, 8817, 9415, 6075, 8965, 11152, 10795, 
8524, 6322, 5704, 9622, 10641, 10727, 9306, 10531, 9153, 8417, 
7675, 6717, 7153, 7546, 6591, 10164, 10270, 8508, 8679, 10085, 
9150, 11755, 8044, 9570, 8484, 10342, 11484, 10356, 8257, 8773, 
9911, 8398, 12638, 11496, 9467, 9672, 7560, 10902, 12793, 10235, 
8438, 9813, 8184, 9927, 11604, 5101, 9969, 8199, 10280, 
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
