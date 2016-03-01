#include "showutility.h"



showutility::showutility(QWidget *parent) : QWidget(parent),has_stl(false),has_line(false)
{
    vsp(m_render);
    vsp(m_stlactor);
    vsp(m_lineactor);
    qvtk = new QVTKWidget(this);
    layout = new QGridLayout;
    layout -> addWidget(qvtk,0,0,1,1);
    setLayout(layout);
    m_renwin = qvtk -> GetRenderWindow();
    m_renwin->AddRenderer(m_render);
    iren = qvtk -> GetRenderWindow() -> GetInteractor();

//    iren->SetRenderWindow(renwin);

//    iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//    iren->SetRenderWindow(renwin);

    Instantiate(style,vtkInteractorStyleTrackballCamera);

    iren -> SetInteractorStyle(style);

//    annotation = vtkSmartPointer<vtkCornerAnnotation>::New();
//    annotation->SetLinearFontScaleFactor(2);
//    annotation->SetNonlinearFontScaleFactor(1);
//    annotation->SetMaximumFontSize(20);
//    annotation->GetProperty()->SetColor(0,1,0);
//    annotation->SetText(0,"test");
//    render->AddViewProp(annotation);

//    cmd = vtkSmartPointer< vtkCallbackCommand >::New();
//    cmd -> SetCallback(pickControl);
//    pointpicker = vtkSmartPointer< vtkPointPicker >::New();
//    iren -> SetPicker(pointpicker);
//    pointpicker -> AddObserver(vtkCommand::EndPickEvent,cmd);
//    numPt = 0;

    Connections = vtkEventQtSlotConnect::New();
    popup2 = new QMenu(qvtk);
    popup2->addAction("Background Grey");
    popup2->addAction("Background Black");
    connect(popup2,SIGNAL(triggered(QAction*)),this,SLOT(color2(QAction*)));
    Connections->Connect(iren,vtkCommand::RightButtonPressEvent,
                         this,
                         SLOT(popup(vtkObject*,unsigned long,void*,void*,vtkCommand*)),
                         popup2,1.0);




//    textactor = vtkSmartPointer<vtkTextActor>::New();
//    textactor->SetPosition2(10,40);
//    textactor->SetInput("test!!!");
//    textactor->GetProperty()->SetColor(0.0,1.0,0.0);
//    render->AddActor2D(textactor);
//    createSliderTool();
    m_renwin->Render();
    m_renwin->SetCurrentCursor(VTK_CURSOR_HAND);
//    iren->Start();

}

void showutility::openModel(const char *filename)
{
    Instantiate(reader,vtkSTLReader);
    reader -> SetFileName(filename);
    reader -> Update();

    //build connection
    Instantiate(mapper,vtkPolyDataMapper);
    mapper -> SetInputConnection(reader -> GetOutputPort());
    m_stlactor -> SetMapper(mapper);
    has_stl = true;
    m_stlactor -> GetProperty() -> SetColor(1.0,0,0);
//    sliderWidget -> EnabledOn();
    m_render -> AddActor(m_stlactor);


//    renwin -> AddRenderer(render);

//    arrWidget->SetEnabled( 1 );
//    arrWidget->InteractiveOn();

//    annotation->SetText(0,filename);


    m_render->ResetCamera();
    iren -> Initialize();
    m_renwin -> Render();
//    iren -> Start();
}

void showutility::centerLnDis(std::vector<OutputImageType::PointType> &inpoints)
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

    m_render->AddActor(m_lineactor);

    m_renwin->Render();




}

void showutility::pickControl(vtkObject *caller, unsigned long eid,
                              void *clientdata, void *calldata)
{
//    double *selPt;
//    double *pickpos;
//    vtkSmartPointer<vtkPointPicker> pointerpicker= iren->GetPicker();
//    //    vtkPointPicker *pointerpicker = (vtkPointPicker *)iren->GetPicker();
//    selPt = pointerpicker->GetSelectionPoint();
//    std::cout << "screen location:" << selPt[0] << " " << selPt[1] << std::endl;
//    if (numPt >= 3)   return;
//    if(pointerpicker->GetPointId() >= 0){
//        numPt += 1;
//        pickpos = pointerpicker->GetPickPosition();
//        std::cout << "point location:" << pickpos[0]
//                   << " " << pickpos[1]
//                   << " " << pickpos[2] << std::endl;
//        points = vtkPoints::New();
//        points -> InsertNextPoint(pickpos);
//        points -> Modified();
//        Instantiate(profile,vtkPolyData);
//        profile -> SetPoints(points);
//        Instantiate(sphere,vtkSphereSource);
//        sphere -> SetRadius(0.02);
//        Instantiate(glyph,vtkGlyph3D);
//        glyph -> SetInputData(profile);
//        glyph -> SetSourceData(sphere->GetOutput());
//        Instantiate(glyphMapper,vtkPolyDataMapper);
//        glyphMapper -> SetInputData(glyph->GetOutput());
//        Instantiate(glyActor,vtkActor);
//        glyActor -> SetMapper(glyphMapper);
//        glyActor -> GetProperty() -> SetColor(0,1,0);
//        render -> AddActor(glyActor);
//        renwin -> Render();

//    }

}

