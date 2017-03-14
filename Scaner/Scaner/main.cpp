#include "Worker.h"

[STAThread]
void Main(array<String^>^ args)
{
    Worker ^worker = gcnew Worker();
}