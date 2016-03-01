#ifndef SHOWUTILITY_H
#define SHOWUTILITY_H

#include <QWidget>
#include <QDebug>
#include <QString>
#include <QVTKWidget.h>
#include <QGridLayout>
#include <QMenu>

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkActor.h>
#include <vtkMapper.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkCamera.h>
#include <vtkSTLReader.h>
#include <vtkTextRenderer.h>
#include <vtkCornerAnnotation.h>

#include <vtkCommand.h>
#include <vtkSliderWidget.h>
#include <vtkSliderRepresentation2D.h>
#include <vtkWidgetEventTranslator.h>
#include <vtkWidgetEvent.h>
#include <vtkCallbackCommand.h>
#include <vtkTextProperty.h>
#include <vtkEventQtSlotConnect.h>

#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkPropAssembly.h>
#include <vtkTransform.h>

#include <vtkPointPicker.h>
#include <vtkGlyph3D.h>
#include <vtkSelectPolyData.h>
#include <vtkClipPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkPointData.h>
#include <vtkSphereSource.h>
#include <vtkCellArray.h>
#include <vtkUnsignedCharArray.h>

#include <vtkTextActor.h>
#include <vtkTextProperty.h>


#include <iostream>
#include "tools/tklib.h"
#include "tools/initials.h"
#include "tools/fastdef.h"
#include "centerlineproc.h"
//class QGridLayout;


//class vtkSliderCallback : public vtkCommand
//{
//public:
//    static vtkSliderCallback *New()
//    {
//        return new vtkSliderCallback;
//    }
//    virtual void Execute(vtkObject *caller, unsigned long, void*)
//    {
//        vtkSliderWidget *sliderWidget =
//                reinterpret_cast<vtkSliderWidget*>(caller);
//        if(this -> actor){
//            this->actor->GetProperty()->SetOpacity(
//                        static_cast<vtkSliderRepresentation*>(
//                            sliderWidget->GetRepresentation())->GetValue());
//        }
//    }
//    vtkSliderCallback():actor(0) {};
//    bool setBoundActor(vtkActor *giveactor)
//    {
//        actor = giveactor;
//        if(!actor)
//            return false;
//        return true;
//    }
//    vtkActor *actor;
//};


class showutility : public QWidget
{
    Q_OBJECT
public:
    explicit showutility(QWidget *parent = 0);
    static showutility *New()
    {
        return new showutility;
    }
    void openModel(const char *filename);
    void centerLnDis(std::vector<OutputImageType::PointType> &inpoints);
    static void pickControl(vtkObject *caller,unsigned long eid,void *clientdata,void *calldata);
    ~ showutility();

    QVTKWidget * GetQvtkWidget()
    {
        return this->qvtk;
    }
    vtkRenderer * GetRenderer()
    {

    }
    void SetMainModelOn();
    void SetMainModelOff();
    void SetLineModelOn();
    void SetLineModelOff();


signals:

private:
    QVTKWidget *qvtk;
    QGridLayout *layout;
    vtkSmartPointer< vtkRenderer > m_render;
    vtkSmartPointer< vtkRenderWindowInteractor > iren;
    vtkSmartPointer< vtkActor > m_stlactor;
    vtkSmartPointer< vtkRenderWindow > m_renwin;

    vtkSmartPointer< vtkCornerAnnotation > annotation;

    vtkSmartPointer< vtkActor > m_lineactor;

//    vtkSmartPointer< vtkCallbackCommand > cmd;
//    vtkSmartPointer< vtkPointPicker > pointpicker;
//    static int numPt;
//    static vtkPoints *points;

    vtkSmartPointer< vtkSliderRepresentation2D > sliderRep;
    vtkSmartPointer< vtkSliderWidget > sliderWidget;
    //vtkSmartPointer< vtkSliderCallback > slidercallback;

    vtkSmartPointer< vtkTextActor > textactor;



    vtkEventQtSlotConnect *Connections;
    QMenu *popup2;

    vtkSmartPointer<vtkAxesActor> axes;
    vtkSmartPointer<vtkOrientationMarkerWidget> axesWidget;

    bool has_stl;
    bool has_line;
    void initRenderWindow();
   // void createSliderTool();


public slots:
    void pointmode();
    void linemode();
    void framemode();

    void popup(vtkObject * obj, unsigned long,
                 void * client_data, void *,
                 vtkCommand * command);
    void color2(QAction*);
};

#endif // SHOWUTILITY_H
