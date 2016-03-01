#ifndef CENTERLINEPROC_H
#define CENTERLINEPROC_H
// itk includes
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkDanielssonDistanceMapImageFilter.h>
#include <itkSignedDanielssonDistanceMapImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkImageSliceConstIteratorWithIndex.h>
#include <itkGradientMagnitudeImageFilter.h>

#include <itkNumericTraits.h>
#include <itkPolyLineParametricPath.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkGradientDescentOptimizer.h>
#include <itkRegularStepGradientDescentOptimizer.h>
#include <itkPathIterator.h>
#include <itkNearestNeighborInterpolateImageFunction.h>

//#include "findpath/itkSpeedFunctionToPathFilter.h"
//#include "findpath/itkArrivalFunctionToPathFilter.h"
//#include "findpath/itkIterateNeighborhoodOptimizer.h"

#include <vnl/vnl_math.h>
#include <itkPoint.h>

#include <vtkPoints.h>
#include <vtkSmartPointer.h>

//    general includes
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//    customized includes
#include "tools/initials.h"
#include "modules/findpath/itkBinaryThinningImageFilter3D.h"

//const unsigned int dimension = 3;
//typedef unsigned char                                              InputPixelType;
//typedef unsigned int                                             OutputPixelType;
//typedef itk::Image< InputPixelType,dimension >                     InputImageType;
//typedef itk::Image< OutputPixelType,dimension >                    OutputImageType;
//typedef itk::DanielssonDistanceMapImageFilter<
//             InputImageType,OutputImageType>                       FilterType;
//typedef unsigned int                                               CoordType;
//typedef itk::Point< CoordType, dimension>                           PointType;
typedef float                                                         PixelType;
typedef unsigned short                                                VoronoiPixelType;
typedef itk::Image< PixelType,dimension>                              OutputMapType;
typedef itk::Image< VoronoiPixelType,dimension>                       VoronoiMapType;

class centerLineProc
{
public:
    centerLineProc();
//    static centerLineProc *New()
//    {
//        return new centerLineProc;
//    }
    int getDistanceMap();
    int getSignedDistanceMap_Sin(); //初试版本，寻找一条中心线；
    int getSignedDistanceMap_Mul(); //寻找多条中心线；
    void GetThin3dImg();
    std::vector<OutputImageType::PointType>& getpoints()
    {
        return MinPoints;
    }
    int GetCenterlinePointNums();
    void ExportCenterkineData(std::string filename);
    void GetcenterlinePoint(int index, double p[3]);
    // ******************************************************
//    int Path_GradientDescent();
    // ******************************************************
private:
    struct Point3f {
        double x, y, z;
    };

    bool CenterLineFlag;  // has centerline?
    //vector for local-max index of OutputImage
    std::vector<OutputImageType::IndexType> localMinVec;
    //vector for local-max physical point of outputImage
    std::vector<OutputImageType::PointType> MinPoints;

    std::string InputfileName;
//    char *OutputmapName = 0;
    void setInput(std::string input)
    {
        InputfileName = input;
    }
    void clear()
    {
        localMinVec.clear();
    }

};

#endif // CENTERLINEPROC_H
