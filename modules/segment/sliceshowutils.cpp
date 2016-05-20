#include "sliceshowutils.h"

#include "QString"
#include "QFileDialog"
#include "QDebug"
vtkStandardNewMacro(DCMVtkInteractorStyleImage);

sliceshowutils::sliceshowutils()
{
    Instantiate(m_imageViewer,vtkImageViewer2);
    initUsageHint();
    initSliceHint();
    Instantiate(dcmInteractorStyle,DCMVtkInteractorStyleImage);
    dcmInteractorStyle->SetImageViewer(m_imageViewer);
    dcmInteractorStyle->SetStatusMapper(m_sliceTextMapper);

}

void sliceshowutils::openWithITK()
{
//    QString imageSeriesFolder = QFileDialog::getExistingDirectory(this,QString("Open DICOM Files"),
//                                                                  QDir::currentPath(),
//                                                                  QFileDialog::ShowDirsOnly);
//    if(imageSeriesFolder.length() == 0)    return ;
//    typedef itk::Image< unsigned short,3> ImageType;
//    typedef itk::ImageSeriesReader< ImageType > ReaderType;
//    typedef itk::GDCMImageIO ImageIOType;
//    typedef itk::GDCMSeriesFileNames NamesGeneratorType;
//    ReaderType::Pointer reader = ReaderType::New();
//    ImageIOType::Pointer gdcmImageIO = ImageIOType::New();
//    NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
//    try{
//        nameGenerator->SetUseSeriesDetails( true );
//        nameGenerator->AddSeriesRestriction("0008|0021");
//        nameGenerator->SetDirectory( imageSeriesFolder.toStdString() );

//        typedef std::vector< std::string > SeriesIdContainer;
//        const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
//        SeriesIdContainer::const_iterator seriesItr = seriesUID.begin();
//        SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();
//        while( seriesItr != seriesEnd ) {
//            std::cout << seriesItr->c_str() << std::endl;
//            ++seriesItr;
//        }

//        std::string seriesIdentifier = seriesUID.begin()->c_str();
//        std::vector<std::string> fileNames = nameGenerator->GetFileNames( seriesIdentifier );
//        std::cout << fileNames.size() << std::endl;

//        reader->SetFileNames( fileNames );
//        reader->Update();
//    }catch(std::exception &ex) {
//        std::cout << ex.what() << std::endl;
//    }

//    //-----------------------------------------------------------------------
//    // ITK to VTK pipeline connection
//    //-----------------------------------------------------------------------

//    typedef itk::VTKImageExport< ImageType > ImageExportType;
//    ImageExportType::Pointer exporter = ImageExportType::New();
//    exporter->SetInput(reader->GetOutput());

//    vtkImageImport* importer = vtkImageImport::New();
//    ConnectPipelines(exporter,importer);
//    m_imageViewer->SetInputData(importer->GetOutput());
//    m_imageViewer->Render();


}

void sliceshowutils::initSliceHint()
{
    Instantiate(m_sliceTextProp,vtkTextProperty);
    Instantiate(m_sliceTextMapper,vtkTextMapper);
    Instantiate(m_sliceTextActor,vtkActor2D);
    m_sliceTextProp->SetFontFamilyToCourier();
    m_sliceTextProp->SetFontSize(20);
    m_sliceTextProp->SetVerticalJustificationToBottom();

    std::string msg = StatusMessage::Format(m_imageViewer->GetSliceMin(),
                                            m_imageViewer->GetSliceMax());
    m_sliceTextMapper->SetInput(msg.c_str());
    m_sliceTextMapper->SetTextProperty(m_sliceTextProp);
    m_sliceTextActor->SetMapper(m_sliceTextMapper);
    m_sliceTextActor->SetPosition(15,10);
}

void sliceshowutils::initUsageHint()
{
    Instantiate(m_usageTextProp,vtkTextProperty);
    Instantiate(m_usageTextMapper,vtkTextMapper);
    Instantiate(m_usageTextActor,vtkActor2D);
    m_sliceTextProp->SetFontFamilyToCourier();
    m_sliceTextProp->SetFontSize(15);
    m_sliceTextProp->SetVerticalJustificationToTop();
    m_usageTextProp->SetJustificationToLeft();

    m_usageTextMapper->SetInput("- Slice with mouse wheel\n  or Up/Down-Key\n"
                                "- Zoom with pressed right mouse button while dragging");
    m_usageTextMapper->SetTextProperty(m_usageTextProp);

    m_usageTextActor->SetMapper(m_usageTextMapper);
    m_usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToDisplay();
    m_usageTextActor->GetPositionCoordinate()->SetValue(0.05,0.95);
}

