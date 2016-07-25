#-------------------------------------------------
#
# Project created by QtCreator 2015-10-29T00:21:45
#
#-------------------------------------------------

QT       -= core gui

TARGET = actor
TEMPLATE = lib
CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++1y #-pedantic -Weffc++ -Wextra -Wall -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-overflow=5 -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code


DEFINES += ACTOR_LIBRARY

SOURCES += Message.cpp \
    Executer.cpp \
    Actor.cpp \
    TQueue.cpp \
    AbstractActor.cpp \
    Storage.cpp \
    storageHelpers/Runnable.cpp \
    storageHelpers/Task.cpp

HEADERS += Message.h \
    Executer.h \
    baseLib/importWithoutWarning.h \
    Actor.h \
    TQueue.h \
    AbstractActor.h \
    Storage.h \
    storageHelpers/Runnable.h \
    storageHelpers/Task.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
