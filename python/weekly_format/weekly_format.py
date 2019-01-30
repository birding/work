#! python2
# -*- coding: utf-8 -*-

# change format
# https://xlsxwriter.readthedocs.io/working_with_data_validation.html
import xlrd
import xlsxwriter
import os


def get_excel_files():
    # f_name = "F:\\python\\weekly_format\\20181106.xlsx"
    f_name = raw_input("please input file: ")

    if not os.path.exists(f_name):
        print "is not exists"
        exit_code()
    return f_name


def exit_code():
    raw_input("press ENTER key to exit")
    exit(0)


format_center = {}
format_left = {}
format_category = {}
header_format = {}


# https://xlsxwriter.readthedocs.io/worksheet.html
def init_format_setting(workbook):
    global format_center, format_left, header_format, format_category
    format_center = workbook.add_format({'font_name': 'Arial', 'font_size': 11})
    format_center.set_align('center')
    format_left = workbook.add_format({'font_name': 'Arial', 'font_size': 11})
    format_left.set_align('left')
    format_left.set_text_wrap()
    format_category = workbook.add_format({'font_name': 'Arial', 'font_size': 11})
    format_category.set_align('left')
    header_format = workbook.add_format({'font_name': 'Arial', 'font_size': 11, 'bold': True})
    header_format.set_text_wrap()
    header_format.set_align('center')


def set_format_daily(workbook, sh_name):
    worksheet = workbook.get_worksheet_by_name(sh_name)
    # set_column(first_col, last_col, width, cell_format, options)
    worksheet.set_column(0, 0, None, format_center)  # Date
    worksheet.set_column(1, 1, 10, format_category)  # Customer
    worksheet.set_column(2, 2, 8, format_category)  # ProductLine
    worksheet.set_column(3, 3, 13, format_category)  # Project
    worksheet.set_column(4, 4, 35, format_left)  # Issue Description
    worksheet.set_column(5, 5, 70, format_left)  # Status
    worksheet.set_column(6, 6, None, format_center)  # Duration
    # set_row(row, height, cell_format, options)
    worksheet.set_row(0, None, header_format)
    worksheet.set_zoom(90)


def set_format_next(workbook, sh_name):
    worksheet = workbook.get_worksheet_by_name(sh_name)
    # set_column(first_col, last_col, width, cell_format, options)
    worksheet.set_column(0, 0, 10, format_category)  # Customer
    worksheet.set_column(1, 1, 8, format_category)  # ProductLine
    worksheet.set_column(2, 2, 13, format_category)  # Project
    worksheet.set_column(3, 3, 35, format_left)  # Issue Description
    worksheet.set_column(4, 4, 60, format_left)  # Actions
    worksheet.set_column(5, 5, 12, format_left)  # Actions
    # set_row(row, height, cell_format, options)
    worksheet.set_row(0, None, header_format)


def set_format_category(workbook, sh_name):
    worksheet = workbook.get_worksheet_by_name(sh_name)
    # set_column(first_col, last_col, width, cell_format, options)
    worksheet.set_column('A:A', 10, format_category)  # Customer
    worksheet.set_column('B:B', 8, format_category)  # ProductLine
    worksheet.set_column('C:C', 13, format_category)  # Project
    worksheet.set_column('D:D', 10)  # blank
    worksheet.set_column('E:E', 10, format_category)  # Customer
    worksheet.set_column('F:F', 8, format_category)  # ProductLine
    worksheet.set_column('G:G', 13, format_category)  # Project
    # set_row(row, height, cell_format, options)
    worksheet.set_row(0, None, header_format)


def set_format_weekly(workbook, sh_name):
    worksheet = workbook.get_worksheet_by_name(sh_name)
    # set_column(first_col, last_col, width, cell_format, options)
    worksheet.set_column('A:A', 120, format_left)


def rebuild_category_data(wb_src, wb_dst, sh_name):
    try:
        ws_s = wb_src.sheet_by_name(sh_name)
    except xlrd.biffh.XLRDError, e:
        print "didn't find " + sh_name
        exit_code()
    ws_d = wb.add_worksheet(sh_name)

    list_filter = []
    for i in range(0, 3):
        col0 = ws_s.col_values(i)
        ws_d.write_column(0, i, col0)
        # print col0
        ws_d.write_string(0, i+4, col0[0])
        col0.remove(col0[0])
        list_new = list(set(col0))
        list_new.sort()
        ws_d.write_column(1, i+4, list_new)
        list_filter.append(list_new)
        # print list_new

    # print list_filter
    ws_daily = wb.get_worksheet_by_name("daily")
    ws_daily.data_validation('B2:B1048576',  # Same as A:A
                       {'validate': 'list',
                        'source': list_filter[0]})
    ws_daily.data_validation('C2:C1048576',  # Same as A:A
                       {'validate': 'list',
                        'source': list_filter[1]})
    ws_daily.data_validation('D2:D1048576',  # Same as A:A
                       {'validate': 'list',
                        'source': list_filter[2]})
    ws_next = wb.get_worksheet_by_name("next")
    ws_next.data_validation('A2:A1048576',  # Same as A:A
                       {'validate': 'list',
                        'source': list_filter[0]})
    ws_next.data_validation('B2:B1048576',  # Same as A:A
                       {'validate': 'list',
                        'source': list_filter[1]})
    ws_next.data_validation('C2:C1048576',  # Same as A:A
                       {'validate': 'list',
                        'source': list_filter[2]})


if __name__ == '__main__':
    all_xls = get_excel_files()
    # print all_xls
    ret = os.path.splitext(all_xls)
    # print ret[0], ret[1]
    if ret[1] != ".xlsx":
        print "not an excel file"
        exit_code()
    n_name = ret[0] + '_new' + ret[1]
    print n_name
    if os.path.exists(n_name):
        print "delete previous temp file"
        os.remove(n_name)

    if not os.path.exists(all_xls):
        print "is not exists"
    try:
        fh = xlrd.open_workbook(all_xls)
    except IOError:
        print all_xls, " is opened. Please close it first!"
        exit_code()

    wb = xlsxwriter.Workbook(n_name)

    sh = fh.sheets()
    for tbl in sh:
        # print tbl.name
        if tbl.name == "category":
            continue
        ws = wb.add_worksheet(tbl.name)
        for row in range(tbl.nrows):
            r_data = tbl.row_values(row)
            ws.write_row(row, 0, r_data)

    rebuild_category_data(fh, wb, "category")
    init_format_setting(wb)
    set_format_daily(wb, "daily")
    set_format_next(wb, "next")
    set_format_category(wb, "category")
    set_format_weekly(wb, "weekly")

    # ws = wb.add_worksheet("daily")
    # ws.data_validation('A1:A1048576',  # Same as A:A
    #                    {'validate': 'list',
    #                     'source': ['open', 'high', 'close']})

    wb.close()
    exit_code()

