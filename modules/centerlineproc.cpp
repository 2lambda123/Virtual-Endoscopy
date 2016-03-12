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
    clear();
}

int centerLineProc::getDistanceMap(std::string filename)
{
    if(filename.empty())    return EXIT_FAILURE;
    const unsigned int Dimension = 3;
    typedef unsigned char InputPixelType;
    typedef float InterPixelType;
    typedef unsigned char OutputPixelType;
    typedef itk::Image< InputPixelType, Dimension >                       InputImageType;
    typedef itk::Image< InterPixelType, Dimension >                       InterImageType;
    typedef itk::Image< OutputPixelType, Dimension>                       OutputImageType;
    typedef itk::ImageFileReader< InputImageType >                        ReaderType;
    try{
        ReaderType::Pointer reader = ReaderType::New();
        reader->SetFileName(filename);
        reader->Update();
//        typedef itk::SignedMaurerDistanceMapImageFilter<
//                InputImageType,
//                InterImageType> FilterType;
//        FilterType::Pointer dft = FilterType::New();
//        dft->SetInput(reader->GetOutput());
//        typedef itk::RescaleIntensityImageFilter<
//                InterImageType,InterImageType > RescalerType;
//        RescalerType::Pointer scaler = RescalerType::New();

//        scaler->SetInput(dft->GetOutput());
//        scaler->SetOutputMinimum(0.0);
//        scaler->SetOutputMaximum(1.0);
//        typedef itk::InvertIntensityImageFilter<
//                InterImageType,InterImageType> InverterType;
//        InverterType::Pointer inverter = InverterType::New();
//        inverter->SetInput(scaler->GetOutput());
//        inverter->SetMaximum(1.0);
//        InterImageType::Pointer output = inverter->GetOutput();
//        inverter->Update();

            typedef itk::SignedMaurerDistanceMapImageFilter<
                    InputImageType,InterImageType> FilterType;
            FilterType::Pointer dft = FilterType::New();
            dft->SetInput(reader->GetOutput());
            typedef itk::SmoothingRecursiveGaussianImageFilter<
                    InterImageType,InterImageType> SmoothType;
            SmoothType::Pointer smoother = SmoothType::New();
            smoother->SetInput(dft->GetOutput());
            smoother->SetSigma(1.0);
            typedef itk::ThresholdImageFilter<
                    InterImageType> ThresholdType;
            ThresholdType::Pointer thresholder = ThresholdType::New();
            thresholder->SetInput(smoother->GetOutput());
            thresholder->ThresholdBelow(-10);
            thresholder->ThresholdAbove(100);
            thresholder->SetOutsideValue(-10);
            typedef itk::RescaleIntensityImageFilter<
                    InterImageType,InterImageType> RescaleType;
            RescaleType::Pointer scaler = RescaleType::New();
            scaler->SetInput(thresholder->GetOutput());
            scaler->SetOutputMinimum(0.0);
            scaler->SetOutputMaximum(1.0);
            scaler->Update();
            InterImageType::Pointer output = scaler->GetOutput();

        std::ofstream out("localmax.txt");
        if(out.is_open())
            out << filename << std::endl;
//       how to interate an 3d image
//       reference to:
//       http://sourcecodebrowser.com/insighttoolkit/3.16.0/itk_image_slice_iterator_test_8cxx.html
//       http://l3mmings.blogspot.com/2010/08/slices-stacks-and-itk.html
        typedef itk::ImageSliceConstIteratorWithIndex< InterImageType > sliceinterator;
        sliceinterator sliceit(output,output->GetLargestPossibleRegion());
        sliceit.SetFirstDirection(0); // 0 -x
        sliceit.SetSecondDirection(1); // 1 - y
        sliceit.GoToBegin();
        int i = 0;
        for(; i < 100; i++){
            sliceit.NextSlice();
        }
        for(; !sliceit.IsAtEnd() && i < 102;sliceit.NextSlice(),i++){
            out << "Slice No: " << i << "\n";
            while(!sliceit.IsAtEndOfSlice()){
                while(!sliceit.IsAtEndOfLine()){
//                    if(sliceit.Get() > 0.95)
                    out << sliceit.Get() << "\t";
                    ++sliceit;
                }
                sliceit.NextLine();
            }
            out << "**************************" << std::endl;
        }
//        OutputImageType::IndexType preminindex = sliceit.GetIndex();
//        for(int i = 0 ;!sliceit.IsAtEnd();i++,sliceit.NextSlice()){
//            OutputImageType::PixelType minval = sliceit.Get();
//            OutputImageType::IndexType minindex = sliceit.GetIndex();
//            ++sliceit;
//            while(!sliceit.IsAtEndOfSlice()){
//                while(!sliceit.IsAtEndOfLine()){
//                    if(sliceit.Get() < minval){

//                        minval = sliceit.Get();
//                        minindex = sliceit.GetIndex();
//                    }
//                    ++sliceit;
//                }
//                sliceit.NextLine();
//            }
//            if (i == 0){
//                preminindex = minindex;
//            }
//            else{
//                if(abs(preminindex[0] - minindex[0]) > 20
//                        || abs(preminindex[1] - minindex[1]) > 20){
//                    //change too sharply,use last minindex;
//                    minindex[0] = preminindex[0];
//                    minindex[1] = preminindex[1];
//                    minindex[2] = preminindex[2] + 1;
//                }
//                //update  minindex of last slice;
//                preminindex[0] = minindex[0];
//                preminindex[1] = minindex[1];
//                preminindex[2] = minindex[2] - 1;
//            }
//            OutputImageType::PointType minpoint;
//            outImg->TransformIndexToPhysicalPoint(minindex,minpoint);
//            localMinVec.push_back(minindex); // get local min position of a slice
//            MinPoints.push_back(minpoint);
//            if(out.is_open())
//                out << minindex[0] << ", "
//                                   << minindex[1] << ", "
//                                   << minindex[2] << "<----||---->"
//                                   << minpoint[0] << ", "
//                                   << minpoint[1] << ", "
//                                   << minpoint[2] << std::endl;

//        }

        CenterLineFlag = true;
    }
    catch(itk::ExceptionObject &exp)
    {
        std::cerr << "Exception caught !" << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;


}

int centerLineProc::getSignedDistanceMap_Sin(std::string filename)
{
    if(filename.empty())    return EXIT_FAILURE;
    typedef itk::ImageFileReader< InputImageType > ReaderType;
    typedef itk::ImageFileWriter< OutputMapType > WriterType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(filename);
    InputImageType::Pointer inImg = reader->GetOutput();
    reader->Update();
    InputImageType::SpacingType isp = inImg->GetSpacing();
    std::cout << isp[0] << " ," << isp[1] << " ," << isp[2] << std::endl;
    InputImageType::PointType origin = inImg->GetOrigin();
    std::cout << origin[0] << " ," << origin[1] << " ," << origin[2] << std::endl;
    std::ofstream out("localmax.txt");
    if(out.is_open())
        out << filename << std::endl;
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

int centerLineProc::getSignedDistanceMap_Mul(std::string filename)
{
    if(filename.empty())    return EXIT_FAILURE;
    typedef itk::ImageFileReader< InputImageType > ReaderType;
    typedef itk::ImageFileWriter< OutputMapType > WriterType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(filename);
    InputImageType::Pointer inImg = reader->GetOutput();
    reader->Update();
    InputImageType::SpacingType isp = inImg->GetSpacing();
    std::cout << isp[0] << " ," << isp[1] << " ," << isp[2] << std::endl;
    InputImageType::PointType origin = inImg->GetOrigin();
    std::cout << origin[0] << " ," << origin[1] << " ," << origin[2] << std::endl;
    std::ofstream out("localmax.txt");
    if(out.is_open())
        out << filename << std::endl;
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

void centerLineProc::Path_Thin3dImg(std::string filename, double ps[], double pe[])
{
    if(filename.empty())    return ;
    CenterPoints.clear();//remove last centerline results;
    const int Dimension = 3;
    typedef unsigned char                                   InputPixelType;
    typedef unsigned char                                   OutputPixelType;
    typedef itk::Image<InputPixelType, Dimension>           InputImageType;
    typedef itk::Image<OutputPixelType, Dimension>          OutputImageType;
    typedef itk::ImageFileReader< InputImageType>           ReaderType;
    typedef itk::BinaryThinningImageFilter3D< InputImageType,OutputImageType >
                                                            ThinningFilterType;
//    typedef itk::NeighborhoodIterator<OutputImageType>      NeighborhoodIteratorType;
    typedef itk::PolyLineParametricPath< Dimension >                      PathType;
    typedef itk::SpeedFunctionToPathFilter< OutputImageType,PathType >     PathFilterType;
    typedef PathFilterType::CostFunctionType::CoordRepType       CoordRepType;
    typedef itk::PathIterator< OutputImageType, PathType >                PathIteratorType;

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

    OutputImageType::Pointer speed = thinningFilter->GetOutput();

    // create interpolater
    typedef itk::LinearInterpolateImageFunction<OutputImageType, CoordRepType>
            InterpolatorType;
    InterpolatorType::Pointer interp = InterpolatorType::New();

    // create cost function
    PathFilterType::CostFunctionType::Pointer cost =
            PathFilterType::CostFunctionType::New();
    cost->SetInterpolator(interp);

//     create optimizer
    typedef itk::GradientDescentOptimizer OptimizerType;
    OptimizerType::Pointer optimizer = OptimizerType::New();
    optimizer->SetNumberOfIterations( 1000 );

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
    std::cout << "Computing path..." << std::endl;
    itk::TimeProbe time;
    time.Start();
    pathFilter->Update();
    time.Stop();
    std::cout << std::setprecision(3) << "Path computed in: " << time.GetMeanTime()
              << " seconds" << std::endl;

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
        //iterate path and convert to image
        PathIteratorType it(output, path);
        PathType::PointType point;
        Point3f Cenpoint;
        int count = 1;
        for(it.GoToBegin(); !it.IsAtEnd(); ++it){
            it.Set( itk::NumericTraits<OutputPixelType>::max() );
            std::cout << it.Get() << ", "<< (it.GetIndex())[0] << ", "
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
//    NeighborhoodIteratorType::RadiusType  radius;
//    radius[0] = 1; radius[1] = 1;radius[2] = 1;
//    OutputImageType::IndexType regionIndex;
//    regionIndex[0] = 0;regionIndex[1] = 0;regionIndex[2] = 0;
//    OutputImageType::SizeType regionSize = output->GetRequestedRegion().GetSize();
//    OutputImageType::RegionType region;
//    region.SetSize(regionSize);
//    region.SetIndex(regionIndex);
//    NeighborhoodIteratorType it(radius,output,region);
//    OutputImageType::PointType point;
//    Point3f Cenpoint;
//    for(it.GoToBegin();!it.IsAtEnd();++it){
//        if(it.GetCenterPixel() == 0)    continue;
//        OutputImageType::IndexType index;
//        index = it.GetIndex();
//        std::cout << index[0] << ", " << index[1] << ", " << index[2] << "\n";
//        output->TransformIndexToPhysicalPoint(index,point);
//        Cenpoint.x = point[0];Cenpoint.y = point[1];Cenpoint.z = point[2];
//        CenterPoints.push_back(Cenpoint);
//    }




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
    if(filename.empty())    return EXIT_FAILURE;
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
//    // step1: Distance transform
//    typedef itk::SignedMaurerDistanceMapImageFilter<
//            InputImageType,
//            InterImageType> FilterType;
//    FilterType::Pointer dft = FilterType::New();
//    dft->SetInput(reader->GetOutput());
//    // step2: Rescale Distance transform to [0,1]
//    typedef itk::RescaleIntensityImageFilter<
//            InterImageType,InterImageType > RescalerType;
//    RescalerType::Pointer scaler = RescalerType::New();
//    scaler->SetInput(dft->GetOutput());
//    scaler->SetOutputMinimum(.0);
//    scaler->SetOutputMaximum(1.);
//    // step3: reverse intensity value to [1,0]
//    typedef itk::InvertIntensityImageFilter<
//            InterImageType,InterImageType> InverterType;
//    InverterType::Pointer inverter = InverterType::New();
//    inverter->SetInput(scaler->GetOutput());
//    inverter->SetMaximum(1.0);
// //    inverter->Update();
//   // step4: general threshold(only keep pixelvalue >= 0.9)
//    typedef itk::ThresholdImageFilter<
//            InterImageType> ThresholdType;
//    ThresholdType::Pointer thresholder = ThresholdType::New();
//    thresholder->SetInput(inverter->GetOutput());
//    thresholder->ThresholdBelow(0.95);
//    thresholder->SetOutsideValue(0.0);
//    thresholder->Update();
//    // get speed function(DFT --->  Rescale --->  reverse  --->  threshold)
//    InterImageType::Pointer speed = thresholder->GetOutput();

    typedef itk::SignedMaurerDistanceMapImageFilter<
            InputImageType,InterImageType> FilterType;
    FilterType::Pointer dft = FilterType::New();
    dft->SetInput(reader->GetOutput());
    typedef itk::SmoothingRecursiveGaussianImageFilter<
            InterImageType,InterImageType> SmoothType;
    SmoothType::Pointer smoother = SmoothType::New();
    smoother->SetInput(dft->GetOutput());
    smoother->SetSigma(1.0);
    typedef itk::ThresholdImageFilter<
            InterImageType> ThresholdType;
    ThresholdType::Pointer thresholder = ThresholdType::New();
    thresholder->SetInput(smoother->GetOutput());
    thresholder->ThresholdBelow(-10);
    thresholder->ThresholdAbove(100);
    thresholder->SetOutsideValue(-10);
    typedef itk::RescaleIntensityImageFilter<
            InterImageType,InterImageType> RescaleType;
    RescaleType::Pointer scaler = RescaleType::New();
    scaler->SetInput(thresholder->GetOutput());
    scaler->SetOutputMinimum(0.0);
    scaler->SetOutputMaximum(1.0);
    scaler->Update();
    InterImageType::Pointer speed = scaler->GetOutput();


    // create interpolater
    typedef itk::LinearInterpolateImageFunction<InterImageType, CoordRepType>
            InterpolatorType;
    InterpolatorType::Pointer interp = InterpolatorType::New();

    // create cost function
    PathFilterType::CostFunctionType::Pointer cost =
            PathFilterType::CostFunctionType::New();
    cost->SetInterpolator(interp);

//     create optimizer
    typedef itk::GradientDescentOptimizer OptimizerType;
    OptimizerType::Pointer optimizer = OptimizerType::New();
    optimizer->SetNumberOfIterations( 1000 );
//    typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
//    OptimizerType::Pointer optimizer = OptimizerType::New();
//    optimizer->SetNumberOfIterations(50000);
//    optimizer->SetMaximumStepLength(2);
//    optimizer->SetMinimumStepLength(0.5);
//    optimizer->SetRelaxationFactor(0.5);

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
    std::cout << "Computing path..." << std::endl;
    itk::TimeProbe time;
    time.Start();
    pathFilter->Update();
    time.Stop();
    std::cout << std::setprecision(3) << "Path computed in: " << time.GetMeanTime()
              << " seconds" << std::endl;

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
        //iterate path and convert to image
        PathIteratorType it(output, path);
        PathType::PointType point;
        Point3f Cenpoint;
        int count = 1;
        for(it.GoToBegin(); !it.IsAtEnd(); ++it){
//            it.Set( itk::NumericTraits<OutputPixelType>::max() )
            std::cout << (it.GetIndex())[0] << ", "
                      << (it.GetIndex())[1] << ", "
                      << (it.GetIndex())[2] << "\t\t" << it.Get() << "*****";
            speed->TransformIndexToPhysicalPoint(it.GetIndex(),point);
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

