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
win32: LIBS += -L$$PWD/../../3rd/InsightToolkit-4.8.0-minSizeRel/lib/
INCLUDEPATH += $$PWD/../../3rd/InsightToolkit-4.8.0-minSizeRel/include/ITK-4.8
DEPENDPATH += $$PWD/../../3rd/InsightToolkit-4.8.0-minSizeRel/include/ITK-4.8

## vtk-6.3.0
win32: LIBS += -L$$PWD/../../3rd/VTK-6.3.0-minSizeRel/lib/
INCLUDEPATH += $$PWD/../../3rd/VTK-6.3.0-minSizeRel/include/vtk-6.3
DEPENDPATH += $$PWD/../../3rd/VTK-6.3.0-minSizeRel/include/vtk-6.3

SOURCES += main.cpp\
        mainwindow.cpp \
    modules/centerlineproc.cpp \
    modules/showutility.cpp \
    modules/findpath/itkBinaryThinningImageFilter3D.txx \
    modules/displayutils.cpp \
    modules/globalviewutils.cpp \
    modules/findpath/approach2/itkArrivalFunctionToPathFilter.txx \
    modules/findpath/approach2/itkImageToPathFilter.txx \
    modules/findpath/approach2/itkIterateNeighborhoodOptimizer.txx \
    modules/findpath/approach2/itkPhysicalCentralDifferenceImageFunction.txx \
    modules/findpath/approach2/itkSingleImageCostFunction.txx \
    modules/findpath/approach2/itkSpeedFunctionToPathFilter.txx \
    modules/segment/sliceshowutils.cpp

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
    modules/globalviewutils.h \
    modules/findpath/approach2/itkArrivalFunctionToPathFilter.h \
    modules/findpath/approach2/itkImageToPathFilter.h \
    modules/findpath/approach2/itkIterateNeighborhoodOptimizer.h \
    modules/findpath/approach2/itkPhysicalCentralDifferenceImageFunction.h \
    modules/findpath/approach2/itkSingleImageCostFunction.h \
    modules/findpath/approach2/itkSpeedFunctionToPathFilter.h \
    Test/testregiongrow.h \
    modules/segment/sliceshowutils.h \
    tools/itkvtkpipe.h

FORMS    += mainwindow.ui

RESOURCES +=

#DISTFILES +=
