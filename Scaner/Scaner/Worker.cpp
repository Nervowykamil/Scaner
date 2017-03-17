#include "Worker.h"

using namespace System::Windows::Forms;
using namespace System::Threading;

void Worker::Init()
{
    rdr = new Reader();
    formPtr->addLogLine("Started, player guid = " + rdr->GetLocalGUID() + " \n");
    float x, y, z;
    rdr->ReadPlayerXYZ(x, y, z);
    formPtr->addLogLine("x = " + x + " y = " + y + " z = " + z + "\n");
}

long Worker::getMSTime()
{
    using namespace std::chrono;
    auto time = system_clock::now();

    auto since_epoch = time.time_since_epoch();

    auto milis = duration_cast<milliseconds>(since_epoch);
    
    return long(milis.count());
}

void Worker::doWork()
{
    long lastTime = getMSTime();
    bool startmessage = true;
    while (1)
    {
        if (formPtr->Created)
        {
            // serious stuff
            if (startmessage)
            {
                Init();
                startmessage = false;
            }
            else
            {
                long diff = getMSTime() - lastTime; // time diff between loops
                lastTime = getMSTime(); // set lastTime to now
                if (updateTimer - diff <= 0)
                {
                    Tick();
                    updateTimer = 500; // 500 ms = 0.5s
                } else updateTimer -= diff;
            }
        }

    }
}

void Worker::Tick()
{
    // do things
    float x, y, z;
    rdr->ReadPlayerXYZ(x, y, z);
    formPtr->addLogLine("x = " + x + " y = " + y + " z = " + z + "\n");
}

Worker::Worker()
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    Thread ^t1 = gcnew Thread(gcnew ThreadStart(this, &Worker::doWork));
    Scaner::MyForm form(t1);
    formPtr = %form;
    
    Application::Run(%form);

    // set some stuff
    updateTimer = 500;
}
