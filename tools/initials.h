#ifndef INITIALS_H
#define INITIALS_H


/// add this to resolve
/// "Warning: Link to vtkInteractionStyle for default style selection."
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
//VTK_MODULE_INIT(vtkRenderingFreeTypeOpenGL);

/// additional libs
#pragma comment(lib, "snmpapi.lib")
#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "advapi32.lib")

//problem: This application failed to start because it could not find or load the Qt platform plugin "windows".

//         Reinstalling the application may fix this problem.
//reference: http://forum.qt.io/topic/26501/platform-windows-not-found
//           http://stackoverflow.com/questions/16773789/qt5-static-build-yields-failed-to-load-platform-plugin-windows
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Qt5PlatformSupport.lib")

#include "tklib.h"
#include "itkutils.h"



class PreInitials {
public:
    static void Initilize()
    {
        ItkUtils::RegisterAllImageIOFactories();
    }
};

#endif // INITIALS_H

