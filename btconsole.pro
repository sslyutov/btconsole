# ------------------------------------------------------------------------
# author Sergey Slyutov
# date February 13, 2023
# BT profile report internal structure of the device BT element such a service, charcteristics and description
# ------------------------------------------------------------------------


TEMPLATE = app 

TARGET = btmon

TARGET_EXT = .exe #use this tp stop qmake adding a number following TARGET filename.

VERSION = 1.0.1.0

QMAKE_TARGET_PRODUCT = "bluetooth device profiler"

QMAKE_TARGET_COPYRIGHT =  "Biopac Systems, Inc." 

QMAKE_TARGET_DESCRIPTION = "report a profile of BT device using Bluetooth LE Central"

QT +=  widgets bluetooth core gui concurrent bluetooth

CONFIG += console c++17

CONFIG += debug

DESTDIR_DEBUG = ./DEBUG

DETIDIR_RELEASE = ./RELEASE

QMAKE_CXXFLAGS += /MP

include (btconsole.pri)