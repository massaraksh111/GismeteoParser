QT += widgets network concurrent

QMAKE_CXXFLAGS_WARN_ON += -Wextra -Wall -Wno-sign-compare
QMAKE_CXXFLAGS += -pipe -std=c++0x

SOURCES += \
    main.cpp \
    window.cpp \
    task.cpp

HEADERS += \
    window.h \
    task.h \
    day.h
