#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {4924, 4924, 5976, 4924, 5976, 4623, 4924, 5976, 4623, 5245, 4924, 5976, 4623, 5245, 5294, 4924, 5976, 4623, 5245, 5294, 5389, 4924, 5976, 4623, 5245, 5294, 5389, 5103, 4924, 5976, 4623, 5245, 5294, 5389, 5103, 4970, 4924, 5976, 4623, 5245, 5294, 5389, 5103, 4970, 4928, 4924, 5976, 4623, 5245, 5294, 5389, 5103, 4970, 4928, 4664, 4924, 5976, 4623, 5245, 5294, 5389, 5103, 4970, 4928, 4664, 4937, 4863, 4863, 5327, 4863, 5327, 4962, 4863, 5327, 4962, 5203, 4863, 5327, 4962, 5203, 4298, 4863, 5327, 4962, 5203, 4298, 4854, 4863, 5327, 4962, 5203, 4298, 4854, 4987, 4863, 5327, 4962, 5203, 4298, 4854, 4987, 4949, 4863, 5327, 4962, 5203, 4298, 4854, 4987, 4949, 5119, 4863, 5327, 4962, 5203, 4298, 4854, 4987, 4949, 5119, 5728, 5566, 5566, 4817, 5566, 4817, 5344, 5566, 4817, 5344, 5113, 5566, 4817, 5344, 5113, 5204, 5566, 4817, 5344, 5113, 5204, 5631, 5566, 4817, 5344, 5113, 5204, 5631, 5064, 5566, 4817, 5344, 5113, 5204, 5631, 5064, 5287, 5566, 4817, 5344, 5113, 5204, 5631, 5064, 5287, 4677, 5566, 4817, 5344, 5113, 5204, 5631, 5064, 5287, 4677, 4760, 5529, 5529, 5454, 5529, 5454, 4804, 5529, 5454, 4804, 5139, 5529, 5454, 4804, 5139, 4611, 5529, 5454, 4804, 5139, 4611, 4915, 5529, 5454, 4804, 5139, 4611, 4915, 5169, 5529, 5454, 4804, 5139, 4611, 4915, 5169, 5041, 5529, 5454, 4804, 5139, 4611, 4915, 5169, 5041, 5020, 5529, 5454, 4804, 5139, 4611, 4915, 5169, 5041, 5020, 4731, 5108, 5108, 5002, 5108, 5002, 5130, 5108, 5002, 5130, 5014, 5108, 5002, 5130, 5014, 4522, 5108, 5002, 5130, 5014, 4522, 4936, 5108, 5002, 5130, 5014, 4522, 4936, 5005, 5108, 5002, 5130, 5014, 4522, 4936, 5005, 4811, 5108, 5002, 5130, 5014, 4522, 4936, 5005, 4811, 4916, 5108, 5002, 5130, 5014, 4522, 4936, 5005, 4811, 4916, 5576, 4331, 4331, 5217, 4331, 5217, 5504, 4331, 5217, 5504, 5385, 4331, 5217, 5504, 5385, 4888, 4331, 5217, 5504, 5385, 4888, 4907, 4331, 5217, 5504, 5385, 4888, 4907, 5180, 4331, 5217, 5504, 5385, 4888, 4907, 5180, 4858, 4331, 5217, 5504, 5385, 4888, 4907, 5180, 4858, 5136, 4331, 5217, 5504, 5385, 4888, 4907, 5180, 4858, 5136, 5254, 5300, 5300, 5540, 5300, 5540, 4884, 5300, 5540, 4884, 5034, 5300, 5540, 4884, 5034, 4966, 5300, 5540, 4884, 5034, 4966, 4600, 5300, 5540, 4884, 5034, 4966, 4600, 5040, 5300, 5540, 4884, 5034, 4966, 4600, 5040, 5119, 5300, 5540, 4884, 5034, 4966, 4600, 5040, 5119, 5111, 5300, 5540, 4884, 5034, 4966, 4600, 5040, 5119, 5111, 4479, 4957, 4957, 4731, 4957, 4731, 4811, 4957, 4731, 4811, 5424, 4957, 4731, 4811, 5424, 5081, 4957, 4731, 4811, 5424, 5081, 5167, 4957, 4731, 4811, 5424, 5081, 5167, 5639, 4957, 4731, 4811, 5424, 5081, 5167, 5639, 5086, 4957, 4731, 4811, 5424, 5081, 5167, 5639, 5086, 5739, 4957, 4731, 4811, 5424, 5081, 5167, 5639, 5086, 5739, 4283, 4877, 4877, 4530, 4877, 4530, 5321, 4877, 4530, 5321, 5377, 4877, 4530, 5321, 5377, 4848, 4877, 4530, 5321, 5377, 4848, 5585, 4877, 4530, 5321, 5377, 4848, 5585, 4477, 4877, 4530, 5321, 5377, 4848, 5585, 4477, 4854, 4877, 4530, 5321, 5377, 4848, 5585, 4477, 4854, 4881, 4877, 4530, 5321, 5377, 4848, 5585, 4477, 4854, 4881, 4501, 4709, 4709, 4318, 4709, 4318, 5127, 4709, 4318, 5127, 5103, 4709, 4318, 5127, 5103, 4871, 4709, 4318, 5127, 5103, 4871, 5115, 4709, 4318, 5127, 5103, 4871, 5115, 5605, 4709, 4318, 5127, 5103, 4871, 5115, 5605, 5737, 4709, 4318, 5127, 5103, 4871, 5115, 5605, 5737, 5451, };static struct simple_udp_connection unicast_connection;
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