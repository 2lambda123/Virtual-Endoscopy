#include "centerlineproc.h"

#include <fstream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include "itksys/SystemTools.hxx"
centerLineProc::centerLineProc():CenterLineFlag(false)
{
    setInput(std::string("D:\\3dresearch\\heart-artery\\out\\se1ct1.mhd"));
    clear();

}

int centerLineProc::getDistanceMap()
{
    try{
        typedef itk::ImageFileReader< InputImageType > ReaderType;
//        typedef itk::ImageFileWriter< OutputImageType > WriterType;
        ReaderType::Pointer reader = ReaderType::New();
//        WriterType::Pointer writer = WriterType::New();
        reader->SetFileName(InputfileName);
//        writer->SetFileName(OutputmapName);

        //**************test spacing***********************
        InputImageType::Pointer inImg = reader->GetOutput();
        reader->Update();
        InputImageType::SpacingType isp = inImg->GetSpacing();
        std::cout << isp[0] << " ," << isp[1] << " ," << isp[2] << std::endl;
        InputImageType::PointType origin = inImg->GetOrigin();
        std::cout << origin[0] << " ," << origin[1] << " ," << origin[2] << std::endl;

        FilterType::Pointer filter = FilterType::New();
        filter->SetInput(reader->GetOutput());

        typedef itk::RescaleIntensityImageFilter<
                OutputImageType,OutputImageType > RescalerType;
        RescalerType::Pointer scaler = RescalerType::New();

        scaler->SetInput(filter->GetOutput());
        scaler->SetOutputMinimum(0);
        scaler->SetOutputMaximum(65535);
        OutputImageType::Pointer outImg = scaler->GetOutput();
        scaler->Update();
        std::ofstream out("localmax.txt");
        if(out.is_open())
            out << InputfileName << std::endl;
//       how to interate an 3d image
//       reference to:
//       http://sourcecodebrowser.com/insighttoolkit/3.16.0/itk_image_slice_iterator_test_8cxx.html
//       http://l3mmings.blogspot.com/2010/08/slices-stacks-and-itk.html
        typedef itk::ImageSliceConstIteratorWithIndex< OutputImageType > sliceinterator;
        sliceinterator sliceit(outImg,outImg->GetLargestPossibleRegion());
        sliceit.SetFirstDirection(0); // 0 -x
        sliceit.SetSecondDirection(1); // 1 - y
        sliceit.GoToBegin();
        OutputImageType::IndexType preminindex = sliceit.GetIndex();
        for(int i = 0 ;!sliceit.IsAtEnd();i++,sliceit.NextSlice()){
            OutputImageType::PixelType minval = sliceit.Get();
            OutputImageType::IndexType minindex = sliceit.GetIndex();
            ++sliceit;
            while(!sliceit.IsAtEndOfSlice()){
                while(!sliceit.IsAtEndOfLine()){
                    if(sliceit.Get() < minval){

                        minval = sliceit.Get();
                        minindex = sliceit.GetIndex();
                    }
                    ++sliceit;
                }
                sliceit.NextLine();
            }
            if (i == 0){
                preminindex = minindex;
            }
            else{
                if(abs(preminindex[0] - minindex[0]) > 20
                        || abs(preminindex[1] - minindex[1]) > 20){
                    //change too sharply,use last minindex;
                    minindex[0] = preminindex[0];
                    minindex[1] = preminindex[1];
                    minindex[2] = preminindex[2] + 1;
                }
                //update  minindex of last slice;
                preminindex[0] = minindex[0];
                preminindex[1] = minindex[1];
                preminindex[2] = minindex[2] - 1;
            }
            OutputImageType::PointType minpoint;
            outImg->TransformIndexToPhysicalPoint(minindex,minpoint);
            localMinVec.push_back(minindex); // get local min position of a slice
            MinPoints.push_back(minpoint);
            if(out.is_open())
                out << minindex[0] << ", "
                                   << minindex[1] << ", "
                                   << minindex[2] << "<----||---->"
                                   << minpoint[0] << ", "
                                   << minpoint[1] << ", "
                                   << minpoint[2] << std::endl;

        }

        CenterLineFlag = true;
    }
    catch(itk::ExceptionObject &exp)
    {
        std::cerr << "Exception caught !" << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;


}

int centerLineProc::getSignedDistanceMap_Sin()
{
    typedef itk::ImageFileReader< InputImageType > ReaderType;
    typedef itk::ImageFileWriter< OutputMapType > WriterType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(InputfileName);
    InputImageType::Pointer inImg = reader->GetOutput();
    reader->Update();
    InputImageType::SpacingType isp = inImg->GetSpacing();
    std::cout << isp[0] << " ," << isp[1] << " ," << isp[2] << std::endl;
    InputImageType::PointType origin = inImg->GetOrigin();
    std::cout << origin[0] << " ," << origin[1] << " ," << origin[2] << std::endl;
    std::ofstream out("localmax.txt");
    if(out.is_open())
        out << InputfileName << std::endl;
    typedef itk::SignedDanielssonDistanceMapImageFilter<
            InputImageType,
            OutputMapType> FilterType;
    FilterType::Pointer filter = FilterType::New();
    filter -> SetInput(reader -> GetOutput());
    OutputMapType::Pointer outImg = OutputMapType::New();
    outImg = filter -> GetOutput();
    filter -> Update();
    typedef itk::ImageSliceConstIteratorWithIndex< OutputMapType > sliceinterator;
    sliceinterator sliceit(outImg,outImg->GetLargestPossibleRegion());
    sliceit.SetFirstDirection(0); // 0 -x
    sliceit.SetSecondDirection(1); // 1 - y
    sliceit.GoToBegin();
    OutputMapType::IndexType preminindex = sliceit.GetIndex();
    for( ; !sliceit.IsAtEnd(); sliceit.NextSlice()){
        OutputMapType::PixelType minval = sliceit.Get();
        OutputMapType::IndexType minindex = sliceit.GetIndex();
        while(!sliceit.IsAtEndOfSlice()){
            while(!sliceit.IsAtEndOfLine()){
                if(sliceit.Get() < minval){
                    minval = sliceit.Get();
                    minindex = sliceit.GetIndex();
                }
                ++sliceit;

            }
            sliceit.NextLine();
        }
        if(minval < 0){
            if((int)localMinVec.size() == 0){
                preminindex = minindex;
            }else{
                if(abs(preminindex[0] - minindex[0]) > 10
                        || abs(preminindex[1] - minindex[1]) > 10){
                    //change too sharply,use last minindex;
                    minindex[0] = preminindex[0];
                    minindex[1] = preminindex[1];
                    minindex[2] = preminindex[2] + 1;
                }
                //update for next interation;
                preminindex = minindex;
            }
            OutputImageType::PointType minpoint;
            outImg->TransformIndexToPhysicalPoint(minindex,minpoint);
            localMinVec.push_back(minindex); // get local min position of this slice
            MinPoints.push_back(minpoint);
            if(out.is_open())
                out << minindex[0] << ", "
                                   << minindex[1] << ", "
                                   << minindex[2] << "<----||---->"
                                   << minpoint[0] << ", "
                                   << minpoint[1] << ", "
                                   << minpoint[2] << std::endl;
        }
    }
    WriterType::Pointer writer = WriterType::New();
    writer -> SetFileName("distance.mhd");
    writer -> SetInput(filter -> GetOutput());
    try{
        writer -> Update();
    }catch(itk::ExceptionObject & exp){
        std::cerr << "Exception caught !" << std::endl;
        std::cerr <<     exp    << std::endl;
        return EXIT_FAILURE;
    }
    CenterLineFlag = true;
    return EXIT_SUCCESS;


}

int centerLineProc::getSignedDistanceMap_Mul()
{

    typedef itk::ImageFileReader< InputImageType > ReaderType;
    typedef itk::ImageFileWriter< OutputMapType > WriterType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(InputfileName);
    InputImageType::Pointer inImg = reader->GetOutput();
    reader->Update();
    InputImageType::SpacingType isp = inImg->GetSpacing();
    std::cout << isp[0] << " ," << isp[1] << " ," << isp[2] << std::endl;
    InputImageType::PointType origin = inImg->GetOrigin();
    std::cout << origin[0] << " ," << origin[1] << " ," << origin[2] << std::endl;
    std::ofstream out("localmax.txt");
    if(out.is_open())
        out << InputfileName << std::endl;
    typedef itk::SignedDanielssonDistanceMapImageFilter<
            InputImageType,
            OutputMapType> FilterType;
    FilterType::Pointer filter = FilterType::New();
    filter -> SetInput(reader -> GetOutput());
    //        OutputMapType::Pointer outImg = OutputMapType::New();
    //        outImg = filter -> GetOutput();
    //        filter -> Update();
    typedef itk::GradientMagnitudeImageFilter< OutputMapType,OutputMapType> MagfilterType;
    MagfilterType::Pointer filter2 = MagfilterType::New();
    filter2 -> SetInput(filter -> GetOutput());
    filter2 -> Update();
    OutputMapType::Pointer outImg = OutputMapType::New();
    outImg = filter2 -> GetOutput();
    try{

        typedef itk::ImageSliceConstIteratorWithIndex< OutputMapType > sliceinterator;
        sliceinterator sliceit(outImg,outImg->GetLargestPossibleRegion());
        sliceit.SetFirstDirection(0); // 0 -x
        sliceit.SetSecondDirection(1); // 1 - y
        sliceit.GoToBegin();
        for( ; !sliceit.IsAtEnd(); sliceit.NextSlice()){
            while(!sliceit.IsAtEndOfSlice()){
                while(!sliceit.IsAtEndOfLine()){
                    if(abs(sliceit.Get()) == 0.0){
                        OutputMapType::IndexType minindex = sliceit.GetIndex();
                        OutputMapType::PointType minpoint;
                        outImg->TransformIndexToPhysicalPoint(minindex,minpoint);
                        MinPoints.push_back(minpoint);
                    }
                    ++sliceit;
                }
                sliceit.NextLine();
            }
        }
    }
    catch(itk::ExceptionObject & exp){
        std::cerr << "Exception caught !" << std::endl;
        std::cerr <<     exp    << std::endl;
        return EXIT_FAILURE;
    }
    CenterLineFlag = true;
    return EXIT_SUCCESS;

}

void centerLineProc::Path_Thin3dImg(std::string filename)
{
    CenterPoints.clear();//remove last centerline results;
    const int Dimension = 3;
    typedef unsigned char                                   InputPixelType;
    typedef unsigned char                                   OutputPixelType;
    typedef itk::Image<InputPixelType, Dimension>           InputImageType;
    typedef itk::Image<OutputPixelType, Dimension>          OutputImageType;
    typedef itk::ImageFileReader< InputImageType>           ReaderType;
    typedef itk::BinaryThinningImageFilter3D< InputImageType,OutputImageType >
                                                            ThinningFilterType;
    typedef itk::NeighborhoodIterator<OutputImageType>      NeighborhoodIteratorType;

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(filename);
    try{
        reader->Update();
    }catch (itk::ExceptionObject &exp){
        std::cerr << exp.what() << " Exception caught !" << std::endl;
        return ;
    }
    InputImageType::Pointer input = reader->GetOutput();
    ThinningFilterType::Pointer thinningFilter = ThinningFilterType::New();
    thinningFilter->SetInput(input);
    thinningFilter->Update();

    OutputImageType::Pointer output = OutputImageType::New();
    output = thinningFilter->GetOutput();
    output->SetRegions( input->GetLargestPossibleRegion() );
    output->SetSpacing( input->GetSpacing() );
    output->SetOrigin( input->GetOrigin() );
    output->Allocate( );
    NeighborhoodIteratorType::RadiusType  radius;
    radius[0] = 1; radius[1] = 1;radius[2] = 1;
    OutputImageType::IndexType regionIndex;
    regionIndex[0] = 0;regionIndex[1] = 0;regionIndex[2] = 0;
    OutputImageType::SizeType regionSize = output->GetRequestedRegion().GetSize();
    OutputImageType::RegionType region;
    region.SetSize(regionSize);
    region.SetIndex(regionIndex);
    NeighborhoodIteratorType it(radius,output,region);
    OutputImageType::PointType point;
    Point3f Cenpoint;
    for(it.GoToBegin();!it.IsAtEnd();++it){
        if(it.GetCenterPixel() == 0)    continue;
        OutputImageType::IndexType index;
        index = it.GetIndex();
        std::cout << index[0] << ", " << index[1] << ", " << index[2] << "\n";
        output->TransformIndexToPhysicalPoint(index,point);
        Cenpoint.x = point[0];Cenpoint.y = point[1];Cenpoint.z = point[2];
        CenterPoints.push_back(Cenpoint);
    }




}


int centerLineProc::GetCenterlinePointNums()
{
    return MinPoints.size();
}

void centerLineProc::ExportCenterlineData(std::string filename)
{
    std::vector<OutputImageType::PointType> &vec = MinPoints;
    FILE * fp = fopen(filename.c_str(), "w");
    if(fp == NULL){
        return ;
    }

    for(int i = 0; i < vec.size();i++){
        fprintf(fp,"%lf\t%lf\t%lf\n",(vec[i])[0], (vec[i])[1], (vec[i])[2]);
    }
    fclose(fp);
}

void centerLineProc::GetcenterlinePoint(int index, double p[3])
{
    memset(p, .0, 3 * sizeof(double));
    if(0 <= index && index < MinPoints.size()) {
        p[0] = MinPoints[index][0];
        p[1] = MinPoints[index][1];
        p[2] = MinPoints[index][2];
    }
}

int centerLineProc::Path_GradientDescent(std::string filename, double ps[], double pe[])
{
    CenterPoints.clear();//remove last centerline results;
    const unsigned int Dimension = 3;
    typedef unsigned char InputPixelType;
    typedef float InterPixelType;
    typedef unsigned char OutputPixelType;
    typedef itk::Image< InputPixelType, Dimension >                       InputImageType;
    typedef itk::Image< InterPixelType, Dimension >                       InterImageType;
    typedef itk::Image< OutputPixelType, Dimension>                       OutputImageType;
    typedef itk::ImageFileReader< InputImageType >                        ReaderType;
    typedef itk::ImageFileWriter< OutputImageType >                       WriterType;
    typedef itk::PolyLineParametricPath< Dimension >                      PathType;
    typedef itk::SpeedFunctionToPathFilter< InterImageType,PathType >     PathFilterType;
    typedef PathFilterType::CostFunctionType::CoordRepType       CoordRepType;
    typedef itk::PathIterator< OutputImageType, PathType >                PathIteratorType;

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(filename);
    reader->Update();
    // Distance transform
    typedef itk::SignedDanielssonDistanceMapImageFilter<
            InputImageType,
            InterImageType> FilterType;
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(reader->GetOutput());
    // Rescale Distance transform to [0,1]
    typedef itk::RescaleIntensityImageFilter<
            InterImageType,InterImageType > RescalerType;
    RescalerType::Pointer scaler = RescalerType::New();
    scaler->SetInput(filter->GetOutput());
    scaler->SetOutputMinimum(.0);
    scaler->SetOutputMaximum(1.);
    scaler->Update();
    // get speed function
    InterImageType::Pointer speed = scaler->GetOutput();

    // create interpolater
    typedef itk::LinearInterpolateImageFunction<InterImageType, CoordRepType>
            InterpolatorType;
    InterpolatorType::Pointer interp = InterpolatorType::New();

    // create cost function
    PathFilterType::CostFunctionType::Pointer cost =
            PathFilterType::CostFunctionType::New();
    cost->SetInterpolator(interp);

    // create optimizer
    typedef itk::GradientDescentOptimizer OptimizerType;
    OptimizerType::Pointer optimizer = OptimizerType::New();
    optimizer->SetNumberOfIterations( 500 );

    // create path filter
    PathFilterType::Pointer pathFilter = PathFilterType::New();
    pathFilter->SetInput( speed );
    pathFilter->SetCostFunction( cost );
    pathFilter->SetOptimizer( optimizer );
    pathFilter->SetTerminationValue( 2.0 );

    // setup path points;
    PathFilterType::PointType start, end;

    start[0] = ps[0]; start[1] = ps[1]; start[2] = ps[2];
    end[0] = pe[0]; end[1] = pe[1];end[2] = pe[2];

    // add path information
    PathFilterType::PathInfo info;
    info.SetStartPoint( start );
    info.SetEndPoint( end );
    pathFilter->AddPathInfo( info );

    // compute the path
    pathFilter->Update();

    // allocate output image
    OutputImageType::Pointer output = OutputImageType::New();
    output->SetRegions( speed->GetLargestPossibleRegion() );
    output->SetSpacing( speed->GetSpacing() );
    output->SetOrigin( speed->GetOrigin() );
    output->Allocate( );
    output->FillBuffer( itk::NumericTraits<OutputPixelType>::Zero );

    // get valid path
    for(unsigned int i = 0;i< pathFilter->GetNumberOfOutputs();i++){
        PathType::Pointer path = pathFilter->GetOutput(i);
        //check if path is valid
        if(path->GetVertexList()->Size() == 0 ){
            std::cout << "WARNING: Path " << (i+1) << " Contains no Points!"
                      << std::endl;
            continue;
        }
        PathIteratorType it(output, path);
        PathType::PointType point;
        Point3f Cenpoint;
        int count = 1;
        for(it.GoToBegin(); !it.IsAtEnd(); ++it){
            it.Set( itk::NumericTraits<OutputPixelType>::max() );
            std::cout << (it.GetIndex())[0] << ", "
                       << (it.GetIndex())[1] << ", "
                       << (it.GetIndex())[2] << "\t\t";
            output->TransformIndexToPhysicalPoint(it.GetIndex(),point);
            std::cout << point[0] << ", " << point[1] << ", " << point[2] << "\n";
            Cenpoint.x = point[0];Cenpoint.y = point[1];Cenpoint.z = point[2];
//            CenterPoints.push_back(static_cast<Point3f>(point));
            CenterPoints.push_back(Cenpoint);
            count++;
        }
        std::cout << "Successfully find path: " << (i+1)
                  << " and it contains " << count << " points."<< std::endl;
    }

    // writer output
//    WriterType::Pointer writer = WriterType::New();
//    std::string OutputFilename = "testcenterline.mhd";
//    writer->SetFileName( OutputFilename );
//    writer->SetInput( output );
//    writer->Update();
    return EXIT_SUCCESS;
}

