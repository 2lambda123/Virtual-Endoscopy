#ifndef SLICESHOWUTILS_H
#define SLICESHOWUTILS_H

#include "tools/fastdef.h"
#include "tools/itkutils.h"
#include "tools/itkvtkpipe.h"
#ifndef MSGFORMAT
#define MAGFORMAT
#include <sstream>
class StatusMessage {
public:
    static std::string Format(int Slice, int maxSlice) {
        std::stringstream tmp;
        tmp << "Slice Number  " << Slice + 1 << " / " << maxSlice + 1;
        return tmp.str();
    }
};
#endif

#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkImageViewer2.h>
#include <vtkInteractorStyleImage.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkSetGet.h>

#include <itkImage.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageFileReader.h>
#include <itkImageSeriesReader.h>
#include <itkImageFileWriter.h>

class DCMVtkInteractorStyleImage : public vtkInteractorStyleImage
{
public:
    static DCMVtkInteractorStyleImage* New();
    vtkTypeMacro(DCMVtkInteractorStyleImage, vtkInteractorStyleImage);
protected:
    vtkImageViewer2* _ImageViewer;
    vtkTextMapper* _StatusMapper;
    int _Slice;
    int _MinSlice;
    int _MaxSlice;
    bool pickable;
protected:
    void MoveSliceForward()
    {
        if(_Slice < _MaxSlice) {
            _Slice += 1;
            std::cout << "MoveSliceForward::Slice = " << _Slice << std::endl;
            _ImageViewer->SetSlice(_Slice);
            std::string msg = StatusMessage::Format(_Slice,_MaxSlice);
            _StatusMapper->SetInput(msg.c_str());
            _ImageViewer->Render();
        }
    }
    void MoveSliceBackward()
    {
        if(_Slice > _MinSlice) {
            _Slice -= 1;
            std::cout << "MoveSliceBackward::Slice = " << _Slice << std::endl;
            _ImageViewer->SetSlice(_Slice);
            std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
            _StatusMapper->SetInput(msg.c_str());
            _ImageViewer->Render();
        }
    }

    virtual void OnKeyDown()
    {
        std::string key = this->GetInteractor()->GetKeySym();
        if(key.compare("Up") == 0) {
            MoveSliceForward();
        }else if(key.compare("Down") == 0) {
            MoveSliceBackward();
        }
        vtkInteractorStyleImage::OnKeyDown();
    }
    virtual void OnMouseWheelForward()
    {
        MoveSliceForward();
    }
    virtual void OnMouseWheelBackward()
    {
        if(_Slice > _MinSlice) {
            MoveSliceBackward();
        }
    }
public:
    void SetImageViewer(vtkImageViewer2* imageViewer)
    {
        _ImageViewer = imageViewer;
        _MinSlice = imageViewer->GetSliceMin();
        _MaxSlice = imageViewer->GetSliceMax();
        _Slice = _MinSlice;
        cout << "Slicer: Min = " << _MinSlice << ", Max = " << _MaxSlice << std::endl;
    }
    void SetStatusMapper(vtkTextMapper* statusMapper)
    {
        _StatusMapper = statusMapper;
    }
};
class sliceshowutils
{
public:
    sliceshowutils();
    void openWithITK();
    void loadDICOMSeries();
private:
    vtkSmartPointer<DCMVtkInteractorStyleImage> dcmInteractorStyle;
    vtkSmartPointer<vtkImageViewer2> m_imageViewer;
    vtkSmartPointer<vtkImageData> m_vtkImage;
    //slice hint message
    vtkSmartPointer<vtkTextProperty> m_sliceTextProp;
    vtkSmartPointer<vtkTextMapper> m_sliceTextMapper;
    vtkSmartPointer<vtkActor2D> m_sliceTextActor;
    //usage hint message
    vtkSmartPointer<vtkTextProperty> m_usageTextProp;
    vtkSmartPointer<vtkTextMapper> m_usageTextMapper;
    vtkSmartPointer<vtkActor2D> m_usageTextActor;
    void initSliceHint();
    void initUsageHint();
};

#endif // SLICESHOWUTILS_H
