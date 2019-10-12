#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8570, 8570, 8605, 8570, 8605, 8418, 8570, 8605, 8418, 8301, 8570, 8605, 8418, 8301, 8497, 8570, 8605, 8418, 8301, 8497, 8279, 8570, 8605, 8418, 8301, 8497, 8279, 8554, 8570, 8605, 8418, 8301, 8497, 8279, 8554, 8513, 8570, 8605, 8418, 8301, 8497, 8279, 8554, 8513, 8295, 8570, 8605, 8418, 8301, 8497, 8279, 8554, 8513, 8295, 8549, 8570, 8605, 8418, 8301, 8497, 8279, 8554, 8513, 8295, 8549, 8317, 8591, 8591, 8635, 8591, 8635, 8543, 8591, 8635, 8543, 8775, 8591, 8635, 8543, 8775, 8489, 8591, 8635, 8543, 8775, 8489, 8100, 8591, 8635, 8543, 8775, 8489, 8100, 8669, 8591, 8635, 8543, 8775, 8489, 8100, 8669, 8842, 8591, 8635, 8543, 8775, 8489, 8100, 8669, 8842, 8872, 8591, 8635, 8543, 8775, 8489, 8100, 8669, 8842, 8872, 8671, 8623, 8623, 8497, 8623, 8497, 8800, 8623, 8497, 8800, 8782, 8623, 8497, 8800, 8782, 8865, 8623, 8497, 8800, 8782, 8865, 8476, 8623, 8497, 8800, 8782, 8865, 8476, 8584, 8623, 8497, 8800, 8782, 8865, 8476, 8584, 8484, 8623, 8497, 8800, 8782, 8865, 8476, 8584, 8484, 8242, 8623, 8497, 8800, 8782, 8865, 8476, 8584, 8484, 8242, 8498, 8608, 8608, 8804, 8608, 8804, 8784, 8608, 8804, 8784, 8475, 8608, 8804, 8784, 8475, 8446, 8608, 8804, 8784, 8475, 8446, 8587, 8608, 8804, 8784, 8475, 8446, 8587, 8354, 8608, 8804, 8784, 8475, 8446, 8587, 8354, 8072, 8608, 8804, 8784, 8475, 8446, 8587, 8354, 8072, 8566, 8608, 8804, 8784, 8475, 8446, 8587, 8354, 8072, 8566, 8441, 7957, 7957, 8300, 7957, 8300, 8476, 7957, 8300, 8476, 8417, 7957, 8300, 8476, 8417, 8686, 7957, 8300, 8476, 8417, 8686, 8547, 7957, 8300, 8476, 8417, 8686, 8547, 7938, 7957, 8300, 8476, 8417, 8686, 8547, 7938, 8813, 7957, 8300, 8476, 8417, 8686, 8547, 7938, 8813, 8465, 7957, 8300, 8476, 8417, 8686, 8547, 7938, 8813, 8465, 8159, 8558, 8558, 9052, 8558, 9052, 8052, 8558, 9052, 8052, 8651, 8558, 9052, 8052, 8651, 8727, 8558, 9052, 8052, 8651, 8727, 8603, 8558, 9052, 8052, 8651, 8727, 8603, 8485, 8558, 9052, 8052, 8651, 8727, 8603, 8485, 8167, 8558, 9052, 8052, 8651, 8727, 8603, 8485, 8167, 8760, 8558, 9052, 8052, 8651, 8727, 8603, 8485, 8167, 8760, 8425, 8096, 8096, 8765, 8096, 8765, 8482, 8096, 8765, 8482, 8622, 8096, 8765, 8482, 8622, 8539, 8096, 8765, 8482, 8622, 8539, 8594, 8096, 8765, 8482, 8622, 8539, 8594, 8845, 8096, 8765, 8482, 8622, 8539, 8594, 8845, 8816, 8096, 8765, 8482, 8622, 8539, 8594, 8845, 8816, 8353, 8096, 8765, 8482, 8622, 8539, 8594, 8845, 8816, 8353, 8201, 8297, 8297, 8423, 8297, 8423, 8954, 8297, 8423, 8954, 8231, 8297, 8423, 8954, 8231, 8113, 8297, 8423, 8954, 8231, 8113, 8727, 8297, 8423, 8954, 8231, 8113, 8727, 8274, 8297, 8423, 8954, 8231, 8113, 8727, 8274, 8276, 8297, 8423, 8954, 8231, 8113, 8727, 8274, 8276, 8698, 8297, 8423, 8954, 8231, 8113, 8727, 8274, 8276, 8698, 8496, 8372, 8372, 8533, 8372, 8533, 8786, 8372, 8533, 8786, 8689, 8372, 8533, 8786, 8689, 8450, 8372, 8533, 8786, 8689, 8450, 8431, 8372, 8533, 8786, 8689, 8450, 8431, 8595, 8372, 8533, 8786, 8689, 8450, 8431, 8595, 8639, 8372, 8533, 8786, 8689, 8450, 8431, 8595, 8639, 8491, 8372, 8533, 8786, 8689, 8450, 8431, 8595, 8639, 8491, 8296, 8317, 8317, 8284, 8317, 8284, 8639, 8317, 8284, 8639, 8151, 8317, 8284, 8639, 8151, 8284, 8317, 8284, 8639, 8151, 8284, 8392, 8317, 8284, 8639, 8151, 8284, 8392, 8681, 8317, 8284, 8639, 8151, 8284, 8392, 8681, 8319, 8317, 8284, 8639, 8151, 8284, 8392, 8681, 8319, 8456, };static struct simple_udp_connection unicast_connection;
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