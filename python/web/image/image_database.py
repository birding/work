
import os
import linecache
from glob import glob
from os.path import join, dirname


class ImageDB:
    DIR_FILE = ''
    PICS_FILE = ''
    gl_basedir = ''
    total_pics = 0

    def __init__(self):
        self.DIR_FILE = ''
        self.PICS_FILE = ''
        self.total_pics = 0

    def gen_pics_config_file(self):

        if self.DIR_FILE.strip() == '':
            return -1;
        if self.PICS_FILE.strip() == '':
            return -1;

        if os.path.exists(self.DIR_FILE):
            cache_data = linecache.getlines(self.DIR_FILE)
            #linecache.clearcache()

            pics_config = open(self.PICS_FILE, 'w')
            for dirname in cache_data:
                dirname = dirname.strip('\n')
                if not os.path.exists(dirname):
                    continue

                print "dirName : %s" % dirname
                print "dirName2 : %s" % join(dirname, "*.jpg")
                pics = glob(join(dirname, "*.jpg"))
                # print "pics : %s" % pics

                self.total_pics += len(pics)
                print "pic count : %d %d" % (len(pics), self.total_pics)

                sample_list = [line + '\n' for line in pics]
                # print "sample_list : %s" % sample_list

                pics_config.writelines(sample_list)
            pics_config.close()

    def read_uri(self, line):
        if self.PICS_FILE.strip() == '':
            return -1, "static/images/pic_end.jpg";
        if not os.path.exists(self.PICS_FILE):
            return -1, "static/images/pic_oops.jpg";
        if 0 == len(self.PICS_FILE):
            return -1, "static/images/pic_oops.jpg"

        cache_data = linecache.getline(self.PICS_FILE, line)
        uri = cache_data.strip('\n')
        # print "line %d " % line
        # print " uri : %s " % uri
        if os.path.exists(uri):
            return line, uri
        else:
            print "file is not found"
            return 0, "static/images/pic_notfound.jpg"
