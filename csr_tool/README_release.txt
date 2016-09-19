this oct-remote-csr is a X86 tool that can translate the CSR value
into a readable format.

1. prepare the csrdump.txt file
please check the ./csrdump.txt, the format of register dumping:
[root@localhost csr_tool]# cat csrdump.txt
CVMX_PKI_CL0_STYLE63_CFG 0x80011800445001f8 0x000000000001000f
CVMX_PKI_CL1_STYLE0_CFG 0x8001180044510000 0x000000000001000f
CVMX_GSER3_CFG 0x8001180093000080 0x0000000000000014

print example code:
cvmx_dprintf("CVMX_PKI_PKIND%d_ICGSEL 0x%16llx 0x%16llx", pkind, CVMX_PKI_PKINDX_ICGSEL(pkind), 
				cvmx_read_csr_node(node, CVMX_PKI_PKINDX_ICGSEL(pkind));

printk("CVMX_GSERX%d_CFG 0x%16llx 0x%16llx", qlm, CVMX_GSERX_CFG(qlm),
				cvmx_read_csr_node(node, CVMX_GSERX_CFG(qlm));
	
				

2.To Run:
        ./host/remote-utils/oct-remote-csr -v
        ./host/remote-utils/oct-remote-csr -h
        ./host/remote-utils/oct-remote-csr -t cn78XX -i csrdump.txt


  ./host/remote-utils/oct-gen-code  -t cn78XX -g Gser6 > gser6_reg.txt
  ./host/remote-utils/oct-gen-code -t cn78XX -i gser6_reg.txt 
  ./host/remote-utils/oct-gen-code  -t cn78XX -g bgx2 > bgx2_reg.txt
  ./host/remote-utils/oct-gen-code  -t cn78XX -i bgx2_reg.txt
