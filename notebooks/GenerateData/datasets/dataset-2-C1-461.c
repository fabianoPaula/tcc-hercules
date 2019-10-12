#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {9893, 9893, 8971, 9893, 8971, 9261, 9893, 8971, 9261, 5914, 9893, 8971, 9261, 5914, 8059, 9893, 8971, 9261, 5914, 8059, 8204, 9893, 8971, 9261, 5914, 8059, 8204, 8786, 9893, 8971, 9261, 5914, 8059, 8204, 8786, 8378, 9893, 8971, 9261, 5914, 8059, 8204, 8786, 8378, 7326, 9893, 8971, 9261, 5914, 8059, 8204, 8786, 8378, 7326, 9296, 9893, 8971, 9261, 5914, 8059, 8204, 8786, 8378, 7326, 9296, 9736, 9508, 9508, 8032, 9508, 8032, 10880, 9508, 8032, 10880, 5880, 9508, 8032, 10880, 5880, 9102, 9508, 8032, 10880, 5880, 9102, 7004, 9508, 8032, 10880, 5880, 9102, 7004, 11273, 9508, 8032, 10880, 5880, 9102, 7004, 11273, 11776, 9508, 8032, 10880, 5880, 9102, 7004, 11273, 11776, 6781, 9508, 8032, 10880, 5880, 9102, 7004, 11273, 11776, 6781, 8841, 6826, 6826, 9330, 6826, 9330, 10727, 6826, 9330, 10727, 8822, 6826, 9330, 10727, 8822, 11626, 6826, 9330, 10727, 8822, 11626, 12014, 6826, 9330, 10727, 8822, 11626, 12014, 7826, 6826, 9330, 10727, 8822, 11626, 12014, 7826, 8754, 6826, 9330, 10727, 8822, 11626, 12014, 7826, 8754, 7500, 6826, 9330, 10727, 8822, 11626, 12014, 7826, 8754, 7500, 9737, 10830, 10830, 10076, 10830, 10076, 10856, 10830, 10076, 10856, 9635, 10830, 10076, 10856, 9635, 5856, 10830, 10076, 10856, 9635, 5856, 10658, 10830, 10076, 10856, 9635, 5856, 10658, 8423, 10830, 10076, 10856, 9635, 5856, 10658, 8423, 7900, 10830, 10076, 10856, 9635, 5856, 10658, 8423, 7900, 6733, 10830, 10076, 10856, 9635, 5856, 10658, 8423, 7900, 6733, 10527, 9047, 9047, 8748, 9047, 8748, 13933, 9047, 8748, 13933, 9866, 9047, 8748, 13933, 9866, 9260, 9047, 8748, 13933, 9866, 9260, 13450, 9047, 8748, 13933, 9866, 9260, 13450, 10650, 9047, 8748, 13933, 9866, 9260, 13450, 10650, 8124, 9047, 8748, 13933, 9866, 9260, 13450, 10650, 8124, 6862, 9047, 8748, 13933, 9866, 9260, 13450, 10650, 8124, 6862, 8437, 8892, 8892, 7380, 8892, 7380, 8917, 8892, 7380, 8917, 10312, 8892, 7380, 8917, 10312, 8254, 8892, 7380, 8917, 10312, 8254, 10317, 8892, 7380, 8917, 10312, 8254, 10317, 8496, 8892, 7380, 8917, 10312, 8254, 10317, 8496, 6345, 8892, 7380, 8917, 10312, 8254, 10317, 8496, 6345, 7917, 8892, 7380, 8917, 10312, 8254, 10317, 8496, 6345, 7917, 10293, 8676, 8676, 6618, 8676, 6618, 8530, 8676, 6618, 8530, 9271, 8676, 6618, 8530, 9271, 12907, 8676, 6618, 8530, 9271, 12907, 7855, 8676, 6618, 8530, 9271, 12907, 7855, 12034, 8676, 6618, 8530, 9271, 12907, 7855, 12034, 9354, 8676, 6618, 8530, 9271, 12907, 7855, 12034, 9354, 10070, 8676, 6618, 8530, 9271, 12907, 7855, 12034, 9354, 10070, 10616, 8011, 8011, 7567, 8011, 7567, 9030, 8011, 7567, 9030, 9118, 8011, 7567, 9030, 9118, 8483, 8011, 7567, 9030, 9118, 8483, 9088, 8011, 7567, 9030, 9118, 8483, 9088, 9318, 8011, 7567, 9030, 9118, 8483, 9088, 9318, 11570, 8011, 7567, 9030, 9118, 8483, 9088, 9318, 11570, 8375, 8011, 7567, 9030, 9118, 8483, 9088, 9318, 11570, 8375, 11128, 7912, 7912, 8999, 7912, 8999, 7924, 7912, 8999, 7924, 7588, 7912, 8999, 7924, 7588, 7717, 7912, 8999, 7924, 7588, 7717, 8546, 7912, 8999, 7924, 7588, 7717, 8546, 8071, 7912, 8999, 7924, 7588, 7717, 8546, 8071, 10693, 7912, 8999, 7924, 7588, 7717, 8546, 8071, 10693, 9502, 7912, 8999, 7924, 7588, 7717, 8546, 8071, 10693, 9502, 8507, 13857, 13857, 5166, 13857, 5166, 9434, 13857, 5166, 9434, 11419, 13857, 5166, 9434, 11419, 7396, 13857, 5166, 9434, 11419, 7396, 12394, 13857, 5166, 9434, 11419, 7396, 12394, 6448, 13857, 5166, 9434, 11419, 7396, 12394, 6448, 9304, 13857, 5166, 9434, 11419, 7396, 12394, 6448, 9304, 7626, };static struct simple_udp_connection unicast_connection;
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