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
#include <itkImageFileWriter.h>
#include <itkDanielssonDistanceMapImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkImageSliceConstIteratorWithIndex.h>

#include <vnl/vnl_math.h>
#include <itkPoint.h>



const unsigned int dimension = 3;
typedef unsigned char                                              InputPixelType;
typedef unsigned int                                             OutputPixelType;
typedef itk::Image< InputPixelType,dimension >                     InputImageType;
typedef itk::Image< OutputPixelType,dimension >                    OutputImageType;
typedef itk::DanielssonDistanceMapImageFilter<
             InputImageType,OutputImageType>                       FilterType;
typedef unsigned int                                               CoordType;
typedef itk::Point< CoordType, dimension>                           PointType;

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
};

#endif // ITKUTILS_H

