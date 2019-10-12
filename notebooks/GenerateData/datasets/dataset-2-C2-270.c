#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {10913, 10913, 10278, 10913, 10278, 8974, 10913, 10278, 8974, 8956, 10913, 10278, 8974, 8956, 7989, 10913, 10278, 8974, 8956, 7989, 7398, 10913, 10278, 8974, 8956, 7989, 7398, 6792, 10913, 10278, 8974, 8956, 7989, 7398, 6792, 7983, 10913, 10278, 8974, 8956, 7989, 7398, 6792, 7983, 7031, 10913, 10278, 8974, 8956, 7989, 7398, 6792, 7983, 7031, 5987, 10913, 10278, 8974, 8956, 7989, 7398, 6792, 7983, 7031, 5987, 9205, 5579, 5579, 10021, 5579, 10021, 10107, 5579, 10021, 10107, 7949, 5579, 10021, 10107, 7949, 8184, 5579, 10021, 10107, 7949, 8184, 10752, 5579, 10021, 10107, 7949, 8184, 10752, 9297, 5579, 10021, 10107, 7949, 8184, 10752, 9297, 7894, 5579, 10021, 10107, 7949, 8184, 10752, 9297, 7894, 11726, 5579, 10021, 10107, 7949, 8184, 10752, 9297, 7894, 11726, 9069, 9995, 9995, 8807, 9995, 8807, 5816, 9995, 8807, 5816, 5083, 9995, 8807, 5816, 5083, 10917, 9995, 8807, 5816, 5083, 10917, 9171, 9995, 8807, 5816, 5083, 10917, 9171, 7518, 9995, 8807, 5816, 5083, 10917, 9171, 7518, 9858, 9995, 8807, 5816, 5083, 10917, 9171, 7518, 9858, 11302, 9995, 8807, 5816, 5083, 10917, 9171, 7518, 9858, 11302, 8747, 8553, 8553, 8018, 8553, 8018, 7391, 8553, 8018, 7391, 8274, 8553, 8018, 7391, 8274, 9271, 8553, 8018, 7391, 8274, 9271, 7546, 8553, 8018, 7391, 8274, 9271, 7546, 8195, 8553, 8018, 7391, 8274, 9271, 7546, 8195, 9775, 8553, 8018, 7391, 8274, 9271, 7546, 8195, 9775, 8356, 8553, 8018, 7391, 8274, 9271, 7546, 8195, 9775, 8356, 9103, 10174, 10174, 8773, 10174, 8773, 10593, 10174, 8773, 10593, 8466, 10174, 8773, 10593, 8466, 8411, 10174, 8773, 10593, 8466, 8411, 9873, 10174, 8773, 10593, 8466, 8411, 9873, 9601, 10174, 8773, 10593, 8466, 8411, 9873, 9601, 8382, 10174, 8773, 10593, 8466, 8411, 9873, 9601, 8382, 10900, 10174, 8773, 10593, 8466, 8411, 9873, 9601, 8382, 10900, 10266, 10054, 10054, 8000, 10054, 8000, 11304, 10054, 8000, 11304, 8658, 10054, 8000, 11304, 8658, 9616, 10054, 8000, 11304, 8658, 9616, 10435, 10054, 8000, 11304, 8658, 9616, 10435, 6170, 10054, 8000, 11304, 8658, 9616, 10435, 6170, 7085, 10054, 8000, 11304, 8658, 9616, 10435, 6170, 7085, 9307, 10054, 8000, 11304, 8658, 9616, 10435, 6170, 7085, 9307, 9092, 8050, 8050, 7391, 8050, 7391, 10857, 8050, 7391, 10857, 11782, 8050, 7391, 10857, 11782, 9406, 8050, 7391, 10857, 11782, 9406, 6950, 8050, 7391, 10857, 11782, 9406, 6950, 9062, 8050, 7391, 10857, 11782, 9406, 6950, 9062, 7860, 8050, 7391, 10857, 11782, 9406, 6950, 9062, 7860, 8361, 8050, 7391, 10857, 11782, 9406, 6950, 9062, 7860, 8361, 11011, 10696, 10696, 9678, 10696, 9678, 10010, 10696, 9678, 10010, 7624, 10696, 9678, 10010, 7624, 8912, 10696, 9678, 10010, 7624, 8912, 9302, 10696, 9678, 10010, 7624, 8912, 9302, 8945, 10696, 9678, 10010, 7624, 8912, 9302, 8945, 12104, 10696, 9678, 10010, 7624, 8912, 9302, 8945, 12104, 7392, 10696, 9678, 10010, 7624, 8912, 9302, 8945, 12104, 7392, 8081, 8851, 8851, 13324, 8851, 13324, 9122, 8851, 13324, 9122, 7535, 8851, 13324, 9122, 7535, 8252, 8851, 13324, 9122, 7535, 8252, 7909, 8851, 13324, 9122, 7535, 8252, 7909, 12069, 8851, 13324, 9122, 7535, 8252, 7909, 12069, 4889, 8851, 13324, 9122, 7535, 8252, 7909, 12069, 4889, 7716, 8851, 13324, 9122, 7535, 8252, 7909, 12069, 4889, 7716, 11674, 10393, 10393, 10926, 10393, 10926, 8361, 10393, 10926, 8361, 10006, 10393, 10926, 8361, 10006, 8064, 10393, 10926, 8361, 10006, 8064, 8051, 10393, 10926, 8361, 10006, 8064, 8051, 7588, 10393, 10926, 8361, 10006, 8064, 8051, 7588, 11114, 10393, 10926, 8361, 10006, 8064, 8051, 7588, 11114, 8587, };static struct simple_udp_connection unicast_connection;
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