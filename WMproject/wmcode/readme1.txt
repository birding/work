###generate patch###
diff -uNr OCTEON-SDK.orig/bootloader/u-boot/ OCTEON-SDK.wmpatch/bootloader/u-boot/ > board20161101.patch 
diff -uNr OCTEON-SDK.orig/bootloader/u-boot/ OCTEON-SDK.wmpatch/bootloader/u-boot/ > board20161101.patch 

##source code####
mkdir -p /home/yisp/WM71
cd /home/yisp/WM71
rpm -ivh  --prefix=/home/yisp/WM71 --force ../SDK/OCTEON-SDK-3.1.2-568.i386.rpm
rpm -ivh  --prefix=/home/yisp/WM71 --force ../SDK/OCTEON-LINUX-3.1.2-568.i386.rpm 
cd OCTEON-SDK/
patch -s -p1 < ../../SDK/sdk-3.1.2-patch_release7/sdk.patch 
patch -s -p1 < ../../SDK/sdk-3.1.2-patch_release7/linux.patch 
../../SDK/sdk-3.1.2-patch_release7/octeon_copy_3_1_2_p7_files.sh ../../SDK/sdk-3.1.2-patch_release7/

source env-setup OCTEON_CN71XX
cd bootloader/u-boot/
make octeon_sff7000_config
make
make clean
make octeon_generic_cn70xx_spi_stage2_config
make
make clean; make distclean

patch -s -p1 < ../board20161101.patch 
patch -s -p1 < ../board20161101_exe.patch
  
patch -s -R -p1 < ../../../board20161101.patch 

source env-setup OCTEON_CN71XX
cd bootloader/u-boot
cd ../
cd spi-boot/
make clean
make
cp spi-boot.bin ../
 
cd ../u-boot
make clean
make octeon_generic_cn70xx_spi_stage2_config
make
cp u-boot-octeon_generic_cn70xx_spi_stage2.bin ../  
   
make clean
make octeon_sff7000_config
make
cp u-boot-octeon_sff7000.bin ../


cd .. ; rm out2.bin -f
dd if=spi-boot.bin of=out2.bin bs=1024
dd if=u-boot-octeon_generic_cn70xx_spi_stage2.bin of=out2.bin bs=1024 seek=8
dd if=u-boot-octeon_sff7000.bin of=out2.bin bs=1024 seek=1024
sync; cp out2.bin /media/sf_F_DRIVE/WM71/ -f; sync
########################


#############sff7000 demo board###

sf update $(fileaddr) 0 $(filesize)


fatload usb 0 0 u-boot-octeon_generic_cn70xx_spi_stage2.bin
sf update $(fileaddr) 0x2000 $(filesize)

fatload usb 0 0 u-boot-octeon_sff7000.bin
sf update $(fileaddr) 0x100000 $(filesize)

tftpboot 0 u-boot-octeon_sff7000.bin
###############################


PHY 
88E1512  -- RGMII   phyadd=0
88E1548  -- QSGMII  - lan0 -- phyadd=0x1c order=0
port0 - 中间下面的
MDIO0
	"marvell,88e1512",
	"marvell,88e1548",
	
setenv prompt WM7000#
setenv ipaddr 192.168.33.2
setenv serverip 192.168.33.1
setenv ethact octrgmii0


setenv octeon_debug_rx_packets 1
setenv octeon_debug_tx_packets 1

tftpboot 0 abc.out

Octeon sff7000(ram)# read64 0x0001180028000000
attempting to read from addr: 0x8001180028000000
0x8001180028000000: 0xc4040

AGL_GMX_TX(0)_STAT3
read64 0x11800E0000298

 wget https://www.kernel.org/pub/linux/kernel/v4.x/linux-4.8.5.tar.gz
 https://github.com/torvalds/linux/blob/master/drivers/net/phy/marvell.c
https://github.com/torvalds/linux/blob/master/include/linux/marvell_phy.h


PCSX(0)_CONTROL1_REG[LOOPBCK1]
read64 0x00011800B0000800 0x2840
write64 0x00011800B0000800 0x6840

