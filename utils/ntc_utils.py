#
# Copyright (c) 2015 - present LibDriver All rights reserved
#
# The MIT License (MIT)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

import math
import pandas as pd
import numpy as np
import pyqtgraph as pg
import qdarktheme
import typer
import json
import sys

from enum import Enum
from pathlib import Path
from typing import Annotated
from PySide6.QtCore import QRect, QMetaObject, QUrl
from PySide6.QtGui import QIcon, QFont, QPixmap, QDesktopServices
from PySide6.QtWidgets import QApplication, QWidget, QPushButton, QComboBox, QLabel, QLineEdit, \
    QToolButton, QFrame, QMenuBar, QMainWindow, QFileDialog, QMessageBox
from datetime import datetime

def load_excel(file_name) :
    """
    load excel file
    :param file_name: excel file name
    :return: temperature list and resistance list
    """

    # read the ntc table sheet
    sheet = pd.read_excel(file_name, sheet_name = 'ntc table')

    # create a temperature list
    temperature_list = []

    # create a resistance list
    resistance_list = []

    # make the list
    for row in sheet.iterrows() :
        # add to temperature list
        temperature_list.append(float(row[1].temperature_c))

        # add to resistance list
        resistance_list.append(float(row[1].resistance_kohm) * 1000.0)

    # check the list
    for i in range(len(temperature_list) - 1) :
        if temperature_list[i + 1] < temperature_list[i] :
            raise FloatingPointError(f'temperature[{i + 1}]({temperature_list[i + 1]}) is invalid', i)

    # check the list
    for i in range(len(resistance_list) - 1) :
        if resistance_list[i + 1] > resistance_list[i] :
            raise FloatingPointError(f'resistance[{i + 1}]({resistance_list[i + 1]}) is invalid')

    # return the temperature and resistance list
    return temperature_list, resistance_list

def write_header_file(file_name, temperature_list, resistance_list) :
    """
    write c header
    :param file_name: file name
    :param temperature_list: the list of temperature
    :param resistance_list:  the list of resistance
    :return: none
    """

    # init table content
    table_content = f"""
/**
 * @brief ntc table size definition
 */
#define {file_name.upper()}_SIZE        {len(temperature_list)}

/**
 * @brief ntc table definition
 */
"""

    # set ntc table
    table_content = table_content + f'static const ntc_table_t gsc_ntc_table[{file_name.upper()}_SIZE] = \n'

    # add {
    table_content = table_content + '{\n'

    # fill the table
    for index in range(len(temperature_list)):
        table_content = table_content + "    [{}].degrees_celsius = {:.1f},\n".format(index, temperature_list[index])
        table_content = table_content + "    [{}].ohm = {:f},\n".format(index, resistance_list[index])

    # add }
    table_content = table_content + '};'

    # get the data time in format 1
    formatted1 = datetime.now().strftime("%Y-%m-%d")

    # get the data time in format 2
    formatted2 = datetime.now().strftime("%Y/%m/%d")

    # make the header content
    header_content = f"""/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      {file_name.lower()}.h
 * @brief     {file_name.lower().replace('_', ' ')} header file
 * @version   1.0.0
 * @author    LibDriver NTC Table Python Generator
 * @date      {formatted1}
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author                                <th>Description
 * <tr><td>{formatted2}  <td>1.0      <td>LibDriver NTC Table Python Generator  <td>first upload
 * </table>
 */

#ifndef {file_name.upper()}_H
#define {file_name.upper()}_H

#include "driver_ntc.h"

#ifdef __cplusplus
extern "C"{{
#endif

/**
 * @defgroup ntc_table_driver ntc table driver function
 * @brief    ntc table driver modules
 * @ingroup  ntc_driver
 * @{{
 */
{table_content} 

/**
 * @}}
 */

#ifdef __cplusplus
}}
#endif

#endif
"""

    # output the file
    with open(f"{file_name}.h", "w", encoding = "utf-8") as f :
        f.write(header_content)

def calculate_steinhart_hart(r1, t1_c, r2, t2_c, r3, t3_c) :
    """
    calculate steinhart hart coefficient
    :param r1: r1
    :param t1_c: temperature of r1
    :param r2: r2
    :param t2_c: temperature of r2
    :param r3: r3
    :param t3_c: temperature of r3
    :return: a, b and c
    """

    # concert
    t1 = t1_c + 273.15
    t2 = t2_c + 273.15
    t3 = t3_c + 273.15

    # get l
    l1 = np.log(r1)
    l2 = np.log(r2)
    l3 = np.log(r3)

    # get m
    m = np.array([
        [1, l1, l1 ** 3],
        [1, l2, l2 ** 3],
        [1, l3, l3 ** 3]
    ])

    # get y
    y = np.array([1 / t1, 1 / t2, 1 / t3])

    # get a, b and c
    a, b, c = np.linalg.solve(m, y)

    # return a, b and c
    return a, b, c

