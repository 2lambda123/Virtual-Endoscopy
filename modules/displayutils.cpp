#include "displayutils.h"
#include <iostream>
#include <vtkCamera.h>
vtkStandardNewMacro(CusInteractorStylePickPoint);

displayUtils::displayUtils(vtkRenderWindow *renWin)
    :m_renWindow(renWin),has_stl(false),has_line(false)
{
    vsp(m_renderer);
    vsp(m_stlactor);
    vsp(m_lineactor);

    Instantiate(camera,vtkCamera);
    camera->SetPosition(0,0,20);
    camera->SetFocalPoint(0,0,0);
    vsp(m_light);
//    m_light->SetColor(1.,1.,1.);
//    m_light->SetIntensity(.5);
//    m_light->SetPosition(camera->GetPosition());
//    m_light->SetFocalPoint(camera->GetFocalPoint());

    m_renderer->AddLight(m_light);
    m_renderer->SetActiveCamera(camera);
    m_renWindow->AddRenderer(m_renderer);
    m_iren = renWin->GetInteractor();

    vsp(m_lineInfoPointPicker);
    vsp(m_PointPickerInteractor);
    m_iren->SetInteractorStyle(m_PointPickerInteractor);
    m_PointPickerInteractor->PreparedRenderer(m_renderer);

    createSliderTool();

    m_renWindow->Render();

    m_centerline = new centerLineProc;
    m_currentRoamingStep = 2;
}

void displayUtils::OpenSegmentModel(std::string filename)
{
    Instantiate(reader,vtkSTLReader);
    reader -> SetFileName(filename.c_str());
    reader -> Update();

    //build connection
    Instantiate(mapper,vtkPolyDataMapper);
    mapper -> SetInputConnection(reader -> GetOutputPort());
    m_stlactor -> SetMapper(mapper);
 //   m_stlactor->GetProperty()->SetColor(1., .0, .0);
    sliderWidget->EnabledOn();
    has_stl = true;
 //   m_renderer->ResetCamera();

}

void displayUtils::OpenCenterLineModel(std::string filename)
{
    //test centerline display


}

void displayUtils::centerLnDis(std::vector<OutputImageType::PointType> &inpoints)
{
    int num = inpoints.size();
    Instantiate(points,vtkPoints);
    Instantiate(vertices,vtkCellArray);
    Instantiate(lines,vtkCellArray);
    Instantiate(line,vtkLine);
    Instantiate(colors,vtkUnsignedCharArray);
    colors->SetNumberOfComponents(3);
    //green display each point.
    unsigned char green[3] = {0,255,0};
    std::vector<OutputImageType::PointType>::const_iterator it = inpoints.begin();

    for(unsigned int i = 0;i < num && it != inpoints.end();i++,it++){
        vtkIdType pid[1];
        pid[0] = points->InsertNextPoint((*it)[0],(*it)[1],(*it)[2]);
        colors->InsertNextTupleValue(green);
        vertices->InsertNextCell(1,pid);
        if(i < num-1){
            line->GetPointIds()->SetId(0,i);
            line->GetPointIds()->SetId(1,i+1);
            lines->InsertNextCell(line);
        }

    }

    Instantiate(polydata,vtkPolyData);
    polydata->SetPoints(points);
    polydata->SetVerts(vertices);
    polydata->SetLines(lines);
    polydata->GetPointData()->SetScalars(colors);

    Instantiate(cenmapper,vtkPolyDataMapper);
    cenmapper->SetInputData(polydata);

    m_lineactor->SetMapper(cenmapper);
    has_line = true;

    m_renderer->AddActor(m_lineactor);

    m_renWindow->Render();

}

void displayUtils::SetMainModelOn()
{
    if(!has_stl)  return;
    m_renderer->AddActor(m_stlactor);
    sliderWidget->EnabledOn();
    m_renderer->ResetCamera();

}

void displayUtils::SetMainModelOff()
{
    if(!has_stl)  return;
    m_renderer->RemoveActor(m_stlactor);
    sliderWidget->EnabledOff();
}

void displayUtils::SetLineModelOn()
{
    if(!has_line)  return;
    m_renderer->AddActor(m_lineactor);
    m_renderer->ResetCamera();
}

void displayUtils::SetLineModelOff()
{
    if(!has_line)  return;
    m_renderer->RemoveActor(m_lineactor);
}

void displayUtils::pointmode()
{
    if(m_renderer->GetActors()->GetNumberOfItems() != 0 && has_stl){
        m_stlactor->GetProperty()->SetRepresentationToPoints();
    }
}

void displayUtils::linemode()
{
    if(m_renderer->GetActors()->GetNumberOfItems() != 0 && has_stl){
        m_stlactor->GetProperty()->SetRepresentationToWireframe();
    }
}

void displayUtils::framemode()
{
    if(m_renderer->GetActors()->GetNumberOfItems() != 0 && has_stl){
        m_stlactor->GetProperty()->SetRepresentationToSurface();
    }
}

