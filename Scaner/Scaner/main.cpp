#include "Worker.h"

using namespace System;


[STAThread]
void Main(array<String^>^ args)
{
    Worker ^worker = gcnew Worker();
}