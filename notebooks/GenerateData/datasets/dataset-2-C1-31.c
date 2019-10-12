#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8453, 8453, 8599, 8453, 8599, 9345, 8453, 8599, 9345, 7776, 8453, 8599, 9345, 7776, 8806, 8453, 8599, 9345, 7776, 8806, 7826, 8453, 8599, 9345, 7776, 8806, 7826, 7801, 8453, 8599, 9345, 7776, 8806, 7826, 7801, 12528, 8453, 8599, 9345, 7776, 8806, 7826, 7801, 12528, 7929, 8453, 8599, 9345, 7776, 8806, 7826, 7801, 12528, 7929, 9045, 8453, 8599, 9345, 7776, 8806, 7826, 7801, 12528, 7929, 9045, 9718, 8963, 8963, 7431, 8963, 7431, 7546, 8963, 7431, 7546, 11358, 8963, 7431, 7546, 11358, 8585, 8963, 7431, 7546, 11358, 8585, 6034, 8963, 7431, 7546, 11358, 8585, 6034, 9292, 8963, 7431, 7546, 11358, 8585, 6034, 9292, 8170, 8963, 7431, 7546, 11358, 8585, 6034, 9292, 8170, 7938, 8963, 7431, 7546, 11358, 8585, 6034, 9292, 8170, 7938, 7516, 11962, 11962, 12759, 11962, 12759, 8416, 11962, 12759, 8416, 8957, 11962, 12759, 8416, 8957, 8822, 11962, 12759, 8416, 8957, 8822, 7819, 11962, 12759, 8416, 8957, 8822, 7819, 9271, 11962, 12759, 8416, 8957, 8822, 7819, 9271, 10301, 11962, 12759, 8416, 8957, 8822, 7819, 9271, 10301, 8725, 11962, 12759, 8416, 8957, 8822, 7819, 9271, 10301, 8725, 10131, 10436, 10436, 11180, 10436, 11180, 9034, 10436, 11180, 9034, 8449, 10436, 11180, 9034, 8449, 8804, 10436, 11180, 9034, 8449, 8804, 9168, 10436, 11180, 9034, 8449, 8804, 9168, 7057, 10436, 11180, 9034, 8449, 8804, 9168, 7057, 7245, 10436, 11180, 9034, 8449, 8804, 9168, 7057, 7245, 7265, 10436, 11180, 9034, 8449, 8804, 9168, 7057, 7245, 7265, 9225, 9039, 9039, 7984, 9039, 7984, 12061, 9039, 7984, 12061, 8394, 9039, 7984, 12061, 8394, 8262, 9039, 7984, 12061, 8394, 8262, 7951, 9039, 7984, 12061, 8394, 8262, 7951, 11483, 9039, 7984, 12061, 8394, 8262, 7951, 11483, 11720, 9039, 7984, 12061, 8394, 8262, 7951, 11483, 11720, 8078, 9039, 7984, 12061, 8394, 8262, 7951, 11483, 11720, 8078, 9895, 10224, 10224, 6632, 10224, 6632, 5904, 10224, 6632, 5904, 6401, 10224, 6632, 5904, 6401, 13543, 10224, 6632, 5904, 6401, 13543, 9614, 10224, 6632, 5904, 6401, 13543, 9614, 9662, 10224, 6632, 5904, 6401, 13543, 9614, 9662, 8808, 10224, 6632, 5904, 6401, 13543, 9614, 9662, 8808, 8269, 10224, 6632, 5904, 6401, 13543, 9614, 9662, 8808, 8269, 12463, 7589, 7589, 9675, 7589, 9675, 13322, 7589, 9675, 13322, 8518, 7589, 9675, 13322, 8518, 8981, 7589, 9675, 13322, 8518, 8981, 8536, 7589, 9675, 13322, 8518, 8981, 8536, 7013, 7589, 9675, 13322, 8518, 8981, 8536, 7013, 8774, 7589, 9675, 13322, 8518, 8981, 8536, 7013, 8774, 11470, 7589, 9675, 13322, 8518, 8981, 8536, 7013, 8774, 11470, 8608, 9251, 9251, 6895, 9251, 6895, 4843, 9251, 6895, 4843, 11004, 9251, 6895, 4843, 11004, 6447, 9251, 6895, 4843, 11004, 6447, 8181, 9251, 6895, 4843, 11004, 6447, 8181, 8413, 9251, 6895, 4843, 11004, 6447, 8181, 8413, 11834, 9251, 6895, 4843, 11004, 6447, 8181, 8413, 11834, 6733, 9251, 6895, 4843, 11004, 6447, 8181, 8413, 11834, 6733, 10578, 5489, 5489, 10431, 5489, 10431, 11106, 5489, 10431, 11106, 9524, 5489, 10431, 11106, 9524, 9683, 5489, 10431, 11106, 9524, 9683, 9396, 5489, 10431, 11106, 9524, 9683, 9396, 10792, 5489, 10431, 11106, 9524, 9683, 9396, 10792, 7968, 5489, 10431, 11106, 9524, 9683, 9396, 10792, 7968, 13821, 5489, 10431, 11106, 9524, 9683, 9396, 10792, 7968, 13821, 8140, 8780, 8780, 8624, 8780, 8624, 8929, 8780, 8624, 8929, 7609, 8780, 8624, 8929, 7609, 8287, 8780, 8624, 8929, 7609, 8287, 5644, 8780, 8624, 8929, 7609, 8287, 5644, 4989, 8780, 8624, 8929, 7609, 8287, 5644, 4989, 6309, 8780, 8624, 8929, 7609, 8287, 5644, 4989, 6309, 9314, };static struct simple_udp_connection unicast_connection;
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