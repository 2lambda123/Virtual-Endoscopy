#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QWidget>

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkMarchingCubes.h>
#include <vtkVoxelModeller.h>
#include <vtkSphereSource.h>
#include <vtkImageData.h>
#include <vtkDICOMImageReader.h>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //setlayout
    initLayout();
    initSmallLayout();
//***********************************************************************
    initModules();
    initPopup();
    PreRenderWindow();
//******************************************************************************;

    ui->showPanelDock -> setFeatures(QDockWidget::DockWidgetMovable
                                     | QDockWidget::DockWidgetFloatable);
    ui->showPanelDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    initActions();
    initMenus();
    initDockContents();
//    clnproc = 0;
    setWindowTitle(tr("virtual endoscopy view"));
//    UpdateRenderWindow();
}

MainWindow::~MainWindow()
{

    delete ui;
    //    delete clnproc;
}

void MainWindow::initLayout()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;

    QVBoxLayout *leftLayout = new QVBoxLayout;
    QVBoxLayout *rightLayout = new QVBoxLayout;

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    m_qvtk = new QVTKWidget;
    m_qvtk->autoFillBackground();
    leftLayout->addWidget(m_qvtk);

    centralWidget()->setLayout(mainLayout);

}

void MainWindow::initSmallLayout()
{
    QHBoxLayout * mainLayout = new QHBoxLayout;
    QVBoxLayout * leftLayout = new QVBoxLayout;
    QVBoxLayout * rightLayout = new QVBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    m_smallqvtk = new QVTKWidget;
    m_smallqvtk->autoFillBackground();
    leftLayout->addWidget(m_smallqvtk);

    ui->GlobalViewwidget->setLayout(mainLayout);

}

void MainWindow::initActions()
{
    openSTLAct = new QAction(tr("Open 3D-Model"),this);
    openSTLAct -> setShortcut(tr("Ctrl+O"));
    connect(openSTLAct,SIGNAL(triggered(bool)),this,SLOT(openSTL()));
    exitAct = new QAction(tr("&Exit..."),this);
    exitAct->setShortcut(tr("Ctrl+E"));
    connect(exitAct,SIGNAL(triggered(bool)),this,SLOT(exit()));
    aboutAct = new QAction(tr("About..."),this);
    connect(aboutAct,SIGNAL(triggered(bool)),this,SLOT(about()));

    clnAct = new QAction(tr("Center Line"), this);
    connect(clnAct,SIGNAL(triggered(bool)),this,SLOT(processImg()));
    viewdockAct = new QAction(tr("View panel"),this);
    viewdockAct->setCheckable(true);
    viewdockAct->setChecked(true);
    connect(viewdockAct,SIGNAL(toggled(bool)),this,SLOT(viewdockpanel(bool)));

    testAct = new QAction(tr("test module"), this);
    connect(testAct,SIGNAL(triggered(bool)),this,SLOT(testModule()));


}

void MainWindow::initMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu -> addAction(openSTLAct);
    fileMenu -> addSeparator();
    fileMenu -> addAction(exitAct);
    procMenu = new QMenu(tr("&Process"), this);
    procMenu -> addAction(clnAct);
    viewMenu = new QMenu(tr("&View"), this);
    viewMenu -> addAction(viewdockAct);
    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu -> addAction(aboutAct);
    testMenu = new QMenu(tr("&Test"), this);
    testMenu -> addAction(testAct);
    menuBar() -> addMenu(fileMenu);
    menuBar() -> addMenu(procMenu);
    menuBar() -> addMenu(viewMenu);
    menuBar() -> addMenu(helpMenu);
    menuBar() -> addMenu(testMenu);
}

void MainWindow::initModegroup()
{
//    connect(ui->PointModeBtn,SIGNAL(toggled(bool)),leftshowUtil,SLOT(pointmode()));
//    connect(ui->LineModeBtn,SIGNAL(toggled(bool)),leftshowUtil,SLOT(framemode()));
//    connect(ui->FrameModeBtn,SIGNAL(toggled(bool)),leftshowUtil,SLOT(linemode()));
    ui->FrameModeBtn->setChecked(true);
}

void MainWindow::initDockContents()
{
    initModegroup();
    ui -> ModelCbx -> setEnabled(false);
    ui -> LineCbx -> setEnabled(false);
}

void MainWindow::initModules()
{
//    QHBoxLayout *layout = new QHBoxLayout;
//    leftshowUtil = showutility::New();
//    layout -> addWidget(leftshowUtil);
//    centralWidget()->setLayout(layout);

    m_mainShowUtil = new displayUtils(m_qvtk->GetRenderWindow());
    // assistant global view unit
    m_assistShowUtil = new GlobalViewUtils(m_smallqvtk->GetRenderWindow());

}

