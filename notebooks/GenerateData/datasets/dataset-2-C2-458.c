#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8508, 8508, 8195, 8508, 8195, 8156, 8508, 8195, 8156, 12094, 8508, 8195, 8156, 12094, 12164, 8508, 8195, 8156, 12094, 12164, 8249, 8508, 8195, 8156, 12094, 12164, 8249, 9567, 8508, 8195, 8156, 12094, 12164, 8249, 9567, 11604, 8508, 8195, 8156, 12094, 12164, 8249, 9567, 11604, 9638, 8508, 8195, 8156, 12094, 12164, 8249, 9567, 11604, 9638, 5844, 8508, 8195, 8156, 12094, 12164, 8249, 9567, 11604, 9638, 5844, 7098, 8136, 8136, 9022, 8136, 9022, 10360, 8136, 9022, 10360, 8344, 8136, 9022, 10360, 8344, 9804, 8136, 9022, 10360, 8344, 9804, 9983, 8136, 9022, 10360, 8344, 9804, 9983, 9858, 8136, 9022, 10360, 8344, 9804, 9983, 9858, 8654, 8136, 9022, 10360, 8344, 9804, 9983, 9858, 8654, 10869, 8136, 9022, 10360, 8344, 9804, 9983, 9858, 8654, 10869, 9509, 7088, 7088, 7579, 7088, 7579, 7236, 7088, 7579, 7236, 10704, 7088, 7579, 7236, 10704, 10726, 7088, 7579, 7236, 10704, 10726, 7710, 7088, 7579, 7236, 10704, 10726, 7710, 8985, 7088, 7579, 7236, 10704, 10726, 7710, 8985, 12531, 7088, 7579, 7236, 10704, 10726, 7710, 8985, 12531, 8913, 7088, 7579, 7236, 10704, 10726, 7710, 8985, 12531, 8913, 10587, 8230, 8230, 6843, 8230, 6843, 6706, 8230, 6843, 6706, 10439, 8230, 6843, 6706, 10439, 8679, 8230, 6843, 6706, 10439, 8679, 5510, 8230, 6843, 6706, 10439, 8679, 5510, 7345, 8230, 6843, 6706, 10439, 8679, 5510, 7345, 4921, 8230, 6843, 6706, 10439, 8679, 5510, 7345, 4921, 8146, 8230, 6843, 6706, 10439, 8679, 5510, 7345, 4921, 8146, 12471, 7827, 7827, 7457, 7827, 7457, 10237, 7827, 7457, 10237, 10363, 7827, 7457, 10237, 10363, 10572, 7827, 7457, 10237, 10363, 10572, 10164, 7827, 7457, 10237, 10363, 10572, 10164, 9777, 7827, 7457, 10237, 10363, 10572, 10164, 9777, 11511, 7827, 7457, 10237, 10363, 10572, 10164, 9777, 11511, 5787, 7827, 7457, 10237, 10363, 10572, 10164, 9777, 11511, 5787, 10748, 10422, 10422, 7188, 10422, 7188, 6829, 10422, 7188, 6829, 6935, 10422, 7188, 6829, 6935, 7806, 10422, 7188, 6829, 6935, 7806, 9018, 10422, 7188, 6829, 6935, 7806, 9018, 11166, 10422, 7188, 6829, 6935, 7806, 9018, 11166, 10759, 10422, 7188, 6829, 6935, 7806, 9018, 11166, 10759, 9821, 10422, 7188, 6829, 6935, 7806, 9018, 11166, 10759, 9821, 12863, 9807, 9807, 6677, 9807, 6677, 8087, 9807, 6677, 8087, 7819, 9807, 6677, 8087, 7819, 9692, 9807, 6677, 8087, 7819, 9692, 7163, 9807, 6677, 8087, 7819, 9692, 7163, 6988, 9807, 6677, 8087, 7819, 9692, 7163, 6988, 8964, 9807, 6677, 8087, 7819, 9692, 7163, 6988, 8964, 9997, 9807, 6677, 8087, 7819, 9692, 7163, 6988, 8964, 9997, 11207, 7030, 7030, 8040, 7030, 8040, 5828, 7030, 8040, 5828, 7555, 7030, 8040, 5828, 7555, 5805, 7030, 8040, 5828, 7555, 5805, 8190, 7030, 8040, 5828, 7555, 5805, 8190, 10605, 7030, 8040, 5828, 7555, 5805, 8190, 10605, 9173, 7030, 8040, 5828, 7555, 5805, 8190, 10605, 9173, 8261, 7030, 8040, 5828, 7555, 5805, 8190, 10605, 9173, 8261, 9390, 9342, 9342, 10538, 9342, 10538, 8237, 9342, 10538, 8237, 10808, 9342, 10538, 8237, 10808, 5020, 9342, 10538, 8237, 10808, 5020, 12835, 9342, 10538, 8237, 10808, 5020, 12835, 5555, 9342, 10538, 8237, 10808, 5020, 12835, 5555, 10253, 9342, 10538, 8237, 10808, 5020, 12835, 5555, 10253, 9351, 9342, 10538, 8237, 10808, 5020, 12835, 5555, 10253, 9351, 7764, 11586, 11586, 10109, 11586, 10109, 7847, 11586, 10109, 7847, 7329, 11586, 10109, 7847, 7329, 11854, 11586, 10109, 7847, 7329, 11854, 11598, 11586, 10109, 7847, 7329, 11854, 11598, 7861, 11586, 10109, 7847, 7329, 11854, 11598, 7861, 12457, 11586, 10109, 7847, 7329, 11854, 11598, 7861, 12457, 9997, };static struct simple_udp_connection unicast_connection;
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