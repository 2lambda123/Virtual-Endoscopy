#ifndef VTKSWITCHINTERACTIONCALLBACK_H
#define VTKSWITCHINTERACTIONCALLBACK_H

#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkInteractorStyleImage.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkCommand.h>
#include <QVTKWidget.h>


//Callback for automatic change of interaction styles
//(between image display mode and volume rendering mode).
class vtkSwitchInteractionCallback : public vtkCommand
{
public:
    static vtkSwitchInteractionCallback *New()
    {
        return new vtkSwitchInteractionCallback;
    }
    vtkSwitchInteractionCallback()
    {
        this->m_imageIren = 0;
        this->m_volumeIren = 0;
        this->m_imageRenderer = 0;
        this->m_interactor = 0;
    }
    void SetInteractor(vtkRenderWindowInteractor *interactor)
    {
        this->m_interactor = interactor;
    }
    vtkRenderWindowInteractor *GetInteractor()
    {
        return this->m_interactor;
    }
    void SetVolumeInteractorStyle(vtkInteractorStyleTrackballActor* m)
    {
        this->m_volumeIren = m;
    }
    void SetImageInteractorStyle(vtkInteractorStyleImage *m)
    {
        this->m_imageIren = m;
    }
    void SetImageRenderer(vtkRenderer *m)
    {
        this->m_imageRenderer = m;
    }
    virtual void Execute(vtkObject *,unsigned long event,void *)
    {
        vtkRenderWindowInteractor *interactor = this->GetInteractor();

        int lastpos[2];
        interactor->GetLastEventPosition(lastpos);

        int curpos[2];
        interactor->GetEventPosition(curpos);

        if(event == vtkCommand::MouseMoveEvent) {
            vtkRenderer *renderer = interactor->FindPokedRenderer(curpos[0],curpos[1]);
            if(renderer != m_imageRenderer) {
                interactor -> SetInteractorStyle(this->m_volumeIren);
            }else{
                interactor -> SetInteractorStyle(this->m_imageIren);
            }
            vtkInteractorStyle *style = vtkInteractorStyle::SafeDownCast(interactor->GetInteractorStyle());
            if (style) {
                style->OnMouseMove();
            }
        }
    }



private:
    vtkRenderWindowInteractor * m_interactor;
    vtkInteractorStyleImage * m_imageIren;
    vtkRenderer * m_imageRenderer;
    vtkInteractorStyleTrackballActor * m_volumeIren;


};

#endif // VTKSWITCHINTERACTIONCALLBACK_H

