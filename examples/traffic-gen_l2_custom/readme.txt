


Octeon ebb7804# 
dhcp
setenv serverip 10.197.1.219; tftpboot 0 traffic-gen-sdk312p7.78.noflow; bootoct 0 numcores=48



loopback 0x810 internal
rx.display 0x810 on
tx.count 0x810 1
start 0x810

    WORD00: 0000681001010001 aura=0x0 apad=6 chan=0x810 bufs=1 style=0x1 pknd=0x1
    WORD01: 003c000008100000 len=60 grp=0x0 tt=ORDERED tag=0x08100000
    WORD02: 0220020100000000 lcty=8(IP4) lfty=17(UDP)
    WORD03: 3fca0007fabcc036 size=16330 addr=0x7fabcc036
    WORD04: 00002200000e0000 lcptr=14 lfptr=34
                 000fb71173c5000f
                 b71173c408004500
                 002e000000000411
                 a1d90a1000630a11
                 006310001001001a
                 0000616263646566
                 6768696a6b6c6d6e
                 6f707172
#setup txdata
tx.size 0x810 64
tx.data[0] 0x810 0x000fb71173c5000f
tx.data[1] 0x810 0xb71173c408004500
tx.data[2] 0x810 0x002e000000000411
tx.data[3] 0x810 0xa1d90a1000630a11
tx.data[4] 0x810 0x006310001001001a
tx.data[5] 0x810 0x0000616263646566
Command  11(INS)> start 0x810
    WORD00: 0000681001010001 aura=0x0 apad=6 chan=0x810 bufs=1 style=0x1 pknd=0x1
    WORD01: 0040000008100000 len=64 grp=0x0 tt=ORDERED tag=0x08100000
    WORD02: 0220020100000000 lcty=8(IP4) lfty=17(UDP)
    WORD03: 3fca0007fac34036 size=16330 addr=0x7fac34036
    WORD04: 00002200000e0000 lcptr=14 lfptr=34
                 000fb71173c5000f
                 b71173c408004500
                 002e000000000411
                 a1d90a1000630a11
                 006310001001001a
                 0000616263646566
                 6768696a6b6c6d6e
                 6f70717273747576
                 
#4bytes-l2custom
tx.size 0x810 64
tx.data[0] 0x810 0xaabbccdd000fb711
tx.data[1] 0x810 0x73c5000fb71173c4
tx.data[2] 0x810 0x08004500002e0000
tx.data[3] 0x810 0x00000411a1d90a10
tx.data[4] 0x810 0x00630a1100631000
tx.data[5] 0x810 0x1001001a00006162
tx.data[6] 0x810 0x636465666768696a

start 0x810

    WORD00: 0000081001010001 aura=0x0 apad=0 chan=0x810 bufs=1 style=0x1 pknd=0x1
    WORD01: 0040000008100000 len=64 grp=0x0 tt=ORDERED tag=0x08100000
    WORD02: 0000000100000000
    WORD03: 3fd00007fac64030 size=16336 addr=0x7fac64030
                 aabbccdd000fb711
                 73c5000fb71173c4
                 08004500002e0000
                 00000411a1d90a10
                 00630a1100631000
                 1001001a00006162
                 636465666768696a
                 6f70717273747576

csr pki_cl0_pkind1_skip 0x0000000000000004
csr pki_cl1_pkind1_skip 0x0000000000000004
csr pki_cl2_pkind1_skip 0x0000000000000004
csr pki_cl3_pkind1_skip 0x0000000000000004
start 0x810
Command  26(INS)> start 0x810
    WORD00: 0000281001010001 aura=0x0 apad=2 chan=0x810 bufs=1 style=0x1 pknd=0x1
    WORD01: 0040000008100000 len=64 grp=0x0 tt=ORDERED tag=0x08100000
    WORD02: 0220020100000000 lcty=8(IP4) lfty=17(UDP)
    WORD03: 3fce0007fac50032 size=16334 addr=0x7fac50032
    WORD04: 0000260000120004 laptr=4 lcptr=18 lfptr=38
                 aabbccdd000fb711
                 73c5000fb71173c4
                 08004500002e0000
                 00000411a1d90a10
                 00630a1100631000
                 1001001a00006162
                 636465666768696a
                 6f70717273747576



###L2Custom_pcam_config/L2Custom_enable##
setenv serverip 10.197.1.219; tftpboot 0 traffic-gen-sdk312p7.78.noflow_l2custom; bootoct 0 numcores=48

loopback 0x810 internal
rx.display 0x810 on
tx.count 0x810 1

#4bytes-l2custom

tx.size 0x810 64
tx.data[0] 0x810 0xaabbccdd000fb711
tx.data[1] 0x810 0x73c5000fb71173c4
tx.data[2] 0x810 0x08004500002e0000
tx.data[3] 0x810 0x00000411a1d90a10
tx.data[4] 0x810 0x00630a1100631000
tx.data[5] 0x810 0x1001001a00006162
tx.data[6] 0x810 0x636465666768696a

Command  11(INS)> start 0x810
    WORD00: 0000081001060001 aura=0x0 apad=0 chan=0x810 bufs=1 style=0x6 pknd=0x1
    WORD01: 0044000008100000 len=68 grp=0x0 tt=ORDERED tag=0x08100000
    WORD02: 0000000000100000 PF4
    WORD03: 3fd00007fac10030 size=16336 addr=0x7fac10030
                 aabbccdd000fb711
                 73c5000fb71173c4
                 08004500002e0000
                 00000411a1d90a10
                 00630a1100631000
                 1001001a00006162
                 636465666768696a
                 6f70717273747576
                 bc812658
                 
Command  12(INS)> start 0x810
    WORD00: 0000281001060001 aura=0x0 apad=2 chan=0x810 bufs=1 style=0x6 pknd=0x1
    WORD01: 0044000008100000 len=68 grp=0x0 tt=ORDERED tag=0x08100000
    WORD02: 0220020100100000 lcty=8(IP4) lfty=17(UDP) PF4
    WORD03: 3fce0007fac48032 size=16334 addr=0x7fac48032
    WORD04: 0000260000120004 laptr=4 lcptr=18 lfptr=38
                 aabbccdd000fb711
                 73c5000fb71173c4
                 08004500002e0000
                 00000411a1d90a10
                 00630a1100631000
                 1001001a00006162
                 636465666768696a
                 6f70717273747576
                 bc812658
                 
                 
                                 