mdio list
mdio-octeon0:
0 - Marvell 88E1518 <--> octrgmii0
28 - Marvell 88E15480 <--> octeth0
29 - Marvell 88E15480 <--> octeth1
30 - Marvell 88E15480 <--> octeth2
31 - Marvell 88E15480 <--> octeth3

WM7000# mdio read octrgmii0 2  
Reading from bus mdio-octeon0
PHY at address 0:
0x2 - 0x141
WM7000# mdio read octrgmii0 3
Reading from bus mdio-octeon0
PHY at address 0:
0x3 - 0xdd1

WM7000# mdio read octrgmii0 0    
Reading from bus mdio-octeon0
PHY at address 0:
0x0 - 0x1000

mdio read octrgmii0 22
mdio write octrgmii0 22 18
mdio read octrgmii0 22
mdio read octrgmii0 20

###system interface loopback
mdio read octrgmii0 0
mdio write octrgmii0 0 0x5140
mdio write octrgmii0 0 0xD140

mdio write octrgmii0 22 2
mdio read octrgmii0 21

line loopback
mdio write octrgmii0 22 2
mdio read octrgmii0 21
mdio write octrgmii0 21 0x5076
mdio write octrgmii0 22 0

serdes loopback
mdio write octrgmii0 22 1
mdio read octrgmii0 16
mdio write octrgmii0 16 0x1100
mdio write octrgmii0 22 0
#############################

cp u-boot-octeon_generic_cn70xx_spi_stage2.bin /media/sf_F_DRIVE/tftproot/
cp u-boot-octeon_sff7000.bin /media/sf_F_DRIVE/tftproot/

setenv prompt WM7000#
setenv disable_pci 1
setenv ethact octrgmii0;
 dhcp;
setenv serverip 192.168.11.214
setenv ipaddr 192.168.11.21

tftpboot 0 u-boot-octeon_sff7000.bin
sf probe; sf update $(fileaddr) 0x100000 $(filesize)


tftpboot 0 traffic-gen
bootoct 0 numcores=2

sf probe
tftpboot 0 u-boot-octeon_generic_cn70xx_spi_stage2.bin

sf update $(fileaddr) 0x2000 $(filesize)

tftpboot 0 u-boot-octeon_sff7000.bin
sf update $(fileaddr) 0x100000 $(filesize)

mdio read octeth0 22
mdio write octeth0 22 4
mdio read octeth0 22
mdio read octeth0 1

mdio write octeth0 22 3
mdio write octeth0 16 0xb0
mdio write octeth0 16 0x90

mdio write octeth0 22 4
mdio read octeth0 0-27 


Octeon sff7000(ram)# mii info
PHY 0x00: OUI = 0x5043, Model = 0x1D, Rev = 0x01, 100baseT, FDX
PHY 0x1C: OUI = 0x5043, Model = 0x2A, Rev = 0x02,  10baseT, HDX		中间下
PHY 0x1D: OUI = 0x5043, Model = 0x2A, Rev = 0x02, 1000baseT, FDX  中间上
PHY 0x1E: OUI = 0x5043, Model = 0x2A, Rev = 0x02,  10baseT, HDX
PHY 0x1F: OUI = 0x5043, Model = 0x2A, Rev = 0x02,  10baseT, HDX

Octeon sff7000(ram)# mdio write octeth0 22 4
Octeon sff7000(ram)# 
Octeon sff7000(ram)# mdio read octeth0 0-27 
Reading from bus mdio-octeon0
PHY at address 28:
0x0 - 0x1140
0x1 - 0x0
0x2 - 0x0
0x3 - 0x0
0x4 - 0x901
0x5 - 0x0
0x6 - 0x0
0x7 - 0x2001
0x8 - 0x0
0x9 - 0x0
0xa - 0x0
0xb - 0x0
0xc - 0x0
0xd - 0x3
0xe - 0x0
0xf - 0x0
0x10 - 0x6224
0x11 - 0x8000
0x12 - 0x0
0x13 - 0x0
0x14 - 0x0
0x15 - 0x0
0x16 - 0x4
0x17 - 0x0
0x18 - 0x0
0x19 - 0x0
0x1a - 0x3002
0x1b - 0x3f80

