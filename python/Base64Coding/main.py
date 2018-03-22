
import base64, os, os.path


def base64encode(inputfile,outputfile):
    fread = open(inputfile, 'rb')
    fwrite= open(outputfile, 'wb')
    base64.encode(fread,fwrite)
    fread.close()
    fwrite.close()


def base64decode(inputfile, outputfile):
    fileread = open(inputfile, 'rb')

    # fileread.seek(0,2)
    # filesize = fileread.tell()
    # print filesize
    # fileread.seek(0, 0)
    # missing_padding = 4 - filesize % 4
    # if missing_padding != 4:
    #     fileread.close()
    #     print "file format is not right with missing padding %d" % missing_padding
    #     return

    filewrite = open(outputfile, 'wb')
    base64.decode(fileread, filewrite)
    fileread.close()
    filewrite.close()


# s1 = base64.encodestring('hello world')
# s2 = base64.decodestring(s1)
# print s1, s2


while(1):
    # f:\bgx5_reg.txt
    # f:\bgx5_reg.txt.b64
    filename = raw_input("input file:\n>>")
    if filename.strip() == '':
        print "file name(%s) is null" % filename
        continue
    if not os.path.exists(filename):
        print "file (%s) is not exist" % filename
        continue
    # print filename, os.path.basename(filename), os.path.dirname(filename)

    mode = raw_input("\n input choice:\n1.encode\n2.decode\n>>")
    # print mode
    if mode == "1" :
        dstfile = filename + ".b64"
        print dstfile
        if os.path.exists(dstfile):
            print "remove previous file"
            os.remove(dstfile)
        base64encode(filename, dstfile)
    else :
        print filename
        # if filename.endswith(".b64"):
        #     dstfile = filename + ".tmp"
        # else:
        #     print "input file (%s) is wrong" % filename
        dstfile = filename + ".org"
        print dstfile
        if os.path.exists(dstfile):
            print "remove previous file"
            os.remove(dstfile)
        base64decode(filename, dstfile)
    extcode = raw_input("\nPlease input 'q/Q' key to exit or 'Enter' key to continue\n")
    if extcode == 'q' or extcode == 'Q' :
        exit(0)
