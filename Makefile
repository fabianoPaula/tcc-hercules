CONTIKI_PROJECT = hello-world
all: $(CONTIKI_PROJECT) collect_battery collect_OLPM

#UIP_CONF_IPV6=1

CONTIKI = ../..
include $(CONTIKI)/Makefile.include