void MainWindow::initPopup()
{
    Connections = vtkEventQtSlotConnect::New();
    popup2 = new QMenu(m_qvtk);
    popup2->addAction("Background Grey");
    popup2->addAction("Background Black");
    connect(popup2,SIGNAL(triggered(QAction*)),this,SLOT(color2(QAction*)));
    Connections->Connect(m_qvtk->GetRenderWindow()->GetInteractor(),
                         vtkCommand::RightButtonPressEvent,
                         this,
                         SLOT(popup(vtkObject*,unsigned long,void*,void*,vtkCommand*)),
                         popup2,1.0);
}

void MainWindow::PreRenderWindow()
{
    //http://www.vtk.org/Wiki/VTK/Examples/Cxx/Visualization/DisplayCoordinateAxes
    m_qvtk->GetRenderWindow()->AddRenderer(m_mainShowUtil->GetRenderer());
    axes = vtkAxesActor::New();
    axesWidget = vtkOrientationMarkerWidget::New();
    axesWidget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
    axesWidget->SetOrientationMarker( axes );
    axesWidget->SetInteractor( m_qvtk->GetRenderWindow()->GetInteractor() );
    axesWidget->SetViewport( .0, .8, .2, 1 );
    axesWidget->SetEnabled( 1 );
    axesWidget->InteractiveOn();
    UpdateRenderWindow();

}

void MainWindow::UpdateRenderWindow()
{
//    leftshowUtil->GetQvtkWidget()->GetRenderWindow()->Render();
    m_qvtk->GetRenderWindow()->Render();

}



void MainWindow::loadSeriesImg()
{

}

void MainWindow::popup(vtkObject *obj, unsigned long, void *client_data, void *, vtkCommand *command)
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

void MainWindow::color2(QAction *color)
{
    if(color->text() == "Background Grey")
        m_mainShowUtil->GetRenderer()->SetBackground(0.1,0.2,0.3);
    else if(color->text() == "Background Black")
        m_mainShowUtil->GetRenderer()->SetBackground(0,0,0);
    m_qvtk->update();
}

void MainWindow::openSTL()
{
    QString filter("STL File (*.stl)");
    QString filename = QFileDialog::getOpenFileName(this, QString(tr("Open a stl model")),
                                                    filename, filter);
    if(filename.trimmed().isEmpty()){
        return;
    }

    this->setWindowTitle(QString::fromUtf8("Surface Renderer - ") + filename);
//    QByteArray ba = filename.toLocal8Bit();
//    const char *filename_str = ba.data();



    m_mainShowUtil->OpenSegmentModel(filename.toStdString());
    m_mainShowUtil->SetMainModelOn();
    UpdateRenderWindow();
    ui -> ModelCbx -> setEnabled(true);
    ui -> ModelCbx -> setChecked(true);


    m_assistShowUtil->SetMainModelName(filename.toStdString());
    m_assistShowUtil->FormMainModel();
    //m_assistShowUtil->FormMainModel();
    m_smallqvtk->GetRenderWindow()->Render();

}
void MainWindow::processImg()
{
//    centerLineProc *clnProc = centerLineProc::New();
    QElapsedTimer t;
    t.start();
    centerLineProc *clnProc = new centerLineProc;
    if(clnProc){
        //1.--distance transform method
        //clnProc -> getDistanceMap();
        //2.--3d Binary thinning method
//        clnProc -> Path_Thin3dImg();
//        leftshowUtil->centerLnDis(clnProc->getpoints());
        //3.--signed distance transform method
        clnProc -> getSignedDistanceMap_Mul();
        m_mainShowUtil->centerLnDis(clnProc->getpoints());
        //leftshowUtil->centerLnDis(clnProc->getpoints());
        ui -> LineCbx -> setEnabled(true);
        ui -> LineCbx -> setChecked(true);
    }

//    clnproc = new centerLineProc;
//    if(clnproc)
//        clnproc -> getDistanceMap();

//    try{
//        typedef itk::ImageFileReader< InputImageType> ReaderType;

//        typedef itk::ImageFileWriter< OutputImageType > WriterType;
//        ReaderType::Pointer reader = ReaderType::New();

//        //        WriterType::Pointer writer = WriterType::New();

//        std::string input("D:\\3dresearch\\heart-artery\\out\\se1ct1.mhd");
//        reader->SetFileName(input);

//        //        writer->SetFileName(OutputmapName);

//        FilterType::Pointer filter = FilterType::New();
//        filter->SetInput(reader->GetOutput());

//        typedef itk::RescaleIntensityImageFilter<
//                OutputImageType,OutputImageType> RescalerType;
//        RescalerType::Pointer scaler = RescalerType::New();

//        scaler->SetInput(filter->GetOutput());
//        scaler->SetOutputMinimum(0);
//        scaler->SetOutputMaximum(65535);
//        OutputImageType::Pointer outImg = scaler->GetOutput();
//        scaler->Update();
//        std::ofstream out("localmax.txt");
//        if(out.is_open())
//            out << input << std::endl;
// // ***********************************************************************************
// //       how to interate an 3d image
// //       reference to:
// //       http://sourcecodebrowser.com/insighttoolkit/3.16.0/itk_image_slice_iterator_test_8cxx.html
// //       http://l3mmings.blogspot.com/2010/08/slices-stacks-and-itk.html
// // *************************************************************************************
//        typedef itk::ImageSliceConstIteratorWithIndex< OutputImageType > sliceinterator;
//        sliceinterator sliceit(outImg,outImg->GetLargestPossibleRegion());
//        sliceit.SetFirstDirection(0); // 0 -x
//        sliceit.SetSecondDirection(1); // 1 - y
//        localMaxVec.clear();
//        for(sliceit.GoToBegin();!sliceit.IsAtEnd();sliceit.NextSlice()){
//            OutputImageType::PixelType maxval = 0;
//            OutputImageType::IndexType maxindex = sliceit.GetIndex();
//            while(!sliceit.IsAtEndOfSlice()){
//                while(!sliceit.IsAtEndOfLine()){
//                    if(sliceit.Get() > maxval){
//                        maxval = sliceit.Get();
//                        maxindex = sliceit.GetIndex();
//                    }
//                    ++sliceit;
//                }
//                sliceit.NextLine();
//            }
//            localMaxVec.push_back(maxindex); // get local max position of a slice
//            if(out.is_open())
//                out << maxindex[0] << ", "
//                                   << maxindex[1] << ", "
//                                   << maxindex[2] << std::endl;

//        }
//        scaler -> Delete();
//        filter -> Delete();
//        reader -> Delete();
//    }
//    catch(itk::ExceptionObject &exp)
//    {
//        std::cerr << "Exception caught !" << endl;
//        return ;
//    }
    QMessageBox::information(this,tr("process notify"),
           QString("successfully extract centerline "
                      "after take %1 ms").arg(t.elapsed(),0));
}


