
octeon_modexp.tar.gz
:test modexp code in kernel mode. The modexp code is from OCTEON-CRYPTO-CORE-3.1.2-10.i386.rpm.
: based on SDK3.1.2-p13

/home/sdk312/sdk312p13/OCTEON-SDK/linux
cd kernel/linux/drivers/net/ethernet/
tar -czvf octeon_20181010_2.tar.gz  --exclude=*.o --exclude=*.cmd --exclude=*.ko octeon/


/home/sdk312/sdk312p13/OCTEON-SDK/linux
cd kernel/linux/drivers/net/ethernet/
tar xzvf octeon.tar.gz


bootoctlinux 0 numcores=48 octeon_bgx_port.threadCounts=40
echo 3 > /sys/module/octeon_bgx_port/parameters/memPoolSelection
echo 0 > /sys/module/octeon_bgx_port/parameters/modexp_disable
