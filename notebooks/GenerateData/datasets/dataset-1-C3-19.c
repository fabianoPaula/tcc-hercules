#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {5088, 5088, 4863, 5088, 4863, 4768, 5088, 4863, 4768, 4838, 5088, 4863, 4768, 4838, 4758, 5088, 4863, 4768, 4838, 4758, 4902, 5088, 4863, 4768, 4838, 4758, 4902, 5414, 5088, 4863, 4768, 4838, 4758, 4902, 5414, 5232, 5088, 4863, 4768, 4838, 4758, 4902, 5414, 5232, 5100, 5088, 4863, 4768, 4838, 4758, 4902, 5414, 5232, 5100, 4414, 5088, 4863, 4768, 4838, 4758, 4902, 5414, 5232, 5100, 4414, 4380, 5106, 5106, 4999, 5106, 4999, 4593, 5106, 4999, 4593, 5044, 5106, 4999, 4593, 5044, 4523, 5106, 4999, 4593, 5044, 4523, 5622, 5106, 4999, 4593, 5044, 4523, 5622, 4626, 5106, 4999, 4593, 5044, 4523, 5622, 4626, 5210, 5106, 4999, 4593, 5044, 4523, 5622, 4626, 5210, 4940, 5106, 4999, 4593, 5044, 4523, 5622, 4626, 5210, 4940, 5209, 5536, 5536, 5623, 5536, 5623, 5502, 5536, 5623, 5502, 5363, 5536, 5623, 5502, 5363, 4663, 5536, 5623, 5502, 5363, 4663, 5402, 5536, 5623, 5502, 5363, 4663, 5402, 4590, 5536, 5623, 5502, 5363, 4663, 5402, 4590, 5072, 5536, 5623, 5502, 5363, 4663, 5402, 4590, 5072, 4833, 5536, 5623, 5502, 5363, 4663, 5402, 4590, 5072, 4833, 5791, 5766, 5766, 4587, 5766, 4587, 4806, 5766, 4587, 4806, 4741, 5766, 4587, 4806, 4741, 5024, 5766, 4587, 4806, 4741, 5024, 5219, 5766, 4587, 4806, 4741, 5024, 5219, 5338, 5766, 4587, 4806, 4741, 5024, 5219, 5338, 4517, 5766, 4587, 4806, 4741, 5024, 5219, 5338, 4517, 5149, 5766, 4587, 4806, 4741, 5024, 5219, 5338, 4517, 5149, 5376, 4859, 4859, 5183, 4859, 5183, 4980, 4859, 5183, 4980, 5166, 4859, 5183, 4980, 5166, 4719, 4859, 5183, 4980, 5166, 4719, 5074, 4859, 5183, 4980, 5166, 4719, 5074, 5187, 4859, 5183, 4980, 5166, 4719, 5074, 5187, 5015, 4859, 5183, 4980, 5166, 4719, 5074, 5187, 5015, 5286, 4859, 5183, 4980, 5166, 4719, 5074, 5187, 5015, 5286, 4807, 4979, 4979, 4891, 4979, 4891, 5123, 4979, 4891, 5123, 4295, 4979, 4891, 5123, 4295, 5460, 4979, 4891, 5123, 4295, 5460, 4912, 4979, 4891, 5123, 4295, 5460, 4912, 4511, 4979, 4891, 5123, 4295, 5460, 4912, 4511, 4862, 4979, 4891, 5123, 4295, 5460, 4912, 4511, 4862, 5619, 4979, 4891, 5123, 4295, 5460, 4912, 4511, 4862, 5619, 4916, 4903, 4903, 4950, 4903, 4950, 5224, 4903, 4950, 5224, 5523, 4903, 4950, 5224, 5523, 5379, 4903, 4950, 5224, 5523, 5379, 5316, 4903, 4950, 5224, 5523, 5379, 5316, 4923, 4903, 4950, 5224, 5523, 5379, 5316, 4923, 4997, 4903, 4950, 5224, 5523, 5379, 5316, 4923, 4997, 4729, 4903, 4950, 5224, 5523, 5379, 5316, 4923, 4997, 4729, 4696, 4499, 4499, 4693, 4499, 4693, 5530, 4499, 4693, 5530, 5098, 4499, 4693, 5530, 5098, 5027, 4499, 4693, 5530, 5098, 5027, 4727, 4499, 4693, 5530, 5098, 5027, 4727, 4951, 4499, 4693, 5530, 5098, 5027, 4727, 4951, 4669, 4499, 4693, 5530, 5098, 5027, 4727, 4951, 4669, 4407, 4499, 4693, 5530, 5098, 5027, 4727, 4951, 4669, 4407, 5180, 5183, 5183, 5493, 5183, 5493, 4530, 5183, 5493, 4530, 5289, 5183, 5493, 4530, 5289, 5125, 5183, 5493, 4530, 5289, 5125, 5498, 5183, 5493, 4530, 5289, 5125, 5498, 4422, 5183, 5493, 4530, 5289, 5125, 5498, 4422, 4787, 5183, 5493, 4530, 5289, 5125, 5498, 4422, 4787, 4564, 5183, 5493, 4530, 5289, 5125, 5498, 4422, 4787, 4564, 4912, 5081, 5081, 5297, 5081, 5297, 5176, 5081, 5297, 5176, 5131, 5081, 5297, 5176, 5131, 5161, 5081, 5297, 5176, 5131, 5161, 4638, 5081, 5297, 5176, 5131, 5161, 4638, 4979, 5081, 5297, 5176, 5131, 5161, 4638, 4979, 5421, 5081, 5297, 5176, 5131, 5161, 4638, 4979, 5421, 4444, };static struct simple_udp_connection unicast_connection;
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