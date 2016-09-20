the oct-csr-tool is a X86 tool. 

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
a ./oct-csr-tool -t cn78XX -s -i pcierc1 > pcierc1.txt
b ./oct-csr-tool -t cn78XX -g -i pcierc1.txt  
		the output of this command should be a C file "cn78XX_pcierc1.txt.c". 
		then merge the code to your software.
c. once we get the printf result of the dumping code. copy it to 'csrdump.txt'
d ./oct-csr-tool -t cn78XX -f -i csrdump.txt

[root@localhost csr_tool]# ./host/remote-utils/oct-csr-tool -v
./host/remote-utils/oct-csr-tool version: 1.1.1
SDK version: Cavium Inc. OCTEON SDK version 3.1.2-p6, build 587



