#pragma once
#include "MyForm.h"
#include "windows.h"
#include "Reader.h"
#include <vcclr.h>
#include <chrono>

ref class Worker
{
public:
    Worker();
    void doWork();
    void Tick();
    void Init();
    long getMSTime();
private:
    Scaner::MyForm^ formPtr;
    Reader *rdr;
    int updateTimer;
};