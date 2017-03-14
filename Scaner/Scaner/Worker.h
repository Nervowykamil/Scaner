#pragma once
#include "MyForm.h"
#include <vcclr.h>


ref class Worker
{
public:
    Worker();
    void doWork();
private:
    Scaner::MyForm^ formPtr;
};