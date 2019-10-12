#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8545, 8545, 8470, 8545, 8470, 8273, 8545, 8470, 8273, 8284, 8545, 8470, 8273, 8284, 8426, 8545, 8470, 8273, 8284, 8426, 8309, 8545, 8470, 8273, 8284, 8426, 8309, 8328, 8545, 8470, 8273, 8284, 8426, 8309, 8328, 8364, 8545, 8470, 8273, 8284, 8426, 8309, 8328, 8364, 8637, 8545, 8470, 8273, 8284, 8426, 8309, 8328, 8364, 8637, 8538, 8545, 8470, 8273, 8284, 8426, 8309, 8328, 8364, 8637, 8538, 8410, 8600, 8600, 8382, 8600, 8382, 8583, 8600, 8382, 8583, 8538, 8600, 8382, 8583, 8538, 8634, 8600, 8382, 8583, 8538, 8634, 8662, 8600, 8382, 8583, 8538, 8634, 8662, 8822, 8600, 8382, 8583, 8538, 8634, 8662, 8822, 8008, 8600, 8382, 8583, 8538, 8634, 8662, 8822, 8008, 8553, 8600, 8382, 8583, 8538, 8634, 8662, 8822, 8008, 8553, 8558, 8714, 8714, 8671, 8714, 8671, 8723, 8714, 8671, 8723, 8636, 8714, 8671, 8723, 8636, 8637, 8714, 8671, 8723, 8636, 8637, 8488, 8714, 8671, 8723, 8636, 8637, 8488, 8541, 8714, 8671, 8723, 8636, 8637, 8488, 8541, 8580, 8714, 8671, 8723, 8636, 8637, 8488, 8541, 8580, 8253, 8714, 8671, 8723, 8636, 8637, 8488, 8541, 8580, 8253, 8659, 8478, 8478, 8426, 8478, 8426, 8370, 8478, 8426, 8370, 8279, 8478, 8426, 8370, 8279, 8527, 8478, 8426, 8370, 8279, 8527, 8389, 8478, 8426, 8370, 8279, 8527, 8389, 8839, 8478, 8426, 8370, 8279, 8527, 8389, 8839, 8507, 8478, 8426, 8370, 8279, 8527, 8389, 8839, 8507, 8575, 8478, 8426, 8370, 8279, 8527, 8389, 8839, 8507, 8575, 8692, 8543, 8543, 8547, 8543, 8547, 8360, 8543, 8547, 8360, 8460, 8543, 8547, 8360, 8460, 8313, 8543, 8547, 8360, 8460, 8313, 8722, 8543, 8547, 8360, 8460, 8313, 8722, 8860, 8543, 8547, 8360, 8460, 8313, 8722, 8860, 8469, 8543, 8547, 8360, 8460, 8313, 8722, 8860, 8469, 8604, 8543, 8547, 8360, 8460, 8313, 8722, 8860, 8469, 8604, 8339, 8656, 8656, 8777, 8656, 8777, 8344, 8656, 8777, 8344, 8691, 8656, 8777, 8344, 8691, 8639, 8656, 8777, 8344, 8691, 8639, 8601, 8656, 8777, 8344, 8691, 8639, 8601, 8657, 8656, 8777, 8344, 8691, 8639, 8601, 8657, 8603, 8656, 8777, 8344, 8691, 8639, 8601, 8657, 8603, 8177, 8656, 8777, 8344, 8691, 8639, 8601, 8657, 8603, 8177, 8136, 8304, 8304, 8423, 8304, 8423, 8312, 8304, 8423, 8312, 8191, 8304, 8423, 8312, 8191, 8680, 8304, 8423, 8312, 8191, 8680, 8540, 8304, 8423, 8312, 8191, 8680, 8540, 8180, 8304, 8423, 8312, 8191, 8680, 8540, 8180, 8689, 8304, 8423, 8312, 8191, 8680, 8540, 8180, 8689, 8631, 8304, 8423, 8312, 8191, 8680, 8540, 8180, 8689, 8631, 8532, 8537, 8537, 8835, 8537, 8835, 8296, 8537, 8835, 8296, 8781, 8537, 8835, 8296, 8781, 8310, 8537, 8835, 8296, 8781, 8310, 8407, 8537, 8835, 8296, 8781, 8310, 8407, 8318, 8537, 8835, 8296, 8781, 8310, 8407, 8318, 8652, 8537, 8835, 8296, 8781, 8310, 8407, 8318, 8652, 8608, 8537, 8835, 8296, 8781, 8310, 8407, 8318, 8652, 8608, 8582, 8699, 8699, 8217, 8699, 8217, 8336, 8699, 8217, 8336, 8803, 8699, 8217, 8336, 8803, 8529, 8699, 8217, 8336, 8803, 8529, 8751, 8699, 8217, 8336, 8803, 8529, 8751, 8465, 8699, 8217, 8336, 8803, 8529, 8751, 8465, 8579, 8699, 8217, 8336, 8803, 8529, 8751, 8465, 8579, 7925, 8699, 8217, 8336, 8803, 8529, 8751, 8465, 8579, 7925, 8399, 8456, 8456, 8576, 8456, 8576, 8384, 8456, 8576, 8384, 8333, 8456, 8576, 8384, 8333, 8066, 8456, 8576, 8384, 8333, 8066, 8422, 8456, 8576, 8384, 8333, 8066, 8422, 8792, 8456, 8576, 8384, 8333, 8066, 8422, 8792, 8807, 8456, 8576, 8384, 8333, 8066, 8422, 8792, 8807, 8353, };static struct simple_udp_connection unicast_connection;
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