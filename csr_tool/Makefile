OCTEON_ROOT = .
DIR=./host/remote-utils


#BOOTLOADER_FILES = ../../bootloader/u-boot/arch/mips/cpu/octeon
#BOOTLOADER_DEFINES =

CFLAGS = -g -O0 -W -Wall -Wno-unused-parameter \
	-D_GNU_SOURCE -DUSE_RUNTIME_MODEL_CHECKS=1 \
	-I $(OCTEON_ROOT)/executive \
	-I $(OCTEON_ROOT)/host/remote-utils \
	-I $(OCTEON_ROOT)/host/remote-lib 


ifndef CROSS

endif

CROSS=
CC=$(CROSS)gcc
CXX=$(CROSS)g++
STRIP=$(CROSS)strip

BINS = \
	$(DIR)/oct-remote-csr 
	

.PHONY: all
all: $(BINS)
#	- $(STRIP) $(BINS)
	- sudo chown -f root $(BINS)
ifndef OCTEON_LIQUIDIO
	- sudo chmod -f +s,g+r,o+r $(BINS)
endif


CVMX_FILES = $(OCTEON_ROOT)/executive/cvmx-csr-db.c \
		 $(OCTEON_ROOT)/executive/cvmx-csr-db-support.c
		 


$(DIR)/oct-remote-csr: $(DIR)/oct-remote-csr.c $(CVMX_FILES)
	$(CC) $(CFLAGS) -o $@ $^ 
	

.PHONY: clean
clean:
	rm -f $(BINS)
