QT += gui qml quick
TARGET = quick-graphviz
TEMPLATE = app

macx:QT_CONFIG -= no-pkg-config

CONFIG += link_pkgconfig c++11
PKGCONFIG += libcdt libgvc libcgraph #libgraph

SOURCES += main.cpp \
    graphnode.cpp \
    graphedge.cpp \
    graph.cpp

HEADERS += \
    graphnode.h \
    graphedge.h \
    graph.h

