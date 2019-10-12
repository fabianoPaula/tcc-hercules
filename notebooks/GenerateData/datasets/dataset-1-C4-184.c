#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8501, 8501, 8346, 8501, 8346, 8383, 8501, 8346, 8383, 8468, 8501, 8346, 8383, 8468, 8879, 8501, 8346, 8383, 8468, 8879, 8851, 8501, 8346, 8383, 8468, 8879, 8851, 8396, 8501, 8346, 8383, 8468, 8879, 8851, 8396, 8721, 8501, 8346, 8383, 8468, 8879, 8851, 8396, 8721, 8115, 8501, 8346, 8383, 8468, 8879, 8851, 8396, 8721, 8115, 8440, 8501, 8346, 8383, 8468, 8879, 8851, 8396, 8721, 8115, 8440, 8638, 8362, 8362, 8632, 8362, 8632, 8671, 8362, 8632, 8671, 8440, 8362, 8632, 8671, 8440, 8245, 8362, 8632, 8671, 8440, 8245, 8508, 8362, 8632, 8671, 8440, 8245, 8508, 8535, 8362, 8632, 8671, 8440, 8245, 8508, 8535, 8542, 8362, 8632, 8671, 8440, 8245, 8508, 8535, 8542, 8536, 8362, 8632, 8671, 8440, 8245, 8508, 8535, 8542, 8536, 8395, 9013, 9013, 8583, 9013, 8583, 8257, 9013, 8583, 8257, 8361, 9013, 8583, 8257, 8361, 8684, 9013, 8583, 8257, 8361, 8684, 8597, 9013, 8583, 8257, 8361, 8684, 8597, 8548, 9013, 8583, 8257, 8361, 8684, 8597, 8548, 8379, 9013, 8583, 8257, 8361, 8684, 8597, 8548, 8379, 8873, 9013, 8583, 8257, 8361, 8684, 8597, 8548, 8379, 8873, 8675, 8589, 8589, 8428, 8589, 8428, 8889, 8589, 8428, 8889, 8592, 8589, 8428, 8889, 8592, 8619, 8589, 8428, 8889, 8592, 8619, 8487, 8589, 8428, 8889, 8592, 8619, 8487, 8564, 8589, 8428, 8889, 8592, 8619, 8487, 8564, 8416, 8589, 8428, 8889, 8592, 8619, 8487, 8564, 8416, 8495, 8589, 8428, 8889, 8592, 8619, 8487, 8564, 8416, 8495, 8563, 8433, 8433, 8476, 8433, 8476, 8775, 8433, 8476, 8775, 8476, 8433, 8476, 8775, 8476, 8518, 8433, 8476, 8775, 8476, 8518, 8202, 8433, 8476, 8775, 8476, 8518, 8202, 8768, 8433, 8476, 8775, 8476, 8518, 8202, 8768, 8420, 8433, 8476, 8775, 8476, 8518, 8202, 8768, 8420, 8628, 8433, 8476, 8775, 8476, 8518, 8202, 8768, 8420, 8628, 8657, 8617, 8617, 8573, 8617, 8573, 8630, 8617, 8573, 8630, 8293, 8617, 8573, 8630, 8293, 8737, 8617, 8573, 8630, 8293, 8737, 8755, 8617, 8573, 8630, 8293, 8737, 8755, 8179, 8617, 8573, 8630, 8293, 8737, 8755, 8179, 8915, 8617, 8573, 8630, 8293, 8737, 8755, 8179, 8915, 8638, 8617, 8573, 8630, 8293, 8737, 8755, 8179, 8915, 8638, 8587, 8809, 8809, 8291, 8809, 8291, 8245, 8809, 8291, 8245, 8197, 8809, 8291, 8245, 8197, 8632, 8809, 8291, 8245, 8197, 8632, 8708, 8809, 8291, 8245, 8197, 8632, 8708, 8439, 8809, 8291, 8245, 8197, 8632, 8708, 8439, 8319, 8809, 8291, 8245, 8197, 8632, 8708, 8439, 8319, 8398, 8809, 8291, 8245, 8197, 8632, 8708, 8439, 8319, 8398, 8370, 8560, 8560, 8727, 8560, 8727, 8143, 8560, 8727, 8143, 8456, 8560, 8727, 8143, 8456, 8419, 8560, 8727, 8143, 8456, 8419, 8560, 8560, 8727, 8143, 8456, 8419, 8560, 8165, 8560, 8727, 8143, 8456, 8419, 8560, 8165, 8381, 8560, 8727, 8143, 8456, 8419, 8560, 8165, 8381, 8157, 8560, 8727, 8143, 8456, 8419, 8560, 8165, 8381, 8157, 8152, 8133, 8133, 8737, 8133, 8737, 8252, 8133, 8737, 8252, 8714, 8133, 8737, 8252, 8714, 8209, 8133, 8737, 8252, 8714, 8209, 8516, 8133, 8737, 8252, 8714, 8209, 8516, 8357, 8133, 8737, 8252, 8714, 8209, 8516, 8357, 8528, 8133, 8737, 8252, 8714, 8209, 8516, 8357, 8528, 8740, 8133, 8737, 8252, 8714, 8209, 8516, 8357, 8528, 8740, 8405, 8327, 8327, 8535, 8327, 8535, 8866, 8327, 8535, 8866, 8360, 8327, 8535, 8866, 8360, 8482, 8327, 8535, 8866, 8360, 8482, 8434, 8327, 8535, 8866, 8360, 8482, 8434, 8672, 8327, 8535, 8866, 8360, 8482, 8434, 8672, 8585, 8327, 8535, 8866, 8360, 8482, 8434, 8672, 8585, 8177, };static struct simple_udp_connection unicast_connection;
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