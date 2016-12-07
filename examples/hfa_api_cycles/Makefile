#/***********************license start***************                              
#* Copyright (c) 2008-2016 Cavium Inc. All rights reserved.
#*                                                                                 
#*                                                                                 
#* Redistribution and use in source and binary forms, with or without              
#* modification, are permitted provided that the following conditions are          
#* met:                                                                            
#                                                                                  
#*   * Redistributions of source code must retain the above copyright              
#*     notice, this list of conditions and the following disclaimer.               
#*                                                                                 
#*   * Redistributions in binary form must reproduce the above                     
#*     copyright notice, this list of conditions and the following                 
#*     disclaimer in the documentation and/or other materials provided             
#*     with the distribution.                                                      
#*                                                                                 
#*   * Neither the name of Cavium Inc. nor the names of                            
#*     its contributors may be used to endorse or promote products                 
#*     derived from this software without specific prior written                   
#*     permission.                                                                 
#                                                                                  
#* This Software, including technical data, may be subject to U.S. export  control 
#* laws, including the U.S. Export Administration Act and its  associated          
#* regulations, and may be subject to export or import  regulations in other       
#* countries.                                                                      
#                                                                                  
#* TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"        
#* AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR       
#* WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO   
#* THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR  
#* DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM       
#* SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,                
#* MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF     
#* VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR         
#* CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR           
#* PERFORMANCE OF THE SOFTWARE LIES WITH YOU.                                      
#***********************license end**************************************/         
#
#  component Makefile fragment
#


# Makefile to compile all functionl apps
# Apps are valid in their respective environment set (se|seum|kernel)
# Usage: 
#     make [target=]
#

ifeq ($(OCTEON_ROOT),)
$(error OCTEON_ROOT is not set)
endif