class MainWindow(object) :
    """
    main window class
    """

    # init param
    output_name = 'driver_ntc_table.h'
    excel_file_path = 'ntc_table_template.xlsx'
    excel_temperature_list = []
    excel_resistance_list = []

    def __init__(self) :
        """
        init window
        """

        self.menubar = None
        self.t0_combobox = None
        self.t1_combobox = None
        self.t2_combobox = None
        self.t2_label = None
        self.t1_label = None
        self.t0_label = None
        self.language_label = None
        self.line1 = None
        self.line2 = None
        self.c_label = None
        self.b_label = None
        self.a_label = None
        self.calculate_push_button = None
        self.generate_push_button = None
        self.line_edit = None
        self.tool_button = None
        self.plot_view = None
        self.steinhart_hart_c_line_edit = None
        self.steinhart_hart_b_line_edit = None
        self.steinhart_hart_a_line_edit = None
        self.icon_label = None
        self.import_button = None
        self.central_widget = None
        self.language_combobox = None

    def setup_ui(self, main_window) :
        """
        setup ui
        :param main_window: main window
        :return: handle
        """

        # set main window name
        if not main_window.objectName() :
            main_window.setObjectName(u"MainWindow")

        # set window size
        main_window.setFixedSize(800, 600)

        # set icon
        icon = QIcon.fromTheme(QIcon.ThemeIcon.DocumentProperties)
        main_window.setWindowIcon(icon)

        # set widget
        self.central_widget = QWidget(main_window)
        self.central_widget.setObjectName(u"centralWidget")

        # import button
        self.import_button = QPushButton(self.central_widget)
        self.import_button.setObjectName(u"importButton")
        self.import_button.setGeometry(QRect(20, 90, 181, 51))
        self.import_button.clicked.connect(self.import_button_clicked)

        # language comboBox
        self.language_combobox = QComboBox(self.central_widget)
        for i in range(6):
            self.language_combobox.addItem("")
        self.language_combobox.setObjectName(u"languageComboBox")
        self.language_combobox.setGeometry(QRect(640, 48, 111, 25))
        self.language_combobox.currentIndexChanged.connect(self.language_combobox_changed)

        # icon label
        self.icon_label = QLabel(self.central_widget)
        self.icon_label.setObjectName(u"iconLabel")
        self.icon_label.setGeometry(QRect(20, 25, 180, 35))
        pixmap = QPixmap("resource/logo.svg")
        self.icon_label.setPixmap(pixmap)
        self.icon_label.setScaledContents(True)

        # steinhart-hart a
        self.steinhart_hart_a_line_edit = QLineEdit(self.central_widget)
        self.steinhart_hart_a_line_edit.setObjectName(u"steinhartHartAlineEdit")
        self.steinhart_hart_a_line_edit.setGeometry(QRect(20, 380, 180, 25))

        # steinhart-hart b
        self.steinhart_hart_b_line_edit = QLineEdit(self.central_widget)
        self.steinhart_hart_b_line_edit.setObjectName(u"steinhartHartBLineEdit")
        self.steinhart_hart_b_line_edit.setGeometry(QRect(20, 450, 180, 25))

        # steinhart-hart c
        self.steinhart_hart_c_line_edit = QLineEdit(self.central_widget)
        self.steinhart_hart_c_line_edit.setObjectName(u"steinhartHartCLineEdit")
        self.steinhart_hart_c_line_edit.setGeometry(QRect(20, 520, 180, 25))

        # plot view
        self.plot_view = pg.PlotWidget(self.central_widget)
        self.plot_view.setObjectName(u"plotView")
        self.plot_view.setGeometry(QRect(220, 90, 541, 461))
        self.plot_view.setBackground('default')
        self.plot_view.setTitle("Temperature Dependence of Electrical Resistance")
        self.plot_view.setLabel('left', 'Resistance(kΩ)')
        self.plot_view.setLabel('bottom', 'Temperature(℃)')
        self.plot_view.showGrid(x = True, y = True)

        # tool button
        self.tool_button = QToolButton(self.central_widget)
        self.tool_button.setObjectName(u"toolButton")
        self.tool_button.setGeometry(QRect(170, 184, 31, 31))
        self.tool_button.clicked.connect(self.tool_button_clicked)

        # line edit
        self.line_edit = QLineEdit(self.central_widget)
        self.line_edit.setObjectName(u"lineEdit")
        self.line_edit.setGeometry(QRect(20, 184, 141, 31))
        self.line_edit.setText(self.output_name)

        # generate push button
        self.generate_push_button = QPushButton(self.central_widget)
        self.generate_push_button.setObjectName(u"generatePushButton")
        self.generate_push_button.setGeometry(QRect(20, 230, 180, 35))
        self.generate_push_button.clicked.connect(self.generate_button_clicked)

        # calculate push button
        self.calculate_push_button = QPushButton(self.central_widget)
        self.calculate_push_button.setObjectName(u"calculatePushButton")
        self.calculate_push_button.setGeometry(QRect(20, 305, 180, 35))
        self.calculate_push_button.clicked.connect(self.calculate_button_clicked)

        # label a
        self.a_label = QLabel(self.central_widget)
        self.a_label.setObjectName(u"labelA")
        self.a_label.setGeometry(QRect(20, 350, 131, 19))

        # label b
        self.b_label = QLabel(self.central_widget)
        self.b_label.setObjectName(u"labelB")
        self.b_label.setGeometry(QRect(20, 415, 131, 19))

        # label b
        self.c_label = QLabel(self.central_widget)
        self.c_label.setObjectName(u"labelC")
        self.c_label.setGeometry(QRect(20, 485, 131, 19))

        # line2
        self.line2 = QFrame(self.central_widget)
        self.line2.setObjectName(u"line2")
        self.line2.setGeometry(QRect(20, 280, 180, 5))
        self.line2.setFrameShape(QFrame.Shape.HLine)
        self.line2.setFrameShadow(QFrame.Shadow.Sunken)

        # line1
        self.line1 = QFrame(self.central_widget)
        self.line1.setObjectName(u"line1")
        self.line1.setGeometry(QRect(20, 158, 180, 5))
        self.line1.setFrameShape(QFrame.Shape.HLine)
        self.line1.setFrameShadow(QFrame.Shadow.Sunken)

        # language label
        self.language_label = QLabel(self.central_widget)
        self.language_label.setObjectName(u"languageLabel")
        pixmap = QPixmap("resource/language.svg")
        self.language_label.setPixmap(pixmap)
        self.language_label.setScaledContents(True)
        self.language_label.setGeometry(QRect(610, 50, 21, 21))

        # t0 label
        self.t0_label = QLabel(self.central_widget)
        self.t0_label.setObjectName(u"t0Label")
        self.t0_label.setGeometry(QRect(220, 50, 25, 19))

        # t1 label
        self.t1_label = QLabel(self.central_widget)
        self.t1_label.setObjectName(u"t1Label")
        self.t1_label.setGeometry(QRect(340, 50, 25, 19))

        # t2 label
        self.t2_label = QLabel(self.central_widget)
        self.t2_label.setObjectName(u"t2Label")
        self.t2_label.setGeometry(QRect(460, 50, 25, 19))

        # t0 combobox
        self.t0_combobox = QComboBox(self.central_widget)
        self.t0_combobox.addItem("")
        self.t0_combobox.setObjectName(u"t0ComboBox")
        self.t0_combobox.setGeometry(QRect(250, 48, 70, 25))

        # t1 combobox
        self.t1_combobox = QComboBox(self.central_widget)
        self.t1_combobox.addItem("")
        self.t1_combobox.setObjectName(u"t1ComboBox")
        self.t1_combobox.setGeometry(QRect(370, 48, 70, 25))

        # t2 combobox
        self.t2_combobox = QComboBox(self.central_widget)
        self.t2_combobox.addItem("")
        self.t2_combobox.setObjectName(u"t2ComboBox")
        self.t2_combobox.setGeometry(QRect(490, 48, 70, 25))

        # set widget
        main_window.setCentralWidget(self.central_widget)

        # menubar
        self.menubar = QMenuBar(main_window)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 798, 25))
        main_window.setMenuBar(self.menubar)

        # translate
        main_window.setWindowTitle(u"LibDriver NTC Tool")

        # read data
        try :
            with open("config.json", "r", encoding = "utf-8") as f :
                content = json.load(f)
                index = content.get("language")
                if index > 5 :
                    index = 5
                self.language_combobox.setCurrentIndex(index)
        except FileNotFoundError :
            with open("config.json", "w", encoding = "utf-8") as f :
                data = {"language": 0}
                json.dump(data, f, ensure_ascii = False, indent = 4)
                self.language_combobox.setCurrentIndex(0)

        self.retranslate_ui()
        QMetaObject.connectSlotsByName(main_window)

    def retranslate_ui(self) :
        """
        retranslate ui
        :return: handle
        """

        if self.language_combobox.currentIndex() == 0 :
            self.import_button.setText(u"Import Excel")
            font = QFont(["PingFang SC", "Microsoft YaHei", "WenQuanYi Micro Hei", "sans-serif"], 8)
            font.setBold(True)
            self.generate_push_button.setText(u"Generate C Header")
            self.calculate_push_button.setText(u"Calculate Steinhart-Hart")
            self.calculate_push_button.setFont(font)
            self.plot_view.setTitle("Temperature Dependence of Electrical Resistance")
            self.plot_view.setLabel('left', 'Resistance(kΩ)')
            self.plot_view.setLabel('bottom', 'Temperature(℃)')
        elif self.language_combobox.currentIndex() == 1 :
            self.import_button.setText(u"导入Excel")
            font = QFont(["PingFang SC", "Microsoft YaHei", "WenQuanYi Micro Hei", "sans-serif"], 8)
            font.setBold(True)
            self.generate_push_button.setText(u"生成C头文件")
            self.calculate_push_button.setText(u"计算Steinhart-Hart系数")
            self.calculate_push_button.setFont(font)
            self.plot_view.setTitle("电阻-温度特性")
            self.plot_view.setLabel('left', '电阻(kΩ)')
            self.plot_view.setLabel('bottom', '温度(℃)')
        elif self.language_combobox.currentIndex() == 2 :
            self.import_button.setText("匯入Excel")
            font = QFont(["PingFang SC", "Microsoft YaHei", "WenQuanYi Micro Hei", "sans-serif"], 8)
            font.setBold(True)
            self.generate_push_button.setText(u"生成C頭檔案")
            self.calculate_push_button.setText(u"計算Steinhart-Hart係數")
            self.calculate_push_button.setFont(font)
            self.plot_view.setTitle("電阻-溫度特性")
            self.plot_view.setLabel('left', '電阻(kΩ)')
            self.plot_view.setLabel('bottom', '溫度(℃)')
        elif self.language_combobox.currentIndex() == 3 :
            self.import_button.setText(u"Excelをインポート")
            font = QFont(["PingFang SC", "Microsoft YaHei", "WenQuanYi Micro Hei", "sans-serif"], 7)
            font.setBold(True)
            self.generate_push_button.setText(u"Cヘッダーを生成する")
            self.calculate_push_button.setText(u"スタインハート・ハートを計算する")
            self.calculate_push_button.setFont(font)
            self.plot_view.setTitle("電気抵抗の温度依存性")
            self.plot_view.setLabel('left', '抵抗(kΩ)')
            self.plot_view.setLabel('bottom', '温度(℃)')
        elif self.language_combobox.currentIndex() == 4 :
            self.import_button.setText(u"Excel importieren")
            font = QFont(["PingFang SC", "Microsoft YaHei", "WenQuanYi Micro Hei", "sans-serif"], 8)
            font.setBold(True)
            self.generate_push_button.setText(u"C-Header generieren")
            self.calculate_push_button.setText(u"Steinhart-Hart berechnen")
            self.calculate_push_button.setFont(font)
            self.plot_view.setTitle("Temperaturabhängigkeit des elektrischen Widerstands")
            self.plot_view.setLabel('left', 'Widerstand(kΩ)')
            self.plot_view.setLabel('bottom', 'Temperatur(℃)')
        else :
            self.import_button.setText(u"엑셀 가져오기")
            font = QFont(["PingFang SC", "Microsoft YaHei", "WenQuanYi Micro Hei", "sans-serif"], 7)
            font.setBold(True)
            self.generate_push_button.setText(u"C 헤더 생성")
            self.calculate_push_button.setText(u"스테인하트-하트 지수를 계산하세요")
            self.calculate_push_button.setFont(font)
            self.plot_view.setTitle("전기 저항의 온도 의존성")
            self.plot_view.setLabel('left', '저항(kΩ)')
            self.plot_view.setLabel('bottom', '온도(℃)')

        # language
        self.language_combobox.setItemText(0, u"English")
        self.language_combobox.setItemText(1, u"简体中文")
        self.language_combobox.setItemText(2, u"繁體中文")
        self.language_combobox.setItemText(3, u"日本語")
        self.language_combobox.setItemText(4, u"Deutsch")
        self.language_combobox.setItemText(5, u"한국어")

        # name
        self.tool_button.setText(u"...")
        self.a_label.setText(u"Steinhart-Hart A")
        self.b_label.setText(u"Steinhart-Hart B")
        self.c_label.setText(u"Steinhart-Hart C")
        self.t0_label.setText(u"T0")
        self.t1_label.setText(u"T1")
        self.t2_label.setText(u"T2")

    def import_button_clicked(self) :
        """
        import button clicked
        :return: none
        """

        # choose file path
        self.excel_file_path, _ = QFileDialog.getOpenFileName(
            None,
            "",
            "",
            "Microsoft Excel File (*.xlsx)"
        )

        # set the file name
        if self.excel_file_path :
            try:
                # load excel
                self.excel_temperature_list, self.excel_resistance_list = load_excel(self.excel_file_path)

                # print info
                typer.secho(f"excel file path: {self.excel_file_path}", fg = typer.colors.WHITE)

                # init null
                excel_resistance_display_list = []

                # clear all
                self.t0_combobox.clear()
                self.t1_combobox.clear()
                self.t2_combobox.clear()

                # make display line and combobox data
                for i in range(len(self.excel_resistance_list)) :
                    excel_resistance_display_list.append(self.excel_resistance_list[i] / 1000)
                    temp_str = f"{self.excel_temperature_list[i]}"
                    self.t0_combobox.addItem(temp_str)
                    self.t1_combobox.addItem(temp_str)
                    self.t2_combobox.addItem(temp_str)

                # set the default display
                self.t1_combobox.setCurrentIndex(self.t1_combobox.count() / 2)
                self.t2_combobox.setCurrentIndex(self.t1_combobox.count() - 1)

                # fill the data
                pen = pg.mkPen(color = 'y', width = 3)
                self.plot_view.clear()
                self.plot_view.addLegend(offset = (325, 20))
                self.plot_view.plot(self.excel_temperature_list, excel_resistance_display_list,
                                    pen = pen, symbol = 'o', symbolSize = 5, symbolBrush = 'r',
                                    name = "Excel")
            except FileNotFoundError :
                # print error
                typer.secho("file not found", fg = typer.colors.RED, err = True)

                # info the message
                if self.language_combobox.currentIndex() == 0 :
                    QMessageBox.critical(None, "Error", "File not found")
                elif self.language_combobox.currentIndex() == 1 :
                    QMessageBox.critical(None, "错误", "未发现文件")
                elif self.language_combobox.currentIndex() == 2 :
                    QMessageBox.critical(None, "錯誤", "未發現文件")
                elif self.language_combobox.currentIndex() == 3 :
                    QMessageBox.critical(None, "エラー", "ファイルが見つかりません")
                elif self.language_combobox.currentIndex() == 4 :
                    QMessageBox.critical(None, "Fehler", "Datei nicht gefunden")
                else :
                    QMessageBox.critical(None, "오류", "파일을 찾을 수 없습니다.")
            except ValueError :
                # print error
                typer.secho("xlsx table name is not valid", fg = typer.colors.RED, err = True)

                # info the message
                if self.language_combobox.currentIndex() == 0 :
                    QMessageBox.critical(None, "Error", "xlsx table name is not valid")
                elif self.language_combobox.currentIndex() == 1 :
                    QMessageBox.critical(None, "错误", "xlsx表格名字非法")
                elif self.language_combobox.currentIndex() == 2 :
                    QMessageBox.critical(None, "錯誤", "xlsx表格名稱非法")
                elif self.language_combobox.currentIndex() == 3 :
                    QMessageBox.critical(None, "エラー", "xlsxテーブル名は無効です")
                elif self.language_combobox.currentIndex() == 4 :
                    QMessageBox.critical(None, "Fehler", "Der Name der XLSX-Tabelle ist ungültig.")
                else :
                    QMessageBox.critical(None, "오류", "xlsx 테이블 이름이 유효하지 않습니다.")
            except AttributeError :
                # print error
                typer.secho("xlsx format is error", fg = typer.colors.RED, err = True)

                # info the message
                if self.language_combobox.currentIndex() == 0 :
                    QMessageBox.critical(None, "Error", "xlsx format is error")
                elif self.language_combobox.currentIndex() == 1 :
                    QMessageBox.critical(None, "错误", "xlsx格式错误")
                elif self.language_combobox.currentIndex() == 2 :
                    QMessageBox.critical(None, "錯誤", "xlsx格式錯誤")
                elif self.language_combobox.currentIndex() == 3 :
                    QMessageBox.critical(None, "エラー", "xlsx形式にエラーがあります")
                elif self.language_combobox.currentIndex() == 4 :
                    QMessageBox.critical(None, "Fehler", "Das XLSX-Format ist fehlerhaft.")
                else :
                    QMessageBox.critical(None, "오류", "xlsx 형식이 오류입니다")
            except FloatingPointError as e :
                # print error
                typer.secho(f"{e}", fg = typer.colors.RED, err = True)

                # info the message
                if self.language_combobox.currentIndex() == 0 :
                    QMessageBox.critical(None, "Error", f"{e}")
                elif self.language_combobox.currentIndex() == 1 :
                    QMessageBox.critical(None, "错误", f"{e}")
                elif self.language_combobox.currentIndex() == 2 :
                    QMessageBox.critical(None, "錯誤", f"{e}")
                elif self.language_combobox.currentIndex() == 3 :
                    QMessageBox.critical(None, "エラー", f"{e}")
                elif self.language_combobox.currentIndex() == 4 :
                    QMessageBox.critical(None, "Fehler", f"{e}")
                else :
                    QMessageBox.critical(None, "오류", f"{e}")

    def tool_button_clicked(self) :
        """
        tool button clicked
        :return: none
        """

        # choose file path
        file_path, _ = QFileDialog.getOpenFileName(
            None,
            "",
            self.output_name,
            "C Header (*.h)"
        )

        # set output name
        if file_path :
            self.output_name = file_path.split("/")[-1]
            self.line_edit.setText(self.output_name)

            # print info
            typer.secho(f"output name: {self.output_name}", fg = typer.colors.WHITE)

    def generate_button_clicked(self) :
        """
        generate button clicked
        :return: none
        """

        if len(self.excel_temperature_list) > 0 and len(self.excel_resistance_list) > 0 :
            try :
                # get output name
                self.output_name = self.line_edit.text()

                # if including the .h and remove
                self.output_name = self.output_name.replace(".h", "")

                # write the c header file
                write_header_file(self.output_name, self.excel_temperature_list, self.excel_resistance_list)

                # print info
                typer.secho("success", fg = typer.colors.GREEN)

                # open the folder
                current_dir = Path(__file__).resolve().parent
                QDesktopServices.openUrl(QUrl.fromLocalFile(str(current_dir)))
            except FileNotFoundError :
                # print error
                typer.secho("file not found", fg = typer.colors.RED, err = True)

                # info the message
                if self.language_combobox.currentIndex() == 0 :
                    QMessageBox.critical(None, "Error", "File not found")
                elif self.language_combobox.currentIndex() == 1 :
                    QMessageBox.critical(None, "错误", "未发现文件")
                elif self.language_combobox.currentIndex() == 2 :
                    QMessageBox.critical(None, "錯誤", "未發現文件")
                elif self.language_combobox.currentIndex() == 3 :
                    QMessageBox.critical(None, "エラー", "ファイルが見つかりません")
                elif self.language_combobox.currentIndex() == 4 :
                    QMessageBox.critical(None, "Fehler", "Datei nicht gefunden")
                else :
                    QMessageBox.critical(None, "오류", "파일을 찾을 수 없습니다.")
            except ValueError :
                # print error
                typer.secho("xlsx table name is not valid", fg = typer.colors.RED, err = True)

                # info the message
                if self.language_combobox.currentIndex() == 0 :
                    QMessageBox.critical(None, "Error", "xlsx table name is not valid")
                elif self.language_combobox.currentIndex() == 1 :
                    QMessageBox.critical(None, "错误", "xlsx表格名字非法")
                elif self.language_combobox.currentIndex() == 2 :
                    QMessageBox.critical(None, "錯誤", "xlsx表格名稱非法")
                elif self.language_combobox.currentIndex() == 3 :
                    QMessageBox.critical(None, "エラー", "xlsxテーブル名は無効です")
                elif self.language_combobox.currentIndex() == 4 :
                    QMessageBox.critical(None, "Fehler", "Der Name der XLSX-Tabelle ist ungültig.")
                else :
                    QMessageBox.critical(None, "오류", "xlsx 테이블 이름이 유효하지 않습니다.")
            except AttributeError :
                # print error
                typer.secho("xlsx format is error", fg = typer.colors.RED, err = True)

                # info the message
                if self.language_combobox.currentIndex() == 0 :
                    QMessageBox.critical(None, "Error", "xlsx format is error")
                elif self.language_combobox.currentIndex() == 1 :
                    QMessageBox.critical(None, "错误", "xlsx格式错误")
                elif self.language_combobox.currentIndex() == 2 :
                    QMessageBox.critical(None, "錯誤", "xlsx格式錯誤")
                elif self.language_combobox.currentIndex() == 3 :
                    QMessageBox.critical(None, "エラー", "xlsx形式にエラーがあります")
                elif self.language_combobox.currentIndex() == 4 :
                    QMessageBox.critical(None, "Fehler", "Das XLSX-Format ist fehlerhaft.")
                else :
                    QMessageBox.critical(None, "오류", "xlsx 형식이 오류입니다")
            except FloatingPointError as e :
                # print error
                typer.secho(f"{e}", fg = typer.colors.RED, err = True)

                # info the message
                if self.language_combobox.currentIndex() == 0 :
                    QMessageBox.critical(None, "Error", f"{e}")
                elif self.language_combobox.currentIndex() == 1 :
                    QMessageBox.critical(None, "错误", f"{e}")
                elif self.language_combobox.currentIndex() == 2 :
                    QMessageBox.critical(None, "錯誤", f"{e}")
                elif self.language_combobox.currentIndex() == 3 :
                    QMessageBox.critical(None, "エラー", f"{e}")
                elif self.language_combobox.currentIndex() == 4 :
                    QMessageBox.critical(None, "Fehler", f"{e}")
                else :
                    QMessageBox.critical(None, "오류", f"{e}")
        else :
            # info the message
            if self.language_combobox.currentIndex() == 0 :
                QMessageBox.critical(None, "Error", "No excel data")
            elif self.language_combobox.currentIndex() == 1 :
                QMessageBox.critical(None, "错误", "未发现表格数据")
            elif self.language_combobox.currentIndex() == 2 :
                QMessageBox.critical(None, "錯誤", "未發現表格資料")
            elif self.language_combobox.currentIndex() == 3 :
                QMessageBox.critical(None, "エラー", "Excelデータはありません")
            elif self.language_combobox.currentIndex() == 4 :
                QMessageBox.critical(None, "Fehler", "Keine Excel-Daten")
            else:
                QMessageBox.critical(None, "오류", "엑셀 데이터 없음")

    @staticmethod
    def calculate_temperature(r, a, b, c) :
        """
        calculate the steinhart-hart temperature
        :param r: input param r
        :param a: input param a
        :param b: input param b
        :param c: input param c
        :return: temperature
        """

        ln_r = math.log(r)
        inv_t = a + (b * ln_r) + (c * (ln_r ** 3))
        t_kelvin = 1.0 / inv_t
        t_celsius = t_kelvin - 273.15

        return t_celsius

    def calculate_button_clicked(self) :
        """
        calculate button clicked
        :return: none
        """

        if len(self.excel_temperature_list) > 0 and len(self.excel_resistance_list) > 0 :
            try :
                # get the index
                index_t0 = self.t0_combobox.currentIndex()
                index_t1 = self.t1_combobox.currentIndex()
                index_t2 = self.t2_combobox.currentIndex()

                # calculate steinhart hart
                A, B, C = calculate_steinhart_hart(self.excel_resistance_list[index_t0], self.excel_temperature_list[index_t0],
                                                   self.excel_resistance_list[index_t1], self.excel_temperature_list[index_t1],
                                                   self.excel_resistance_list[index_t2], self.excel_temperature_list[index_t2])

                # print info
                typer.secho(f"temperature 0 is {self.excel_temperature_list[index_t0]:.2f}", fg = typer.colors.GREEN)
                typer.secho(f"temperature 1 is {self.excel_temperature_list[index_t1]:.2f}", fg = typer.colors.GREEN)
                typer.secho(f"temperature 2 is {self.excel_temperature_list[index_t2]:.2f}", fg = typer.colors.GREEN)
                typer.secho(f"steinhart-hart A is {A:.8e}", fg = typer.colors.GREEN)
                typer.secho(f"steinhart-hart B is {B:.8e}", fg = typer.colors.GREEN)
                typer.secho(f"steinhart-hart C is {C:.8e}", fg = typer.colors.GREEN)

                # set the steinhart hart A, B and C
                self.steinhart_hart_a_line_edit.setText(f"{A:.8e}")
                self.steinhart_hart_b_line_edit.setText(f"{B:.8e}")
                self.steinhart_hart_c_line_edit.setText(f"{C:.8e}")

                # print info
                typer.secho("success", fg = typer.colors.GREEN)

                excel_resistance_display_list = []
                excel_steinhart_hart_display_list = []

                # make display line and combobox data
                for i in range(len(self.excel_resistance_list)) :
                    excel_resistance_display_list.append(self.excel_resistance_list[i] / 1000)
                    excel_steinhart_hart_display_list.append(self.calculate_temperature(self.excel_resistance_list[i], A, B, C))

                # fill the data
                pen1 = pg.mkPen(color = 'y', width = 3)
                self.plot_view.clear()
                self.plot_view.addLegend(offset = (325, 20))
                self.plot_view.plot(self.excel_temperature_list, excel_resistance_display_list,
                                    pen = pen1, symbol = 'o', symbolSize = 5, symbolBrush = 'r',
                                    name = "Excel")

                pen2 = pg.mkPen(color = 'g', width = 3)
                self.plot_view.addLegend(offset = (325, 20))
                self.plot_view.plot(excel_steinhart_hart_display_list, excel_resistance_display_list,
                                    pen = pen2, symbol = 'o', symbolSize = 5, symbolBrush = 'b',
                                    name = "Steinhart Hart")
            except FileNotFoundError :
                # print error
                typer.secho("file not found", fg = typer.colors.RED, err = True)

                # info the message
                if self.language_combobox.currentIndex() == 0 :
                    QMessageBox.critical(None, "Error", "File not found")
                elif self.language_combobox.currentIndex() == 1 :
                    QMessageBox.critical(None, "错误", "未发现文件")
                elif self.language_combobox.currentIndex() == 2 :
                    QMessageBox.critical(None, "錯誤", "未發現文件")
                elif self.language_combobox.currentIndex() == 3 :
                    QMessageBox.critical(None, "エラー", "ファイルが見つかりません")
                elif self.language_combobox.currentIndex() == 4 :
                    QMessageBox.critical(None, "Fehler", "Datei nicht gefunden")
                else :
                    QMessageBox.critical(None, "오류", "파일을 찾을 수 없습니다.")
            except ValueError :
                # print error
                typer.secho("xlsx table name is not valid", fg = typer.colors.RED, err = True)

                # info the message
                if self.language_combobox.currentIndex() == 0 :
                    QMessageBox.critical(None, "Error", "xlsx table name is not valid")
                elif self.language_combobox.currentIndex() == 1 :
                    QMessageBox.critical(None, "错误", "xlsx表格名字非法")
                elif self.language_combobox.currentIndex() == 2 :
                    QMessageBox.critical(None, "錯誤", "xlsx表格名稱非法")
                elif self.language_combobox.currentIndex() == 3 :
                    QMessageBox.critical(None, "エラー", "xlsxテーブル名は無効です")
                elif self.language_combobox.currentIndex() == 4 :
                    QMessageBox.critical(None, "Fehler", "Der Name der XLSX-Tabelle ist ungültig.")
                else :
                    QMessageBox.critical(None, "오류", "xlsx 테이블 이름이 유효하지 않습니다.")
            except AttributeError :
                # print error
                typer.secho("xlsx format is error", fg = typer.colors.RED, err = True)

                # info the message
                if self.language_combobox.currentIndex() == 0 :
                    QMessageBox.critical(None, "Error", "xlsx format is error")
                elif self.language_combobox.currentIndex() == 1 :
                    QMessageBox.critical(None, "错误", "xlsx格式错误")
                elif self.language_combobox.currentIndex() == 2 :
                    QMessageBox.critical(None, "錯誤", "xlsx格式錯誤")
                elif self.language_combobox.currentIndex() == 3 :
                    QMessageBox.critical(None, "エラー", "xlsx形式にエラーがあります")
                elif self.language_combobox.currentIndex() == 4 :
                    QMessageBox.critical(None, "Fehler", "Das XLSX-Format ist fehlerhaft.")
                else :
                    QMessageBox.critical(None, "오류", "xlsx 형식이 오류입니다")
            except FloatingPointError as e :
                # print error
                typer.secho(f"{e}", fg = typer.colors.RED, err = True)

                # info the message
                if self.language_combobox.currentIndex() == 0 :
                    QMessageBox.critical(None, "Error", f"{e}")
                elif self.language_combobox.currentIndex() == 1 :
                    QMessageBox.critical(None, "错误", f"{e}")
                elif self.language_combobox.currentIndex() == 2 :
                    QMessageBox.critical(None, "錯誤", f"{e}")
                elif self.language_combobox.currentIndex() == 3 :
                    QMessageBox.critical(None, "エラー", f"{e}")
                elif self.language_combobox.currentIndex() == 4 :
                    QMessageBox.critical(None, "Fehler", f"{e}")
                else :
                    QMessageBox.critical(None, "오류", f"{e}")
        else:
            # info the message
            if self.language_combobox.currentIndex() == 0 :
                QMessageBox.critical(None, "Error", "No excel data")
            elif self.language_combobox.currentIndex() == 1 :
                QMessageBox.critical(None, "错误", "未发现表格数据")
            elif self.language_combobox.currentIndex() == 2 :
                QMessageBox.critical(None, "錯誤", "未發現表格資料")
            elif self.language_combobox.currentIndex() == 3 :
                QMessageBox.critical(None, "エラー", "Excelデータはありません")
            elif self.language_combobox.currentIndex() == 4 :
                QMessageBox.critical(None, "Fehler", "Keine Excel-Daten")
            else:
                QMessageBox.critical(None, "오류", "엑셀 데이터 없음")

    def language_combobox_changed(self) :
        """
        language combobox changed
        :return: none
        """

        with open("config.json", "w", encoding = "utf-8") as f :
            data = {"language": int(f"{self.language_combobox.currentIndex()}")}
            json.dump(data, f, ensure_ascii = False, indent = 4)

        # run the translation
        self.retranslate_ui()

