TARGET = QDR

TEMPLATE = app

CONFIG += console
CONFIG += c++11

INCLUDEPATH += inc
LIBS += -L../lib -lsndfile

SOURCES += src/main.cpp\
    src/ttdr.cpp \
    src/soundfile.cpp

HEADERS  += \
    src/ttdr.h \
    src/soundfile.h
