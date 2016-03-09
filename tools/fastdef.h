#ifndef FASTDEF_H
#define FASTDEF_H

# include <vtkSmartPointer.h>
# include "initials.h"

#define Instantiate(obj,class) vtkSmartPointer<class> obj = vtkSmartPointer<class>::New();
//#define vsp(class,var) \
//  vtkSmartPointer<class> tmpvar = vtkSmartPointer<class>::New(); \
//  var = tmpvar;
template <typename T>
inline void vsp(vtkSmartPointer<T> &var)
{
    vtkSmartPointer<T> temp = vtkSmartPointer<T>::New();
    var = temp;
}

struct Point3f {
    double x, y, z;
};
#endif // FASTDEF_H

