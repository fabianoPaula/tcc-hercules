#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8123, 8123, 8492, 8123, 8492, 11616, 8123, 8492, 11616, 9084, 8123, 8492, 11616, 9084, 8854, 8123, 8492, 11616, 9084, 8854, 8220, 8123, 8492, 11616, 9084, 8854, 8220, 9626, 8123, 8492, 11616, 9084, 8854, 8220, 9626, 9597, 8123, 8492, 11616, 9084, 8854, 8220, 9626, 9597, 11131, 8123, 8492, 11616, 9084, 8854, 8220, 9626, 9597, 11131, 9776, 8123, 8492, 11616, 9084, 8854, 8220, 9626, 9597, 11131, 9776, 5914, 10506, 10506, 5477, 10506, 5477, 8383, 10506, 5477, 8383, 9162, 10506, 5477, 8383, 9162, 10022, 10506, 5477, 8383, 9162, 10022, 9241, 10506, 5477, 8383, 9162, 10022, 9241, 9114, 10506, 5477, 8383, 9162, 10022, 9241, 9114, 6879, 10506, 5477, 8383, 9162, 10022, 9241, 9114, 6879, 8135, 10506, 5477, 8383, 9162, 10022, 9241, 9114, 6879, 8135, 10773, 9620, 9620, 6809, 9620, 6809, 6250, 9620, 6809, 6250, 9944, 9620, 6809, 6250, 9944, 10452, 9620, 6809, 6250, 9944, 10452, 7305, 9620, 6809, 6250, 9944, 10452, 7305, 8119, 9620, 6809, 6250, 9944, 10452, 7305, 8119, 7816, 9620, 6809, 6250, 9944, 10452, 7305, 8119, 7816, 11319, 9620, 6809, 6250, 9944, 10452, 7305, 8119, 7816, 11319, 11366, 9828, 9828, 11838, 9828, 11838, 8982, 9828, 11838, 8982, 13047, 9828, 11838, 8982, 13047, 10051, 9828, 11838, 8982, 13047, 10051, 8770, 9828, 11838, 8982, 13047, 10051, 8770, 9464, 9828, 11838, 8982, 13047, 10051, 8770, 9464, 12077, 9828, 11838, 8982, 13047, 10051, 8770, 9464, 12077, 5223, 9828, 11838, 8982, 13047, 10051, 8770, 9464, 12077, 5223, 9043, 9652, 9652, 9730, 9652, 9730, 7583, 9652, 9730, 7583, 10608, 9652, 9730, 7583, 10608, 6148, 9652, 9730, 7583, 10608, 6148, 8954, 9652, 9730, 7583, 10608, 6148, 8954, 9304, 9652, 9730, 7583, 10608, 6148, 8954, 9304, 7540, 9652, 9730, 7583, 10608, 6148, 8954, 9304, 7540, 8564, 9652, 9730, 7583, 10608, 6148, 8954, 9304, 7540, 8564, 8116, 5943, 5943, 7895, 5943, 7895, 4889, 5943, 7895, 4889, 12461, 5943, 7895, 4889, 12461, 7229, 5943, 7895, 4889, 12461, 7229, 6942, 5943, 7895, 4889, 12461, 7229, 6942, 9152, 5943, 7895, 4889, 12461, 7229, 6942, 9152, 7209, 5943, 7895, 4889, 12461, 7229, 6942, 9152, 7209, 8134, 5943, 7895, 4889, 12461, 7229, 6942, 9152, 7209, 8134, 10720, 11347, 11347, 10408, 11347, 10408, 8638, 11347, 10408, 8638, 9933, 11347, 10408, 8638, 9933, 8565, 11347, 10408, 8638, 9933, 8565, 6972, 11347, 10408, 8638, 9933, 8565, 6972, 9158, 11347, 10408, 8638, 9933, 8565, 6972, 9158, 6434, 11347, 10408, 8638, 9933, 8565, 6972, 9158, 6434, 9902, 11347, 10408, 8638, 9933, 8565, 6972, 9158, 6434, 9902, 9794, 8208, 8208, 7484, 8208, 7484, 12769, 8208, 7484, 12769, 10458, 8208, 7484, 12769, 10458, 9134, 8208, 7484, 12769, 10458, 9134, 8608, 8208, 7484, 12769, 10458, 9134, 8608, 11293, 8208, 7484, 12769, 10458, 9134, 8608, 11293, 8855, 8208, 7484, 12769, 10458, 9134, 8608, 11293, 8855, 9485, 8208, 7484, 12769, 10458, 9134, 8608, 11293, 8855, 9485, 6090, 8830, 8830, 13733, 8830, 13733, 10926, 8830, 13733, 10926, 11198, 8830, 13733, 10926, 11198, 11652, 8830, 13733, 10926, 11198, 11652, 10666, 8830, 13733, 10926, 11198, 11652, 10666, 7861, 8830, 13733, 10926, 11198, 11652, 10666, 7861, 11060, 8830, 13733, 10926, 11198, 11652, 10666, 7861, 11060, 7969, 8830, 13733, 10926, 11198, 11652, 10666, 7861, 11060, 7969, 8219, 10502, 10502, 8075, 10502, 8075, 11100, 10502, 8075, 11100, 7811, 10502, 8075, 11100, 7811, 6695, 10502, 8075, 11100, 7811, 6695, 9992, 10502, 8075, 11100, 7811, 6695, 9992, 10086, 10502, 8075, 11100, 7811, 6695, 9992, 10086, 9174, 10502, 8075, 11100, 7811, 6695, 9992, 10086, 9174, 8314, };static struct simple_udp_connection unicast_connection;
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