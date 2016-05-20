#ifndef ITKVTKPIPE
#define ITKVTKPIPE



//#include <itkVTKImageExport.h>
//#include <vtkImageImport.h>
//#include <itkVTKImageImport.h>
//#include <vtkImageExport.h>
#include <vtkObjectFactory.h>
#include <vtkSetGet.h>

template <typename ITK_Exporter,typename VTK_Importer>
void ConnectPipelines(ITK_Exporter exporter,VTK_Importer* importer)
{
    importer->SetUpdateInformationCallback(
                        exporter->GetUpdateInformationCallback());
    importer->SetPipelineModifiedCallback(
                        exporter->GetPipelineModifiedCallback());
    importer->SetWholeExtentCallback(exporter->GetWholeExtentCallback());
    importer->SetSpacingCallback(exporter->GetSpacingCallback());
    importer->SetOriginCallback(exporter->GetOriginCallback());
    importer->SetScalarTypeCallback(exporter->GetScalarTypeCallback());
    importer->SetNumberOfComponentsCallback(
                        exporter->GetNumberOfComponentsCallback());
    importer->SetPropagateUpdateExtentCallback(
                        exporter->GetPropagateUpdateExtentCallback());
    importer->SetUpdateDataCallback(exporter->GetUpdateDataCallback());
    importer->SetDataExtentCallback(exporter->GetDataExtentCallback());
    importer->SetBufferPointerCallback(exporter->GetBufferPointerCallback());
    importer->SetCallbackUserData(exporter->GetCallbackUserData());
}
template <typename VTK_Exporter,typename ITK_Importer>
void ConnectPipelines(VTK_Exporter* exporter,ITK_Importer importer)
{
    importer->SetUpdateInformationCallback(
                        exporter->GetUpdateInformationCallback());
    importer->SetPipelineModifiedCallback(
                        exporter->GetPipelineModifiedCallback());
    importer->SetWholeExtentCallback(exporter->GetWholeExtentCallback());
    importer->SetSpacingCallback(exporter->GetSpacingCallback());
    importer->SetOriginCallback(exporter->GetOriginCallback());
    importer->SetScalarTypeCallback(exporter->GetScalarTypeCallback());
    importer->SetNumberOfComponentsCallback(
                        exporter->GetNumberOfComponentsCallback());
    importer->SetPropagateUpdateExtentCallback(
                        exporter->GetPropagateUpdateExtentCallback());
    importer->SetUpdateDataCallback(exporter->GetUpdateDataCallback());
    importer->SetDataExtentCallback(exporter->GetDataExtentCallback());
    importer->SetBufferPointerCallback(exporter->GetBufferPointerCallback());
    importer->SetCallbackUserData(exporter->GetCallbackUserData());
}
#endif // ITKVTKPIPE
