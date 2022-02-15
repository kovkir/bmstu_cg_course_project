QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20
CONFIG += sdk_no_version_check

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#нужно для работы omp на маке
#macx:
#{
#QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp -lomp -I/usr/local/include
#QMAKE_LFLAGS += -lomp
#LIBS += -L /usr/local/lib /usr/local/lib/libomp.dylib
#}

SOURCES += \
    additivemathelements.cpp \
    illuminantplacechooser.cpp \
    main.cpp \
    mainwindow.cpp \
    movechooser.cpp \
    objectchanger.cpp \
    objecthangman.cpp \
    objects.cpp \
    placechooser.cpp \
    sizechooser.cpp \
    specialgraphicsview.cpp \
    usagefacade.cpp

HEADERS += \
    additivemathelements.hpp \
    config.hpp \
    illuminantplacechooser.hpp \
    mainwindow.h \
    movechooser.hpp \
    objectchanger.hpp \
    objecthangman.hpp \
    objects.hpp \
    placechooser.h \
    sizechooser.hpp \
    specialgraphicsview.hpp \
    usagefacade.hpp

FORMS += \
    illuminantplacechooser.ui \
    mainwindow.ui \
    movechooser.ui \
    objectchanger.ui \
    objecthangman.ui \
    placecarсhooser.ui \
    placehousechooser.ui \
    placeroadсhooser.ui \
    placetreechooser.ui \
    sizechooser.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
