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
8343, 6441, 8333, 9942, 8166, 12039, 10917, 11733, 4588, 7882, 10498, 
8222, 8506, 7160, 10260, 7233, 10473, 8545, 8275, 9563, 12035, 
9853, 9462, 9408, 9743, 10626, 9831, 7358, 6971, 8776, 9576, 
6272, 10197, 8354, 10731, 8639, 10524, 9923, 7363, 8250, 10644, 
10503, 11756, 6157, 11017, 9494, 8407, 9896, 10463, 8196, 12302, 
9751, 10577, 12416, 7339, 10656, 8837, 8351, 11051, 6997, 6004, 
9675, 10244, 10096, 10288, 11187, 11644, 10194, 7992, 8546, 8321, 
9247, 10648, 8138, 9959, 8106, 7221, 10649, 10484, 7840, 8410, 
6090, 9900, 9378, 9769, 11543, 13730, 7555, 9067, 12395, 8247, 
9979, 12904, 5865, 7596, 7735, 10832, 9821, 12584, 8965, 8010, 
8110, 12396, 9430, 8238, 9039, 11496, 7289, 8830, 9886, 8799, 
10953, 9502, 7878, 10441, 10043, 9361, 7862, 9089, 10337, 6912, 
10039, 9166, 11492, 8733, 11257, 10402, 6370, 13282, 12357, 11387, 
9906, 11912, 9589, 8349, 5626, 12758, 12118, 10217, 8177, 9396, 
9746, 9009, 9906, 8367, 9597, 7856, 10287, 8370, 9818, 7831, 
8898, 11521, 9147, 10033, 8287, 12533, 7117, 10993, 6322, 9490, 
11593, 7308, 8705, 10814, 11671, 8581, 8432, 7346, 10219, 8456, 
10195, 7940, 10854, 7265, 11301, 9454, 10404, 8084, 8522, 8391, 
10134, 8843, 9079, 9150, 8202, 6717, 11960, 10547, 8224, 8809, 
10755, 9373, 9925, 9330, 7115, 9336, 6548, 13181, 9371, 
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
