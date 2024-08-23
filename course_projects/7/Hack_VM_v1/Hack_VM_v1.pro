TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        VM_translator.cpp \
        code_writer.cpp \
        main.cpp \
        parser.cpp

HEADERS += \
    VM_translator.h \
    code_writer.h \
    parser.h

LIBS += -lstdc++fs
