
#build for X86
[root@localhost txcsr_tool]# pwd
/home/sdk620/txcsr_tool
[root@localhost txcsr_tool]# ls
Makefile  readme.txt  txcsr  txcsr_SI  txcsr_tool
[root@localhost txcsr_tool]# make clean
[root@localhost txcsr_tool]# make

#build for ARM
make -f Makefile.ARM clean
make -f Makefile.ARM

or 

make -f Makefile.ARM CROSS_COMPILE=/home/sdk620/sdk620p3/OCTEONTX-SDK/tools/bin/aarch64-thunderx-linux-gnu-


#run on X86
only for generate the script.
    ./txcsr_x86 --listregdetail cn83xx mode2 sata

    ./txcsr_x86 --listregdetail cn83xx mode2 mpi
    
    ./txcsr_x86 --listregdetail cn83xx mode2 pko > script_pko.sh
    grep -i "./txcsr" script_pko.sh > script_tmp.sh
    grep -iv "pko_vf" script_tmp.sh > script_pko.sh
    sed -i "s/txcsr_x86/txcsr/g" script_pko.sh

    ./txcsr_x86 --listregdetail cn83xx mode2 fpa > script_tmp.sh
    grep -i "./txcsr" script_tmp.sh > script_fpa.sh
    sed -i "s/txcsr_x86/txcsr/g" script_fpa.sh
    
    ./txcsr_x86 --listregdetail cn83xx mode2 pki > script_tmp.sh
    grep -i "./txcsr" script_tmp.sh > script_pki.sh
    sed -i "s/txcsr_x86/txcsr/g" script_pki.sh

    ./txcsr_x86 --listregdetail cn83xx mode2 bgx > script_tmp.sh
    grep -i "./txcsr" script_tmp.sh > script_bgx.sh
    sed -i "s/txcsr_x86/txcsr/g" script_bgx.sh
    grep "\-a 0" script_bgx.sh > script_bgx0.sh   ####-BGX0
    grep "\-a 1" script_bgx.sh > script_bgx1.sh   ####-BGX1
            
#run on ARM
#    ./txcsr_arm --listregd2 mpi
#    ./txcsr_arm MPI_CFG -d
#    
#    ./txcsr_arm --listregd2 gser > gser.txt
#    grep "\-a 0" gser.txt > gser0.sh
#    chmod 777 gser0.sh 
#    ./gser0.sh > gser0_dump.txt
#    ls -lh gser0_dump.txt
#    -rw-r--r--    1 root     root      126.9K Oct 31 00:13 gser0_dump.txt
#    
#    ./txcsr_arm --listregd2 pko > pko.sh
#    grep -iv "pko_vf" pko.sh > pko_novf.sh
#    chmod 777 pko_novf.sh
#    time ./pko_novf.sh > pko_dump.txt
#    ~ # ls -lh pko_dump.txt 
#    -rw-r--r--    1 root     root        4.2M Oct 31 00:57 pko_dump.txt
#
#    set -x  
#     
#    ./txcsr_arm --listregd2 pko_vfx_ > pko_vf.sh
#    
#    sed -i "s/txcsr_arm/txcsr/g" pko_novf.sh
    