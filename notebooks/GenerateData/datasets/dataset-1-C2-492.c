#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8912, 8912, 8367, 8912, 8367, 8876, 8912, 8367, 8876, 8619, 8912, 8367, 8876, 8619, 8777, 8912, 8367, 8876, 8619, 8777, 8689, 8912, 8367, 8876, 8619, 8777, 8689, 8275, 8912, 8367, 8876, 8619, 8777, 8689, 8275, 8299, 8912, 8367, 8876, 8619, 8777, 8689, 8275, 8299, 8368, 8912, 8367, 8876, 8619, 8777, 8689, 8275, 8299, 8368, 8383, 8912, 8367, 8876, 8619, 8777, 8689, 8275, 8299, 8368, 8383, 8543, 8417, 8417, 8187, 8417, 8187, 8758, 8417, 8187, 8758, 8665, 8417, 8187, 8758, 8665, 8267, 8417, 8187, 8758, 8665, 8267, 8627, 8417, 8187, 8758, 8665, 8267, 8627, 8498, 8417, 8187, 8758, 8665, 8267, 8627, 8498, 8379, 8417, 8187, 8758, 8665, 8267, 8627, 8498, 8379, 8733, 8417, 8187, 8758, 8665, 8267, 8627, 8498, 8379, 8733, 8403, 8885, 8885, 8348, 8885, 8348, 8397, 8885, 8348, 8397, 8381, 8885, 8348, 8397, 8381, 8282, 8885, 8348, 8397, 8381, 8282, 8137, 8885, 8348, 8397, 8381, 8282, 8137, 8808, 8885, 8348, 8397, 8381, 8282, 8137, 8808, 8523, 8885, 8348, 8397, 8381, 8282, 8137, 8808, 8523, 8654, 8885, 8348, 8397, 8381, 8282, 8137, 8808, 8523, 8654, 8441, 8403, 8403, 8429, 8403, 8429, 8512, 8403, 8429, 8512, 8305, 8403, 8429, 8512, 8305, 8432, 8403, 8429, 8512, 8305, 8432, 8559, 8403, 8429, 8512, 8305, 8432, 8559, 8622, 8403, 8429, 8512, 8305, 8432, 8559, 8622, 8479, 8403, 8429, 8512, 8305, 8432, 8559, 8622, 8479, 8787, 8403, 8429, 8512, 8305, 8432, 8559, 8622, 8479, 8787, 8649, 8355, 8355, 8434, 8355, 8434, 8226, 8355, 8434, 8226, 8211, 8355, 8434, 8226, 8211, 8498, 8355, 8434, 8226, 8211, 8498, 8443, 8355, 8434, 8226, 8211, 8498, 8443, 8493, 8355, 8434, 8226, 8211, 8498, 8443, 8493, 8536, 8355, 8434, 8226, 8211, 8498, 8443, 8493, 8536, 8840, 8355, 8434, 8226, 8211, 8498, 8443, 8493, 8536, 8840, 8554, 8449, 8449, 8177, 8449, 8177, 8328, 8449, 8177, 8328, 8209, 8449, 8177, 8328, 8209, 8679, 8449, 8177, 8328, 8209, 8679, 8250, 8449, 8177, 8328, 8209, 8679, 8250, 8560, 8449, 8177, 8328, 8209, 8679, 8250, 8560, 8682, 8449, 8177, 8328, 8209, 8679, 8250, 8560, 8682, 8455, 8449, 8177, 8328, 8209, 8679, 8250, 8560, 8682, 8455, 8113, 8360, 8360, 8841, 8360, 8841, 8384, 8360, 8841, 8384, 8567, 8360, 8841, 8384, 8567, 8587, 8360, 8841, 8384, 8567, 8587, 8591, 8360, 8841, 8384, 8567, 8587, 8591, 8567, 8360, 8841, 8384, 8567, 8587, 8591, 8567, 8822, 8360, 8841, 8384, 8567, 8587, 8591, 8567, 8822, 8684, 8360, 8841, 8384, 8567, 8587, 8591, 8567, 8822, 8684, 8473, 8456, 8456, 8376, 8456, 8376, 8190, 8456, 8376, 8190, 8619, 8456, 8376, 8190, 8619, 8624, 8456, 8376, 8190, 8619, 8624, 8843, 8456, 8376, 8190, 8619, 8624, 8843, 8608, 8456, 8376, 8190, 8619, 8624, 8843, 8608, 8519, 8456, 8376, 8190, 8619, 8624, 8843, 8608, 8519, 8634, 8456, 8376, 8190, 8619, 8624, 8843, 8608, 8519, 8634, 8644, 8527, 8527, 8480, 8527, 8480, 8337, 8527, 8480, 8337, 8549, 8527, 8480, 8337, 8549, 8106, 8527, 8480, 8337, 8549, 8106, 8502, 8527, 8480, 8337, 8549, 8106, 8502, 8543, 8527, 8480, 8337, 8549, 8106, 8502, 8543, 8740, 8527, 8480, 8337, 8549, 8106, 8502, 8543, 8740, 8557, 8527, 8480, 8337, 8549, 8106, 8502, 8543, 8740, 8557, 8475, 8372, 8372, 8420, 8372, 8420, 8253, 8372, 8420, 8253, 8447, 8372, 8420, 8253, 8447, 8006, 8372, 8420, 8253, 8447, 8006, 8435, 8372, 8420, 8253, 8447, 8006, 8435, 8530, 8372, 8420, 8253, 8447, 8006, 8435, 8530, 8574, 8372, 8420, 8253, 8447, 8006, 8435, 8530, 8574, 8397, };static struct simple_udp_connection unicast_connection;
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