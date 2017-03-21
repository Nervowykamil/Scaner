#pragma once

namespace Scaner {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
    using namespace System::Threading;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(Thread ^thread)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            bm = gcnew Bitmap(panel1->Width, panel1->Height);
            g1 = Graphics::FromImage(bm);
            g2 = panel1->CreateGraphics();
            t1 = thread;
		}

        void addLogLine(String ^line);
        void RedrawMap();
        void DrawDot(int x, int y, int r, Color color);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::RichTextBox^  richTextBox1;
    protected:
    private: System::Windows::Forms::Panel^  panel1;

	private:
        Graphics ^g1; // offscreen buffer
        Graphics ^g2; // onscreen buffer
        Bitmap ^bm; // bitmap for offscreen buffer
        Thread ^t1;

		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
            this->panel1 = (gcnew System::Windows::Forms::Panel());
            this->SuspendLayout();
            // 
            // richTextBox1
            // 
            this->richTextBox1->Location = System::Drawing::Point(12, 12);
            this->richTextBox1->Name = L"richTextBox1";
            this->richTextBox1->ReadOnly = true;
            this->richTextBox1->Size = System::Drawing::Size(210, 118);
            this->richTextBox1->TabIndex = 0;
            this->richTextBox1->Text = L"";
            // 
            // panel1
            // 
            this->panel1->Location = System::Drawing::Point(12, 136);
            this->panel1->Name = L"panel1";
            this->panel1->Size = System::Drawing::Size(210, 210);
            this->panel1->TabIndex = 1;
            // 
            // MyForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(236, 358);
            this->Controls->Add(this->panel1);
            this->Controls->Add(this->richTextBox1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->Name = L"MyForm";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Text = L"Object Scaner";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MyForm::MyForm_FormClosing);
            this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
            this->ResumeLayout(false);

        }
#pragma endregion
    private: System::Void MyForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
    {
        t1->Abort();
    }
    private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) 
    {
        t1->Start();
    }
    };
}
