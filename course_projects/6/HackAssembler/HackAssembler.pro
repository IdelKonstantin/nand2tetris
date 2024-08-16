TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        assembler.cpp \
        code.cpp \
        main.cpp \
        parser.cpp \
        symboltable.cpp

LIBS += -lstdc++fs

HEADERS += \
    assembler.h \
    code.h \
    parser.h \
    symboltable.h