void MainWindow::viewdockpanel(bool checked)
{
    if(checked) {
        ui->showPanelDock->show();
    }
    else{
        ui->showPanelDock->hide();
    }

}

void MainWindow::testModule()
{
//    QString filter("STL File (*.stl)");
//    QString filename = QFileDialog::getOpenFileName(this, QString(tr("Open a stl model")),
//                                                    filename, filter);
//    if(filename.trimmed().isEmpty()){
//        return;
//    }
//    m_assistShowUtil->FormMainModel(filename.toStdString());
//    m_smallqvtk->GetRenderWindow()->Render();
//    std::cout << " rendering completed" << std::endl;
    QElapsedTimer t;
    t.start();
    m_mainShowUtil->GetCenterline();
    QMessageBox::information(this,tr("process notify"),
           QString("test process complete "
                      "after %1 ms").arg(t.elapsed(),0));
 }

void MainWindow::on_LineCbx_toggled()
{
    if(ui->LineCbx->isChecked()){
        m_mainShowUtil->SetLineModelOn();
    }else{
        m_mainShowUtil->SetLineModelOff();
    }
    UpdateRenderWindow();
}

void MainWindow::on_ModelCbx_toggled()
{
    if(ui->ModelCbx->isChecked()){
        m_mainShowUtil->SetMainModelOn();
    }else{
        m_mainShowUtil->SetMainModelOff();
    }
    UpdateRenderWindow();
}

void MainWindow::on_PointModeBtn_toggled()
{
    m_mainShowUtil->pointmode();
    UpdateRenderWindow();
}

void MainWindow::on_LineModeBtn_toggled()
{
    m_mainShowUtil->linemode();
    UpdateRenderWindow();
}

void MainWindow::on_FrameModeBtn_toggled()
{
    m_mainShowUtil->framemode();
    UpdateRenderWindow();
}

void MainWindow::on_PointPickerCbx_toggled(bool checked)
{
    m_mainShowUtil->SetPointerPickEnabled(checked);
}

void MainWindow::on_StartBtn_clicked()
{
    m_mainShowUtil->OnRoam();
    UpdateRenderWindow();

    //m_assistShowUtil->FormMainModel();
}

void MainWindow::on_PreviousBtn_clicked()
{
    m_mainShowUtil->RoamPrevious();
    UpdateRenderWindow();
}

void MainWindow::on_NextBtn_clicked()
{
    m_mainShowUtil->RoamNext();
    UpdateRenderWindow();
}
