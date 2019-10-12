#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8471, 8471, 8459, 8471, 8459, 8667, 8471, 8459, 8667, 8356, 8471, 8459, 8667, 8356, 8285, 8471, 8459, 8667, 8356, 8285, 8589, 8471, 8459, 8667, 8356, 8285, 8589, 8725, 8471, 8459, 8667, 8356, 8285, 8589, 8725, 8441, 8471, 8459, 8667, 8356, 8285, 8589, 8725, 8441, 9089, 8471, 8459, 8667, 8356, 8285, 8589, 8725, 8441, 9089, 8184, 8471, 8459, 8667, 8356, 8285, 8589, 8725, 8441, 9089, 8184, 8315, 8345, 8345, 8577, 8345, 8577, 8683, 8345, 8577, 8683, 8342, 8345, 8577, 8683, 8342, 8485, 8345, 8577, 8683, 8342, 8485, 8613, 8345, 8577, 8683, 8342, 8485, 8613, 8709, 8345, 8577, 8683, 8342, 8485, 8613, 8709, 8304, 8345, 8577, 8683, 8342, 8485, 8613, 8709, 8304, 8392, 8345, 8577, 8683, 8342, 8485, 8613, 8709, 8304, 8392, 8496, 8500, 8500, 8107, 8500, 8107, 8553, 8500, 8107, 8553, 8516, 8500, 8107, 8553, 8516, 8661, 8500, 8107, 8553, 8516, 8661, 8531, 8500, 8107, 8553, 8516, 8661, 8531, 8476, 8500, 8107, 8553, 8516, 8661, 8531, 8476, 8487, 8500, 8107, 8553, 8516, 8661, 8531, 8476, 8487, 8501, 8500, 8107, 8553, 8516, 8661, 8531, 8476, 8487, 8501, 9049, 8504, 8504, 8205, 8504, 8205, 8358, 8504, 8205, 8358, 8571, 8504, 8205, 8358, 8571, 8525, 8504, 8205, 8358, 8571, 8525, 8530, 8504, 8205, 8358, 8571, 8525, 8530, 8640, 8504, 8205, 8358, 8571, 8525, 8530, 8640, 8438, 8504, 8205, 8358, 8571, 8525, 8530, 8640, 8438, 8500, 8504, 8205, 8358, 8571, 8525, 8530, 8640, 8438, 8500, 8623, 8760, 8760, 8402, 8760, 8402, 8270, 8760, 8402, 8270, 8580, 8760, 8402, 8270, 8580, 8432, 8760, 8402, 8270, 8580, 8432, 8599, 8760, 8402, 8270, 8580, 8432, 8599, 8310, 8760, 8402, 8270, 8580, 8432, 8599, 8310, 8560, 8760, 8402, 8270, 8580, 8432, 8599, 8310, 8560, 8770, 8760, 8402, 8270, 8580, 8432, 8599, 8310, 8560, 8770, 8860, 8286, 8286, 8435, 8286, 8435, 8475, 8286, 8435, 8475, 8743, 8286, 8435, 8475, 8743, 8480, 8286, 8435, 8475, 8743, 8480, 8264, 8286, 8435, 8475, 8743, 8480, 8264, 8222, 8286, 8435, 8475, 8743, 8480, 8264, 8222, 8670, 8286, 8435, 8475, 8743, 8480, 8264, 8222, 8670, 8312, 8286, 8435, 8475, 8743, 8480, 8264, 8222, 8670, 8312, 8484, 8561, 8561, 8119, 8561, 8119, 8697, 8561, 8119, 8697, 8537, 8561, 8119, 8697, 8537, 8484, 8561, 8119, 8697, 8537, 8484, 8681, 8561, 8119, 8697, 8537, 8484, 8681, 8848, 8561, 8119, 8697, 8537, 8484, 8681, 8848, 8244, 8561, 8119, 8697, 8537, 8484, 8681, 8848, 8244, 8012, 8561, 8119, 8697, 8537, 8484, 8681, 8848, 8244, 8012, 8513, 8446, 8446, 8641, 8446, 8641, 8458, 8446, 8641, 8458, 8845, 8446, 8641, 8458, 8845, 8246, 8446, 8641, 8458, 8845, 8246, 8270, 8446, 8641, 8458, 8845, 8246, 8270, 8606, 8446, 8641, 8458, 8845, 8246, 8270, 8606, 8804, 8446, 8641, 8458, 8845, 8246, 8270, 8606, 8804, 8707, 8446, 8641, 8458, 8845, 8246, 8270, 8606, 8804, 8707, 8733, 8823, 8823, 8332, 8823, 8332, 8204, 8823, 8332, 8204, 8715, 8823, 8332, 8204, 8715, 8634, 8823, 8332, 8204, 8715, 8634, 8710, 8823, 8332, 8204, 8715, 8634, 8710, 8476, 8823, 8332, 8204, 8715, 8634, 8710, 8476, 8279, 8823, 8332, 8204, 8715, 8634, 8710, 8476, 8279, 8414, 8823, 8332, 8204, 8715, 8634, 8710, 8476, 8279, 8414, 8181, 8244, 8244, 8611, 8244, 8611, 8752, 8244, 8611, 8752, 8843, 8244, 8611, 8752, 8843, 8529, 8244, 8611, 8752, 8843, 8529, 8717, 8244, 8611, 8752, 8843, 8529, 8717, 8555, 8244, 8611, 8752, 8843, 8529, 8717, 8555, 8350, 8244, 8611, 8752, 8843, 8529, 8717, 8555, 8350, 8432, };static struct simple_udp_connection unicast_connection;
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