void displayUtils::GetCenterline()
{
     if(m_PointPickerInteractor->GetEnabled() == false) {
         std::cout << "Please first set start and end point!" << std::endl;
         return ;
     }
     double s[3],e[3];
     m_PointPickerInteractor->GetMarkedPoints(s,e);
     m_centerline->Path_GradientDescent("D:\\3dresearch\\heart-artery\\out\\se1ct1.mhd",s,e);
     std::cout << "complete centerline extraction!" << std::endl;
}

void displayUtils::OnRoam()
{
    // first calculate centerline if hasn't done
    if(m_centerline->GetCenterlinePointNums() == 0){
        m_centerline->getSignedDistanceMap_Sin();
        centerLnDis(m_centerline->getpoints());
    }
    m_currentRoamingIndex = 0;
    UpdateRoamingCamera();
}

void displayUtils::SetRoamingStep(int step)
{
    m_currentRoamingStep = step;
}

void displayUtils::RoamNext()
{
    m_currentRoamingIndex += m_currentRoamingStep;
    int indexLimit = m_centerline->GetCenterlinePointNums();
    if(m_currentRoamingIndex >= indexLimit ){
        m_currentRoamingIndex = std::max(0,indexLimit - 1);
    }
    UpdateRoamingCamera();
}

void displayUtils::RoamPrevious()
{
    m_currentRoamingIndex -= m_currentRoamingStep;
    if(m_currentRoamingIndex < 0){
        m_currentRoamingIndex = 0;
    }
    UpdateRoamingCamera();
}

void displayUtils::GetCurrentRoamingPoint(double p[])
{
    m_centerline->GetcenterlinePoint(m_currentRoamingIndex,p);
}

void displayUtils::UpdateRoamingCamera()
{
    double p[3];
    GetCurrentRoamingPoint(p);
    double fp[3];
    // to get current centerline point
    m_centerline->GetcenterlinePoint(m_currentRoamingIndex-m_currentRoamingStep/2,p);

    if (m_currentRoamingIndex - m_currentRoamingStep / 2 < 0){
        fp[0] = p[0],fp[1] = p[1], fp[2] = p[2];
    }

    double h[3];

    // to get current centerline point
    m_centerline->GetcenterlinePoint(m_currentRoamingIndex+m_currentRoamingStep/2,h);

    int indexLimit = m_centerline->GetCenterlinePointNums();
    if(m_currentRoamingIndex+m_currentRoamingStep/2 >= indexLimit){
        h[0] = p[0],h[1] = p[1],h[2] = p[2];
    }
    fp[0] = p[0] + h[0] - fp[0];
    fp[1] = p[1] + h[1] - fp[1];
    fp[2] = p[2] + h[2] - fp[2];

    m_renderer->GetActiveCamera()->SetPosition(p);
    m_renderer->GetActiveCamera()->SetFocalPoint(fp);

    m_light->SetPosition(m_renderer->GetActiveCamera()->GetPosition());
    m_light->SetFocalPoint(m_renderer->GetActiveCamera()->GetFocalPoint());

    m_renderer->Render();

}

void displayUtils::SetPointerPickEnabled(bool enabled)
{
    m_PointPickerInteractor->SetPickerEnabled(enabled);
}




void displayUtils::createSliderTool()
{

    vsp(sliderRep);
    sliderRep->SetMinimumValue(0.0);
    sliderRep->SetMaximumValue(1.0);
    sliderRep->SetValue(1.0);
    sliderRep->SetTitleText("Opacity");
    // Set color properties:
      // Change the color of the knob that slides
    sliderRep->GetSliderProperty()->SetColor(1,0,0);//red

      // Change the color of the text indicating what the slider controls
    sliderRep->GetTitleProperty()->SetColor(1,0,0);//red
//    sliderRep->SetTitleHeight(10);
      // Change the color of the text displaying the value
    sliderRep->GetLabelProperty()->SetColor(1,0,0);//red

      // Change the color of the knob when the mouse is held on it
    sliderRep->GetSelectedProperty()->SetColor(0,1,0);//green

      // Change the color of the bar
    sliderRep->GetTubeProperty()->SetColor(1,1,0);//yellow

      // Change the color of the ends of the bar
    sliderRep->GetCapProperty()->SetColor(1,1,0);//yellow
    sliderRep->GetPoint1Coordinate()->SetCoordinateSystemToDisplay();
    sliderRep->GetPoint1Coordinate()->SetValue(40 ,40);
    sliderRep->GetPoint2Coordinate()->SetCoordinateSystemToDisplay();
    sliderRep->GetPoint2Coordinate()->SetValue(160, 40);

    vsp(sliderWidget);
    sliderWidget->SetRepresentation(sliderRep);


    vsp(slidercallback);
    if(slidercallback->setBoundActor(vtkActor::SafeDownCast(m_stlactor))){
        sliderWidget->AddObserver(vtkCommand::InteractionEvent,slidercallback);
        sliderWidget->SetInteractor(m_iren);
        sliderWidget->SetAnimationModeToAnimate();
        sliderWidget->EnabledOff();
    }

}
