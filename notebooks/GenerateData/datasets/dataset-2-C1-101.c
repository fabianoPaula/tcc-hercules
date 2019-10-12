#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {14072, 14072, 10330, 14072, 10330, 10834, 14072, 10330, 10834, 10106, 14072, 10330, 10834, 10106, 10372, 14072, 10330, 10834, 10106, 10372, 8625, 14072, 10330, 10834, 10106, 10372, 8625, 7673, 14072, 10330, 10834, 10106, 10372, 8625, 7673, 10290, 14072, 10330, 10834, 10106, 10372, 8625, 7673, 10290, 5567, 14072, 10330, 10834, 10106, 10372, 8625, 7673, 10290, 5567, 10532, 14072, 10330, 10834, 10106, 10372, 8625, 7673, 10290, 5567, 10532, 10151, 8139, 8139, 9539, 8139, 9539, 7834, 8139, 9539, 7834, 7520, 8139, 9539, 7834, 7520, 10919, 8139, 9539, 7834, 7520, 10919, 9543, 8139, 9539, 7834, 7520, 10919, 9543, 12761, 8139, 9539, 7834, 7520, 10919, 9543, 12761, 13890, 8139, 9539, 7834, 7520, 10919, 9543, 12761, 13890, 10430, 8139, 9539, 7834, 7520, 10919, 9543, 12761, 13890, 10430, 9744, 12248, 12248, 6129, 12248, 6129, 7113, 12248, 6129, 7113, 8957, 12248, 6129, 7113, 8957, 9902, 12248, 6129, 7113, 8957, 9902, 9500, 12248, 6129, 7113, 8957, 9902, 9500, 9532, 12248, 6129, 7113, 8957, 9902, 9500, 9532, 10653, 12248, 6129, 7113, 8957, 9902, 9500, 9532, 10653, 9331, 12248, 6129, 7113, 8957, 9902, 9500, 9532, 10653, 9331, 10349, 9793, 9793, 8305, 9793, 8305, 7842, 9793, 8305, 7842, 7501, 9793, 8305, 7842, 7501, 10072, 9793, 8305, 7842, 7501, 10072, 8989, 9793, 8305, 7842, 7501, 10072, 8989, 12623, 9793, 8305, 7842, 7501, 10072, 8989, 12623, 9628, 9793, 8305, 7842, 7501, 10072, 8989, 12623, 9628, 12793, 9793, 8305, 7842, 7501, 10072, 8989, 12623, 9628, 12793, 7412, 9554, 9554, 7154, 9554, 7154, 9200, 9554, 7154, 9200, 11046, 9554, 7154, 9200, 11046, 8918, 9554, 7154, 9200, 11046, 8918, 9143, 9554, 7154, 9200, 11046, 8918, 9143, 10369, 9554, 7154, 9200, 11046, 8918, 9143, 10369, 13078, 9554, 7154, 9200, 11046, 8918, 9143, 10369, 13078, 8101, 9554, 7154, 9200, 11046, 8918, 9143, 10369, 13078, 8101, 7840, 8576, 8576, 9464, 8576, 9464, 8336, 8576, 9464, 8336, 10205, 8576, 9464, 8336, 10205, 11044, 8576, 9464, 8336, 10205, 11044, 7533, 8576, 9464, 8336, 10205, 11044, 7533, 12553, 8576, 9464, 8336, 10205, 11044, 7533, 12553, 7159, 8576, 9464, 8336, 10205, 11044, 7533, 12553, 7159, 10298, 8576, 9464, 8336, 10205, 11044, 7533, 12553, 7159, 10298, 9894, 12951, 12951, 8522, 12951, 8522, 9614, 12951, 8522, 9614, 10426, 12951, 8522, 9614, 10426, 11063, 12951, 8522, 9614, 10426, 11063, 9143, 12951, 8522, 9614, 10426, 11063, 9143, 12691, 12951, 8522, 9614, 10426, 11063, 9143, 12691, 7390, 12951, 8522, 9614, 10426, 11063, 9143, 12691, 7390, 7864, 12951, 8522, 9614, 10426, 11063, 9143, 12691, 7390, 7864, 9536, 7880, 7880, 6710, 7880, 6710, 11868, 7880, 6710, 11868, 10930, 7880, 6710, 11868, 10930, 5345, 7880, 6710, 11868, 10930, 5345, 10986, 7880, 6710, 11868, 10930, 5345, 10986, 11346, 7880, 6710, 11868, 10930, 5345, 10986, 11346, 7315, 7880, 6710, 11868, 10930, 5345, 10986, 11346, 7315, 11526, 7880, 6710, 11868, 10930, 5345, 10986, 11346, 7315, 11526, 6558, 8310, 8310, 8907, 8310, 8907, 10074, 8310, 8907, 10074, 9906, 8310, 8907, 10074, 9906, 9598, 8310, 8907, 10074, 9906, 9598, 7660, 8310, 8907, 10074, 9906, 9598, 7660, 11975, 8310, 8907, 10074, 9906, 9598, 7660, 11975, 10399, 8310, 8907, 10074, 9906, 9598, 7660, 11975, 10399, 8231, 8310, 8907, 10074, 9906, 9598, 7660, 11975, 10399, 8231, 8176, 11733, 11733, 10355, 11733, 10355, 7570, 11733, 10355, 7570, 8495, 11733, 10355, 7570, 8495, 11050, 11733, 10355, 7570, 8495, 11050, 5649, 11733, 10355, 7570, 8495, 11050, 5649, 7639, 11733, 10355, 7570, 8495, 11050, 5649, 7639, 10497, 11733, 10355, 7570, 8495, 11050, 5649, 7639, 10497, 6998, };static struct simple_udp_connection unicast_connection;
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