showutility::~showutility()
{
    Connections -> Delete();
}

void showutility::SetMainModelOn()
{
    if(has_stl){
        m_render->AddActor(m_stlactor);
        m_renwin->Render();
    }
}

void showutility::SetMainModelOff()
{
    if(has_stl){
        m_render->RemoveActor(m_stlactor);
        m_renwin->Render();
    }
}

void showutility::SetLineModelOn()
{
    if(has_line){
        m_render->AddActor(m_lineactor);
        m_renwin->Render();
    }
}

void showutility::SetLineModelOff()
{
    if(has_line){
        m_render->RemoveActor(m_lineactor);
        m_renwin->Render();
    }
}


void showutility::initRenderWindow()
{
    vsp(axes);
    vsp(axesWidget);
    axesWidget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
    axesWidget->SetOrientationMarker( axes );
    axesWidget->SetInteractor( qvtk->GetRenderWindow()->GetInteractor() );
    axesWidget->SetViewport( .0, .8, .2, 1 );
    axesWidget->SetEnabled( 1 );
    axesWidget->InteractiveOn();

}

//void showutility::createSliderTool()
//{
//    vsp(sliderRep);
//    sliderRep->SetMinimumValue(0.0);
//    sliderRep->SetMaximumValue(1.0);
//    sliderRep->SetValue(1.0);

//    // Set color properties:
//      // Change the color of the knob that slides
//    sliderRep->GetSliderProperty()->SetColor(1,0,0);//red

//      // Change the color of the text indicating what the slider controls
//    sliderRep->GetTitleProperty()->SetColor(1,0,0);//red

//      // Change the color of the text displaying the value
//    sliderRep->GetLabelProperty()->SetColor(1,0,0);//red

//      // Change the color of the knob when the mouse is held on it
//    sliderRep->GetSelectedProperty()->SetColor(0,1,0);//green

//      // Change the color of the bar
//    sliderRep->GetTubeProperty()->SetColor(1,1,0);//yellow

//      // Change the color of the ends of the bar
//    sliderRep->GetCapProperty()->SetColor(1,1,0);//yellow
//    sliderRep->GetPoint1Coordinate()->SetCoordinateSystemToDisplay();
//    sliderRep->GetPoint1Coordinate()->SetValue(40 ,40);
//    sliderRep->GetPoint2Coordinate()->SetCoordinateSystemToDisplay();
//    sliderRep->GetPoint2Coordinate()->SetValue(120, 40);

//    vsp(sliderWidget);
//    sliderWidget->SetRepresentation(sliderRep);


//    vsp(slidercallback);
//    if(slidercallback->setBoundActor(vtkActor::SafeDownCast(m_stlactor)))
//        sliderWidget->AddObserver(vtkCommand::InteractionEvent,slidercallback);
//    sliderWidget->SetInteractor(iren);
//    sliderWidget->SetAnimationModeToAnimate();
//    sliderWidget->EnabledOff();

//}


void showutility::pointmode()
{
    if(m_render->GetActors()->GetNumberOfItems() != 0){
        m_stlactor->GetProperty()->SetRepresentationToPoints();
        m_render->Render();
        qvtk->update();
    }
}

void showutility::linemode()
{
    if(m_render->GetActors()->GetNumberOfItems() != 0){
        m_stlactor->GetProperty()->SetRepresentationToSurface();
        m_render->Render();
        qvtk->update();
    }
}

void showutility::framemode()
{
    if(m_render->GetActors()->GetNumberOfItems() != 0){
        m_stlactor->GetProperty()->SetRepresentationToWireframe();
        m_render->Render();
        qvtk->update();
    }
}

void showutility::popup(vtkObject *obj, unsigned long, void *client_data, void *, vtkCommand *command)
{
    vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
      // consume event so the interactor style doesn't get it
    command->AbortFlagOn();
      // get popup menu
    QMenu* popupMenu = static_cast<QMenu*>(client_data);
      // get event location
      int* sz = iren->GetSize();
      int* position = iren->GetEventPosition();
      // remember to flip y
    QPoint pt = QPoint(position[0], sz[1]-position[1]);
      // map to global
    QPoint global_pt = popupMenu->parentWidget()->mapToGlobal(pt);
      // show popup menu at global point
    popupMenu->popup(global_pt);
}

void showutility::color2(QAction *color)
{

    if(color->text() == "Background Grey")
        m_render->SetBackground(0.1,0.2,0.3);
    else if(color->text() == "Background Black")
        m_render->SetBackground(0,0,0);
    qvtk->update();
}

