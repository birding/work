the oct-csr-tool is a tool that run on x86 linux server. 

Features:
1.search all the related registers.
2.generate the dumping code
3.translate the CSR value into a readable format.


Usage:
1. dump all the registers of PEM1
a ./oct-csr-tool -t cn78XX -s -i pem1 > pem1.txt
b ./oct-csr-tool -t cn78XX -g -i pem1.txt 
		the output of this command should be a C file "cn78XX_pem1.txt.c". 
		then merge the code to your software.
c. once we get the printf result of the dumping code. copy it to 'csrdump.txt'
d ./oct-csr-tool -t cn78XX -f -i csrdump.txt


2. dump all the PCIE RC-mode configuration registers of pcierc1
a ./oct-csr-tool -t cn78XXp2 -s -i pcierc1 > pcierc1
b ./oct-csr-tool -t cn78XXp2 -g -i pcierc1
		the output of this command should be a C file "cn78XXp2_pcierc1.c". 
		then merge the code to your software.
c. once we get the printf result of the dumping code. copy it to 'csrdump.txt'
d ./oct-csr-tool -t cn78XXp2 -f -i csrdump.txt

[root@localhost csr_tool]# ./host/remote-utils/oct-csr-tool -v
./host/remote-utils/oct-csr-tool version: 1.1.7
SDK version: Cavium Inc. OCTEON SDK version 3.1.2-p7, build 591



