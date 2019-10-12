#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {9589, 9589, 8477, 9589, 8477, 11696, 9589, 8477, 11696, 9278, 9589, 8477, 11696, 9278, 6422, 9589, 8477, 11696, 9278, 6422, 10317, 9589, 8477, 11696, 9278, 6422, 10317, 9818, 9589, 8477, 11696, 9278, 6422, 10317, 9818, 9449, 9589, 8477, 11696, 9278, 6422, 10317, 9818, 9449, 7746, 9589, 8477, 11696, 9278, 6422, 10317, 9818, 9449, 7746, 12048, 9589, 8477, 11696, 9278, 6422, 10317, 9818, 9449, 7746, 12048, 5484, 6615, 6615, 10228, 6615, 10228, 8911, 6615, 10228, 8911, 6132, 6615, 10228, 8911, 6132, 9927, 6615, 10228, 8911, 6132, 9927, 6315, 6615, 10228, 8911, 6132, 9927, 6315, 11299, 6615, 10228, 8911, 6132, 9927, 6315, 11299, 8759, 6615, 10228, 8911, 6132, 9927, 6315, 11299, 8759, 8999, 6615, 10228, 8911, 6132, 9927, 6315, 11299, 8759, 8999, 11911, 9124, 9124, 8697, 9124, 8697, 12725, 9124, 8697, 12725, 11738, 9124, 8697, 12725, 11738, 7672, 9124, 8697, 12725, 11738, 7672, 9543, 9124, 8697, 12725, 11738, 7672, 9543, 10848, 9124, 8697, 12725, 11738, 7672, 9543, 10848, 8638, 9124, 8697, 12725, 11738, 7672, 9543, 10848, 8638, 14658, 9124, 8697, 12725, 11738, 7672, 9543, 10848, 8638, 14658, 10097, 8575, 8575, 9422, 8575, 9422, 7448, 8575, 9422, 7448, 11059, 8575, 9422, 7448, 11059, 12150, 8575, 9422, 7448, 11059, 12150, 10355, 8575, 9422, 7448, 11059, 12150, 10355, 9680, 8575, 9422, 7448, 11059, 12150, 10355, 9680, 8257, 8575, 9422, 7448, 11059, 12150, 10355, 9680, 8257, 9077, 8575, 9422, 7448, 11059, 12150, 10355, 9680, 8257, 9077, 8008, 11029, 11029, 10117, 11029, 10117, 8374, 11029, 10117, 8374, 10794, 11029, 10117, 8374, 10794, 7982, 11029, 10117, 8374, 10794, 7982, 6342, 11029, 10117, 8374, 10794, 7982, 6342, 7822, 11029, 10117, 8374, 10794, 7982, 6342, 7822, 10297, 11029, 10117, 8374, 10794, 7982, 6342, 7822, 10297, 9920, 11029, 10117, 8374, 10794, 7982, 6342, 7822, 10297, 9920, 8894, 10570, 10570, 11216, 10570, 11216, 11388, 10570, 11216, 11388, 9296, 10570, 11216, 11388, 9296, 6767, 10570, 11216, 11388, 9296, 6767, 9527, 10570, 11216, 11388, 9296, 6767, 9527, 8236, 10570, 11216, 11388, 9296, 6767, 9527, 8236, 7940, 10570, 11216, 11388, 9296, 6767, 9527, 8236, 7940, 6352, 10570, 11216, 11388, 9296, 6767, 9527, 8236, 7940, 6352, 8180, 8588, 8588, 5457, 8588, 5457, 7152, 8588, 5457, 7152, 8031, 8588, 5457, 7152, 8031, 8877, 8588, 5457, 7152, 8031, 8877, 12676, 8588, 5457, 7152, 8031, 8877, 12676, 9834, 8588, 5457, 7152, 8031, 8877, 12676, 9834, 10063, 8588, 5457, 7152, 8031, 8877, 12676, 9834, 10063, 9147, 8588, 5457, 7152, 8031, 8877, 12676, 9834, 10063, 9147, 7269, 11682, 11682, 8705, 11682, 8705, 11224, 11682, 8705, 11224, 9031, 11682, 8705, 11224, 9031, 10436, 11682, 8705, 11224, 9031, 10436, 7194, 11682, 8705, 11224, 9031, 10436, 7194, 8822, 11682, 8705, 11224, 9031, 10436, 7194, 8822, 8504, 11682, 8705, 11224, 9031, 10436, 7194, 8822, 8504, 12408, 11682, 8705, 11224, 9031, 10436, 7194, 8822, 8504, 12408, 11071, 8713, 8713, 13573, 8713, 13573, 8904, 8713, 13573, 8904, 10019, 8713, 13573, 8904, 10019, 8830, 8713, 13573, 8904, 10019, 8830, 11825, 8713, 13573, 8904, 10019, 8830, 11825, 8581, 8713, 13573, 8904, 10019, 8830, 11825, 8581, 10148, 8713, 13573, 8904, 10019, 8830, 11825, 8581, 10148, 8851, 8713, 13573, 8904, 10019, 8830, 11825, 8581, 10148, 8851, 11597, 11400, 11400, 4505, 11400, 4505, 10881, 11400, 4505, 10881, 6688, 11400, 4505, 10881, 6688, 9404, 11400, 4505, 10881, 6688, 9404, 9686, 11400, 4505, 10881, 6688, 9404, 9686, 7864, 11400, 4505, 10881, 6688, 9404, 9686, 7864, 6740, 11400, 4505, 10881, 6688, 9404, 9686, 7864, 6740, 8881, };static struct simple_udp_connection unicast_connection;
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