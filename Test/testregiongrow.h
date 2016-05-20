#ifndef TESTREGIONGROW
#define TESTREGIONGROW
/*
 * test region growing
 * */
#include <itkImage.h>
#include <itkImageFileWriter.h>
#include <itkRegionGrowImageFilter.h>
#include <itkCastImageFilter.h>

#include <itkImageToVTKImageFilter.h>

#include <vtkImageViewer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
class Test_RegionGrow {
public:
    int RunTest()
    {
        ImageType::Pointer image = ImageType::New();
        CreateImage(image);
        typedef itk::RegionGrowImageFilter<
                ImageType,ImageType> RegionGrowImageFilterType;
        RegionGrowImageFilterType::Pointer grower = RegionGrowImageFilterType::New();
        float lower = 95.0;
        float upper = 105.0;
        grower->SetLower(lower);
        grower->SetUpper(upper);
        grower->SetReplaceValue(255);
        //seed 1
        ImageType::IndexType seed1;
        seed1[0] = 25;
        seed1[1] = 35;
        grower->SetSeed(seed1);
        grower->SetInput(image);
        //seed 2
        ImageType::IndexType seed2;
        seed2[0] = 110;
        seed2[1] = 120;
        grower->SetSeed(seed2);
        grower->SetReplaceValue(150);
        grower->SetInput(image);

        typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;
        ConnectorType::Pointer connector = ConnectorType::New();
        connector->SetInput(image2);

        vtkSmartPointer<vtkImageActor> actor2 = vtkSmartPointer<vtkImageActor>::New();
        actor2->SetInputData(connector->GetOutput());

        ConnectorType::Pointer addConnector = ConnectorType::New();
        addConnector->SetInput(addFilter->GetOutput());

        vtkSmartPointer<vtkImageActor> addActor = vtkSmartPointer<vtkImageActor>::New();
        addActor->SetInputData(addConnector->GetOutput());

        vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
        renderWindow->SetSize(900, 300);
        vtkSmartPointer<vtkRenderWindowInteractor> interactor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
        interactor->SetRenderWindow(renderWindow);

        double leftViewport[4] = {0.0, 0.0, 0.33, 1.0};
        double centerViewport[4] = {0.33, 0.0, 0.66, 1.0};
        double rightViewport[4] = {0.66, 0.0, 1.0, 1.0};
        vtkSmartPointer<vtkRenderer> leftRenderer =
           vtkSmartPointer<vtkRenderer>::New();
        renderWindow->AddRenderer(leftRenderer);
        leftRenderer->SetViewport(leftViewport);
        leftRenderer->SetBackground(.6, .5, .4);

        vtkSmartPointer<vtkRenderer> centerRenderer =
           vtkSmartPointer<vtkRenderer>::New();
        renderWindow->AddRenderer(centerRenderer);
        centerRenderer->SetViewport(centerViewport);
        centerRenderer->SetBackground(.4, .5, .6);

        vtkSmartPointer<vtkRenderer> rightRenderer =
            vtkSmartPointer<vtkRenderer>::New();
        renderWindow->AddRenderer(rightRenderer);
        rightRenderer->SetViewport(rightViewport);
        rightRenderer->SetBackground(.4, .5, .6);

        leftRenderer->AddActor(actor1);
        centerRenderer->AddActor(actor2);
        rightRenderer->AddActor(addActor);

        leftRenderer->ResetCamera();
        centerRenderer->ResetCamera();
        rightRenderer->ResetCamera();

        renderWindow->Render();

        vtkSmartPointer<vtkInteractorStyleImage> style =
                vtkSmartPointer<vtkInteractorStyleImage>::New();
        interactor->SetInteractorStyle(style);
        interactor->Start();

        return EXIT_SUCCESS;

    }
private:
    const int Dimension = 2;
    typedef float  PixelType;
    typedef itk::Image<PixelType, Dimension> ImageType;
    void CreateImage(ImageType::Pointer image)
    {
        ImageType::RegionType region;
        ImageType::IndexType start;
        start[0] = 0;
        start[1] = 0;
        ImageType::SizeType size;
        size[0] = 200;
        size[1] = 300;
        region.SetSize(size);
        region.SetIndex(index);
        image->SetRegions(region);
        image->Allocate();
        //make a square
        for(unsigned int r = 20; r < 80; r++) {
            for(unsigned int c = 30; c < 100; c++) {
                ImageType::IndexType pixelIndex;
                pixelIndex[0] = r;
                pixelIndex[1] = c;
                image->SetPixel(pixelIndex, 100.0);
            }
        }
        //make another square
        for(unsigned int r = 100; r < 130; r++) {
            for(unsigned int c = 115; c < 160; c++) {
                ImageType::IndexType pixelIndex;
                pixelIndex[0] = r;
                pixelIndex[1] = c;
                image->SetPixel(pixelIndex, 100.0);
            }
        }
    }
};
#endif // TESTREGIONGROW

