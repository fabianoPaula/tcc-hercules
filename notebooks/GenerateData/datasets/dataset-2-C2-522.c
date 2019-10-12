#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {9897, 9897, 8782, 9897, 8782, 9887, 9897, 8782, 9887, 4971, 9897, 8782, 9887, 4971, 10023, 9897, 8782, 9887, 4971, 10023, 4995, 9897, 8782, 9887, 4971, 10023, 4995, 11054, 9897, 8782, 9887, 4971, 10023, 4995, 11054, 10845, 9897, 8782, 9887, 4971, 10023, 4995, 11054, 10845, 9290, 9897, 8782, 9887, 4971, 10023, 4995, 11054, 10845, 9290, 7731, 9897, 8782, 9887, 4971, 10023, 4995, 11054, 10845, 9290, 7731, 7147, 8369, 8369, 9813, 8369, 9813, 6145, 8369, 9813, 6145, 12099, 8369, 9813, 6145, 12099, 6284, 8369, 9813, 6145, 12099, 6284, 11502, 8369, 9813, 6145, 12099, 6284, 11502, 9538, 8369, 9813, 6145, 12099, 6284, 11502, 9538, 9541, 8369, 9813, 6145, 12099, 6284, 11502, 9538, 9541, 7941, 8369, 9813, 6145, 12099, 6284, 11502, 9538, 9541, 7941, 8663, 9931, 9931, 10368, 9931, 10368, 13140, 9931, 10368, 13140, 7416, 9931, 10368, 13140, 7416, 10260, 9931, 10368, 13140, 7416, 10260, 9116, 9931, 10368, 13140, 7416, 10260, 9116, 10597, 9931, 10368, 13140, 7416, 10260, 9116, 10597, 10029, 9931, 10368, 13140, 7416, 10260, 9116, 10597, 10029, 10819, 9931, 10368, 13140, 7416, 10260, 9116, 10597, 10029, 10819, 10171, 9046, 9046, 10611, 9046, 10611, 7587, 9046, 10611, 7587, 8280, 9046, 10611, 7587, 8280, 7603, 9046, 10611, 7587, 8280, 7603, 8921, 9046, 10611, 7587, 8280, 7603, 8921, 8018, 9046, 10611, 7587, 8280, 7603, 8921, 8018, 8976, 9046, 10611, 7587, 8280, 7603, 8921, 8018, 8976, 6533, 9046, 10611, 7587, 8280, 7603, 8921, 8018, 8976, 6533, 10616, 7295, 7295, 10108, 7295, 10108, 6014, 7295, 10108, 6014, 8476, 7295, 10108, 6014, 8476, 10189, 7295, 10108, 6014, 8476, 10189, 8215, 7295, 10108, 6014, 8476, 10189, 8215, 9063, 7295, 10108, 6014, 8476, 10189, 8215, 9063, 6265, 7295, 10108, 6014, 8476, 10189, 8215, 9063, 6265, 9689, 7295, 10108, 6014, 8476, 10189, 8215, 9063, 6265, 9689, 12848, 9501, 9501, 6672, 9501, 6672, 6043, 9501, 6672, 6043, 8176, 9501, 6672, 6043, 8176, 9868, 9501, 6672, 6043, 8176, 9868, 6377, 9501, 6672, 6043, 8176, 9868, 6377, 8641, 9501, 6672, 6043, 8176, 9868, 6377, 8641, 9098, 9501, 6672, 6043, 8176, 9868, 6377, 8641, 9098, 7890, 9501, 6672, 6043, 8176, 9868, 6377, 8641, 9098, 7890, 8856, 10621, 10621, 6261, 10621, 6261, 10751, 10621, 6261, 10751, 11175, 10621, 6261, 10751, 11175, 8717, 10621, 6261, 10751, 11175, 8717, 10722, 10621, 6261, 10751, 11175, 8717, 10722, 9574, 10621, 6261, 10751, 11175, 8717, 10722, 9574, 8822, 10621, 6261, 10751, 11175, 8717, 10722, 9574, 8822, 9574, 10621, 6261, 10751, 11175, 8717, 10722, 9574, 8822, 9574, 11341, 11517, 11517, 11833, 11517, 11833, 6052, 11517, 11833, 6052, 10588, 11517, 11833, 6052, 10588, 9115, 11517, 11833, 6052, 10588, 9115, 10455, 11517, 11833, 6052, 10588, 9115, 10455, 6145, 11517, 11833, 6052, 10588, 9115, 10455, 6145, 9412, 11517, 11833, 6052, 10588, 9115, 10455, 6145, 9412, 8238, 11517, 11833, 6052, 10588, 9115, 10455, 6145, 9412, 8238, 5382, 8573, 8573, 8562, 8573, 8562, 6831, 8573, 8562, 6831, 11076, 8573, 8562, 6831, 11076, 11025, 8573, 8562, 6831, 11076, 11025, 10978, 8573, 8562, 6831, 11076, 11025, 10978, 8418, 8573, 8562, 6831, 11076, 11025, 10978, 8418, 10112, 8573, 8562, 6831, 11076, 11025, 10978, 8418, 10112, 9231, 8573, 8562, 6831, 11076, 11025, 10978, 8418, 10112, 9231, 10051, 9223, 9223, 7383, 9223, 7383, 6610, 9223, 7383, 6610, 6993, 9223, 7383, 6610, 6993, 13724, 9223, 7383, 6610, 6993, 13724, 10764, 9223, 7383, 6610, 6993, 13724, 10764, 9194, 9223, 7383, 6610, 6993, 13724, 10764, 9194, 6207, 9223, 7383, 6610, 6993, 13724, 10764, 9194, 6207, 7940, };static struct simple_udp_connection unicast_connection;
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