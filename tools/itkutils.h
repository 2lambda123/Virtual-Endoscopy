#ifndef ITKUTILS_H
#define ITKUTILS_H

# include <itkImageIOFactory.h>
# include <itkPNGImageIOFactory.h>
# include <itkMetaImageIOFactory.h>
# include <itkJPEGImageIOFactory.h>
# include <itkGDCMImageIOFactory.h>
# include <itkBMPImageIOFactory.h>

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageSeriesReader.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageFileWriter.h>
#include <itkMetaImageIO.h>
#include <itkImageToVTKImageFilter.h>
#include <gdcmUIDGenerator.h>
#include <itkNumericSeriesFileNames.h>
#include <itkDanielssonDistanceMapImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkImageSliceConstIteratorWithIndex.h>

#include <vnl/vnl_math.h>
#include <itkPoint.h>

#include <vtkSmartPointer.h>
#include <vtkImageFlip.h>
#include <vtkImageData.h>

#include <string>
#include <sstream>
//const unsigned int dimension = 3;
//typedef unsigned char                                              InputPixelType;
//typedef unsigned int                                             OutputPixelType;
//typedef itk::Image< InputPixelType,dimension >                     InputImageType;
//typedef itk::Image< OutputPixelType,dimension >                    OutputImageType;
//typedef itk::DanielssonDistanceMapImageFilter<
//             InputImageType,OutputImageType>                       FilterType;
//typedef unsigned int                                               CoordType;
//typedef itk::Point< CoordType, dimension>                           PointType;

class ItkUtils
{
public:

    static void RegisterAllImageIOFactories()
    {
        itk::PNGImageIOFactory::RegisterOneFactory();
        itk::BMPImageIOFactory::RegisterOneFactory();
        itk::MetaImageIOFactory::RegisterOneFactory();
        itk::JPEGImageIOFactory::RegisterOneFactory();
        itk::GDCMImageIOFactory::RegisterOneFactory();
    }

    template<typename ItkImageType>
    static vtkImageData* ConvToVtkImageData(ItkImageType* itkImageData)
    {
        typedef itk::ImageToVTKImageFilter<ItkImageType> Itk2VtkFilterType;
        Itk2VtkFilterType::Pointer filter = Itk2VtkFilterType::New();
        filter->SetInput(itkImageData);
        filter->Update();

        vtkSmartPointer<vtkImageFlip> flipX = vtkSmartPointer<vtkImageFlip>::New();
        vtkSmartPointer<vtkImageFlip> flipY = vtkSmartPointer<vtkImageFlip>::New();

        flipX->SetInputData(filter->GetOutput());
        flipX->SetFilteredAxis(1);
        flipX->Update();

        flipY->SetInputData(flipX->GetOutput());
        flipY->SetFilteredAxis(2);
        flipY->Update();

        vtkImageData* vtkImage = flipY->GetOutput();

        return vtkImage;

    }
};

#endif // ITKUTILS_H