ifneq ($(HFAARCH), octeon)
$(error 'use top Makefile to compile apps)'
endif

ifeq ($(HFAROOT),)
$(error HFAROOT is not set)
endif

APPSDIR = $(HFAROOT)/apps/perf

BASE := $(APPSDIR)

HFA_LFLAGS += -DLOOPBACK -DMATCHCB -O2 -g -Wall

#Ideally this is needed for applications runnable on multiple
#OCTEON HFA targets

LIBS_LIST += $(HFAROOT)/lib/octeon/hfa/$(HFAENV)/libcvmhfa.a
ifeq (OCTEON_CN7, $(findstring OCTEON_CN7, $(OCTEON_MODEL)))
ifeq (OCTEON_CN70XX, $(findstring OCTEON_CN70XX, $(OCTEON_MODEL)))
HFA_LFLAGS += -DGEN3_HFA -DHFA_HWFAU 
LIBS_LIST += $(HFAROOT)/lib/octeon/pp/$(HFAENV)/libpp$(HFAABI).a
else
ifeq (OCTEON_CN71XX, $(findstring OCTEON_CN71XX, $(OCTEON_MODEL)))
HFA_LFLAGS += -DGEN3_HFA -DHFA_HWFAU
LIBS_LIST += $(HFAROOT)/lib/octeon/pp/$(HFAENV)/libpp$(HFAABI).a
else
HFA_LFLAGS += -DGEN4_HFA
LIBS_LIST += $(HFAROOT)/lib/octeon/pp/$(HFAENV)/libpph$(HFAABI).a
endif
endif
else
HFA_LFLAGS += -DGEN3_HFA -DHFA_HWFAU
LIBS_LIST += $(HFAROOT)/lib/octeon/pp/$(HFAENV)/libpp$(HFAABI).a
endif

HFA_LFLAGS +=-DH3C_TEST

HFA_LFLAGS += -I$(BASE)/../../drv/include
HFA_LFLAGS += -I$(BASE)/../../utils/app-utils
HFA_LFLAGS += -I$(BASE)/../../utils/pcap 
HFA_LFLAGS += -I$(BASE)/../../utils/cvmgz 

#KERNEL Environment
ifeq ($(HFAENV), kernel)
#Check the target
ifeq ($(target), )
HFATARGET=hfa-lnx-ctx
HFAFILE=hfa-kernel-ctx
else
HFATARGET=hfa-lnx-$(target)
HFAFILE=hfa-kernel-$(target)
endif

DEFAULT_CROSS_COMPILE = $(shell \
	if grep -q ^CONFIG_CPU_LITTLE_ENDIAN $(KERNELSRC)/../../kernel/linux/.config && \
	which mips64el-octeon-linux-gnu-gcc > /dev/null 2>&1 ; then \
		echo -n mips64el-octeon-linux-gnu-; \
	else \
		echo -n mips64-octeon-linux-gnu-; fi)

export CROSS_COMPILE ?= $(strip $(DEFAULT_CROSS_COMPILE))
export ARCH = mips

KERNELSRC := $(OCTEON_ROOT)/linux/kernel/linux
$(shell cp -rf $(BASE)/../../lib/octeon/hfa/kernel/Module.symvers $(BASE)/)

EXTRA_CFLAGS += $(HFA_LFLAGS) -DKERNEL -DH3C_TEST

obj-m := $(HFAFILE).o	

dir := $(BASE)/../../utils/cvmgz
include $(dir)/cvmgz-lib.mk

$(HFAFILE)-objs :=										\
	../../utils/pcap/pcap.o								\
	../../utils/app-utils/app-utils.o					\
	../../utils/cvmgz/cvmgz-lib.o						\
	$(HFATARGET).o

all:
	$(MAKE) -C $(KERNELSRC) SUBDIRS=$(APPSDIR) modules
clean:
	$(MAKE) -C $(KERNELSRC) SUBDIRS=$(APPSDIR) clean
	@rm -f ../../utils/app-utils/*.o
	@rm -f ../../utils/app-utils/*.ko
	@rm -f ../../utils/app-utils/app-utils.mod.c
	@rm -f ../../utils/app-utils/.*.cmd
	@rm -f ../../utils/pcap/*.o
	@rm -f ../../utils/pcap/*.ko
	@rm -f ../../utils/cvmgz/*.o
	@rm -f ../../utils/cvmgz/*.ko
	@rm -f ../../utils/cvmgz/.*.cmd
	@rm -f ../../utils/cvmgz/*.mod.c
	@rm -f ../../utils/pcap/pcap.mod.c
	@rm -f ../../utils/pcap/.*.cmd

else #SE and SE-UM 
#Check the target
ifeq ($(target), )
HFATARGET=hfa-$(HFAENV)-ctx
HFAFILE=hfa-se-ctx
else
HFATARGET=hfa-$(HFAENV)-$(target)
HFAFILE=hfa-se-$(target)
endif
CFLAGS_GLOBAL += -G0
CFLAGS_LOCAL += $(HFA_LFLAGS)

#Simple Executive Environment
ifeq ($(HFAENV),se)

default: $(HFATARGET)

include $(HFAROOT)/drv/common.mk

CC += -I../../drv/include

dir := $(APPSDIR)/../../utils/cvmgz
include $(dir)/cvmgz.mk

LDFLAGS_GLOBAL += -Wl,--no-warn-mismatch

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk

$(OBJ_DIR)/pcap.o: $(d)../../utils/pcap/pcap.c
	$(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

$(OBJ_DIR)/app-utils-pktio.o: $(d)../../utils/app-utils/app-utils-pktio.c
	$(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

$(OBJ_DIR)/app-utils.o: $(d)../../utils/app-utils/app-utils.c
	$(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

TARGET = $(HFATARGET)
OBJS = $(OBJ_DIR)/$(HFAFILE).o
OBJS += $(OBJ_DIR)/pcap.o
OBJS += $(OBJ_DIR)/app-utils-pktio.o
OBJS += $(OBJ_DIR)/app-utils.o

include $(OCTEON_ROOT)/application.mk

call:
	rm -f config/cvmx-config.h 
	rm -f $(OBJ_DIR)/$(TARGET).o
	rm -f $(OBJ_DIR)/$(TARGET).d
	rm -rf $(TARGET)
clean:
	rm -f config/cvmx-config.h 
	rm -rf $(OBJ_DIR)
	rm -rf $(TARGET)
else                                        #End of HFAENV == se
	
#Simple Executive -User mode Environment
ifeq ($(HFAENV),seum)

default: $(HFATARGET)

include $(HFAROOT)/drv/common.mk

CC += -I../../drv/include

dir := $(APPSDIR)/../../utils/cvmgz
include $(dir)/cvmgz.mk

LDFLAGS_GLOBAL += -Wl,--no-warn-mismatch

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk


$(OBJ_DIR)/hfa-malloc.o: $(d)../../utils/hfa-malloc/malloc.c
	$(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

$(OBJ_DIR)/pcap.o: $(d)../../utils/pcap/pcap.c
	$(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

$(OBJ_DIR)/app-utils-pktio.o: $(d)../../utils/app-utils/app-utils-pktio.c
	$(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

$(OBJ_DIR)/app-utils.o: $(d)../../utils/app-utils/app-utils.c
	$(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

TARGET = $(HFATARGET)
OBJS = $(OBJ_DIR)/$(HFAFILE).o
OBJS += $(OBJ_DIR)/pcap.o
OBJS += $(OBJ_DIR)/app-utils-pktio.o
OBJS += $(OBJ_DIR)/hfa-malloc.o
OBJS += $(OBJ_DIR)/app-utils.o

CFLAGS_LOCAL += -I../../utils/hfa-malloc 

include $(OCTEON_ROOT)/application.mk

call:
	rm -f config/cvmx-config.h 
	rm -f $(OBJ_DIR)/$(TARGET).o
	rm -f $(OBJ_DIR)/$(TARGET).d
	rm -rf $(TARGET)
clean:
	rm -f config/cvmx-config.h 
	rm -rf $(OBJ_DIR)
	rm -rf $(TARGET)

endif #ifeq HFAENV == seum
endif #ifeq HFAENV == se
endif #ifeq (HFAENV == kernel)
