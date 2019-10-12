#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {5146, 5146, 5277, 5146, 5277, 4742, 5146, 5277, 4742, 4287, 5146, 5277, 4742, 4287, 5570, 5146, 5277, 4742, 4287, 5570, 4869, 5146, 5277, 4742, 4287, 5570, 4869, 4369, 5146, 5277, 4742, 4287, 5570, 4869, 4369, 5362, 5146, 5277, 4742, 4287, 5570, 4869, 4369, 5362, 4868, 5146, 5277, 4742, 4287, 5570, 4869, 4369, 5362, 4868, 4880, 5146, 5277, 4742, 4287, 5570, 4869, 4369, 5362, 4868, 4880, 5297, 4464, 4464, 4813, 4464, 4813, 6133, 4464, 4813, 6133, 5825, 4464, 4813, 6133, 5825, 5074, 4464, 4813, 6133, 5825, 5074, 4960, 4464, 4813, 6133, 5825, 5074, 4960, 4378, 4464, 4813, 6133, 5825, 5074, 4960, 4378, 5345, 4464, 4813, 6133, 5825, 5074, 4960, 4378, 5345, 4571, 4464, 4813, 6133, 5825, 5074, 4960, 4378, 5345, 4571, 5374, 5479, 5479, 5308, 5479, 5308, 4602, 5479, 5308, 4602, 4682, 5479, 5308, 4602, 4682, 5852, 5479, 5308, 4602, 4682, 5852, 4994, 5479, 5308, 4602, 4682, 5852, 4994, 5353, 5479, 5308, 4602, 4682, 5852, 4994, 5353, 5672, 5479, 5308, 4602, 4682, 5852, 4994, 5353, 5672, 5356, 5479, 5308, 4602, 4682, 5852, 4994, 5353, 5672, 5356, 4971, 5600, 5600, 4774, 5600, 4774, 3902, 5600, 4774, 3902, 5015, 5600, 4774, 3902, 5015, 5719, 5600, 4774, 3902, 5015, 5719, 5052, 5600, 4774, 3902, 5015, 5719, 5052, 5226, 5600, 4774, 3902, 5015, 5719, 5052, 5226, 4668, 5600, 4774, 3902, 5015, 5719, 5052, 5226, 4668, 5269, 5600, 4774, 3902, 5015, 5719, 5052, 5226, 4668, 5269, 5311, 5002, 5002, 4904, 5002, 4904, 5061, 5002, 4904, 5061, 5038, 5002, 4904, 5061, 5038, 4780, 5002, 4904, 5061, 5038, 4780, 4538, 5002, 4904, 5061, 5038, 4780, 4538, 5088, 5002, 4904, 5061, 5038, 4780, 4538, 5088, 5012, 5002, 4904, 5061, 5038, 4780, 4538, 5088, 5012, 5029, 5002, 4904, 5061, 5038, 4780, 4538, 5088, 5012, 5029, 5258, 4885, 4885, 4638, 4885, 4638, 4896, 4885, 4638, 4896, 4856, 4885, 4638, 4896, 4856, 4600, 4885, 4638, 4896, 4856, 4600, 5065, 4885, 4638, 4896, 4856, 4600, 5065, 5240, 4885, 4638, 4896, 4856, 4600, 5065, 5240, 5637, 4885, 4638, 4896, 4856, 4600, 5065, 5240, 5637, 4712, 4885, 4638, 4896, 4856, 4600, 5065, 5240, 5637, 4712, 4937, 5940, 5940, 4965, 5940, 4965, 5037, 5940, 4965, 5037, 4260, 5940, 4965, 5037, 4260, 5228, 5940, 4965, 5037, 4260, 5228, 5678, 5940, 4965, 5037, 4260, 5228, 5678, 5038, 5940, 4965, 5037, 4260, 5228, 5678, 5038, 4984, 5940, 4965, 5037, 4260, 5228, 5678, 5038, 4984, 4907, 5940, 4965, 5037, 4260, 5228, 5678, 5038, 4984, 4907, 5456, 5111, 5111, 4234, 5111, 4234, 4981, 5111, 4234, 4981, 5290, 5111, 4234, 4981, 5290, 5084, 5111, 4234, 4981, 5290, 5084, 5119, 5111, 4234, 4981, 5290, 5084, 5119, 4514, 5111, 4234, 4981, 5290, 5084, 5119, 4514, 4605, 5111, 4234, 4981, 5290, 5084, 5119, 4514, 4605, 4866, 5111, 4234, 4981, 5290, 5084, 5119, 4514, 4605, 4866, 5446, 5359, 5359, 5071, 5359, 5071, 4635, 5359, 5071, 4635, 5109, 5359, 5071, 4635, 5109, 4897, 5359, 5071, 4635, 5109, 4897, 4356, 5359, 5071, 4635, 5109, 4897, 4356, 5203, 5359, 5071, 4635, 5109, 4897, 4356, 5203, 5037, 5359, 5071, 4635, 5109, 4897, 4356, 5203, 5037, 4651, 5359, 5071, 4635, 5109, 4897, 4356, 5203, 5037, 4651, 5505, 4392, 4392, 5762, 4392, 5762, 4987, 4392, 5762, 4987, 5271, 4392, 5762, 4987, 5271, 5626, 4392, 5762, 4987, 5271, 5626, 5376, 4392, 5762, 4987, 5271, 5626, 5376, 4877, 4392, 5762, 4987, 5271, 5626, 5376, 4877, 4806, 4392, 5762, 4987, 5271, 5626, 5376, 4877, 4806, 5870, };static struct simple_udp_connection unicast_connection;
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