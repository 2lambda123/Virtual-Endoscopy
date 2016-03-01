#include "globalviewutils.h"
#include <vtkSTLReader.h>
#include <vtkProperty.h>
#include <vtkTextProperty.h>
GlobalViewUtils::GlobalViewUtils(vtkRenderWindow *renWin)
    :m_renWindow(renWin)
{
    vsp(m_renderer);
    vsp(m_mainactor);
    vsp(m_moveactor);
//    Instantiate(camera,vtkCamera);
//    camera->SetPosition(0,0,20);
//    camera->SetFocalPoint(0,0,0);
//    vsp(m_light);
//    m_light->SetColor(1.,1.,1.);
//    m_light->SetIntensity(.5);
//    m_light->SetPosition(camera->GetPosition());
//    m_light->SetFocalPoint(camera->GetFocalPoint());
//    m_renderer->AddLight(m_light);
//    m_renderer->SetActiveCamera(camera);

    vsp(m_annotation);
    m_annotation->SetLinearFontScaleFactor( 2 );
    m_annotation->SetNonlinearFontScaleFactor( 1 );
    m_annotation->SetMaximumFontSize( 15 );
    m_annotation->SetText(1 ,"current position:\n(.0, .0, .0)");
    m_annotation->GetTextProperty()->SetColor( 1, 0, 0 );
    m_renderer->AddViewProp(m_annotation);
    m_renderer->SetBackground(0.1, 0.2, 0.4);
    m_renWindow->AddRenderer(m_renderer);
    m_iren = renWin->GetInteractor();
    m_renWindow->Render();

}

void GlobalViewUtils::SetMainModelName(std::string filename)
{
    m_mainModelName = filename;
}

void GlobalViewUtils::UpdatePosInfo(double p[])
{

}

void GlobalViewUtils::FormMainModel()
{
    if(m_mainModelName.empty())   return;

    Instantiate(reader, vtkSTLReader);
    reader->SetFileName(m_mainModelName.c_str());
    reader->Update();
    std::cout << "load file: " << m_mainModelName << std::endl;
    Instantiate(mapper, vtkPolyDataMapper);
    mapper -> SetInputConnection(reader->GetOutputPort());
    m_mainactor -> SetMapper(mapper);
//    m_mainactor -> GetProperty()->SetOpacity(0.5);
    m_renderer -> AddActor(m_mainactor);
    m_renderer -> ResetCamera();
    m_renWindow -> Render();

}

void GlobalViewUtils::UpdateView()
{

}

