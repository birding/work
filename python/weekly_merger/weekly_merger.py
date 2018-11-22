
# -*- coding: utf-8 -*-

# 将多个Excel文件合并成一个

import xlrd
import xlsxwriter
import os
import sys
import datetime
from glob import glob
from os.path import join

table1_name = u"daily"
data_dir = ""


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


# https://xlsxwriter.readthedocs.io/worksheet.html
def set_format(workbook, worksheet, t_rows, t_cols):
    format_center = workbook.add_format({'font_name': 'Arial', 'font_size': 11})
    format_center.set_align('center')
    format_left = workbook.add_format({'font_name': 'Arial', 'font_size': 11})
    format_left.set_align('left')
    format_left.set_text_wrap()

    # set_column(first_col, last_col, width, cell_format, options)
    worksheet.set_column(0, 0, None, format_center)
    worksheet.set_column(1, 1, 10, format_center)
    worksheet.set_column(2, 2, 8, format_center)
    worksheet.set_column(3, 3, 15, format_left)
    worksheet.set_column(4, 4, 35, format_left)
    worksheet.set_column(5, 5, 70, format_left)
    worksheet.set_column(6, 6, None, format_center)
    # autofilter(first_row, first_col, last_row, last_col)
    worksheet.autofilter(0, 0, t_rows, t_cols)

    # set_row(row, height, cell_format, options)
    header_format = workbook.add_format({'bold': True})
    header_format.set_text_wrap()
    header_format.set_align('center')
    worksheet.set_row(0, None, header_format)


if __name__ == '__main__':
    # print sys.path[0]
    os.chdir(sys.path[0])
    all_xls = get_excel_files()
    # print all_xls

    sheet_content1 = []
    header_content1 = []
    for fl in all_xls:
        # print os.path.basename(fl)[0:7]
        if os.path.basename(fl)[0:7] == "Summary":
            # print "skip result file"
            continue
        if os.path.basename(fl)[1] == "$":
            print "skip temp file"
            continue
        print "collecting ", os.path.basename(fl), "..."

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
            # print "\tTable %s with %d rows" % (tbl.name, num)
            if tbl.name == table1_name:
                for row in range(num):
                    if row == 0:
                        if len(header_content1) == 0:
                            r_data = tbl.row_values(row)
                            header_content1.append(r_data)
                    else:
                        r_data = tbl.row_values(row)
                        sheet_content1.append(r_data)
            else:
                # print "unexpected sheet name %s in %s" % (tbl.name, os.path.basename(ui_path))
                continue
        # print "sheet counts: %d\n" % sheet_count
    # print header_content1
    # print sheet_content1

    uri = os.getcwd()
    uri = uri + data_dir
    out_file = uri + '\\Summary-'+ datetime.datetime.now().strftime('%Y%m%d%H%M%S') + '.xlsx'
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

    ws = wb1.add_worksheet(table1_name)
    for b in range(len(header_content1[0])):
        c = header_content1[0][b]
        ws.write(0, b, c)

    for a in range(len(sheet_content1)):
        for b in range(len(sheet_content1[a])):
            c = sheet_content1[a][b]
            ws.write(a+1, b, c)

    set_format(wb1, ws, len(sheet_content1), len(sheet_content1[0]))

    wb1.close()
    print "create file ", out_file, " done\n"
    exit_code()

