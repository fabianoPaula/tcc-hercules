#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8261, 8261, 8381, 8261, 8381, 8722, 8261, 8381, 8722, 8371, 8261, 8381, 8722, 8371, 8838, 8261, 8381, 8722, 8371, 8838, 8222, 8261, 8381, 8722, 8371, 8838, 8222, 8267, 8261, 8381, 8722, 8371, 8838, 8222, 8267, 8394, 8261, 8381, 8722, 8371, 8838, 8222, 8267, 8394, 8237, 8261, 8381, 8722, 8371, 8838, 8222, 8267, 8394, 8237, 8300, 8261, 8381, 8722, 8371, 8838, 8222, 8267, 8394, 8237, 8300, 8309, 8403, 8403, 8337, 8403, 8337, 8185, 8403, 8337, 8185, 8528, 8403, 8337, 8185, 8528, 8257, 8403, 8337, 8185, 8528, 8257, 8702, 8403, 8337, 8185, 8528, 8257, 8702, 8396, 8403, 8337, 8185, 8528, 8257, 8702, 8396, 8516, 8403, 8337, 8185, 8528, 8257, 8702, 8396, 8516, 8461, 8403, 8337, 8185, 8528, 8257, 8702, 8396, 8516, 8461, 8516, 8561, 8561, 8852, 8561, 8852, 8673, 8561, 8852, 8673, 8202, 8561, 8852, 8673, 8202, 8235, 8561, 8852, 8673, 8202, 8235, 8717, 8561, 8852, 8673, 8202, 8235, 8717, 8468, 8561, 8852, 8673, 8202, 8235, 8717, 8468, 8800, 8561, 8852, 8673, 8202, 8235, 8717, 8468, 8800, 8694, 8561, 8852, 8673, 8202, 8235, 8717, 8468, 8800, 8694, 8708, 8268, 8268, 8491, 8268, 8491, 8297, 8268, 8491, 8297, 8928, 8268, 8491, 8297, 8928, 8919, 8268, 8491, 8297, 8928, 8919, 8945, 8268, 8491, 8297, 8928, 8919, 8945, 8366, 8268, 8491, 8297, 8928, 8919, 8945, 8366, 8444, 8268, 8491, 8297, 8928, 8919, 8945, 8366, 8444, 8861, 8268, 8491, 8297, 8928, 8919, 8945, 8366, 8444, 8861, 8315, 8448, 8448, 8738, 8448, 8738, 8216, 8448, 8738, 8216, 8595, 8448, 8738, 8216, 8595, 8622, 8448, 8738, 8216, 8595, 8622, 8456, 8448, 8738, 8216, 8595, 8622, 8456, 8357, 8448, 8738, 8216, 8595, 8622, 8456, 8357, 8519, 8448, 8738, 8216, 8595, 8622, 8456, 8357, 8519, 8571, 8448, 8738, 8216, 8595, 8622, 8456, 8357, 8519, 8571, 8469, 8578, 8578, 8659, 8578, 8659, 8491, 8578, 8659, 8491, 8321, 8578, 8659, 8491, 8321, 8303, 8578, 8659, 8491, 8321, 8303, 8276, 8578, 8659, 8491, 8321, 8303, 8276, 8878, 8578, 8659, 8491, 8321, 8303, 8276, 8878, 8229, 8578, 8659, 8491, 8321, 8303, 8276, 8878, 8229, 8472, 8578, 8659, 8491, 8321, 8303, 8276, 8878, 8229, 8472, 8537, 8838, 8838, 8612, 8838, 8612, 8460, 8838, 8612, 8460, 8850, 8838, 8612, 8460, 8850, 8431, 8838, 8612, 8460, 8850, 8431, 8613, 8838, 8612, 8460, 8850, 8431, 8613, 8386, 8838, 8612, 8460, 8850, 8431, 8613, 8386, 8303, 8838, 8612, 8460, 8850, 8431, 8613, 8386, 8303, 9028, 8838, 8612, 8460, 8850, 8431, 8613, 8386, 8303, 9028, 8884, 8492, 8492, 8560, 8492, 8560, 8455, 8492, 8560, 8455, 8115, 8492, 8560, 8455, 8115, 8520, 8492, 8560, 8455, 8115, 8520, 8339, 8492, 8560, 8455, 8115, 8520, 8339, 8483, 8492, 8560, 8455, 8115, 8520, 8339, 8483, 8390, 8492, 8560, 8455, 8115, 8520, 8339, 8483, 8390, 8885, 8492, 8560, 8455, 8115, 8520, 8339, 8483, 8390, 8885, 8707, 8308, 8308, 8380, 8308, 8380, 8546, 8308, 8380, 8546, 8713, 8308, 8380, 8546, 8713, 8481, 8308, 8380, 8546, 8713, 8481, 8373, 8308, 8380, 8546, 8713, 8481, 8373, 8851, 8308, 8380, 8546, 8713, 8481, 8373, 8851, 8458, 8308, 8380, 8546, 8713, 8481, 8373, 8851, 8458, 8564, 8308, 8380, 8546, 8713, 8481, 8373, 8851, 8458, 8564, 8665, 8345, 8345, 8547, 8345, 8547, 8698, 8345, 8547, 8698, 8205, 8345, 8547, 8698, 8205, 8654, 8345, 8547, 8698, 8205, 8654, 8898, 8345, 8547, 8698, 8205, 8654, 8898, 8316, 8345, 8547, 8698, 8205, 8654, 8898, 8316, 7878, 8345, 8547, 8698, 8205, 8654, 8898, 8316, 7878, 8255, };static struct simple_udp_connection unicast_connection;
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