500 Mbps, Full duplex

680 Mbps, Full duplex

SGMII0: Port 2 link timeout
[cvmx_qlm_measure_clock][2089]incr_count 0x1
[cvmx_qlm_get_gbaud_mhz][1037]meas_refclock 99999736
[cvmx_qlm_get_gbaud_mhz][1042]freq 2500
[__cvmx_helper_sgmii_link_get][669]speed 2000
[__cvmx_helper_sgmii_link_get][674]speed 500
SGMII0: Port 3 link timeout



#############2016/10/30 8:45:45#########
setenv prompt WM7000#
setenv disable_pci 1
setenv ethact octrgmii0;
 dhcp;
setenv serverip 192.168.11.214
setenv ipaddr 192.168.11.21

tftpboot 0 u-boot-octeon_sff7000.bin
sf probe; sf update $(fileaddr) 0x100000 $(filesize)

cvmx_nand_initialize: NAND chip 2 has ID 0x98d3912676160800
      0000: 98d39126 76160800                                                      
    __cvmx_nand_low_level_read: returned bytes(8)
  cvmx_nand_read_id: returned bytes(8)
cvmx_nand_initialize: Chip 2 doesn't support ONFI.  ID: 0x98 0xd3
cvmx_nand_initialize:Unable to determine NAND parameters, and no defaults supplied.

Toshiba TH58NVG3S0HTAI0

cvmx_nand_initialize: Samsung NAND chip detected, using parameters decoded from ID bytes.
cvmx_nand_initialize: Defaults: page size: 4096, OOB size: 128, pages per block 64, part size: 2048 MBytes, timing mode: 2
cvmx_nand_initialize: Address cycles: 5, column bits: 12, row bits: 19, block count: 8192

octeon_nand_hw_bch_init(c01b7de8) ENTER
  ecc size: 0, ecc bytes: 0
cvmx_nand_get_oob_size: called
  cvmx_nand_get_oob_size: param chip = 2
cvmx_nand_get_oob_size: returned cvmx_nand_state[chip].oob_size(128)
octeon_nand_calc_ecc_layout(c01b7de8)
  oob size: 128, eccbytes: 0, steps: 0, bytes: 0

      __cvmx_nand_get_free_cmd_bytes: returned (int)ndf_misc.s.fr_byt(1456)
    cvmx_nand_submit: returned CVMX_NAND_SUCCESS(0)
  __cvmx_nand_build_post_cmd: returned CVMX_NAND_SUCCESS(0)
cvmx_nand_reset: returned status(0)

octeon_read_byte: offset: 0x0, returning 0x98
octeon_nand_init_size(c02c4bc0, c01b7e68, c0f11c10)
cvmx_nand_get_oob_size: called
  cvmx_nand_get_oob_size: param chip = 2
cvmx_nand_get_oob_size: returned cvmx_nand_state[chip].oob_size(128)
cvmx_nand_get_page_size: called
  cvmx_nand_get_page_size: param chip = 2
cvmx_nand_get_page_size: returned cvmx_nand_state[chip].page_size(4096)
cvmx_nand_get_pages_per_block: called
  cvmx_nand_get_pages_per_block: param chip = 2
cvmx_nand_get_pages_per_block: returned cvmx_nand_state[chip].pages_per_block(64)
NAND 2 OOB size: 128, write size: 4096, erase size: 262144
BUG: failure at nand_base.c:3305/nand_scan_tail()!
BUG!

			chip->ecc.strength =
				(chip->ecc.bytes * 8) / fls(8 * chip->ecc.size);
				
				
WM7000# nand info

Device 0: nand0, sector size 256 KiB
  Size           1024 MiB
  Page size      4096 b
  OOB size        128 b
  Erase size   262144 b