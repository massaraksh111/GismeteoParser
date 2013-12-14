QT += widgets network

QMAKE_CXXFLAGS_WARN_ON += -Wextra -Wall -Wno-sign-compare
QMAKE_CXXFLAGS += -pipe -std=c++11

SOURCES += \
    main.cpp \
    window.cpp \
    task.cpp

HEADERS += \
    window.h \
    task.h \
    Day.h
