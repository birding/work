
import base64, os, os.path
from glob import glob
from os.path import join, dirname
import sys

VER_STR='1.0.0'

def exit_code():
    raw_input("error occured. press ENTER key to exit")
    exit(0)


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


def generate_dest_name(c_mode, fname, n_mode, p_mode):
    if c_mode == "1":
        if n_mode == "1":
            # print os.path.basename(in_name), os.path.dirname(in_name)
            gen_name = os.path.join(os.path.dirname(fname),base64.urlsafe_b64encode(os.path.basename(fname))+".b64")
        else:
            gen_name = fname + ".b64"
    else:
        if n_mode == "1":
            name = os.path.basename(fname)
            gen_name = name[:-4]  # remove ".b64"
            try:
                gen_name = os.path.join(os.path.dirname(fname), base64.urlsafe_b64decode(gen_name))
            except TypeError:
                print "decoding file name error:" + fname
                exit_code()
        else:
            gen_name = fname[:-4] # remove ".b64"

    if os.path.exists(gen_name) and p_mode == "1":
        gen_name = gen_name + ".new"

    if os.path.exists(gen_name):
        print "remove previous file"
        os.remove(gen_name)
    return gen_name


def process_file(c_mode, fname, n_mode, p_mode):
    print fname
    dstfile = generate_dest_name(c_mode, fname, n_mode, p_mode)
    print dstfile
    if c_mode == "1":
        base64encode(fname, dstfile)
    else:
        base64decode(fname, dstfile)


# s1 = base64.encodestring('hello world')
# s2 = base64.decodestring(s1)
# print s1, s2

def help_verbose():
    print "Coding ", VER_STR
    print "for help: python " + sys.argv[0] + " -h \n"


def usage():
    print "Interactive mode: python " + sys.argv[0]
    help_str = "Commandline mode: python " + sys.argv[0] + " [input] " + " [coding mode] "
    help_str = help_str + " [coding file name] " + " [preserve file] "
    help_str = help_str + "\n\t[input]:input file/directory path"
    help_str = help_str + "\n\t[coding mode]:1.encode 2.decode"
    help_str = help_str + "\n\t[coding file name]:1.yes 0.no"
    help_str = help_str + "\n\t[preserve file]:preserve dest file if it already exist:1.yes 0.no"
    print help_str


help_verbose()
running_mode = 0 # 0: Intercative mode 1: Commandline mode
argc_num = len(sys.argv)


if argc_num == 1:
    running_mode = 0  # 0: Intercative mode
elif argc_num == 5:
    running_mode = 1  # 0: Commandline mode
    filename = sys.argv[1]
    code_mode = sys.argv[2]
    name_mode = sys.argv[3]
    preserve_mode = sys.argv[4]
else:
    help_verbose()
    usage()
    exit(0)

while(1):
    # f:\bgx5_reg.txt
    # f:\bgx5_reg.txt.b64
    if running_mode == 0:
        filename = raw_input("input file/directory path:\n>>")

    if filename.strip() == '':
        print "file name %s is null" % filename
        continue
    if not os.path.exists(filename):
        print "file %s is not exist" % filename
        continue
    # print filename, os.path.basename(filename), os.path.dirname(filename)
    # filename = unicode(filename, 'utf-8')
    if running_mode == 0:
        code_mode = raw_input("\n input choice:\n1.encode\n2.decode\n>>")
        name_mode = raw_input("\ncode the filename:\n1.yes\n0:no\n>>")
        preserve_mode = raw_input("\npreserve dest file if it already exist:\n1.yes\n0:no\n>>")

    # print mode
    if os.path.isfile(filename):
        process_file(code_mode, filename, name_mode, preserve_mode)
    elif os.path.isdir(filename):
        print "dir input ..."
        files = glob(join(filename, "*"))
        for fltmp in files:
            # print fltmp
            # fltmp = unicode(fltmp, 'utf-8')
            process_file(code_mode, fltmp, name_mode, preserve_mode)
    else:
        print "error input format", filename

    if running_mode == 0:
        extcode = raw_input("\nPlease input 'q/Q' key to exit or 'Enter' key to continue\n")
        if extcode == 'q' or extcode == 'Q' :
            exit(0)
        # extcode = raw_input("\nInput any key to exit\n")
        # exit(0)
    else:
        exit(0)
