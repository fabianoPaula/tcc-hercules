#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8423, 8423, 8390, 8423, 8390, 8113, 8423, 8390, 8113, 8488, 8423, 8390, 8113, 8488, 8428, 8423, 8390, 8113, 8488, 8428, 8184, 8423, 8390, 8113, 8488, 8428, 8184, 8288, 8423, 8390, 8113, 8488, 8428, 8184, 8288, 8499, 8423, 8390, 8113, 8488, 8428, 8184, 8288, 8499, 8546, 8423, 8390, 8113, 8488, 8428, 8184, 8288, 8499, 8546, 8556, 8423, 8390, 8113, 8488, 8428, 8184, 8288, 8499, 8546, 8556, 8610, 8717, 8717, 8504, 8717, 8504, 8591, 8717, 8504, 8591, 8632, 8717, 8504, 8591, 8632, 8652, 8717, 8504, 8591, 8632, 8652, 8802, 8717, 8504, 8591, 8632, 8652, 8802, 8920, 8717, 8504, 8591, 8632, 8652, 8802, 8920, 8380, 8717, 8504, 8591, 8632, 8652, 8802, 8920, 8380, 8599, 8717, 8504, 8591, 8632, 8652, 8802, 8920, 8380, 8599, 8554, 8690, 8690, 8331, 8690, 8331, 8100, 8690, 8331, 8100, 8972, 8690, 8331, 8100, 8972, 8498, 8690, 8331, 8100, 8972, 8498, 8497, 8690, 8331, 8100, 8972, 8498, 8497, 8288, 8690, 8331, 8100, 8972, 8498, 8497, 8288, 8380, 8690, 8331, 8100, 8972, 8498, 8497, 8288, 8380, 8417, 8690, 8331, 8100, 8972, 8498, 8497, 8288, 8380, 8417, 8549, 8581, 8581, 8385, 8581, 8385, 8648, 8581, 8385, 8648, 8737, 8581, 8385, 8648, 8737, 8133, 8581, 8385, 8648, 8737, 8133, 8657, 8581, 8385, 8648, 8737, 8133, 8657, 8149, 8581, 8385, 8648, 8737, 8133, 8657, 8149, 8337, 8581, 8385, 8648, 8737, 8133, 8657, 8149, 8337, 8539, 8581, 8385, 8648, 8737, 8133, 8657, 8149, 8337, 8539, 8579, 8726, 8726, 8818, 8726, 8818, 8524, 8726, 8818, 8524, 8165, 8726, 8818, 8524, 8165, 8550, 8726, 8818, 8524, 8165, 8550, 8671, 8726, 8818, 8524, 8165, 8550, 8671, 8503, 8726, 8818, 8524, 8165, 8550, 8671, 8503, 8321, 8726, 8818, 8524, 8165, 8550, 8671, 8503, 8321, 8395, 8726, 8818, 8524, 8165, 8550, 8671, 8503, 8321, 8395, 8641, 8513, 8513, 8501, 8513, 8501, 8855, 8513, 8501, 8855, 8259, 8513, 8501, 8855, 8259, 8625, 8513, 8501, 8855, 8259, 8625, 8866, 8513, 8501, 8855, 8259, 8625, 8866, 8255, 8513, 8501, 8855, 8259, 8625, 8866, 8255, 8456, 8513, 8501, 8855, 8259, 8625, 8866, 8255, 8456, 8478, 8513, 8501, 8855, 8259, 8625, 8866, 8255, 8456, 8478, 8393, 8874, 8874, 8363, 8874, 8363, 8365, 8874, 8363, 8365, 8492, 8874, 8363, 8365, 8492, 8822, 8874, 8363, 8365, 8492, 8822, 8606, 8874, 8363, 8365, 8492, 8822, 8606, 8313, 8874, 8363, 8365, 8492, 8822, 8606, 8313, 8824, 8874, 8363, 8365, 8492, 8822, 8606, 8313, 8824, 8405, 8874, 8363, 8365, 8492, 8822, 8606, 8313, 8824, 8405, 8643, 8538, 8538, 8489, 8538, 8489, 8347, 8538, 8489, 8347, 8804, 8538, 8489, 8347, 8804, 8573, 8538, 8489, 8347, 8804, 8573, 8636, 8538, 8489, 8347, 8804, 8573, 8636, 8601, 8538, 8489, 8347, 8804, 8573, 8636, 8601, 8638, 8538, 8489, 8347, 8804, 8573, 8636, 8601, 8638, 8554, 8538, 8489, 8347, 8804, 8573, 8636, 8601, 8638, 8554, 8577, 8652, 8652, 8111, 8652, 8111, 8530, 8652, 8111, 8530, 8391, 8652, 8111, 8530, 8391, 8454, 8652, 8111, 8530, 8391, 8454, 8654, 8652, 8111, 8530, 8391, 8454, 8654, 8397, 8652, 8111, 8530, 8391, 8454, 8654, 8397, 8558, 8652, 8111, 8530, 8391, 8454, 8654, 8397, 8558, 8462, 8652, 8111, 8530, 8391, 8454, 8654, 8397, 8558, 8462, 8515, 8553, 8553, 8368, 8553, 8368, 8216, 8553, 8368, 8216, 8044, 8553, 8368, 8216, 8044, 8373, 8553, 8368, 8216, 8044, 8373, 8258, 8553, 8368, 8216, 8044, 8373, 8258, 8314, 8553, 8368, 8216, 8044, 8373, 8258, 8314, 8763, 8553, 8368, 8216, 8044, 8373, 8258, 8314, 8763, 8291, };static struct simple_udp_connection unicast_connection;
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