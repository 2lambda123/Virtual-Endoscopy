#ifndef TESTUNIT_H
#define TESTUNIT_H

#include "testpointpicker.h"
class TestUnit
{
public:
    static void RunTest()
    {
        Test_PointPicker *testpointpicker = new Test_PointPicker();
        testpointpicker->RunTest();
        delete testpointpicker;
    }
};


#endif // TESTUNIT_H

