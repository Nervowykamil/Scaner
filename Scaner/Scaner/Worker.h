#pragma once
#include "MyForm.h"
#include <vcclr.h>
#include <chrono>

ref class Worker
{
public:
    Worker();
    void doWork();
    unsigned int getMSTime();
private:
    Scaner::MyForm^ formPtr;
};