TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    ../csr_tool/host/remote-utils/oct-csr-tool.c \
    ../csr_tool/host/remote-utils/oct-dump-format.c \
    ../csr_tool/host/remote-utils/oct-gen-code.c \
    ../csr_tool/host/remote-utils/oct-reg-search.c \
    ../csr_tool/executive/cvmx-csr-db.c \
    ../csr_tool/executive/cvmx-csr-db-support.c

HEADERS += \
    endian.h

INCLUDEPATH += ../csr_tool/executive/
DEFINES = _GNU_SOURCE USE_RUNTIME_MODEL_CHECKS=1 QT_PROJECT
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-unused-parameter -std=gnu99
