tested on sdk3.1.2-p13

DMac_config();

mips64-octeon-linux-gnu-strip traffic-gen
cp traffic-gen /media/sf_F_DRIVE/tftproot/traffic-gen-sdk312p13.78.dmac_pcam


Octeon ebb7800# printenv                                                        
autoload=n                                                                      
baudrate=115200                                                                 
bf=bootoct $(flash_unused_addr) forceboot numcores=$(numcores)                  
bgx_for_mix0=0x800                                                              
boardname=ebb7800                                                               
bootcmd_tftp=setenv ipaddr 192.168.16.78;setenv serverip 192.168.16.1;tftpboot 8
bootdelay=0                                                                     
bootloader_flash_update=bootloaderupdate                                        
burn_app=erase $(flash_unused_addr) +$(filesize);cp.b $(fileaddr) $(flash_unuse)
env_addr=1fbfe000                                                               
env_size=2000                                                                   
ethact=octmgmt0                                                                 
ethaddr=00:0f:b7:11:76:e3                                                       
fdtaddr=80000                                                                   
flash_base_addr=1f400000                                                        
flash_size=800000                                                               
flash_unused_addr=1f700000                                                      
flash_unused_size=500000                                                        
ipaddr=192.168.16.78                                                            
linux_mmc=fatload mmc 1 $(loadaddr) vmlinux.64;bootoctlinux $(loadaddr)         
loadaddr=0x20000000                                                             
ls=fatls mmc 0                                                                  
numcores=48                                                                     
numcoreshex=0x30                                                                
octeon_failsafe_mode=0                                                          
octeon_ram_mode=0                                                               
qlm0:0_mode=sgmii                                                               
qlm4:0_mode=xlaui                                                               
qlm5:0_mode=xlaui                                                               
qlm7:0_mode=xfi                                                                 
serial#=1620-3.0-00007                                                          
serverip=192.168.16.1                                                           
stderr=serial                                                                   
stdin=serial,pci,bootcmd                                                        
stdout=serial                                                                   
uboot_flash_addr=bf590000                                                       
uboot_flash_size=60170000                                                       
ver=U-Boot 2013.07 ( (U-BOOT build: 145, SDK version: 3.1.2-p13-610), svnversio)
                                                                                
Environment size: 1243/8188 bytes

 
 

tftpboot 0 traffic-gen-sdk312p13.78.dmac_pcam
bootoct 0 numcores=48

loopback 0x810 internal  
rx.display 0x810 on 
tx.count 0x810 1 
row 6 38 off

dest.mac [[<port range>] <address>]       Set or show the destination MAC address. Set using xx:xx:xx:xx:xx:xx.

Command   8(INS)> start 0x810
    WORD00: 0000681001010001 aura=0x0 apad=6 chan=0x810 bufs=1 style=0x1 pknd=0x1
    WORD01: 003c000008100000 len=60 grp=0x0 tt=ORDERED tag=0x08100000
    WORD02: 0220020100000000 lcty=8(IP4) lfty=17(UDP)
    WORD03: 3fca0003fab90036 size=16330 addr=0x3fab90036
    WORD04: 00002200000e0000 lcptr=14 lfptr=34
                 000fb7117ef4000f
                 b7117ef308004500
                 002e000000000411
                 a1d90a1000630a11
                 006310001001001a
                 0000616263646566
                 6768696a6b6c6d6e
                 6f707172

dest.mac 0x810 01:02:03:04:05:06

Command   9(INS)> dest.mac 0x810 01:02:03:04:05:06
Command  10(INS)> start 0x810
    WORD00: 0000681001090001 aura=0x0 apad=6 chan=0x810 bufs=1 style=0x9 pknd=0x1
    WORD01: 003c000008100000 len=60 grp=0x0 tt=ORDERED tag=0x08100000
    WORD02: 0220020100001000 lcty=8(IP4) lfty=17(UDP) L2 Multicast
    WORD03: 3fca0003fab94036 size=16330 addr=0x3fab94036
    WORD04: 00002200000e0000 lcptr=14 lfptr=34
                 010203040506000f
                 b7117ef308004500
                 002e000000000411
                 a1d90a1000630a11
                 006310001001001a
                 0000616263646566
                 6768696a6b6c6d6e
                 6f707172
Command  11(INS)> 

dest.mac 0x810 00:00:03:04:05:06

Command  12(INS)> start 0x810
    WORD00: 0000681001060001 aura=0x0 apad=6 chan=0x810 bufs=1 style=0x6 pknd=0x1
    WORD01: 003c000008100000 len=60 grp=0x0 tt=ORDERED tag=0x08100000
    WORD02: 0220020100000000 lcty=8(IP4) lfty=17(UDP)
    WORD03: 3fca0003fab98036 size=16330 addr=0x3fab98036
    WORD04: 00002200000e0000 lcptr=14 lfptr=34
                 000003040506000f
                 b7117ef308004500
                 002e000000000411
                 a1d90a1000630a11
                 006310001001001a
                 0000616263646566
                 6768696a6b6c6d6e
                 6f707172

dest.mac 0x810 01:02:00:00:00:00

Command  14(INS)> start 0x810
    WORD00: 0000681001040001 aura=0x0 apad=6 chan=0x810 bufs=1 style=0x4 pknd=0x1
    WORD01: 003c000008100000 len=60 grp=0x0 tt=ORDERED tag=0x08100000
    WORD02: 0220020100001000 lcty=8(IP4) lfty=17(UDP) L2 Multicast
    WORD03: 3fca0003fab9c036 size=16330 addr=0x3fab9c036
    WORD04: 00002200000e0000 lcptr=14 lfptr=34
                 010200000000000f
                 b7117ef308004500
                 002e000000000411
                 a1d90a1000630a11
                 006310001001001a
                 0000616263646566
                 6768696a6b6c6d6e
                 6f707172
