#include "Worker.h"

using namespace System::Windows::Forms;
using namespace System::Threading;

unsigned int Worker::getMSTime()
{
    using namespace std::chrono;

    static const steady_clock::time_point ApplicationStartTime = steady_clock::now();

    return unsigned int(duration_cast<milliseconds>(steady_clock::now() - ApplicationStartTime).count());
}

void Worker::doWork()
{
    bool startmessage = true;
    while (1)
    {
        if (formPtr->Created)
        {
            // serious stuff
            if (startmessage)
            {
                formPtr->addLogLine("Started \n");
                startmessage = false;
            }
        }
    }
}

Worker::Worker()
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    Scaner::MyForm form;
    formPtr = %form;
    Thread ^t1 = gcnew Thread(gcnew ThreadStart(this, &Worker::doWork));
    t1->Start();
    Application::Run(%form);
}
