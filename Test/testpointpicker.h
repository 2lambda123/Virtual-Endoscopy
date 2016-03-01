#ifndef TESTPOINTPICKER_H
#define TESTPOINTPICKER_H

#include <vtkSmartPointer.h>
#include <vtkRendererCollection.h>
#include <vtkPointPicker.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>

// Define interaction style
class MouseInteractorStylePointPicker : public vtkInteractorStyleTrackballCamera
{
public:
    static MouseInteractorStylePointPicker* New();
    vtkTypeMacro(MouseInteractorStylePointPicker, vtkInteractorStyleTrackballCamera);

    virtual void OnLeftButtonDown()
    {
        std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0] << " " << this->Interactor->GetEventPosition()[1] << std::endl;
        this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0],
                                            this->Interactor->GetEventPosition()[1],
                                            0,  // always zero.
                                            this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        double picked[3];
        this->Interactor->GetPicker()->GetPickPosition(picked);
        std::cout << "Picked value: " << picked[0] << " " << picked[1] << " " << picked[2] << std::endl;
        // Forward events
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

};

vtkStandardNewMacro(MouseInteractorStylePointPicker);

class Test_PointPicker
{
public:
    int RunTest()
    {
        vtkSmartPointer<vtkSphereSource> sphereSource =
            vtkSmartPointer<vtkSphereSource>::New();
        sphereSource->Update();

        vtkSmartPointer<vtkPointPicker> pointPicker =
            vtkSmartPointer<vtkPointPicker>::New();

        // Create a mapper and actor
        vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(sphereSource->GetOutputPort());
        vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);

        // Create a renderer, render window, and interactor
        vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
        vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
        renderWindow->AddRenderer(renderer);
        vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
        renderWindowInteractor->SetPicker(pointPicker);
        renderWindowInteractor->SetRenderWindow(renderWindow);

        vtkSmartPointer<MouseInteractorStylePointPicker> style =
            vtkSmartPointer<MouseInteractorStylePointPicker>::New();
        renderWindowInteractor->SetInteractorStyle( style );

        // Add the actor to the scene
        renderer->AddActor(actor);
        renderer->SetBackground(1,1,1); // Background color white

        // Render and interact
        renderWindow->Render();
        renderWindowInteractor->Start();
        return EXIT_SUCCESS;
    }

};

#endif // TESTPOINTPICKER_H