class GuiMainWindow(QMainWindow) :
    """
    gui main window class
    """

    def __init__(self) :
        """
        init the main window
        """
        super().__init__()
        self.ui = MainWindow()
        self.ui.setup_ui(self)

def run_gui() :
    """
    run the qt gui
    :return: none
    """

    # create an app
    app = QApplication(sys.argv)

    # apply style sheet
    app.setStyleSheet(qdarktheme.load_stylesheet("dark"))

    # set font
    font = QFont(["PingFang SC", "Microsoft YaHei", "WenQuanYi Micro Hei", "sans-serif"], 10)
    font.setBold(True)
    app.setFont(font)

    # create a window
    window = GuiMainWindow()
    window.show()

    # run the app
    sys.exit(app.exec())

class CommandMode(str, Enum) :
    """
    command mode enum class
    """

    header = "header"
    steinhart = "steinhart-hart"
    gui = "gui"

def main(input_file_name: Annotated[str, typer.Option("--input-file", "-i", help = "Input Excel file name")] = 'ntc_table_template.xlsx',
         output_file_name: Annotated[str, typer.Option("--output-file", "-o", help = "Output C header file name")] = 'driver_ntc_table.h',
         command: Annotated[CommandMode, typer.Option("--command", "-c", help = "Command type, only header, steinhart-hart or gui")] = CommandMode.gui,
         t0: Annotated[float, typer.Option("--temperature0", "-t0", help = "Steinhart-Hart temperature point 0")] = 0.0,
         t1: Annotated[float, typer.Option("--temperature1", "-t1", help = "Steinhart-Hart temperature point 1")] = 25.0,
         t2: Annotated[float, typer.Option("--temperature2", "-t2", help = "Steinhart-Hart temperature point 2")] = 80.0) :
    # header mode
    if command == CommandMode.header :
        # print info
        typer.secho("c header generator", fg = typer.colors.WHITE)

        # print info
        typer.secho(f"input excel file: {input_file_name}", fg = typer.colors.WHITE)

        # print info
        typer.secho(f"output c header file: {output_file_name}", fg = typer.colors.WHITE)

        # if including the .h and remove
        output_file_name = output_file_name.replace(".h", "")

        try :
            # load excel
            excel_temperature_list, excel_resistance_list = load_excel(input_file_name)

            # write the c header file
            write_header_file(output_file_name, excel_temperature_list, excel_resistance_list)

            # print info
            typer.secho("success", fg = typer.colors.GREEN)
        except FileNotFoundError :
            # print error
            typer.secho("file not found", fg = typer.colors.RED, err = True)
        except ValueError :
            # print error
            typer.secho("xlsx table name is not valid", fg = typer.colors.RED, err = True)
        except AttributeError :
            # print error
            typer.secho("xlsx format is error", fg = typer.colors.RED, err = True)
        except FloatingPointError as e :
            # print error
            typer.secho(f"{e}", fg = typer.colors.RED, err = True)

    elif command == CommandMode.steinhart :
        # print info
        typer.secho("Steinhart-Hart A B C calculator", fg = typer.colors.WHITE)

        # print info
        typer.secho(f"input excel file: {input_file_name}", fg = typer.colors.WHITE)

        # print info
        typer.secho(f"temperature point 0: {t0}℃", fg = typer.colors.WHITE)
        typer.secho(f"temperature point 1: {t1}℃", fg = typer.colors.WHITE)
        typer.secho(f"temperature point 2: {t2}℃", fg = typer.colors.WHITE)

        try :
            # load excel
            excel_temperature_list, excel_resistance_list = load_excel(input_file_name)

            # init - 1
            index_t0 = -1

            # init -1
            index_t1 = -1

            # init -1
            index_t2 = -1

            # save t0 index
            for i in range(len(excel_temperature_list)) :
                # find the index
                if excel_temperature_list[i] == t0 :
                    # save the index
                    index_t0 = i

                    break

            # save t1 index
            for i in range(len(excel_temperature_list)) :
                # find the index
                if excel_temperature_list[i] == t1 :
                    # save the index
                    index_t1 = i

                    break

            # save t2 index
            for i in range(len(excel_temperature_list)) :
                # find the index
                if excel_temperature_list[i] == t2 :
                    # save the index
                    index_t2 = i

                    break

            if (index_t0 == -1) | (index_t1 == -1) | (index_t2 == -1) :
                # print error
                typer.secho("no temperature point", fg = typer.colors.RED, err=True)

                return

            A, B, C = calculate_steinhart_hart(excel_resistance_list[index_t0], excel_temperature_list[index_t0],
                                               excel_resistance_list[index_t1], excel_temperature_list[index_t1],
                                               excel_resistance_list[index_t2], excel_temperature_list[index_t2])

            # print info
            typer.secho(f"steinhart-hart A is {A:.8e}", fg = typer.colors.GREEN)
            typer.secho(f"steinhart-hart B is {B:.8e}", fg = typer.colors.GREEN)
            typer.secho(f"steinhart-hart C is {C:.8e}", fg = typer.colors.GREEN)

            # print info
            typer.secho("success", fg = typer.colors.GREEN)
        except FileNotFoundError :
            # print error
            typer.secho("file not found", fg = typer.colors.RED, err = True)
        except ValueError :
            # print error
            typer.secho("xlsx table name is not valid", fg = typer.colors.RED, err = True)
        except AttributeError :
            # print error
            typer.secho("xlsx format is error", fg = typer.colors.RED, err = True)
        except FloatingPointError as e :
            # print error
            typer.secho(f"{e}", fg = typer.colors.RED, err=True)

    elif command == CommandMode.gui :
        # print info
        print("gui program")

        # run the gui
        run_gui()
    else:
        # print info
        print("command not found")

if __name__ == '__main__' :
    # run the main function
    typer.run(main)
