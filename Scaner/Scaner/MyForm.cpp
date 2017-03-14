#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

void Scaner::MyForm::addLogLine(String ^ line)
{
    this->richTextBox1->AppendText(line);
}

void Scaner::MyForm::ClearMap()
{
    throw gcnew System::NotImplementedException();
}

void Scaner::MyForm::DrawDot(int x, int y, int r, System::Drawing::Color color)
{
    SolidBrush ^b = gcnew SolidBrush(color);
    g1->FillEllipse(b, x, y, r, r);
}
