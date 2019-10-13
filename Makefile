CONTIKI_PROJECT = hercules
all: $(CONTIKI_PROJECT) hepheastus dataset-1-C1-101
APPS+=servreg-hack
APPS+=powertrace
CONTIKI = ../..

WITH_UIP6=1
UIP_CONF_IPV6=1
CFLAGS+= -DUIP_CONF_IPV6_RPL

include $(CONTIKI)/Makefile.include