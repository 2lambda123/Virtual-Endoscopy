#-------------------------------------------------
#
# Project created by QtCreator 2015-12-07T14:21:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VirtualEndo
TEMPLATE = app

## itk-4.8.0
win32: LIBS += -L$$PWD/../../3rd/InsightToolkit-4.8.0/lib/
INCLUDEPATH += $$PWD/../../3rd/InsightToolkit-4.8.0/include/ITK-4.8
DEPENDPATH += $$PWD/../../3rd/InsightToolkit-4.8.0/include/ITK-4.8

## vtk-6.3.0
win32: LIBS += -L$$PWD/../../3rd/VTK-6.3.0/lib/
INCLUDEPATH += $$PWD/../../3rd/VTK-6.3.0/include/vtk-6.3
DEPENDPATH += $$PWD/../../3rd/VTK-6.3.0/include/vtk-6.3

SOURCES += main.cpp\
        mainwindow.cpp \
    modules/centerlineproc.cpp \
    modules/showutility.cpp \
    modules/findpath/itkBinaryThinningImageFilter3D.txx \
    modules/displayutils.cpp \
    modules/globalviewutils.cpp

HEADERS  += mainwindow.h \
    tools/fastdef.h \
    tools/tklib.h \
    tools/initials.h \
    tools/itkutils.h \
    modules/centerlineproc.h \
    modules/showutility.h \
    tools/vtkswitchinteractioncallback.h \
    modules/findpath/itkBinaryThinningImageFilter3D.h \
    modules/displayutils.h \
    Test/testpointpicker.h \
    Test/testunit.h \
    modules/globalviewutils.h

FORMS    += mainwindow.ui

RESOURCES +=

#DISTFILES +=
