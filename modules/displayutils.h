#ifndef DISPLAYUTILS_H
#define DISPLAYUTILS_H
//qt part
#include <QWidget>
#include <QDebug>
#include <QString>
#include <QVTKWidget.h>
// vtk part
#include <vtkObjectFactory.h>
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
#include <vtkLight.h>
#include <vtkLightActor.h>

#include <vtkRendererCollection.h>
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

#include <vtkSphereSource.h>

// std part
#include <iostream>
#include <string>
#include <iomanip>
#include <map>
#include <vector>
#include <utility>

// custom part
#include "tools/tklib.h"
#include "tools/initials.h"
#include "tools/fastdef.h"
#include "tools/itkutils.h"
#include "centerlineproc.h"

class vtkSliderCallback : public vtkCommand
{
public:
    static vtkSliderCallback *New()
    {
        return new vtkSliderCallback;
    }
    virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
        vtkSliderWidget *sliderWidget =
                reinterpret_cast<vtkSliderWidget*>(caller);
        if(this -> actor){
            this->actor->GetProperty()->SetOpacity(
                        static_cast<vtkSliderRepresentation*>(
                            sliderWidget->GetRepresentation())->GetValue());
        }
    }
    vtkSliderCallback():actor(0) {};
    bool setBoundActor(vtkActor *giveactor)
    {
        actor = giveactor;
        if(!actor)
            return false;
        return true;
    }
    vtkActor *actor;
};

class CusInteractorStylePickPoint : public vtkInteractorStyleTrackballCamera
{
public:
    static CusInteractorStylePickPoint* New();
    vtkTypeMacro(CusInteractorStylePickPoint,vtkInteractorStyleTrackballCamera);

    void PreparedRenderer( vtkRenderer *renderer)
    {
        m_renderer = renderer;
        m_beginPointActor = vtkSmartPointer<vtkActor>::New();
        m_endPointActor = vtkSmartPointer<vtkActor>::New();

        SetVisiblityOn();
    }

    void SetPickerEnabled(bool enabled = true)
    {
        m_enabled = enabled;
        m_PickedPointsCnt = 0;
    }

    bool GetPickerEnabled()
    {
        return m_enabled;
    }
    virtual void OnKeyPress(){
        vtkRenderWindowInteractor *rwi = this->Interactor;

        std::string key = rwi->GetKeySym();

        if(key == "space") {
            ProcessPick();
        }
    }
    void GetMarkedPoints(double p1[3],double p2[3])
    {
        p1[0] = m_pbeg.x;
        p1[1] = m_pbeg.y;
        p1[2] = m_pbeg.z;

        p2[0] = m_pend.x;
        p2[1] = m_pend.y;
        p2[2] = m_pend.z;
    }
    virtual void OnMiddleButtonDown()
    {
        ProcessPick();
        vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
    }


private:
    bool m_enabled;
    int m_PickedPointsCnt;

//    struct Point3f{
//      double x, y, z;
//    };
    Point3f m_pbeg;
    Point3f m_pend;

    vtkRenderer * m_renderer;
    vtkSmartPointer<vtkActor> m_beginPointActor;
    vtkSmartPointer<vtkActor> m_endPointActor;
    void SetVisiblityOff()
    {
        m_renderer->RemoveActor(m_beginPointActor);
        m_renderer->RemoveActor(m_endPointActor);
    }
    void SetVisiblityOn()
    {
        m_renderer->AddActor(m_beginPointActor);
        m_renderer->AddActor(m_endPointActor);
    }
    void ProcessPick()
    {
        if(m_enabled){
            m_PickedPointsCnt %= 2;
            if(m_PickedPointsCnt == 0)
                SetVisiblityOff();
            this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0],
                                                this->Interactor->GetEventPosition()[1],
                                                0,
                                                this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
            double p[3];
            this->Interactor->GetPicker()->GetPickPosition(p);
            ++m_PickedPointsCnt;
            switch(m_PickedPointsCnt) {
            case 1: {
                m_pbeg = {p[0], p[1], p[2]};
                vtkSmartPointer<vtkSphereSource> spheresource =
                        vtkSmartPointer<vtkSphereSource>::New();
                spheresource->SetCenter(p);
                spheresource->SetRadius(1.0);
                spheresource->Update();
                vtkSmartPointer<vtkPolyDataMapper> mapper =
                        vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper->SetInputConnection(spheresource->GetOutputPort());
                m_beginPointActor->SetMapper(mapper);
                m_beginPointActor->GetProperty()->SetColor(.0, 1., .0);
                m_renderer->AddActor(m_beginPointActor);
                break;

            }
            case 2: {
                m_pend = {p[0], p[1], p[2]};
                vtkSmartPointer<vtkSphereSource> spheresource =
                        vtkSmartPointer<vtkSphereSource>::New();
                spheresource->SetCenter(p);
                spheresource->SetRadius(1.0);
                spheresource->Update();
                vtkSmartPointer<vtkPolyDataMapper> mapper =
                        vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper->SetInputConnection(spheresource->GetOutputPort());
                m_endPointActor->SetMapper(mapper);
                m_endPointActor->GetProperty()->SetColor(.0, 1., .0);
                m_renderer->AddActor(m_endPointActor);

            }
            }
        }
    }


};
class displayUtils
{
public:
    displayUtils(vtkRenderWindow * renWin);
    ~displayUtils();
    vtkRenderer* GetRenderer()
    {
        return m_renderer;
    }

    std::string GetRawFilename();
    void OpenSegmentModel(std::string filename);
    void OpenCenterLineModel(std::string filename);
    void centerLnDis(std::vector<OutputImageType::PointType> &inpoints);
    void SetMainModelOn();
    void SetMainModelOff();
    void SetLineModelOn();
    void SetLineModelOff();
    //three display mode;
    void pointmode();
    void linemode();
    void framemode();

    // centerline extraction
    void TestDistanceTransform();
    void GetCenterline();
    //roaming
    void OnRoam();
    void SetRoamingStep(int step = 1);
    void RoamNext();
    void RoamPrevious();
    void GetCurrentRoamingPoint(double p[3]);
    void UpdateRoamingCamera();
    //pick point
    void SetPointerPickEnabled(bool enabled=true);

    //draw centerline points
    void ShowCenterPoints(std::vector<Point3f> &CenterPoints);
    void DrawCenterLine();


private:
    std::pair<std::string,std::string> m_filename;

    //rendering
    vtkRenderWindow * m_renWindow;
    vtkRenderWindowInteractor * m_iren;
    vtkSmartPointer<vtkRenderer> m_renderer;
    //model
    vtkSmartPointer< vtkActor > m_stlactor;
    vtkSmartPointer< vtkActor > m_lineactor;
    centerLineProc *m_centerline;

    bool has_stl;
    bool has_line;

    int m_currentRoamingIndex;
    int m_currentRoamingStep;

    vtkSmartPointer< vtkLight > m_light;
    //releted slider
    vtkSmartPointer< vtkSliderRepresentation2D > sliderRep;
    vtkSmartPointer< vtkSliderWidget > sliderWidget;
    vtkSmartPointer< vtkSliderCallback > slidercallback;

    vtkSmartPointer< vtkPointPicker > m_lineInfoPointPicker;
    vtkSmartPointer< CusInteractorStylePickPoint > m_PointPickerInteractor;

    void createSliderTool();

};

#endif // DISPLAYUTILS_H
