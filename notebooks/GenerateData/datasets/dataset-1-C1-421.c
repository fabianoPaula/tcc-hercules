#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {5402, 5402, 5242, 5402, 5242, 4908, 5402, 5242, 4908, 4929, 5402, 5242, 4908, 4929, 5479, 5402, 5242, 4908, 4929, 5479, 4669, 5402, 5242, 4908, 4929, 5479, 4669, 4596, 5402, 5242, 4908, 4929, 5479, 4669, 4596, 4899, 5402, 5242, 4908, 4929, 5479, 4669, 4596, 4899, 5303, 5402, 5242, 4908, 4929, 5479, 4669, 4596, 4899, 5303, 4625, 5402, 5242, 4908, 4929, 5479, 4669, 4596, 4899, 5303, 4625, 4497, 5353, 5353, 5124, 5353, 5124, 5502, 5353, 5124, 5502, 4960, 5353, 5124, 5502, 4960, 4648, 5353, 5124, 5502, 4960, 4648, 5449, 5353, 5124, 5502, 4960, 4648, 5449, 5094, 5353, 5124, 5502, 4960, 4648, 5449, 5094, 5375, 5353, 5124, 5502, 4960, 4648, 5449, 5094, 5375, 5325, 5353, 5124, 5502, 4960, 4648, 5449, 5094, 5375, 5325, 4679, 4814, 4814, 4632, 4814, 4632, 4785, 4814, 4632, 4785, 5190, 4814, 4632, 4785, 5190, 4863, 4814, 4632, 4785, 5190, 4863, 5017, 4814, 4632, 4785, 5190, 4863, 5017, 5158, 4814, 4632, 4785, 5190, 4863, 5017, 5158, 4724, 4814, 4632, 4785, 5190, 4863, 5017, 5158, 4724, 5558, 4814, 4632, 4785, 5190, 4863, 5017, 5158, 4724, 5558, 4989, 4713, 4713, 4851, 4713, 4851, 4867, 4713, 4851, 4867, 6006, 4713, 4851, 4867, 6006, 5605, 4713, 4851, 4867, 6006, 5605, 5146, 4713, 4851, 4867, 6006, 5605, 5146, 4485, 4713, 4851, 4867, 6006, 5605, 5146, 4485, 5146, 4713, 4851, 4867, 6006, 5605, 5146, 4485, 5146, 5316, 4713, 4851, 4867, 6006, 5605, 5146, 4485, 5146, 5316, 5330, 4367, 4367, 5152, 4367, 5152, 4614, 4367, 5152, 4614, 4805, 4367, 5152, 4614, 4805, 4648, 4367, 5152, 4614, 4805, 4648, 4620, 4367, 5152, 4614, 4805, 4648, 4620, 4849, 4367, 5152, 4614, 4805, 4648, 4620, 4849, 4848, 4367, 5152, 4614, 4805, 4648, 4620, 4849, 4848, 4966, 4367, 5152, 4614, 4805, 4648, 4620, 4849, 4848, 4966, 5519, 5236, 5236, 5191, 5236, 5191, 5346, 5236, 5191, 5346, 4447, 5236, 5191, 5346, 4447, 4904, 5236, 5191, 5346, 4447, 4904, 4617, 5236, 5191, 5346, 4447, 4904, 4617, 4689, 5236, 5191, 5346, 4447, 4904, 4617, 4689, 4944, 5236, 5191, 5346, 4447, 4904, 4617, 4689, 4944, 5037, 5236, 5191, 5346, 4447, 4904, 4617, 4689, 4944, 5037, 4446, 4574, 4574, 5531, 4574, 5531, 4964, 4574, 5531, 4964, 4288, 4574, 5531, 4964, 4288, 5213, 4574, 5531, 4964, 4288, 5213, 4820, 4574, 5531, 4964, 4288, 5213, 4820, 4845, 4574, 5531, 4964, 4288, 5213, 4820, 4845, 5561, 4574, 5531, 4964, 4288, 5213, 4820, 4845, 5561, 5748, 4574, 5531, 4964, 4288, 5213, 4820, 4845, 5561, 5748, 5091, 4807, 4807, 5303, 4807, 5303, 4492, 4807, 5303, 4492, 5297, 4807, 5303, 4492, 5297, 5688, 4807, 5303, 4492, 5297, 5688, 5198, 4807, 5303, 4492, 5297, 5688, 5198, 4409, 4807, 5303, 4492, 5297, 5688, 5198, 4409, 4991, 4807, 5303, 4492, 5297, 5688, 5198, 4409, 4991, 5354, 4807, 5303, 4492, 5297, 5688, 5198, 4409, 4991, 5354, 4086, 5035, 5035, 5396, 5035, 5396, 5589, 5035, 5396, 5589, 5464, 5035, 5396, 5589, 5464, 4776, 5035, 5396, 5589, 5464, 4776, 5679, 5035, 5396, 5589, 5464, 4776, 5679, 4832, 5035, 5396, 5589, 5464, 4776, 5679, 4832, 4639, 5035, 5396, 5589, 5464, 4776, 5679, 4832, 4639, 4415, 5035, 5396, 5589, 5464, 4776, 5679, 4832, 4639, 4415, 4713, 4827, 4827, 4854, 4827, 4854, 5383, 4827, 4854, 5383, 4478, 4827, 4854, 5383, 4478, 5223, 4827, 4854, 5383, 4478, 5223, 5223, 4827, 4854, 5383, 4478, 5223, 5223, 4986, 4827, 4854, 5383, 4478, 5223, 5223, 4986, 4640, 4827, 4854, 5383, 4478, 5223, 5223, 4986, 4640, 5299, };static struct simple_udp_connection unicast_connection;
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