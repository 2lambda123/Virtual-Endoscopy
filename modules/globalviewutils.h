#ifndef GLOBALVIEWUTILS_H
#define GLOBALVIEWUTILS_H

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkLight.h>
#include <vtkCornerAnnotation.h>

#include "tools/tklib.h"
#include "tools/initials.h"
#include "tools/fastdef.h"
#include "tools/itkutils.h"
#include "centerlineproc.h"
class GlobalViewUtils
{
public:
    GlobalViewUtils(vtkRenderWindow *renWin);
    void SetMainModelName(std::string filename);
    void UpdatePosInfo(double p[3]);
    void FormMainModel();
    void UpdateView();

private:
    vtkRenderWindow * m_renWindow;
    vtkSmartPointer< vtkRenderer > m_renderer;
    vtkSmartPointer< vtkRenderWindowInteractor > m_iren;
//    vtkSmartPointer< vtkLight > m_light;

    vtkSmartPointer< vtkActor > m_mainactor;
    vtkSmartPointer< vtkActor > m_moveactor;

    vtkSmartPointer< vtkCornerAnnotation > m_annotation;

    std::string m_mainModelName;
    centerLineProc *m_centerline;

};

#endif // GLOBALVIEWUTILS_H
