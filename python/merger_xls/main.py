
# -*- coding: utf-8 -*-

# 将多个Excel文件合并成一个

import xlrd
import xlsxwriter
import os
import sys
from glob import glob
from os.path import join, dirname

table1_name = u"日报"
table2_name = u"说明"
data_dir = "\\data"


def get_excel_files():
    ret_files = []
    uri2 = os.getcwd()
    uri2 = uri2 + data_dir
    # print uri
    xlsx_files = glob(join(uri2, "*.xlsx"))
    for py in xlsx_files:
        ret_files.append(py)
    xls_files = glob(join(uri2, "*.xls"))
    for py in xls_files:
        ret_files.append(py)
    # print "xlsx_files : %s" % xlsx_files
    # print "ret_files : %s" % ret_files

    # test code
    # for py in ret_files:
    #     print os.path.basename(py)
    #     print os.path.exists(py)
    return ret_files


def exit_code():
    raw_input("press ENTER key to exit")
    exit(0)


if __name__ == '__main__':
    # print sys.path[0]
    os.chdir(sys.path[0])
    # all_xls = ["F:/python/merger_xls/1.xlsx", "F:/python/merger_xls/all.xlsx", "F:/python/merger_xls/李四.xlsx"]
    all_xls = get_excel_files()
    # print all_xls

    sheet_content1 = []
    sheet_content2 = []
    for fl in all_xls:
        # print os.path.basename(fl)
        if os.path.basename(fl) == "all.xlsx":
            # print "skip result file"
            continue
        print "collecting ", os.path.basename(fl), "..."
        # print "fl:", fl
        # ui_path = unicode(fl, "utf8")
        # print "ui_path:", ui_path
        # tmp_path = ui_path.replace("\\", '/')
        # print tmp_path,
        ui_path = fl
        # print "ui_path:", ui_path

        if not os.path.exists(ui_path):
            print "is not exists"
            continue

        try:
            fh = xlrd.open_workbook(ui_path)
        except IOError:
            print ui_path, " is opened. Please close it first!"
            exit_code()

        sheet_count = 0
        sh = fh.sheets()
        for tbl in sh:
            sheet_count += 1
            num = tbl.nrows
            print "\t%s with %d records" % (tbl.name, num)
            # print "rows %d" % num
            if tbl.name == table1_name:
                for row in range(num):
                    if row == 0:
                        continue
                    r_data = tbl.row_values(row)
                    # print r_data[0]
                    # print r_data
                    sheet_content1.append(r_data)
            elif tbl.name == table2_name:
                for row in range(num):
                    r_data = tbl.row_values(row)
                    # print r_data
                    sheet_content2.append(r_data)
            else:
                print "unexpected sheet name %s in %s" % (tbl.name, os.path.basename(ui_path))
                continue
        # print "sheet counts: %d\n" % sheet_count
    # print sheet_content1
    # print sheet_content2
    uri = os.getcwd()
    uri = uri + data_dir
    out_file = uri + '\\all.xlsx'
    # print "uri: ", uri, out_file

    try:
        if os.path.exists(out_file):
            os.remove(out_file)
            print "remove previous file"
        wb1 = xlsxwriter.Workbook(out_file)
    except IOError:
        print out_file, " is opened. Please close it first!"
        exit_code()
        # raise

    format05 = wb1.add_format()
    format05.set_num_format('yyyy/mm/dd')
    format05.set_shrink()
    format_center = wb1.add_format()
    format_center.set_align('center')
    format_left = wb1.add_format()
    format_left.set_align('left')
    format_left.set_text_wrap()

    ws = wb1.add_worksheet(table1_name)
    for a in range(len(sheet_content1)):
        for b in range(len(sheet_content1[a])):
            c = sheet_content1[a][b]
            if b == 0:
                # print c
                # print xlrd.xldate.xldate_as_datetime(c, 0)
                ws.write_datetime(a+1, b, xlrd.xldate.xldate_as_datetime(c, 0), format05)
                # ws.write(a, b, c)
            else:
                ws.write(a+1, b, c)
    # https: // xlsxwriter.readthedocs.io / worksheet.html
    # set_column(first_col, last_col, width, cell_format, options)
    ws.set_column(1, 3, None, format_center)
    ws.set_column(2, 2, 3, format_center)
    ws.set_column(4, 4, 40, format_left)
    ws.set_column(5, 5, 50, format_left)
    ws.autofilter(0, 0, len(sheet_content1), len(sheet_content1[0]))

    ws = wb1.add_worksheet(table2_name)
    for a in range(len(sheet_content2)):
        for b in range(len(sheet_content2[a])):
            c = sheet_content2[a][b]
            ws.write(a, b, c)

    wb1.close()
    print "create file ", out_file, " done\n"
    exit_code()

