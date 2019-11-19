https://docs.python.org/2/library/xml.dom.minidom.html
https://docs.python.org/2/library/xml.etree.elementtree.html


1. use filesCollect_*.py to collect information of files from database.The output is a XML file
Design: this script will search the hard drive, and generate xml file.


2.use diff_AtoB.py to generate the differences from A to B. The output is a XML file.
Design: 
a.diff_AtoB.py will load A.xml and B.xml. 
b.iterate the rootdir in A.xml. 
	if rootdirA don't have a opponent in B.xml, delete rootdirA from A.xml
	if the opponent, rootdirB has static type, delete rootdirA from A.xml
b.iterate file and dir in A.xml, Assuming itemA from A and itemB from B. 
    if find itemB is equal with itemA(all the attributes are equal), then delete itemA from A.xml
    if find itemB has the same name with itemA, then delete itemA when itemA's
timestamp is older than itemB
c.Add child to rootNode.
	child name is 'difftarget', the attributes are target=B, date=current
time. 
d.write the final A.xml to diff_AtoB.xml


3.copy_diff.py: copy the files based on the diff_AtoB.xml.
Design: copy files based on diff_AtoB.xml to a temporary directory. Mostly, the
temporary dirctory is a diffDIR in a USB disk
a.checking the file status in based on diff_AtoB.xml
b.The copy should be same as syncFiles_*.py


4.sync_diff_ToDisk.py
using Disk*.xml as config file

